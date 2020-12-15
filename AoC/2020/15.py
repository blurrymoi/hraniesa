def load():
    
    # naive
    l = [ 6, 13, 1, 15, 2, 0 ]

    for i in range( 2020 ):
        if l.count( l[-1] ) == 1:
            l.append(0)
        else:
            pos = len(l) - 2
            while l[pos] != l[-1]:
                pos -= 1
            pos += 1
            l.append( len( l ) - pos )

    print( "#1:", l[ 2019 ] )


    # remember last two positions for each number
    d = { 6: 1, 13: 2, 1:3, 15:4, 2:5 }
    count = 7
    last = 0

    while count < 30000001:

        if last not in d:
            d[last] = count - 1
            last = 0
            count += 1

        elif type( d[last] ) == int:
            d[last] = ( d[last], count - 1 )
            last = d[last][1] - d[last][0]
            count += 1

        else:
            a,b = d[last]
            d[last] = (b, count-1)
            last = d[last][1] - d[last][0]
            count += 1

    print( "pos", count, "last", last )


if __name__ == '__main__':
    load()

