#include "fail.hpp"

#include <exception>        // std::(exception_ptr, rethrow_exception)
#include <optional>         // std::optional
#include <utility>          // std::move, std::swap

namespace my::failure_handling {
    using   std::current_exception, std::exception_ptr, std::rethrow_exception, // <exception>
            std::optional,                          // <optional>
            std::string,                            // <string>
            std::exception, std::runtime_error,     // <stdexcept>
            std::exchange, std::move, std::swap;    // <utility>

    template< class Value >
    class Possible_result_
    {
        optional<Value>     m_value;
        exception_ptr       m_exception;

        auto class_invariant_holds() const -> bool { return (!!m_value + !!m_exception <= 1); }

        void attempt_specific_throw_if_no_value() const
        {
            if( m_exception ) { rethrow_exception( m_exception ); }
        }

    public:
        Possible_result_() = default;
        Possible_result_( Value v ): m_value( move( v ) ), m_exception() {}
        Possible_result_( exception_ptr e ): m_value(), m_exception( move( e ) ) {}

        Possible_result_( const exception& x ):
            Possible_result_()
        { try{ throw x; } catch( ... ) { m_exception = current_exception(); } }

        Possible_result_( const Possible_result_& other ) = default;
        Possible_result_( Possible_result_&& other ) = default;

        friend void swap( Possible_result_& a, Possible_result_& b ) noexcept
        {
            swap( a.m_value, b.m_value );
            swap( a.m_exception, b.m_exception );
        }

        auto operator=( const Possible_result_& other )
            -> Possible_result_&
        {
            Possible_result_ temp = other;
            swap( *this, temp );
            return *this;
        }

        auto operator=( Possible_result_&& other )
            -> Possible_result_&
        {
            swap( *this, other );
            return *this;
        }

        auto has_value() const -> bool { return m_value.has_value(); }
        explicit operator bool() const  { return has_value(); }

        auto value() const
            -> Value
        {
            attempt_specific_throw_if_no_value();
            return m_value.value();         // Throws if `m_value` is empty.
        }

        auto value_or( const Value& a_default ) const noexcept
            -> Value
        { return (has_value()? m_value.value() : a_default); }

        auto moved_value()
            -> Value
        {
            attempt_specific_throw_if_no_value();
            optional<Value> result = {};
            swap( result, m_value );
            return move( result.value() );  // Throws if `result` is empty.
        }

        auto moved_value_or( Value a_default )
            -> Value
        { return move( has_value()? moved_value() : a_default ); }
    };
}  // namespace my::failure_handling
