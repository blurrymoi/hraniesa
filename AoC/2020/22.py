def score( l ):

    res = 0
    i = 1

    for item in reversed( l ):
        res += i * item
        i += 1
    return res

def load():

    with open( "22.txt", "r", encoding="utf-8" ) as f:

        player1, player2 = f.read().split( '\n\n' )
        player1 = [ int( i ) for i in player1.split()[2:] ]
        player2 = [ int( i ) for i in player2.split()[2:] ]

        while player1 and player2:
            p1, *player1 = player1
            p2, *player2 = player2
            if p1 > p2:
                player1 += [p1, p2]
            elif p2 > p1:
                player2 += [p2, p1]

        if player1:
            print( score( player1 ) )
        else:
            print( score( player2 ) )

if __name__ == '__main__':
    load()
