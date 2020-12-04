def is_number( i ):
    try:
        int( i )
        return True
    except ValueError:
        return False

def valid_num( i, min_, max_ ):
    return is_number( i ) and min_ <= int( i ) <= max_

def valid( k, v ):
    if k == 'byr':
        return valid_num( v, 1920, 2002 )
    elif k == 'iyr':
        return valid_num( v, 2010, 2020 )
    elif k == 'eyr':
        return valid_num( v, 2020, 2030 )
    elif k == 'hgt':
        if v.endswith( 'in' ):
            return valid_num( v[:-2], 59, 76 )
        elif v.endswith( 'cm' ):
            return valid_num( v[:-2], 150, 193 )
    elif k == 'hcl':
        return v[0] == '#' and all( [( i.isdigit() or i in "abcdef" ) for i in v[1:]] )
    elif k == 'ecl':
        return v in [ 'amb', 'blu', 'brn', 'gry', 'grn', 'hzl', 'oth' ]
    elif k == 'pid':
        return len( v ) == 9 and is_number( v )
    elif k == 'cid':
        return True
    return False

def load():
    counter = 0
    data = []
    requires = ['byr', 'iyr', 'eyr', 'hgt', 'hcl', 'ecl', 'pid', 'cid' ]

    with open("04.txt", "r", encoding="utf-8") as f:

        for line in f:
            if not line.strip():
                if any( [ (d not in requires) for d,_ in data ] ):
                    print( "unknown key" )
                else:
                    if all( [ (r in [d for d,_ in data]) for r in requires if r != 'cid' ] ):
                        if all( [ ( valid( k, v.strip() ) ) for k,v in data ] ):
                            counter += 1
                data = []
            else:
                pairs = line.split( ' ' )
                data.extend( [ tuple( kv.split(':') ) for kv in pairs ] )

        if data:
            if all( (valid( k, v.strip() ) ) for k,v in data ):
                counter += 1

    print(counter)

if __name__ == '__main__':
    load()
