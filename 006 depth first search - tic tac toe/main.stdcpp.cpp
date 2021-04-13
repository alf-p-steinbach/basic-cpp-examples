#include <assert.h>

#include <array>
#include <optional>

template< class Integral_or_enum >
auto successor( const Integral_or_enum v, const int delta = 1 )
    -> Integral_or_enum
{ return Integral_or_enum( v + delta ); }

template< class Integral_or_enum >
void advance( Integral_or_enum& v, const int delta = 1 ) { v = successor( v, delta ); }

namespace ttt {
    using std::array, std::optional;

    struct Symbol       { enum Enum{ none, x, o, N }; };
    struct Player_id    { enum Enum{ none, user, machine, N }; };
    constexpr Symbol::Enum player_symbols[] = {Symbol::none, Symbol::x, Symbol::o};
    
    constexpr auto opposite_of( const Player_id::Enum player_id )
        -> Player_id::Enum
    { return Player_id::Enum( (Player_id::N - player_id) % Player_id::N ); }

    constexpr int size = 3;
    using Board = array<Symbol::Enum, size*size>;
    enum Position: int {};

    struct Directions{ enum Enum{ horizontal, vertical, down_right, down_left, N }; };
    constexpr int step_sizes[] = { 1, size, size + 1, size - 1 };
    constexpr auto x_of( const Position pos ) -> int { return pos % size; }
    constexpr auto y_of( const Position pos ) -> int { return pos / size; }
    constexpr auto pos_of( const int x, const int y ) -> Position { return Position( y*size + x ); }

    auto is_on_full_line( const Position pos, const Directions::Enum direction )
        -> bool
    {
        const int   pos_x   = x_of( pos );
        const int   pos_y   = y_of( pos );
        const bool  ungood_diagonal =
            (direction == Directions::down_right and pos_x != pos_y) or
            (direction == Directions::down_left and pos_x != size - 1 - pos_y);

        return not ungood_diagonal;
    }
        
    auto start_pos_for_line( const Position pos, const Directions::Enum direction )
        -> Position
    {
        assert( is_on_full_line( pos, direction ) );
        Position result = pos;
        switch( direction ) {
            case Directions::horizontal:
            case Directions::down_right: {
                while( x_of( result ) != 0 ) {
                    advance( result, -step_sizes[direction] );
                }
                break;
            }
            case Directions::vertical:
            case Directions::down_left: {
                while( y_of( result ) != 0 ) {
                    advance( result, -step_sizes[direction] );
                }
                break;
            }
            case Directions::N: { assert( false ); }    // Suppresses a warning.
        }
        return result;
    }

    class State
    {
        int    m_n_moves;
        Board  m_board;

    public:
        State(): m_n_moves(), m_board() {}

        auto n_moves() const -> int { return m_n_moves; }
        
        auto cell_value( const Position pos ) const
            -> Symbol::Enum
        { return m_board[pos]; }

        void set_cell_value( const Position pos, Symbol::Enum value )
        {
            const Symbol::Enum old_value = m_board[pos];
            if( old_value == Symbol::none ) {
                m_n_moves += (value != Symbol::none);
            } else {
                m_n_moves -= (value == Symbol::none);
            }
            m_board[pos] = value;
        }
    };

    auto is_win_at( const Position pos, const State& state )
        -> bool
    {
        const Symbol::Enum symbol = state.cell_value( pos );
        if( symbol != Symbol::none ) {
            for( auto dir = Directions::horizontal; dir < Directions::N; advance( dir ) ) {
                if( is_on_full_line( pos, dir ) ) {
                    int count = 0;
                    Position pos_in_line = start_pos_for_line( pos, dir );
                    for( int i = 1; i <= size; ++i ) {
                        count += (state.cell_value( pos_in_line ) == symbol);
                        advance( pos_in_line, step_sizes[dir] );
                    }
                    if( count == size ) { return true; }
                }
            }
        }
        return false;
    }
    
    struct Scored_move { Position pos; int score; };

