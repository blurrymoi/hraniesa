using System;
using System.IO;
using System.Collections.Generic;

class Sum
{
    static int hash( int i )
    {
        return i % 17;
    }

    static int str_to_int( String s )
    {
        int i = -1;
        try
        {
            i = Int32.Parse( s );
        }
        catch( FormatException )
        {
            Console.WriteLine( $"Cannot parse {s}" );
        }
        return i;
    }

    static bool check_opposite( int i, int sum, ref Dictionary< int, List< int > > hs )
    {
        int opp = sum - i;
        return ( hs.ContainsKey( hash( opp ) ) && hs[ hash( opp ) ].Contains( opp ) );
    }

    static void Main()
    {
        StreamReader file = new StreamReader( "01.txt" );
        string line;
        Dictionary< int, List< int > > hs = new Dictionary< int, List< int > >();

        while( ( line = file.ReadLine() ) != null )
        {
            int line_i = str_to_int( line );
            int key = hash( line_i );

            if ( ! hs.ContainsKey( key ) )
                hs[ key ] = new List< int >();
            hs[ key ].Add( line_i );

            /*
                if( check_opposite( line_i, 2020, ref hs ) )
                {
                    Console.WriteLine( "Has a twin! " + line_i + " " + ( 2020 - line_i ) );
                    Console.WriteLine( line_i * ( 2020 - line_i ) );
                }
            */
            foreach( int k in hs.Keys )
            {
                foreach( int i in hs[ k ] )
                    if( check_opposite( i, 2020 - line_i, ref hs ) )
                    {
                        Console.WriteLine( "Has a t..triplet? " + line_i + " "
                                           + i + " " + ( 2020 - line_i - i ) );
                        Console.WriteLine( line_i * i * ( 2020 - line_i - i ) );
                        return;
                    }
            }
        }

        file.Close();
    }
}

