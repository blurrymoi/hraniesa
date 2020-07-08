#include <iostream>
#include <sstream>
#include <fstream>
#include <map> //multimap
#include <string>
#include <vector>
#include <cassert>

using point = std::pair< int, int >;
using std::abs;

int distance( point a )
{
    return abs( a.first ) + abs( a.second );
}

struct wire
{
    // x -> y
    std::multimap< int, int > coords;

    void add( int x, int y )
    {
        coords.emplace( x, y );
    }
};

std::pair< int, int > direction( char dir )
{
    switch( dir )
    {
        case 'R': return {  1,  0 };
        case 'L': return { -1,  0 };
        case 'U': return {  0,  1 };
        case 'D': return {  0, -1 };
    }
}

// R72 -> { { 1,0 }, 72 }
std::pair< point, int > parse_line( std::string prompt )
{
    point dir = direction( prompt[ 0 ] );
    prompt.erase( 0, 1 );
    return { dir, std::stoi( prompt ) };
}

void append_coord_line( point& from, std::string prompt, wire& w )
{
    auto [ dir, dist ] = parse_line( prompt );

    int &x = from.first;
    int &y = from.second;

    for( int i = 0; i < dist; i++ )
    {
        x += dir.first;
        y += dir.second;
        w.add( x, y );
    }
}

bool crosses( point a, wire& w )
{
    if( w.coords.find( a.first ) != w.coords.end() )
    {
        auto range = w.coords.equal_range( a.first );
        for( auto it = range.first; it != range.second; ++it )
            if( (*it).second == a.second )
                return true;
    }
    return false;
}

std::pair< std::string, std::string > read( const char* file )
{
    std::ifstream f( file );
    std::pair< std::string, std::string > wires;
    std::string s;

    std::getline( f, s );
    wires.first = s;

    std::getline( f, s );
    wires.second = s;

    return wires;
}

std::vector< std::string > split( std::string s )
{
    std::vector< std::string > coords;
    std::string c;

    std::istringstream ss( s );
    while( std::getline( ss, c, ',' ) )
        coords.push_back( c );

    return coords;
}

int closest_distance( std::string wire1, std::string wire2 )
{
    std::vector< std::string > coords = split( wire1 );
    wire w1;
    point origin( 0, 0 );

    for( auto line : coords )
        append_coord_line( origin, line, w1 );

    int x = 0, y = 0;
    int res_dist = -1;
    std::vector< std::string > w2 = split( wire2 );
    for( auto line : w2 )
    {
        auto [ dir, dist ] = parse_line( line );
        for( int i = 0; i < dist; i++ )
        {
            x += dir.first;
            y += dir.second;
            if( crosses( {x,y}, w1 ) )
            {
                int d = distance( {x,y} );
                res_dist = res_dist == -1? d : d < res_dist? d : res_dist;
            }
        }
    }
    return res_dist;
}

void test();

int main()
{
    auto [ w1, w2 ] = read( "3.txt" );

    std::cout << closest_distance( w1, w2 ) << std::endl;
    //test();
}

void test()
{
    std::string w1 = "R8,U5,L5,D3";
    std::string w2 = "U7,R6,D4,L4";
    //std::cout << closest_distance( w1, w2 ) << std::endl;
    assert( closest_distance( w1, w2 ) == 6 );

    std::string w3 = "R75,D30,R83,U83,L12,D49,R71,U7,L72";
    std::string w4 = "U62,R66,U55,R34,D71,R55,D58,R83";
    assert( closest_distance( w3, w4 ) == 159 );

    std::string w5 = "R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51";
    std::string w6 = "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7";
    assert( closest_distance( w5, w6 ) == 135 );
}