    auto best_move_for( const Player_id::Enum player, State& state )
        -> Scored_move
    {
        assert( state.n_moves() < size*size );
        auto result = Scored_move{ Position( -1 ), -999 };
        for( auto pos = Position( 0 ); pos < size*size; advance( pos ) ) {
            if( state.cell_value( pos ) == Symbol::none ) {
                state.set_cell_value( pos, player_symbols[player] );
                if( is_win_at( pos, state ) ) {
                    result = Scored_move{ pos, 1 };
                } else if( state.n_moves() == size*size ) {
                    result = Scored_move{ pos, 0 };
                } else {
                    const Player_id::Enum opponent = opposite_of( player );
                    // “Nega-max” logic:
                    const int best_counter_score = best_move_for( opponent, state ).score;
                    if( -best_counter_score > result.score ) {
                        result = Scored_move{ pos, -best_counter_score };
                    }
                }
                state.set_cell_value( pos, Symbol::none );
            }
        }
        assert( result.score >= -1 );
        return result;
    }
}  // namespace ttt

#include <string>
#include <string_view>
#include <vector>
using   std::string,
        std::string_view,
        std::vector;

#include <iostream>
#include <stdexcept>

#define FAIL( s ) chill::fail( std::string() + __func__ + " - " + s )

namespace chill {
    using   std::cin, std::cout, std::getline,
            std::runtime_error;

    using Byte = unsigned char;

    auto is_ascii_space( const Byte code )
        -> bool
    { return (code < 128 and !!::isspace( code )); }

    auto ascii_suffixtrimmed_view( const string_view& s )
        -> string_view
    {
        const auto n = (int) s.size();
        int i = n;
        while( i > 0 and is_ascii_space( s[i - 1] ) ) { --i; }
        return string_view( s.data(), i );
    }

    auto ascii_prefixtrimmed_view( const string_view& s )
        -> string_view
    {
        const auto n = (int) s.size();
        int i = 0;
        while( i < n and is_ascii_space( s[i] ) ) { ++i; }
        return string_view( s.data() + i, n - i );
    }
    
    auto ascii_trimmed_view( const string_view& s )
        -> string_view
    { return ascii_prefixtrimmed_view( ascii_suffixtrimmed_view( s ) ); }
    
    auto ascii_trimmed( const string& s )
        -> string
    { return string( ascii_trimmed_view( s ) ); }
    
    auto fail( const string& s )
        -> bool
    { throw runtime_error( s ); }

    auto input( const string_view& prompt )
        -> string
    {
        // Note: this does not support UTF-8 in Windows, but OK for this app.
        string result;
        cout << prompt;
        getline( cin, result ) or FAIL( "getline(cin) failed" );
        return result;
    }
}  // namespace chill

struct State_display
{
    vector<string>  m_lines;
    
    static void write_at_col( const int col, string& s, const string_view& t )
    {
        s.replace( col, t.size(), t );
    }

public:
    static constexpr string_view cell_chars[] = { ".", "x", "o" };

    State_display(): m_lines( 5, string( 128, ' ' ) ) {}
    
    void put_at_column( const int col, const string_view& legend, const ttt::State& state )
    {
        write_at_col( col, m_lines[0], legend );
        for( int y = 0; y < ttt::size; ++y ) {
            string line;
            for( int x = 0; x < ttt::size; ++x ) {
                if( x > 0 ) { line += ' '; }
                const ttt::Symbol::Enum symbol = state.cell_value( ttt::pos_of( x, y ) );
                line += cell_chars[symbol];
            }
            write_at_col( col, m_lines[y + 2], line );
        }
    }
    
    void put_move_specs_at_column( const int col )
    {
        write_at_col( col, m_lines[0], "Moves:" );
        for( int y = 0; y < ttt::size; ++y ) {
            string line;
            for( int x = 0; x < ttt::size; ++x ) {
                if( x > 0 ) { line += ' '; }
                line += "789456123"[ttt::pos_of( x, y )];
            }
            write_at_col( col, m_lines[y + 2], line );
        }
    }

    auto to_string() const
        -> string
    {
        string result;
        for( const string& line: m_lines ) {
            if( &line != &m_lines[0] ) { result += '\n'; }
            result += chill::ascii_suffixtrimmed_view( line );
        }
        return result;
    }
};

auto input_valid_move_spec()
    -> char
{
    using std::cout, std::endl;

    for( ;; ) {
        const string result = chill::ascii_trimmed( chill::input( "Your move, please? " ) );
        if( result.size() == 1 ) {
            const char c = result[0];
            if( '1' <= c and c <= '9' ) { return c; }
        }
        cout << "Sorry, that’s not a valid move spec. Type a digit 1 through 9." << endl;
        cout << endl;
    }
}

