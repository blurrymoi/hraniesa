from functools import reduce

def load_row( cubes, s, y ):
    x = 0
    for c in s:
        if c != '\n':
            cubes[ (x,y,0) ] = c == '#'
            x += 1

def cubes_around( cubes, coords ):
    x,y,z = coords
    l = [ cubes[(x_,y_,z_)] for x_ in [x-1,x,x+1] for y_ in [y-1,y,y+1] for z_ in [z-1,z,z+1]
          if (x_,y_,z_) != coords and (x_,y_,z_) in cubes ]
    return l.count( True )

def update( num_cubes, state ):
    if state and (num_cubes == 2 or num_cubes == 3):
        return True
    if not state and num_cubes == 3:
        return True
    return False

def update_surrounding( cubes, new_cubes, coords ):
    x,y,z = coords
    for x_ in [ x-1,x,x+1 ]:
        for y_ in [ y-1,y,y+1 ]:
            for z_ in [ z-1,z,z+1 ]:
                new_coords = ( x_,y_,z_ )
                if new_coords not in new_cubes:
                    state = new_coords in cubes and cubes[new_coords]
                    new_cubes[ new_coords ] = update( cubes_around( cubes, new_coords ), state )

def load():

    with open( "17_1.txt", "r", encoding="utf-8" ) as f:

        cubes = {}
        y = 0

        for row in f:
            load_row( cubes, row, y )
            y += 1

        for i in range( 6 ):
            new_cubes = {}
            for coords in cubes:
                update_surrounding( cubes, new_cubes, coords )
            cubes = new_cubes

        print( list( cubes.values() ).count( True ) )

if __name__ == '__main__':
    load()
