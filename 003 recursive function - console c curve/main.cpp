#include <kickstart/all.hpp>        // https://github.com/alf-p-steinbach/kickstart
using namespace kickstart::all;     // ascii::*, math::*, two_d_grid::*, out, endl

#include <bitset>
namespace box_drawing {
    using std::bitset;

    using Position  = two_d_grid::Position;
    using Size      = two_d_grid::Size;
    
    //---------------------------------------------- Direction
    struct Direction{ enum Enum{ right, up, left, down, N }; };
    using Dir = Direction;
    static_assert( Dir::N == 4 );
    
    constexpr auto unit_offset_for( const Direction::Enum dir )
        -> Size
    { return array<Size, Dir::N>{{ {1, 0}, {0, -1}, {-1, 0}, {0, 1} }}[dir]; }

    constexpr auto opposite_of( const Direction::Enum dir )
        -> Direction::Enum
    { return array{ Dir::left, Dir::down, Dir::right, Dir::up }[dir]; }

    constexpr auto left_from( const Direction::Enum dir )
        -> Direction::Enum
    { return Direction::Enum( (dir + 1) % Direction::N ); }

    constexpr auto right_from( const Direction::Enum dir )
        -> Direction::Enum
    { return Direction::Enum( (dir - 1 + Direction::N) % Direction::N ); }

    //---------------------------------------------- Lines_in_box
    using Lines_in_box = bitset<Direction::N>;
    constexpr int n_box_states = math::intpow( 2, Direction::N );

    auto line_char( const Lines_in_box box_lines )
        -> string_view
    {
        static const array<string_view, n_box_states> the_chars =
        {
            " ", "╶", "╵", "└", "╴", "─", "┘", "┴", "╷", "┌", "│", "├", "┐", "┬", "┤", "┼"
        };
        return the_chars[box_lines.to_ulong()];
    }
    
    //---------------------------------------------- Canvas
    class Canvas
    {
        Matrix_<Lines_in_box>   m_boxes;
        
    public:
        Canvas( const Size size ): m_boxes( size ) {}
        
        auto contains( const Position& p ) const
            -> bool
        {
            const Size s = m_boxes.size();
            return (0 <= p.x and p.x < s.w and 0 <= p.y and p.y < s.h);
        }

        auto unit_line( const Position start_point, const Direction::Enum dir )
            -> Position
        {
            const auto end_point = start_point + unit_offset_for( dir );
            if( contains( start_point ) ) {
                m_boxes( start_point ).set( dir );  // Set bit
            }
            if( contains( end_point ) ) {
                m_boxes( end_point ).set( opposite_of( dir ) );
            }
            return end_point;
        }

        auto line( const Position start_point, const Direction::Enum dir, const int length )
            -> Position
        {
            Position where = start_point;
            for( int i = 1; i <= length; ++i ) {
                where = unit_line( where, dir );
            }
            return where;
        }

        auto widened() const
            -> Canvas
        {
            auto result = Canvas( Size{ 2*m_boxes.width() + 1, m_boxes.height() } );
            for( int y = 0, h = m_boxes.height(); y < h; ++y ) {
                Lines_in_box previous;
                for( int x = 0, w = m_boxes.width(); x <= w; ++x ) {
                    auto& filler = result.m_boxes({2*x, y});
                    if( previous[Direction::right] ) { filler.set( Direction::left ); }
                    if( x < w ) {
                        auto& current = result.m_boxes({1 + 2*x, y});
                        current = m_boxes({x, y});
                        if( current[Direction::left] ) { filler.set( Direction::right ); }
                        previous = current;
                    }
                }
            }
            return result;
        }

        auto to_full_string() const
            -> string
        {
            string result;
            for( int y = 0, h = m_boxes.height(); y < h; ++y ) {
                for( int x = 0, w = m_boxes.width(); x < w; ++x ) {
                    result += line_char( m_boxes( x, y ).to_ulong() );
                }
                result += '\n';
            }
            return result;
        }
        
        auto to_string() const
            -> string
        { return ascii::suffixtrimmed_string( to_full_string() ) + "\n"; }
    };
}  // namespace box_drawing

using Position  = two_d_grid::Position;
using Direction = box_drawing::Direction;   // right, up, left, down

class C_curve
{
    box_drawing::Canvas&    m_canvas;

public:
    C_curve( box_drawing::Canvas& canvas ): m_canvas( canvas ) {}
    
    void draw( Position& where, const Direction::Enum direction, const int level )
    {
        if( level == 0 ) {
            where = m_canvas.unit_line( where, direction ); // Returns line end point.
        } else {
            draw( where, direction, level - 1 );
            draw( where, left_from( direction ), level - 1 );
        }
    }

    void draw( const Position& start_pos, const Direction::Enum direction, const int level )
    {
        Position where = start_pos;
        draw( where, direction, level );
    }
};

void draw_plus_sign( box_drawing::Canvas& canvas, const Position& start_point, const int size )
{
    Position            where   = start_point;
    Direction::Enum     dir     = Direction::right;

    for( int i = 1; i <= 4; ++i ) {
        where = canvas.line( where, dir, size );
        dir = right_from( dir );
        where = canvas.line( where, dir, size );
        dir = left_from( dir );
        where = canvas.line( where, dir, size );
        dir = right_from( dir );
    }        
}

auto main() -> int
{
    auto drawing = box_drawing::Canvas({40 + 20, 64});

    C_curve( drawing ).draw( {31, 15}, Direction::left, 10 );
    draw_plus_sign( drawing, {34, 21}, 7 );
    draw_plus_sign( drawing, {42, 20}, 7 );
    out << drawing.widened().to_string() << endl;   // "widened": 2x width for console display.
}
