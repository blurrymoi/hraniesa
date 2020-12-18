class Int():

    def __init__( self, n ):
        self.n = [n]

    def __getitem__( self, i ):
        return self.n[i]
    def __len__( self ):
        return len( self.n )
    def __int__( self ):
        if len( self ) != 1:
            raise ValueError
        return self.n[0]

    def __add__( self, o ):
        self.n.append( '+' )
        self.n += o.n
        return self
    def __mul__( self, o ):
        self.n.append( '*' )
        self.n += o.n
        return self

class IntSwitched():

    def __init__( self, n ):
        self.n = n
    def __int__( self ):
        return self.n

    def __add__( self, o ):
        return IntSwitched( self.n * o.n )
    def __mul__( self, o ):
        return IntSwitched( self.n + o.n )

def _( exp ):
    res = exp[0]

    i = 1
    while i < len( exp ):

        if exp[i] == '+':
            res += exp[i+1]
            i += 1
        elif exp[i] == '*':
            res *= exp[i+1]
            i += 1
        i += 1
    return Int( res )

def swap( c ):
    return '*' if c == '+' else '+' if c == '*' else c

def load():

    with open( "18.txt", "r", encoding="utf-8" ) as f:
        sum1 = 0
        sum2 = 0

        for line in f:
            line1 = line.replace( '(', '_(' )[:-1] # throw away '\n'
            line1 = '_(' + line1 + ')'
            line1 = ''.join( list( map( lambda c: f"Int({c})" if c.isdigit() else c, line1 ) ) )
            sum1 += int( eval( line1 ) )

            line2 = [ swap( c ) for c in line ]
            line2 = ''.join( list( map( lambda c: f"IntSwitched({c})" if c.isdigit() else c, line2 ) ) )
            sum2 += int( eval( line2 ) )

        print( sum1 )
        print( sum2 )

if __name__ == '__main__':
    load()
