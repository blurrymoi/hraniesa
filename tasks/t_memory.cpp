#include "memory.hpp"
#include <cassert>

void assert_size_eq( void* ptr, int size )
{
    std::cout << "SIZE: " << memory<32>::get_size( ptr ) << std::endl;
    assert( memory<32>::get_size( ptr ) == size );
}

void assert_index_eq( void* ptr, int index )
{
    assert( memory<32>::get_index( ptr ) == index );
}

void test()
{
    std::array< uint8_t, 10 > a;
    void* a0 = &a;
    void* a3 = &a[3];
    assert( (char*)a3 - (char*)a0 == 3);

    memory<32> m;
    assert_size_eq( &m.arena, 28 );
    assert_index_eq( &m.arena, -1 );

    auto m5 = m.alloc( 26 );
    assert_size_eq( (reinterpret_cast<int*>(m5) - 1), 28 );
    assert( m.freelist_head == nullptr );
    auto m4 = m.alloc( 4 );
    assert( m4 == nullptr );

    m.free( m5 );
    assert( m.freelist_head != nullptr );
    assert_size_eq( m.freelist_head, 28 );

    //std::cout << "alloced size: " << memory<32>::get_size( (reinterpret_cast<int*>(m4) - 1) ) << std::endl;
    //std::cout << "available size: " << memory<32>::get_size( m.freelist_head ) << std::endl;
    //std::cout << "next free: " << memory<32>::get_index( m.freelist_head ) << std::endl;
}

void test_several_allocs()
{
    memory<32> m;
    auto m2 = m.alloc( 2 );
    assert_size_eq( (reinterpret_cast<int*>(m2) - 1), 6 );
    assert_size_eq( m.freelist_head, 22 );
    assert_index_eq( m.freelist_head, -1 );

    auto m7 = m.alloc( 7 );
    assert_size_eq( (reinterpret_cast<int*>(m7) - 1), 11 );
    assert_size_eq( m.freelist_head, 11 );

    auto m1 = m.alloc( 1 );
    assert_size_eq( (reinterpret_cast<int*>(m1) - 1), 5 );
    assert_size_eq( m.freelist_head, 6 );

    auto m72 = m.alloc( 7 );
    assert( m72 == nullptr );
}



int main()
{
    test();
    std::cout << "***" << std::endl;
    test_several_allocs();
}
