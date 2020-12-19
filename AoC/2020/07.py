def add_colours( r_to_l, col = 'shiny gold' ):
    colours = []
    if col not in r_to_l:
        return []
    for c in r_to_l[col]:
        colours.append( c )
        colours.extend( add_colours( r_to_l, c ) )
    return colours

def count_bags( l_to_r, bag ):
    bags = 0
    if not l_to_r[ bag ]:
        return 1
    for bg,amt in l_to_r[ bag ]:
        bags += amt * count_bags( l_to_r, bg )
    return 1 + bags

def load():
    with open( "07.txt", "r", encoding="utf-8" ) as f:
        left_to_right = {}
        right_to_left = {}

        # light red bags contain 1 bright white bag, 2 muted yellow bags
        for line in f:
            left, right = line.split( ' bags contain ' )

            left_to_right[left] = []
            if not right.startswith( 'no ' ):
                right = right.split( ',' )
                for item in right:
                    amt,color = item.strip().split(' ', 1)
                    color = color.rsplit(' ', 1)[0]
                    if not color in right_to_left:
                        right_to_left[color] = []
                    left_to_right[left].append( ( color, int( amt ) ) )
                    right_to_left[color].append( left )

    print( len( set( add_colours( right_to_left ) ) ) )
    print( count_bags( left_to_right, 'shiny gold' ) - 1 ) # shiny gold also counted

if __name__ == '__main__':
    load()
