#include <kickstart/all.hpp>        // https://github.com/alf-p-steinbach/kickstart
using namespace kickstart::all;

#include <algorithm>
#include <functional>
using   std::sort,
        std::reference_wrapper;

#define RANGE_OF( c ) std::begin( c ), std::end( c )

struct Student
{
    string      name;
    int         birth_year;
    
    static auto ordered_by_name( const Student& a, const Student& b )
        -> bool
    { return (a.name < b.name); }

    static auto ordered_by_birth_year( const Student& a, const Student& b )
        -> bool
    { return (a.birth_year < b.birth_year); }
};

template< class Order_func >
void display( const vector<Student>& students, const Order_func order )
{
    vector<reference_wrapper<const Student>> ordered_students;

    for( const Student& student: students ) {
        ordered_students.push_back( student );
    }
    sort( RANGE_OF( ordered_students ), order );
    for( const auto student_ref: ordered_students ) {
        out << student_ref.get().name << " " << student_ref.get().birth_year
            << endl;
    }
}

auto main() -> int
{
    const vector<Student> students =
    {
        {"B", 2004 }, { "C", 2002 }, { "A", 2005 }, { "E", 2001 }, { "D", 2003 }
    };
    display( students, &Student::ordered_by_name );
    out << endl;
    display( students, &Student::ordered_by_birth_year );
}
