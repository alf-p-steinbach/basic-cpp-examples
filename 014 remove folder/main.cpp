#include <stdlib.h>         // EXIT_...

#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <string>
using   std::cin, std::cout, std::cerr, std::endl,
        std::string,
        std::exception, std::runtime_error;
using Path = std::filesystem::path;

auto hopefully( const bool e ) -> bool { return e; }
auto fail( const string& s ) -> bool { throw runtime_error( s ); }

#define FAIL( s ) fail( string() + __func__ + " - " + (s) )

auto to_utf8( const Path& path )
    -> string
{
    const auto s = path.u8string();         // In C++20 typed to sabotage Windows ...
    return string( s.begin(), s.end() );    // ... incurring this copying inefficiency.
}

void remove_folder( const Path& path )
{
    hopefully( is_directory( path ) )
        or FAIL( "“" + to_utf8( path ) + "” is not a folder." );

    const auto n_removed = remove_all( path );

    hopefully( n_removed > 0 )
        or FAIL( "Unable to (fully) remove “" + to_utf8( path ) + "”." );
}

auto main() -> int
{
    static const auto folder_path = Path( "full_folder" );
    try {
        remove_folder( folder_path );
        return EXIT_SUCCESS;
    } catch( const exception& x ) {
        cerr << "!" << x.what() << endl;
    }
    return EXIT_FAILURE;
}
