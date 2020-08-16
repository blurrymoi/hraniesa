#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <set>
#include <map>
#include <cassert>


bool close_enough( double a, double b )
{
    return std::abs( a - b ) < 0.000001;
}

struct point
{
    double x;
    double y;

    point( int a, int b ) : x( a ), y( b ) {}

    double length()
    {
        return sqrt( x*x + y*y );
    }

    void normalize()
    {
        double l = length();
        x /= l;
        y /= l;
    }

    bool operator==( const point& o ) const
    {
        return close_enough( this->x, o.x ) &&
               close_enough( this->y, o.y );
    }

    /*      [0,-1]
     *  [-1,0]  [1,0] 
     *      [0,1]
     */

    int quadrant() const
    {
        if( x >= 0 && x <= 1 )
        {
            if( y < 0 ) return 1;
            return 2;
        }
        else
        {
            assert( x < 0 && x >= -1 );
            if( y > 0 ) return 3;
            return 4;
        }
    }

    bool operator<( const point& o ) const
    {
        if( *this == o ) return false;

        int quad  = this->quadrant();
        int oquad = o.quadrant();

        if( quad < oquad ) return true;
        if( oquad < quad ) return false;

        // same quadrant
        if( quad == 1 || quad == 2 )
            return this->y < o.y;
        else
            return this->y > o.y;
    }
};

void parse_input( std::string file, std::vector< point >& v )
{
    std::ifstream f( file );
    std::string s;

    int x = 0, y = 0;
    while( std::getline( f, s ) )
    {
        for( char c : s )
        {
            if( c == '#' )
                v.emplace_back( x, y );
            x++;
        }
        x = 0;
        y++;
    }
}

void place( std::vector< point >& v, point p, point origin )
{
    for( auto it = v.begin(); it != v.end(); ++it )
    {
        point inplace( it->x - origin.x, it->y - origin.y );
        point adding( p.x - origin.x, p.y - origin.y );
        if( adding.length() < inplace.length() )
        {
            v.insert( it, p );
            return;
        }
    }
    v.push_back( p );
}

void vaporize( std::map< point, std::vector< point > >& m )
{
    bool shot = true;
    int count = 1;

    for( size_t i = 0; shot; ++i )
    {
        shot = false;

        for( auto it = m.begin(); it != m.end(); ++it )
        {
            if( (*it).second.size() >= i + 1 )
            {
                std::cout << count << " vaporizing " << (*it).second[i].x << " " \
                          << (*it).second[i].y << std::endl;
                count++;
                shot = true;
            }
        }
    }
}

int main()
{
    std::vector< point > v;
    parse_input( "10.txt", v );

    std::map< point, std::vector< point > > vectors, vectors_temp;
    size_t max_asteroids = 0;

    for( point p : v )
    {
        vectors_temp.clear();
        for( point r : v )
        {
            if( p == r ) continue;
            point s( r.x - p.x, r.y - p.y ); // relative distance = vector
            s.normalize();

            if( vectors_temp.find( s ) == vectors_temp.end() )
                vectors_temp.insert( { s, { r } } );
            else // find its correct place by distance from origin
                place( (*vectors_temp.find( s )).second, r, p );
        }
        if( vectors_temp.size() > max_asteroids )
        {
            max_asteroids = vectors_temp.size();
            vectors = vectors_temp;
        }
    }

    vaporize( vectors );
    std::cout << max_asteroids << std::endl;
}

