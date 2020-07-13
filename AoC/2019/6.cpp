#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cassert>

// COM     -- center of mass
// AAA)BBB -- BBB orbits AAA


/*

COM)B
B)C
C)D
D)E
E)F
B)G
G)H
D)I
E)J
J)K
K)L



        G - H       J - K - L
       /           /
COM - B - C - D - E - F
               \
                I

*/

struct OrbitMap
{
    std::map< std::string, std::string > orbits;

    int count_to_origin( std::string s )
    {
        int count = 0;
        while( s != "COM" )
        {
            s = orbits[ s ];
            count++;
        }
        return count;
    }

    void add( std::string key, std::string val )
    {
        orbits[ key ] = val;
    }
};

OrbitMap parse( const char* file )
{
    OrbitMap orbm;
    std::ifstream f( file );
    std::string s;

    while( std::getline( f, s ) )
    {
        std::size_t pos = s.find( ")" );
        orbm.add( s.substr( pos + 1, s.length() ), s.substr( 0, pos ) );
    }
    return orbm;
}

int count_orbits( const char* file )
{
    OrbitMap orbm = parse( file );
    
    int count = 0;
    for( auto [key, val] : orbm.orbits )
        count += orbm.count_to_origin( key );

    return count;
}

int main()
{
    assert( count_orbits( "6-test.txt" ) == 42 );
    std::cout << count_orbits( "6.txt" ) << std::endl;
}
