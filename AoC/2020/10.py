from functools import reduce

def load():
    with open( "10.txt", "r", encoding="utf-8" ) as f:
        numlist = list( map( lambda i: int( i ), f.read().splitlines() ) )
        numlist = sorted( [0] + numlist )
        l = [ b - a for (a,b) in zip( (numlist + [numlist[-1] + 3]), numlist[1:] ) ]
        l.append( 3 )
        print( l )        
        print( l.count( 1 ), l.count( 3 ) )

        ll = ''.join( [ str(i) for i in l ] )
        ll = ll.replace( '1111', '7' ).replace( '111', '4' ).replace( '11', '2' )
        ll = ll.replace( '3', '' ).replace( '1','' )
        print( ll )
        print( reduce( lambda x,y: x * y, [ int(i) for i in ll ] ) )

if __name__ == '__main__':
    load()
