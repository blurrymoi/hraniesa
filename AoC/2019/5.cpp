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
std::pair< bool, int > perform_op( vec_int& v, unsigned pos, int& in )
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
      
        default:
            return { false, 0 };
    }
}

void run( vec_int& v )
{
    int pos = 0;
    int inout = 1;

    std::pair< bool, int > result = perform_op( v, pos, inout );

    while( result.first ) 
    {
        pos += result.second + 1;
        result = perform_op( v, pos, inout );
    }
}

int main()
{
    std::vector< int > v;
    parse_code( "5.txt", v );
    //print( v );

    run( v );
    //print( v );

    return 0;
}

