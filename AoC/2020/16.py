from functools import reduce

# d is a dict( 'category': [two tuples of ranges (min,max)] )
def within_any_range( n, d ):
    for item in d:
        for ran in d[item]:
            if ran[0] <= n <= ran[1]:
                return True
    return False

def possible_categories( n, d ):
    ctg = set()
    for item in d:
        ran1, ran2 = d[item]
        if ran1[0] <= n <= ran1[1] or ran2[0] <= n <= ran2[1]:
            ctg.add( item )
    return ctg

# we have a set of size 1, the item can now be removed
# from all other categories as a possibility
def restructure( categories, idx ):

    idxs = [] # any other size 1 that might arise

    for i in range( len( categories ) ):
        if i != idx:
            l = len( categories[i] )
            categories[i].discard( list( categories[idx] )[0] )
            if len( categories[i] ) == 1 and l == 2:
                idxs.append( i )
    for i in idxs:
        restructure( categories, i )
    return categories

def load():
    with open( "16.txt", "r", encoding="utf-8" ) as f:
        l = f.read().splitlines()
        d = {}
        i = 0

        while not l[i].startswith( 'your ticket' ):

            if not l[i].strip():
                i += 1
                continue

            category, ranges = l[i].split(':')
            pairs = ranges.split( ' or ' )
            r1_min, r1_max = [ int( p.strip() ) for p in pairs[0].split('-') ]
            r2_min, r2_max = [ int( p.strip() ) for p in pairs[1].split('-') ]
            d[ category.strip() ] = [ (r1_min, r1_max), (r2_min, r2_max) ]
            i += 1

        i += 1
        mine = l[i].split(',')
        i += 1

        while not l[i].strip() or l[i].startswith( 'nearby tickets' ):
            i += 1

        count = 0
        categories = [] # set of possible categories for each column
        for j in range( len( d.keys() ) ):
            categories.append( set( d.keys() ) )

        while i < len( l ):

            nums = list( map( lambda i: int( i ), l[i].split(',') ) )
            idx = 0

            for n in nums:
                if not within_any_range( n, d ):
                    count += n
                else:
                    categories[idx] = categories[idx].intersection( possible_categories( n, d ) )
                    if len( categories[idx] ) == 1:
                        restructure( categories, idx )
                idx += 1
            i += 1

        print( "#1 count", count )
        print( categories )                                           # str.startswith( *ctg, 'departure' )
        ctg_idx =  [ idx for ( idx, ctg ) in enumerate( categories ) if [*ctg][0].startswith( 'departure' ) ]

        print( reduce( lambda x,y: x * y, [ int( mine[e] ) for e in ctg_idx ] ) )

if __name__ == '__main__':
    load()
