from collections import Counter

def restructure( d ):

    for allergen in d:
        if len( d[ allergen ] ) == 1: # definite food source
            for other in d:
                if allergen != other and list(d[ allergen ])[0] in d[ other ]:
                    d[ other ].remove( *d[ allergen ] )
                    if len( d[ other ] ) == 1:
                        restructure( d )

def load():

    c = Counter()
    allergens = {}

    with open( "21.txt", "r", encoding="utf-8" ) as f:

        for line in f:
            ingr, aller = line.split( '(contains' )
            ingr = ingr.split()
            c.update( ingr )
            aller = aller[:-2].split() # remove ')'

            for a in aller:
                if a.endswith( ',' ):
                    a = a[:-1].strip()
                if a not in allergens:
                    allergens[ a ] = set( ingr )
                else:
                    allergens[ a ] = allergens[ a ].intersection( set( ingr ) )

    restructure( allergens )
    print( allergens )

    # sort by key, get value out of single-item set
    d = [ list( i )[0] for _,i in sorted( allergens.items(), key=lambda item: item[0] ) ]

    # 2
    print( ','.join(d) )

    # 1 get count of non-allergen ingredients
    foods = [ list(i)[0] for i in allergens.values() ]
    print( sum( [ c[ item ] for item in c if item not in foods ] ) )

if __name__ == '__main__':
    load()
