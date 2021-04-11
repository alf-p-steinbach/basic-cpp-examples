#include <kickstart/all.hpp>        // https://github.com/alf-p-steinbach/kickstart
using namespace kickstart::all;     // math::intpow, spaces, out, endl

auto main() -> int
{
    constexpr int n = math::intpow( 2, 5 );
    for( int y = 0; y < n; ++y ) {
        out << spaces( n - y );
        for( int x = 0; x <= y; ++x ) {
            out << (x & ~y? ' ' : 'o') << ' ';
        }
        out << endl;
    }
}
