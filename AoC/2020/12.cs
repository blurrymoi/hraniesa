using System;
using System.IO;
using System.Collections.Generic;

enum Dir { North = 1, East = 2, South = 3, West = 4 }

class Position
{
    public int x,y;
    public Dir facing;

    public Position()
    {
        x = y = 0;
        facing = Dir.East;
    }

    public Dir get_dir( char c )
    {
        var dirs = new Dictionary< char, Dir >
                   { ['N'] = Dir.North, ['S'] = Dir.South,
                     ['E'] = Dir.East, ['W'] = Dir.West };
        return dirs[c];
    }

    public void move( Dir d, int steps, bool part2 = false )
    {
        var moves = new Dictionary< Dir, Tuple< char, int > >
        { [ Dir.North ] = ( 'y', 1 ).ToTuple(),
          [ Dir.South ] = ( 'y', -1 ).ToTuple(),
          [ Dir.East ] =  ( 'x' ,1 ).ToTuple(),
          [ Dir.West ] =  ( 'x', -1 ).ToTuple() };
        if ( moves[d].Item1 == 'x' )
            this.x += steps * moves[d].Item2;
        else
            this.y += steps * moves[d].Item2;
    }

    public void turn( char dir, int degrees )
    {
        if( dir == 'F' )
        {
            move( facing, degrees );
            return;
        }
    
        if( dir == 'L' )
            facing = (Dir) ( (( (int)facing -  degrees / 90 ) + 4 ) % 4 );
        if( dir == 'R' )
            facing = (Dir) ( ( (int)facing +  degrees / 90 ) % 4 );
        if ( facing == 0 )
            facing = Dir.West; // % 4
    }

    public void turn_way( char dir, int degrees )
    {
        if( degrees == 90 && dir == 'L' )
        {
            int new_x = y;
            int new_y = x;
            x = - new_x;
            y = new_y;
        }
        else
        Console.WriteLine( "Non conforming degrees or direction." );
    }
}

class Steering
{
    static void Main()
    {
        StreamReader file = new StreamReader( "12.txt" );
        string line;

        // part 1
        Position p = new Position();
        // part 2
        Position ship = new Position();
        Position waypoint = new Position();
        waypoint.x = 10;
        waypoint.y = 1;
        //waypoint.facing = Dir.East; // who cares

        while( ( line = file.ReadLine() ) != null )
        {
            char d = line[0];
            int n = Int32.Parse( line.Substring( 1 ) );
            if( d == 'F' || d == 'L' || d == 'R' )
            {
                p.turn( d, n );

                // part 2
                if( d == 'F' )
                {
                    ship.x += n * waypoint.x;
                    ship.y += n * waypoint.y;
                }
                else
                {
                    if( d == 'R' )
                    {
                        n = 360 - n;
                        d = 'L';
                    }
                    // turn left by 90 degrees as many times as needed
                    for( int i = 1; i <= (n / 90); i++ )
                        waypoint.turn_way( d, 90 );
                }
            }
            else
            {
                p.move( p.get_dir( d ), n );
                waypoint.move( waypoint.get_dir( d ), n, true );
            }
        }

        file.Close();

        Console.WriteLine( $"#1: { Math.Abs( p.x ) + Math.Abs( p.y ) } " );
        Console.WriteLine( $"#2: { Math.Abs( ship.x ) + Math.Abs( ship.y ) } " );
    }
}

