#include "failure_handling.hpp"     // my::failure_handling::*, FAIL

#include <exception>        // std::current_exception
#include <iostream>         // std::(cin, cout, endl)
#include <string>           // std::(getline, stoi, string)

namespace my::stream_io {
    namespace fh = failure_handling;
    using   fh::hopefully, fh::Possible_result_;
    using   std::current_exception,                     // <exception>
            std::cin, std::cout,                        // <iostream>
            std::getline, std::stoi, std::string;       // <string>

    inline auto to_int( const string& s, const int base = 10 )
        -> int
    {
        size_t n_chars_consumed = 0;
        const int result = stoi( s, &n_chars_consumed, base );
        hopefully( n_chars_consumed == s.size() )
            or FAIL( "Not all chars used." );
        return result;
    }

    inline auto input( const string& prompt = "" ) -> string
    {
        string line;
        if( prompt != "" ) { cout << prompt; }
        getline( cin, line )
            or FAIL( "getline(cin) failed" );
        return line;
    }
    
    inline auto input_int_or_x(  const string& prompt = "" )
        -> int
    { return to_int( input( prompt ) ); }
    
    inline auto input_int(  const string& prompt = "" )
        -> Possible_result_<int>
    {
        try {
            return to_int( input( prompt ) );
        } catch( ... ) {
            return current_exception();
        }
    }
}  // namespace my::stream_io
