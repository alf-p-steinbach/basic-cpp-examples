#include "windows_h.for-hybrid-utf8-utf16.hpp"
#include <algorithm>
#include <iterator>
#include <string>
using   std::replace,                                                   // <algorithm>
        std::size,                                                      // <iterator>
        std::string, std::string_view, std::wstring, std::wstring_view; // <string>

template< class Container >
auto int_size( const Container& c ) -> int { return static_cast<int>( size( c ) ); }

using Utf8_c_string     = const char*;
using Utf16_c_string    = const wchar_t*;

const unsigned  replacement_char_code   = L'\uFFFD';
const unsigned  invalid_char_code       = 127;      // ASCII DEL, better for e.g. UTF_8.

const auto replacement_wchar    = wchar_t( replacement_char_code );
const auto invalid_wchar        = wchar_t( invalid_char_code );

auto to_utf16_wstring( const string_view& s )
    -> wstring
{
    if( s.empty() ) { return L""; }
    wstring buffer( s.size(), L'\0' );      // UTF-16 string length ≤ UTF-8 string length.
    const DWORD flags = 0;                  // Must be 0 for UTF-8.
    const int n_chars = MultiByteToWideChar(
        CP_UTF8, flags, s.data(), int_size( s ), buffer.data(), int_size( buffer )
        );
    buffer.resize( n_chars );
    replace( buffer.begin(), buffer.end(), replacement_wchar, invalid_wchar );
    return buffer;
}

class Utf16
{
    wstring m_text;
    
public:
    Utf16( const string_view& s ): m_text( to_utf16_wstring( s ) ) {}
    operator Utf16_c_string() const { return m_text.c_str(); }
};

auto startup() -> int
{
    const UINT as_infobox = MB_ICONINFORMATION | MB_SETFOREGROUND;
    const Utf8_c_string text = "Every 日本国 кошка likes Norwegian blåbærsyltetøy.";

    MessageBoxW( 0, Utf16( text ), Utf16( "Did you know?" ), as_infobox );
    return 0;
}
