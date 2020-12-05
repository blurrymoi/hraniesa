def half( min_, max_, left = False ):
    r = ( max_ + 1 - min_ ) / 2
    return round( r - 1 ) if left else round( r )

def bin( s, min_, max_ ):
    if len( s ) == 1:
        return min_ if s in ['F', 'L'] else max_
    if s[0] in ['F', 'L']:
        return bin( s[1:],  min_, min_ + half( min_, max_, True ) )
    else:
        return bin( s[1:], min_ + half( min_, max_ ), max_ )

def row_col( s ):
    return ( bin( s[:8], 0, 127 ), bin( s[7:], 0, 7 ) )

def seat( row, col ):
    return row * 8 + col

def load():
    max_ = 0
    all_seats = []

    with open( "05.txt", "r", encoding="utf-8" ) as f:

        for line in f:
            row, col = row_col( line )
            s = seat( row, col )
            all_seats.append( s )
            if s > max_:
                max_ = s
    print( "max", max_ )
    mine = [ s + 1 for s in all_seats if s + 1 not in all_seats and s + 2 in all_seats ]
    print( mine )

if __name__ == '__main__':
    row, col = row_col( "BFFFBBFRLR" )
    assert row == 70
    assert col == 5

    assert row_col( "FFFBBBFRRR" ) == ( 14, 7 )
    assert seat( 14, 7 ) == 119

    assert row_col( "BBFFBBFRLL" ) == ( 102, 4 )
    assert seat( 102, 4 ) == 820
    
    load()
