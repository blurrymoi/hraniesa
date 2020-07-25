#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <cmath>
#include <algorithm>

using vec_int = std::vector< long long >;

void parse_code( const char* filename, vec_int& v )
{
    std::ifstream f( filename );
    long long i;
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
    for( auto p : v )
        std::cout << p << " ";
    std::cout << std::endl;
}

vec_int::iterator at( vec_int& v, unsigned pos, int rel_base, int offset, int modes )
{
    int base = std::pow( 10, offset - 1 );
    int mode = ( modes / base ) % 10;

    // assert that write to is in positional mode
    if( offset == 3 ) assert( mode == 0 || mode == 2 );

    // mode 0 = positional; 1 = immediate; 2 = relative
    int position =  mode == 0 ? ( v[ pos + offset ] ) :
                    mode == 1 ? pos + offset :
                /* mode == 2 */ rel_base + v[ pos + offset ];

    if( position >= v.size() )
        v.resize( position + 1, 0 );
    return v.begin() + position;
}

// < success, steps/operands >
std::pair< bool, int > perform_op( vec_int& v, int& pos, int& rel, long long& in )
{
    int opcode = v[ pos ];
    int modes  = opcode / 100;
    opcode = opcode % 100;

    switch( opcode )
    {
        case 1:
            *at( v, pos, rel, 3, modes ) = *at( v, pos, rel, 1, modes )
                                         + *at( v, pos, rel, 2, modes );
            return { true, 3 };
        case 2:
            *at( v, pos, rel, 3, modes ) = *at( v, pos, rel, 1, modes )
                                         * *at( v, pos, rel, 2, modes );
            return { true, 3 };
        case 3:
            *at( v, pos, rel, 1, modes ) = in;
            return { true, 100 };
        case 4:
            in = *at( v, pos, rel, 1, modes );
            std::cout << "OUTPUT: " << in << std::endl;
            return { true, 200 };

        // jump-if-true, jump-if-false
        case 5:
        case 6:
         {
            int cond = *at( v, pos, rel, 1, modes );
            if( (opcode == 5 && cond != 0) || (opcode == 6 && cond == 0) )
            {
                pos = *at( v, pos, rel, 2, modes );
                return { true, 0 };
            }
            return { true, 2 }; 
         }

        // lesser than, equal
        case 7:
        case 8:
         {
            long long i1 = *at( v, pos, rel, 1, modes );
            long long i2 = *at( v, pos, rel, 2, modes );
            if( (opcode == 7 && i1 < i2) || (opcode == 8 && i1 == i2) )
                *at( v, pos, rel, 3, modes ) = 1;
            else
                *at( v, pos, rel, 3, modes ) = 0;
            return { true, 3 };
         }
        case 9:
            rel += *at( v, pos, rel, 1, modes );
            return { true, 1 };
        default:
            return { false, 0 };
    }
}

// <input, output>, position
std::pair< std::pair< long long, long long >, int > run( vec_int& v, long long& input, int pos = 0 )
{
    long long inout = input;
    int rel_base = 0;

    std::pair< bool, int > result = perform_op( v, pos, rel_base, inout );

    while( result.first ) 
    {
        if ( result.second == 100 ) // input was used, request next input
            pos += 2;
        else if( result.second == 200 ) // output was produced
            pos += 2;

        else if( result.second != 0 )
            pos += result.second + 1;

        result = perform_op( v, pos, rel_base, inout );
    }

    return { { 99, 0 }, pos }; // halted
}

int main()
{
    vec_int v;
    parse_code( "9.txt", v );

    long long input = 2;

    run( v, input );

    return 0;
}

