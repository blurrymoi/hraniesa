#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cassert>

int mass( int i )
{
    return i / 3 - 2;
}

int main()
{
    std::ifstream f( "1-mass.txt" );

    std::string s;
    unsigned res = 0;
    while( std::getline( f, s ) )
    {
        int m = mass( std::atoi( s.c_str() ) );
        while( m > 0 )
        {
            res += m;
            m = mass( m );
        }
    }

    assert( mass(12) == 2 );
    assert( mass(14) == 2 );
    assert( mass(1969) == 654 );
    assert( mass(100756) == 33583 );
    std::cout << res << std::endl;
}
