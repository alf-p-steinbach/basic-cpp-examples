#include <stdlib.h>         // EXIT_...

#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>
namespace fs = std::filesystem;
using   std::cin, std::cout, std::cerr, std::endl,
        std::exception,
        std::string,
        std::vector;

auto to_utf8( const fs::path& path )
    -> string
{
    const auto s = path.u8string();         // In C++20 typed to sabotage Windows ...
    return string( s.begin(), s.end() );    // ... incurring this copying inefficiency.
}

auto main() -> int
{
    static const auto dir_path =
        fs::path( "example/movies" );       // "C:/Users/Lady Dora/Desktop/july/c++";

    try {
        vector<string> filenames;
        for( const auto& dir_entry: fs::directory_iterator( dir_path ) ) {
            filenames.push_back( to_utf8( dir_entry.path().filename() ) );
        }
        for ( const auto& filename: filenames ) { cout << filename << endl; }
        return EXIT_SUCCESS;
    } catch( const exception& x ) {
        cerr << "!" << x.what() << endl;
    }
    return EXIT_FAILURE;
}
