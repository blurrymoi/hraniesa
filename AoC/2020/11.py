def first_seat( row, col, l_orig, dir_row, dir_col ):

    row += dir_row
    col += dir_col
    while 0 < row < len( l_orig ) and 0 < col < len( l_orig[0] ):
        if l_orig[row][col] in "L#":
            return l_orig[row][col]
        row += dir_row
        col += dir_col
    return '.'

def apply_rules( l_orig ):

    l_new = [] # string can't be [] accessed
    l_new.append( '.'*len( l_orig[0] ) )
    sth_changed = False

    for row in range( 1, len( l_orig ) - 1 ):
        row_new = ''

        for col in range( 1, len( l_orig[0] ) - 1 ):
            if l_orig[row][col] == '.':
                row_new += '.'
                continue
            #surround = [ l_orig[ r ][ c ] for r in [ row-1, row, row+1 ] for c in [ col-1,col,col+1 ]
            #             if r != row or c != col ]
            surround = [ first_seat( row, col, l_orig, r, c ) for r in [ -1,0,1 ] for c in [ -1,0,1 ]
                         if (r,c) != (0,0) ]
 
            if all( [ c in ".L" for c in surround ] ) and l_orig[row][col] == 'L':
                row_new += '#'
                sth_changed = True
            elif [ c == '#' for c in surround ].count( True ) >= 5 and l_orig[row][col] == '#':
                row_new += 'L'
                sth_changed = True
            else:
                row_new += l_orig[ row ][ col ]
        l_new.append( '.' + row_new + '.' )
    l_new.append( '.'*len( l_orig[0] ) )

    return apply_rules( l_new ) if sth_changed else l_new

def load():
    with open( "11.txt", "r", encoding="utf-8" ) as f:
        l = f.read().splitlines()
        l.insert( 0, '.'*len( l[0] ) )
        l.append( '.'*len( l[0] ) )
        l = [ '.' + row + '.' for row in l ]

        fixp = [ c for l in apply_rules( l ) for c in l ]
        print( fixp.count( '#' ) )

if __name__ == '__main__':
    load()
