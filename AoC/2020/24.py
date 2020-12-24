def parse_dir( s ):

    i = 0
    while i < len( s ):
        if s[i] in ['w','e']:
            yield s[i]
            i += 1
        elif s[i] in ['n','s']:
            yield s[i] + s[i+1]
            i += 2
    return

delta = { 0: { 'ne': (0,1), 'se': (0,-1), 'nw': (-1,1), 'sw': (-1,-1), 'e': (1,0), 'w': (-1,0) },
          1: { 'sw': (0,-1), 'nw': (0,1), 'se': (1,-1), 'ne': (1,1), 'e': (1,0), 'w': (-1,0) } }

def coords( dirs ):
    x,y = 0,0

    for d in dirs:
        if d == 'e':
            x += 1
        elif d == 'w':
            x -= 1
        else:
            xd,yd = delta[y % 2][d]
            x += xd
            y += yd
    return ( x, y )

# get list of coords of neighbour tiles
def neighbours( coords ):
    x,y = coords
    return [ (x+xd, y+yd) for (xd,yd) in delta[y % 2].values() ]

# check how many black neighbours tile at 'coords' has
def black_neighbours( coords, black_tiles ):
    return [ tile in black_tiles for tile in neighbours( coords ) ].count( True )

def load():

    black_tiles = []

    with open( "24.txt", "r", encoding="utf-8" ) as f:

        # find number of tiles flipped odd number of times
        for line in f:
            dirs = parse_dir( line.strip() )
            coor = coords( dirs )
            if coor in black_tiles:
                black_tiles.remove( coor )
            else:
                black_tiles.append( coor )
        print( len( black_tiles ) )

    for i in range( 100 ):

        new_black_tiles = []

        for tile in black_tiles:
            if black_neighbours( tile, black_tiles ) in [1,2]:
                new_black_tiles.append( tile )
            for neigh in neighbours( tile ):
                if neigh not in black_tiles and black_neighbours( neigh, black_tiles ) == 2:
                    new_black_tiles.append( neigh )

        black_tiles = list( set( new_black_tiles ) )
        print( i+1, len( black_tiles ) )

if __name__ == '__main__':
    load()
