# one side of  1 2 3 | 4 5 6, try first, queue others
def matches( word, rule, rules, remaining ):
    rem = [ r.strip() for r in rule.strip().split( ' ' ) ]
    if len( rem ) > 1:
        remaining = rem[1:] + remaining
    return matches_rule( word, int( rem[0] ), rules, remaining )

# queue == remaining rules yet to match
def matches_rule( word, rule_no, rules, queue = [] ):

    if len( word ) == 0:
        return False

    #print( f"word: {word}, rule_no: {rule_no}, remaining: {remaining}" )
    #print( f"rules[{rule_no}]: ", rules[ rule_no ] )

    c = word[0]
    rule_rhs = rules[ rule_no ].strip()

    # terminal char symbol
    if rule_rhs.startswith( "\"" ) and len( rule_rhs ) == 3:
        if c == rule_rhs[1]:
            if len( word ) == 1 and not queue:
                return True
            return queue and matches_rule( word[1:], int( queue[0] ), rules, queue[1:] )
        return False

    # rule -> rule (| rule)
    return any( matches( word, rule, rules, queue ) for rule in rule_rhs.split( '|' ) )


def load():

    count = 0

    with open( "19.txt", "r", encoding="utf-8" ) as f:
        cond = {}

        rules,words = f.read().split( '\n\n' )
        rules = rules.split( '\n' )

        for rule in rules:
            n,rule = rule.split( ':' )
            cond[ int(n) ] = rule.strip()

        cond[8] = "42 | 42 8"
        cond[11] = "42 31 | 42 11 31"

        words = words.strip().split('\n')
        for word in words:
            if( matches_rule( word, 0, cond ) ):
                count += 1
        print( count )

if __name__ == '__main__':
    load()
