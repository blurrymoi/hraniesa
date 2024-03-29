using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;

// mcs -debug -out:08.exe 08.cs
// mono 08.exe

class Instruction
{
    public string op;
    public int arg;

    public Instruction( string s )
    {
        string[] parts = s.Split( ' ', 2 );
        op = parts[0];
        arg = Int32.Parse( parts[1] );
    }

    public Instruction( string op, int arg )
    {
        this.op = op;
        this.arg = arg;
    }
}

class Code
{
    static int run_code( ref List< ( Instruction, bool ) > code )
    {
        int counter = 0;
        int idx = 0;
        while( !code[idx].Item2 )  // unvisited
        {
            int new_idx;
            switch( code[idx].Item1.op )
            {
                case "nop":
                    new_idx = idx + 1;
                    break;
                case "acc":
                    new_idx = idx + 1;
                    counter += code[idx].Item1.arg;
                    break;
                case "jmp":
                    new_idx = idx + code[idx].Item1.arg;
                    break;
                default:
                    new_idx = -1;
                    Console.WriteLine( "unknown op: " + code[idx].Item1.op );
                    break;
            }
            code[idx] = ( code[idx].Item1, true );
            if( new_idx == code.Count )
            {
                Console.WriteLine( "terminated, " + counter );
                return counter;
            }
            idx = new_idx;
        }
        return counter;
    }

    static void Main()
    {
        StreamReader file = new StreamReader( "08.txt" );
        string line;
        var code = new List< ( Instruction, bool ) >(); 

        while( ( line = file.ReadLine() ) != null )
        {
            var i = new Instruction( line );
            code.Add( ( i, false ) );
        }

        foreach( int idx in Enumerable.Range( 0, code.Count ) )
        {
            var code_cp = new List< ( Instruction, bool ) >( code );
            if( code[idx].Item1.op == "acc" )
                continue;
            code_cp[idx] = ( new Instruction( code[idx].Item1.op == "jmp"? "nop" : "jmp",
                                              code[idx].Item1.arg ), false );
            run_code( ref code_cp ); // TODO stop at terminated
        }

        file.Close();

        Console.WriteLine( run_code( ref code ) );
    }
}

