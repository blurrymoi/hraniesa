#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

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


vec_int::iterator at( vec_int& v, unsigned pos, int offset )
{
    int position = v[ pos + offset ];
    return v.begin() + position;
}

bool perform_op( vec_int& v, unsigned pos )
{
    int opcode = v[ pos ];

    switch( opcode )
    {
        case 1:
            *at( v, pos, 3 ) = *at( v, pos, 1 ) + *at( v, pos, 2 );
            break;
        case 2:
            *at( v, pos, 3 ) = *at( v, pos, 1 ) * *at( v, pos, 2 );
            break;

        default:
            return false;
    }
    return true;
}

void run( vec_int& v )
{
    int pos = 0;
    
    while( perform_op( v, pos ) )
        pos += 4;

    //assert( v[ pos ] == 99 );

    if( v[ pos ] != 99 )
        std::cerr << "unknown opcode: " << v[ pos ] << std::endl;
    else
        std::cerr << "success" << std::endl;
}


#include  "2-test.cpp"

int main()
{
    test();

    std::vector< int > v;
    parse_code( "2.txt", v );

    v[ 1 ] = 12;
    v[ 2 ] = 2;

    run( v );
    print( v );
}


