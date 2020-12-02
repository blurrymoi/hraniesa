using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;

public class Limit
{
    public int min;
    public int max;
    public char c;
    public string word;

    public bool legit()
    {
        int count = word.Count( c => c == this.c );
        return count >= min && count <= max;
    }

    public bool sled_legit()
    {
        char c1 = word[ min - 1 ];
        if( max > word.Length )
            return c1 == c;
        char c2 = word[ max - 1 ];
        return c1 == c ^ c2 == c;
    }
}

class Sum
{
    static int str_to_int( string s )
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

    static void Main()
    {
        StreamReader file = new StreamReader( "02.txt" );
        string line;
        List< Limit > pass = new List< Limit >();
        int count_correct = 0;
        int count_new = 0;

        while( ( line = file.ReadLine() ) != null && line != "" )
        {
            Limit lim = new Limit();
            string[] words = line.Split(':');
            lim.word = words[1].Trim();

            string[] limits = words[0].Split( ' ' );
            lim.c = Convert.ToChar( limits[1] );
            lim.min = str_to_int( limits[0].Split( '-' )[0] );
            lim.max = str_to_int( limits[0].Split( '-' )[1] );
            pass.Add( lim );
        }
        
        file.Close();

        foreach ( var lim in pass )
        {
            Console.WriteLine( $"{lim.min} {lim.max} {lim.c} {lim.word}" );
            if ( lim.legit() )
                count_correct += 1;
            if( lim.sled_legit() )
                count_new += 1;
        }
        Console.WriteLine( count_correct );
        Console.WriteLine( count_new );
    }
}

