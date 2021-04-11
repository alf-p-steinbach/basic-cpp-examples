#include <kickstart/all.hpp>
using namespace kickstart::all;     // Abstract_c_file, fsx::*, Array_span_, out, etc.
#include <stdio.h>      // fwrite
#include <stddef.h>     // offset_of

#include <algorithm>    // std::copy
#include <bitset>

class Binary_writer:
    public Abstract_c_file
{
public:
    Binary_writer( const fsx::Path& path )
        : Abstract_c_file( fsx::open_c_file_or_x( path, "wb" ) )    // UTF-8-ish.
    {}
    
    void output( const Array_span_<const Byte> bytes )
    {
        const Size n = ::fwrite( bytes.data(), 1, bytes.size(), c_file() );
        hopefully( n == bytes.size() )
            or KS_FAIL( ""s << "Only " << n << " of " << bytes.size() << "  bytes written." );
    }
};

template< class T >
void to_little_endian( const T, Type_<Byte*>& ) = delete;

void to_little_endian( const uint16_t value, Type_<Byte*>& p_bytes )
{
    static_assert( CHAR_BIT == 8 );
    *p_bytes++ = Byte( value & 0xFF );
    *p_bytes++ = Byte( value >> 8 );
}
    
namespace tga {
    // <url: https://en.wikipedia.org/wiki/Truevision_TGA#Header>

    namespace rgb {
        using std::copy;

        class Pixel
        {
            friend class Image;
            using Data = Byte[3];   // BGR order as in an image file.
            Data data;

        public:
            auto r() const -> Byte { return data[2]; }
            auto g() const -> Byte { return data[1]; }
            auto b() const -> Byte { return data[0]; }
            
            Pixel( const Byte r, const Byte g, const Byte b ):
                data{ b, g, r }
            {}
        };

        class Image:
            private Matrix_<Pixel::Data>
        {
        public:
            using Matrix_::Item;

            using Matrix_::Matrix_;
            using Matrix_::width;
            using Matrix_::height;
            using Matrix_::p_items;
            using Matrix_::n_items;

            void set( const int x, const int y, const Pixel& value )
            {
                Pixel::Data& image_data = (*this)( x, y );
                copy( value.data, value.data + sizeof( Pixel::Data ), image_data );
            }
            
            auto get( const int x, const int y ) const
                -> Pixel
            {
                const Pixel::Data& image_data = (*this)( x, y );
                return Pixel( image_data[2], image_data[1], image_data[0] );
            }
        };
    }  // namespace rgb

    namespace file {
        struct Spec
        {
            static constexpr int n_bytes = 10;

            uint16_t    lower_left_x;
            uint16_t    lower_left_y;
            uint16_t    width;
            uint16_t    height;
            Byte        bits_per_pixel;
            Byte        alpha_channel_depth;        // 4 bits
            Byte        direction;                  // 2 bits

            auto descriptor() const -> Byte { return (direction << 4) | alpha_channel_depth; }
            
            void to_bytes_in( Byte*& p_bytes ) const
            {
                const auto p_start_of_bytes = p_bytes;
                to_little_endian( lower_left_x,     p_bytes );
                to_little_endian( lower_left_y,     p_bytes );
                to_little_endian( width,            p_bytes );
                to_little_endian( height,           p_bytes );
                *p_bytes++ = bits_per_pixel;
                *p_bytes++ = descriptor();
                assert( p_bytes - p_start_of_bytes == n_bytes );
            }
        };

        class Image
        {
            Byte                        m_id_length;
            Byte                        m_color_map_kind;
            Byte                        m_image_kind;
            array<Byte, 5>              m_color_map_spec;
            Spec                        m_spec;             // 10 bytes
            vector<Byte>                m_id;               // id_length bytes
            vector<Byte>                m_color_map;        // length implied by color_map_kind
            Array_span_<const Byte>     m_data;
            // Extension fields can follow, 495 bytes.
            
        public:
            Image( const rgb::Image& image ):
                m_id_length( 0 ),           // No id.
                m_color_map_kind( 0 ),      // No color map.
                m_image_kind( 2 ),          // Uncompressed true-color image.
                m_color_map_spec{},         // No color map.
                m_spec{ 0, 0, uint16_t( image.width() ), uint16_t( image.height() ), 3*8, 0, 2 },
                m_id(),                     // No id.
                m_color_map(),              // No color map.
                m_data{ byte_span_of(
                    image.p_items(), image.n_items()*sizeof( rgb::Image::Item )
                    ) }
            {}

            void write_to( const fsx::Path& path ) const
            {
                auto writer = Binary_writer( path );
                auto header = array<Byte, sizeof( Image )>();   // More than sufficient.
                Byte* p_current = header.data();
                
                *p_current++ = m_id_length;
                *p_current++ = m_color_map_kind;
                *p_current++ = m_image_kind;
                for( const Byte byte: m_color_map_spec ) { *p_current++ = byte; }
                m_spec.to_bytes_in( p_current );
                assert( m_id.size() == 0 );
                assert( m_color_map.size() == 0 );

                const auto n_header_bytes = int( p_current - header.data() );
                writer.output( byte_span_of( header.data(), n_header_bytes ) );
                writer.output( m_data );
            }
        };
    }  // namespace file

    void write_to( const fsx::Path& path, const rgb::Image& rgb_image )
    {
        file::Image( rgb_image ).write_to( path );
    }
}  // namespace tga

void cppmain()
{
    namespace rgb = tga::rgb;
    using std::bitset;

    const fsx::Path image_path = fsx::exe_directory_path()/"rule-90-sierpinskies.tga";
    const int size = math::intpow( 2, 11 ) - 1;
    auto image = rgb::Image( size );
    const auto bg = rgb::Pixel( 0xFF, 0xFF, 0xF8 );     // Light yellow.
    const auto fg = rgb::Pixel( 0xA0, 0x20, 0xFF );     // Purplish.

    auto current = vector<bool>( size + 2 );
    current[size/2] = true;
    for( int y = 0; y < size; ++y ) {
        if( y == 31 ) { current[7] = true; }            // Just some “random”
        if( y == 127 ) { current[size - 228] = true; }  // dots, for better image.
        for( int x = 0; x < size; ++x ) {
            image.set( x, y, (current[x + 1]? fg : bg) );
        }

        auto next = vector<bool>( current.size() );
        for( int x = 0; x < size; ++x ) {
            const unsigned pattern_above = 
                (+current[x] << 2) | (+current[x + 1] << 1) | (+current[x + 2] << 0);
            const auto nks_rule_90 = bitset<8>( 0b01011010 );
            next[x + 1] = nks_rule_90[pattern_above];
        }
        next.swap( current );
    }
    tga::write_to( image_path, image );
    out << "Created “" << image_path.to_string() << "”." << endl;
}

auto main() -> int { return with_exceptions_displayed( cppmain ); }
