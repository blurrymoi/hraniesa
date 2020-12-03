using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Numerics; //BigInteger

// mcs -debug -r:System.Numerics.dll -out:03.exe 03.cs
// mono 03.exe

using TreeList = System.Collections.Generic.List< System.Collections.Generic.List< bool > >;

class Trees
{
    static List< bool > load_row( string s )
    {
        List< bool > trees = new List< bool >();
        foreach ( char c in s )
            trees.Add( c == '#' );
        return trees;
    }

    static int check_line( ref TreeList trees, int x, int y )
    {
        int x_ = 0, y_ = 0;
        int trees_hit = 0;
        while( y_ < trees.Count )
        {
            if ( trees[y_][x_] )
                trees_hit++;

            x_ = ( x_ + x ) % (trees[0].Count);
            y_ += y;
        }
        return trees_hit;
    }

    static void Main()
    {
        StreamReader file = new StreamReader( "03.txt" );
        string line;
        var tree_coords = new TreeList();

        while( ( line = file.ReadLine() ) != null )
            tree_coords.Add( load_row( line ) );
        
        file.Close();

        Console.WriteLine( "Hit " + check_line( ref tree_coords, 3, 1 ) + " trees (:" );

        var slopes = new List< ( int, int ) > { (1,1),(3,1),(5,1),(7,1),(1,2) };
        
        var trees_hit =
                slopes
                .Select( t => new BigInteger( check_line( ref tree_coords, t.Item1, t.Item2 ) ) )
                .Aggregate( ( x, y ) => x * y );

        Console.WriteLine( "Hit " + trees_hit  + " trees altogether." );
    }
}

