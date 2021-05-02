#ifdef __MINGW32__
#   if __GNUC__ < 10 && !defined( _GLIBCXX_USE_RANDOM_TR1 )
#       // With MinGW and no _GLIBCXX_USE_RANDOM_TR1 one risks getting the same sequence always.
#       pragma GCC warning "_GLIBCXX_USE_RANDOM_TR1 should be defined for use of std::random_device"
#   endif    
#endif

#include <random>
#include <iostream>
using   std::random_device, std::mt19937, std::uniform_int_distribution,
        std::cout, std::endl;
 
auto main() -> int
{
    auto entropy = random_device(); // Provides a really random seed for the random number engine.
    auto bits = mt19937( entropy() );  // Standard mersenne_twister_engine pseudo random sequence.
    auto number_from = uniform_int_distribution( 10, 49 );     // Inclusive limits.
 
    for( int i = 0; i < 10; ++i ) {
        if( i > 0 ) { cout << ", "; }
        cout << number_from( bits );
    }
    cout << "." << endl;
}
