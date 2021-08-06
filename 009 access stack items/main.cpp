#include <stack>

namespace my {
    using std::stack;
    
    template< class Item >
    auto container_of( const stack<Item>& st )
        -> const auto&
    {
        struct Hacked: stack<Item>
        {
            using stack<Item>::c;
        };
        
        return st.*&Hacked::c;
    }
}  // namespace my

#include <iostream>
using   std::stack,
        std::cout, std::endl;

auto main() -> int
{
    stack<int>  st;
    for( const int v: {1, 2, 3, 4, 5} ) { st.push( v ); }

    // Inspect the stack:
    for( const int& v: my::container_of( st ) ) {
        cout << v << endl;
    }
}
