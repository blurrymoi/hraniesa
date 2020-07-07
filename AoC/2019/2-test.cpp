void test_one( vec_int v1, vec_int result )
{
    run( v1 );
    assert( v1 == result );
}

void test()
{
    vec_int v1 = { 1,9,10,3,2,3,11,0,99,30,40,50 };
    perform_op( v1, 0 );
    assert( ( v1 == vec_int{ 1,9,10,70,2,3,11,0,99,30,40,50 } ) );
    perform_op( v1, 4 );
    assert( ( v1 == vec_int{ 3500,9,10,70,2,3,11,0,99,30,40,50 } ) );

    test_one( { 1,0,0,0,99 }, { 2,0,0,0,99 } );
    test_one( { 2,3,0,3,99 }, { 2,3,0,6,99 } );
    test_one( { 2,4,4,5,99,0 }, { 2,4,4,5,99,9801 } );
    test_one( { 1,1,1,4,99,5,6,0,99 }, { 30,1,1,4,2,5,6,0,99 } );
}

