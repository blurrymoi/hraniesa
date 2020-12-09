def check_nums( n, numlist ):

    for i in range( n, len( numlist ) ):
        l_tmp = numlist[i-n:n+(i-n)]
        assert len( l_tmp ) == n
        comb = [ a + b for a in l_tmp for b in l_tmp if a != b ]
        if int( numlist[i] ) not in comb:
            return( numlist[i] )

def find_cont( i, numlist ):
    start_idx = 0
    end_idx = 0
    sum_ = numlist[0]

    while sum_ != i:
        if sum_ < i:
            end_idx += 1
            sum_ += numlist[end_idx]
        elif sum_ > i:
            sum_ -= numlist[start_idx]
            start_idx += 1
    rg = numlist[start_idx:end_idx+1]
    assert sum( rg ) == i
    print( max( rg ) + min( rg ) )

def load():
    with open( "09.txt", "r", encoding="utf-8" ) as f:
        numlist = f.read().splitlines()
        numlist = list( map( lambda x: int( x ), numlist ) )
        i = check_nums( 25, numlist )
        print( i )
        find_cont( i, numlist )

if __name__ == '__main__':
    load()
