#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <cmath>

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
            return { true, 1 };
        case 4:
            in = *at( v, pos, 1, modes );
            std::cout << "OUTPUT: " << in << std::endl;
            return { true, 1 };

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

void run( vec_int& v )
{
    int pos = 0;
    int inout = 5;

    std::pair< bool, int > result = perform_op( v, pos, inout );

    while( result.first ) 
    {
        if( result.second != 0 )
            pos += result.second + 1;
        result = perform_op( v, pos, inout );
    }
}

int main()
{
    std::vector< int > v;
    parse_code( "5.txt", v );
    run( v );

    return 0;
}

