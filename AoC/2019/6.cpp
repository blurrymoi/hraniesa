#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
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

    int count_to( std::string s, std::string to )
    {
        int count = 0;
        while( s != to )
        {
            s = orbits[ s ];
            count++;
        }
        return count;
    }

    int count_to_origin( std::string s )
    {
        return count_to( s, "COM" );
    }

    std::vector< std::string > path( std::string s )
    {
        std::vector< std::string > res;
        while( s != "COM" )
        {
            res.push_back( s );
            s = orbits[ s ];
        }
        res.push_back( s );
        return res;
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

std::string first_common( std::vector< std::string >& v1, std::vector< std::string >& v2 )
{
    for( auto s : v1 )
        if( std::find( v2.begin(), v2.end(), s ) != v2.end() )
            return s;
    return "nil";
}

int main()
{
    assert( count_orbits( "6-test.txt" ) == 42 );
    std::cout << count_orbits( "6.txt" ) << std::endl;

    OrbitMap o = parse( "6.txt" );

    std::vector< std::string > path_YOU = o.path( "YOU" );
    std::vector< std::string > path_SAN = o.path( "SAN" );
    std::string common = first_common( path_YOU, path_SAN );
    std::cout << common << std::endl;

    std::cout << o.count_to( "YOU", common ) - 1 << " + " << o.count_to( "SAN", common ) - 1 << std::endl;
}
