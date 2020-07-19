#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <cmath>
#include <algorithm>

using vec_int = std::vector< int >;

void parse_code( const char* filename, vec_int& v )
{
    std::ifstream f( filename );
    int i;
    char c;

    while( f >> i >> c )
    {
        v.push_back( i );

        if( c == '\n' )
            std::cerr << "newline" << std::endl;
        else if( c != ',' )
            std::cerr << "unknown char: " << c << std::endl;
    }

    assert( f.eof() );
    v.push_back( i );
}

void print( vec_int& v )
{
    for( int p : v )
        std::cout << p << " ";
    std::cout << std::endl;
}


vec_int::iterator at( vec_int& v, unsigned pos, int offset, int modes )
{
    int position = v[ pos + offset ];
    int mode = modes & ( int( std::pow( 10, ( offset-1 ) ) ) ) ? 1 : 0;

    // assert that write to is in positional mode
    if( offset == 3 ) assert( mode == 0 );

    // mode 0 = positional; 1 = immediate
    return mode == 0 ? v.begin() + position :
                       v.begin() + pos + offset;
}

// < success, steps/operands >
std::pair< bool, int > perform_op( vec_int& v, int& pos, int& in )
{
    int opcode = v[ pos ];
    int modes  = opcode / 100;
    opcode = opcode % 100;

    switch( opcode )
    {
        case 1:
            *at( v, pos, 3, modes ) = *at( v, pos, 1, modes ) + *at( v, pos, 2, modes );
            return { true, 3 };
        case 2:
            *at( v, pos, 3, modes ) = *at( v, pos, 1, modes ) * *at( v, pos, 2, modes );
            return { true, 3 };
        case 3:
            *at( v, pos, 1, modes ) = in;
            return { true, 100 };
        case 4:
            in = *at( v, pos, 1, modes );
            std::cout << "OUTPUT: " << in << std::endl;
            return { true, 200 };

        // jump-if-true, jump-if-false
        case 5:
        case 6:
         {
            int cond = *at( v, pos, 1, modes );
            if( (opcode == 5 && cond != 0) || (opcode == 6 && cond == 0) )
            {
                pos = *at( v, pos, 2, modes );
                return { true, 0 };
            }
            return { true, 2 }; 
         }

        // lesser than, equal
        case 7:
        case 8:
         {
            int i1 = *at( v, pos, 1, modes );
            int i2 = *at( v, pos, 2, modes );
            if( (opcode == 7 && i1 < i2) || (opcode == 8 && i1 == i2) )
                *at( v, pos, 3, modes ) = 1;
            else
                *at( v, pos, 3, modes ) = 0;
            return { true, 3 };
         }
        default:
            return { false, 0 };
    }
}

// <input, output>, position
std::pair< std::pair< int, int >, int > run( vec_int& v, int& input, int pos = 0 )
{
    int inout = input;

    std::pair< bool, int > result = perform_op( v, pos, inout );

    while( result.first ) 
    {
        if ( result.second == 100 ) // input was used, request next input
        {
            pos += 2;
            return { { 100, 0 }, pos };
        }
        else if( result.second == 200 ) // output was produced
        {
            pos += 2;
            return { { 200, inout }, pos };
        }

        else if( result.second != 0 )
            pos += result.second + 1;

        result = perform_op( v, pos, inout );
    }

    return { { 99, 0 }, pos }; // halted
}


class Amplifier
{
    std::vector< int > _code;
    int pos = 0;
    int _input;
    int _output;

  public:
    bool halted = false;

    Amplifier( vec_int v, int phase, int signal )
      : _code( v ), _input( phase )
    {
        auto ret = run( _code, _input );
        pos = ret.second;
        resume_execution( signal );
    }

    void resume_execution( int input )
    {
        _input = input;

        auto ret = run( _code, _input, pos );
        if( ret.first.first == 100 )
            ret = run( _code, _input, ret.second );

        assert( ret.first.first != 100 ); // output or halt
        _output = ret.first.second;
        pos = ret.second;
        if( ret.first.first == 99 )
            halted = true;
    }

    int output() { return _output; }
};


int main()
{
    vec_int vec_orig;
    parse_code( "7.txt", vec_orig );
    int signal = 0;
    int max = -1;

    vec_int phases { 5, 6, 7, 8, 9 };
    do {
        print( phases );
        signal = 0;
        std::vector< Amplifier > amps;

        for( int i : phases )
        {
            if( i != phases[0] )
                signal = amps[ amps.size() - 1 ].output();
            amps.push_back( Amplifier( vec_orig, i, signal ) );
        }

        while( !amps[ 4 ].halted )
        {
            for( int i = 0; i < 5; ++i )
            {
                amps[ i ].resume_execution( amps[ ((i - 1) + 5) % 5 ].output() );
                if( amps[ i ].halted )
                {
                    amps[ 4 ].halted = true;
                    break;
                }
            }
        }

        signal = amps[ 4 ].output();
        if( max == -1 || signal > max ) max = signal;

    } while( std::next_permutation( phases.begin(), phases.end() ) );

    std::cout << "MAX: " << max << std::endl;

    return 0;
}

