# 6 digit
# range 137683-596253
# two adjacent digits are the same
# non-decreasing

POSITIONS = 5
MIN = 137683
MAX = 596253

def meets_criteria( x ):

    has_double = False
    x = str( x )

    for c in range( 5 ):
        if not has_double and x[c] == x[c+1]:
            has_double = True
        if x[c] > x[c+1]:
            return False
    return has_double

def bruteforce():

    count = 0

    for x in range( MIN, MAX+1 ):
        if meets_criteria( x ):
            # print( x )
            count += 1
    print( count )



bruteforce()

