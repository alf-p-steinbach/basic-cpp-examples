#include <string>           // std::string
#include <stdexcept>        // std::runtime_error

#define FAIL( s ) my::failure_handling::fail( std::string() + __func__ + " - " + s )

namespace my::failure_handling {
    using   std::string,                // <string>
            std::runtime_error;         // <stdexcept>

    inline auto hopefully( const bool expr ) -> bool { return expr; }
    inline auto fail( const string& s ) -> bool { throw runtime_error( s ); }

}  // namespace my::failure_handling
