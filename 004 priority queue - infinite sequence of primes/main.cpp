#include <kickstart/all.hpp>
using namespace kickstart::all;

#include <queue>
#include <vector>

template< class Item, class Compare_func >
using Priority_q = std::priority_queue<Item, std::vector<Item>, Compare_func>;

struct Multiple
{
    int base; int n;
    auto value() const -> int{ return n*base; }
};
    
struct Descending_order
{
    auto operator()( const Multiple& a, const Multiple& b ) const
        -> bool
    { return a.value() > b.value(); }       // Note: direction of comparison.
};

auto main() -> int
{
    out << 2;
    auto larger_composites = Priority_q<Multiple, Descending_order>();
    larger_composites.push( Multiple{2, 2} );
    for( int current = 3; /*current < 100*/; ++current ) {
        bool is_composite = false;
        for( ;; ) {
            const Multiple next_composite = larger_composites.top();
            if( current < next_composite.value() ) {
                break;
            }
            larger_composites.pop();
            is_composite = true;
            larger_composites.push( Multiple{next_composite.base, 1 + next_composite.n} );
        }
        if( not is_composite ) {
            out << ' ' << current;
            larger_composites.push( Multiple{current, 2} );
        }
    }
    /*out << endl;*/
}