auto position_from_move_spec( char const spec )
    -> ttt::Position
{
    const int i = spec - '1';
    const int x = i % ttt::size;
    const int y = i / ttt::size;
    return ttt::pos_of( x, ttt::size - 1 - y ); // Upside down compared to numeric keypad.
}

auto move_spec_from_position( const ttt::Position pos )
    -> char
{
    const int i = ttt::pos_of( x_of( pos ), ttt::size - 1 - ttt::y_of( pos ) );
    return char( '1' + i );
}

void welcome_message()
{
    using std::cout, std::endl;

    constexpr string_view x_sym = State_display::cell_chars[ttt::Symbol::x];
    constexpr string_view o_sym = State_display::cell_chars[ttt::Symbol::o];
    const string welcome = string()
        + "Welcome to the Las Vegas You-Can’t-Win Tic-Tac-Toe game. 😃\n"
        + "You have ‘" + string( x_sym ) + "’, and the computer (well, this program,"
           " me!) has ‘" + string( o_sym ) + "’.\n"
        + "\n"
        + "Let’s begin — it’s your turn!";
    cout << welcome << endl;
    cout << endl;
}

void closing_message( const ttt::Player_id::Enum winner )
{
    using std::cout, std::endl;

    switch( winner ) {
        case ttt::Player_id::none: {
            cout << "Thanks for the game! It’s a tie. 😃" << endl;
            break;
        }
        case ttt::Player_id::user: {
            cout << "Congratulations, you won! 😃" << endl;
            cout << "But that means that there’s a bug in me… 😮" << endl;
            assert( false );
            break;
        }
        case ttt::Player_id::machine: {
            cout << "Thanks for the game! I won. 😃" << endl;
            break;
        }
        case ttt::Player_id::N: { assert( false ); }
    }
}

#ifdef _WIN32   // Any Windows system.
#   include <stdlib.h>      // The `system` function.
#endif
    
auto main() -> int
{
    using namespace ttt;
    using namespace std::string_literals;       // ""s notation
    using std::cout, std::endl;

    constexpr int ttt_size = ttt::size;

    constexpr int   moves_col       = 4;
    constexpr int   state_col_1     = 30;
    constexpr int   state_col_2     = 44;

    State                       state;
    State_display               display;
    optional<Player_id::Enum>   winner;

    const auto check_game_over = [&]( const Player_id::Enum last_player, const Position pos )
    {
        assert( state.cell_value( pos ) == player_symbols[last_player] );
        if( is_win_at( pos, state ) ) {
            winner = last_player;
        } else if( state.n_moves() == ttt_size*ttt_size ) {
            winner = Player_id::none;
        }
    };

    #ifdef _WIN32  // Any Windows system.
        system( "chcp 65001" );     // Let the console assume UTF-8 encoding for output.
        // Ideally the console should also be reset to original encoding at the end, but.
    #endif
    display.put_move_specs_at_column( moves_col );
    display.put_at_column( state_col_1, "New game:", state );
    welcome_message();
    for( ;; ) {
        cout << display.to_string() << endl;
        cout << endl;
        if( winner ) {
            closing_message( winner.value() );
            return (winner.value() != Player_id::user? EXIT_SUCCESS : EXIT_FAILURE);
        }
        const char move_pos_spec = input_valid_move_spec();
        const Position move_pos = position_from_move_spec( move_pos_spec );
        if( state.cell_value( move_pos ) != Symbol::none ) {
            cout << "Sorry, that position is not available." << endl;
        } else {
            state.set_cell_value( move_pos, player_symbols[Player_id::user] );
            display = {};
            display.put_move_specs_at_column( moves_col );
            display.put_at_column( state_col_1, "Your "s + move_pos_spec + ":", state );
            check_game_over( Player_id::user, move_pos );
            if( not winner ) {
                const Position counter_move_pos = best_move_for( Player_id::machine, state ).pos;
                state.set_cell_value( counter_move_pos, player_symbols[Player_id::machine] );
                const char counter_move_pos_spec = move_spec_from_position( counter_move_pos );
                display.put_at_column( state_col_2, "My "s + counter_move_pos_spec + ":", state );
                check_game_over( Player_id::machine, counter_move_pos );
            }
        }
        cout << endl;
    }
}
