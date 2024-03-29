using System;
using System.IO;
using System.Collections.Generic;
using System.Numerics; //BigInteger
using System.Linq;

// mcs -debug -r:System.Numerics.dll -out:14.exe 14.cs
// mono 14.exe

class Mask
{
    public BigInteger X_zero = 0;
    public BigInteger X_one = 0;

    /*
        mask == mask with X bits set to 0
        X_ones == mask with X bits set to 1
        the only bits wrong after ( n & X_ones ) are where there
            was '1' in mask and the number had a '0'. we find
            these and only these bits when we set X bits initially
            to 0, flip the whole mask and OR with the number -- then
            the only 00 => 0 bits were the 10 ones. so we flip the
            result, now the only 1s would be the missing bits that
            were supposed to be set, we OR with what we already have.
    */
    public BigInteger apply_mask( BigInteger n )
    {
        return ( n & X_one ) | ( ~( ~X_zero | n ) );
    }

    public List< string > get_addresses( string n, string mask )
    {
        List< string > res = new List< string >();

        if( mask == "" )
        {
            res.Add( "" );
            return res;
        }

        char mask_last = mask[ mask.Length - 1 ];
        string nn = n.Substring( 0, n.Length - 1 );
        string maskn = mask.Substring( 0, n.Length - 1 );

        switch( mask_last )
        {
            case( '1' ):
                foreach( string a in get_addresses( nn, maskn ) )
                    res.Add( a + '1' );
                break;
            case( '0' ):
                foreach( string a in get_addresses( nn, maskn ) )
                    res.Add( a + n[ n.Length - 1 ] );
                break;
            case( 'X' ):
                foreach( string a in get_addresses( nn, maskn ) )
                {
                    res.Add( a + '0' );
                    res.Add( a + '1' );
                }
                break;
        }
        return res;
    }
}

class MaskReader
{
    static void Main()
    {
        StreamReader file = new StreamReader( "14.txt" );
        string line;

        Mask m = new Mask();
        string mask = "";
        var mem = new Dictionary< int, BigInteger >();
        var mem2 = new Dictionary< string, BigInteger >();

        while( ( line = file.ReadLine() ) != null )
        {
            if( line.StartsWith( "mask" ) )
            {
                mask = line.Split( '=' )[1].Trim();
                m.X_zero = Convert.ToInt64( mask.Replace( "X", "0" ), 2 );
                m.X_one = Convert.ToInt64( mask.Replace( "X", "1" ), 2 );
            }
            else
            {
                string[] parts = line.Split( '=' );
                BigInteger n = BigInteger.Parse( parts[1].Trim() );
                int address = Convert.ToInt32( parts[0].Trim().Split('[')[1].Split(']')[0] );
                mem[ address ] = m.apply_mask( n );

                string ad_bin = Convert.ToString( address, 2 ).PadLeft( 36, '0' );
                foreach( string ad in m.get_addresses( ad_bin, mask ) )
                    mem2[ ad ] = n;
            }
        }

        file.Close();
        BigInteger res = mem.Aggregate( BigInteger.Zero, (sum,x) => sum + x.Value );
        Console.WriteLine( $"#1: {res}" );
        BigInteger res2 = mem2.Aggregate( BigInteger.Zero, (sum,x) => sum + x.Value );
        Console.WriteLine( $"#2: {res2}" );
    }
}

