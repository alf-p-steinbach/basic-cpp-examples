#include "stream_input.hpp"
#include <iostream>
#include <string>

namespace app {
    using   my::stream_io::input, my::stream_io::input_int;
    using   std::cout, std::endl, std::flush,   // <iostream>
            std::string;                        // <string>
            
    auto&   out     = cout;
    string  name    = "Nomen Nescio";

    void on_cmd_new_name()
    {
        out << "New name? ";
        name = input();     // TODO: handle empty line, leading and trailing spaces.
    }

    void on_cmd_exit()
    {
        out << "Goodbye, " << name << "!" << endl;
    }

    void show_menu()
    {
        out << "Please choose one option, " << name << ": \n"
            << "1  Change the name.\n"
            << "2  Exit.\n"
            << flush;
    }

    void run()
    {
        for( bool finished = false; not finished; finished or (out << endl) ) {
            show_menu();
            const int choice = input_int( "? " ).value_or( -1 );
            switch( choice ) {
                case 1:     on_cmd_new_name();  break;
                case 2:     on_cmd_exit();  finished = true;  break;
                default:    out << "Oh, I only understand integers 1 and 2." << endl;
            }
        }
    }
}  // namespace app

#include <stdlib.h>     // EXIT_...
auto main() -> int
{
    using std::exception, std::cerr, std::endl;
    try {
        app::run();
        return EXIT_SUCCESS;
    } catch( const exception& x ) {
        cerr << "!" << x.what() << endl;
    }
    return EXIT_FAILURE;
}
