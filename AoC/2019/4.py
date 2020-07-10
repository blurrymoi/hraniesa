# 6 digit
# range 137683-596253
# two adjacent digits are the same
# non-decreasing

MIN = 137683
MAX = 596253

def meets_criteria( x ):

    has_double = False
    x = str( x )
    sqlen = 0

    for c in range( 5 ):
        if x[c] == x[c+1]:
            sqlen += 1
        elif not has_double and sqlen == 1:
            has_double = True
        else:
            sqlen = 0
        if x[c] > x[c+1]:
            return False
    return has_double or sqlen == 1

def bruteforce():

    count = 0

    for x in range( MIN, MAX+1 ):
        if meets_criteria( x ):
            count += 1
    print( count )



bruteforce()

