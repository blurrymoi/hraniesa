from functools import reduce

def load():
    with open( "06.txt", "r", encoding="utf-8" ) as f:

        d = f.read().split( '\n\n' )      # groups split by empty line
        e = [ l.split('\n') for l in d ]  # list of lists of group members
        f = [ set( ''.join( l ) ) for l in e ]
        g = sum( [ len(s) for s in f ] )
        print( g )

        e[-1].pop()  # stray ''
        f = [ reduce( lambda x,y : set(x).intersection( y ), l ) for l in e ]
        g = sum( [ len(s) for s in f if s ] )
        print( g )

if __name__ == '__main__':
    load()
