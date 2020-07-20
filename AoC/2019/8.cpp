#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>

using Layer = std::vector< int >;

std::vector< Layer > parse_input( std::string file, size_t size )
{
    std::vector< Layer > v;
    Layer ly;

    std::ifstream f( file );
    char c;

    while( f >> c )
    {
        if( ly.size() == size )
        {
            v.push_back( ly );
            ly.clear();
        }
        ly.push_back( int( c - '0' ) );
    }

    if( !ly.empty() )
        v.push_back( ly );

    return v;
}

int count( const std::vector< int >& v, int what )
{
    return std::count( v.begin(), v.end(), what );
}

Layer stack_layers( std::vector< Layer >& v )
{
    Layer result;

    for( int i = 0; i < 25*6; ++i )
    {
        for( size_t lay = 0; lay < v.size(); ++lay )
        {
            int pixel = v[lay][i];
            if( pixel == 0 || pixel == 1 )
            {
                result.push_back( pixel );
                break;
            }
        }
    }

    return result;
}

int main()
{
    std::vector< Layer > v = parse_input( "8.txt", 25*6 );
    const Layer * chosen = nullptr;
    int lowest = -1;

    for( const auto &l : v )
    {
        int zeros = count( l, 0 );
        if( !chosen || zeros < lowest )
        {
            chosen = &l;
            lowest = zeros;
        }
    }

    std::cout << "fewest zeroes " << lowest << std::endl;
    std::cout << count( *chosen, 1 ) * count( *chosen, 2 ) << std::endl;

    Layer res = stack_layers( v );

    for( size_t i = 0; i < res.size(); ++i )
    {
        std::cout << ( res[i] == 1? '1' : ' ' );
        if( (i + 1) % 25 == 0 )
            std::cout << std::endl;
    }
    std::cout << std::endl;
}
