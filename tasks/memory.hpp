/* Write a memory allocator with a single fixed-size arena, based on inline
 * free lists. Use a linear scan through the free list to find a sufficiently
 * big chunk to return from `alloc`. Ignore fragmentation. Use 32b indices and
 * sizes to represent the freelist. Do not allocate memory from the normal C++
 * heap (no std::vector, no new, no malloc). */

/* HINTS: It is okay to grab 8 bytes of memory from the arena even if the user
 * calls alloc( 1 ), or 16 if they call alloc( 9 ). (In other words, it is okay
 * to always return 8-aligned addresses, regardless of allocation size.) */

#include <array>
#include <iostream>

// size in bytes?
template< int arena_size >
struct memory
{
    //typedef sizeof( int ) meta_size;
    static_assert( arena_size >= 8, "arena size must be greater than 8" );    

    #define META_SIZE ( sizeof(int) )

    void *alloc( int object_size )
    {

        if( !freelist_head )
        {
            std::cerr << "no more free memory" << std::endl;
            return nullptr;
        }


        int freeblock_size = get_size( freelist_head );
        if( freeblock_size >= object_size )
        {
            int next_index = get_index( freelist_head );
            // we have enough room to just move the ptr
            if( freeblock_size - object_size >= META_SIZE )
            {
                *((int*)((char*)freelist_head + object_size + 4 + 4)) = next_index;
                //std::cout << "freelist_head + object_size " << *((int*)((char*)freelist_head + object_size)) << std::endl;
                *((int*)freelist_head) = object_size + 4;
                auto former_head = freelist_head;
                freelist_head = ((reinterpret_cast<char*>(freelist_head)) + object_size + 4);
                *((int*)freelist_head) = freeblock_size - object_size - 4;
                return (void*)(((int*)former_head) + 1); 
            }
            else // we have to use the whole chunk
            {
                auto former_head = freelist_head;

                if( next_index == -1 )
                    freelist_head = nullptr;
                else
                    freelist_head = &arena[next_index];  //(( reinterpret_cast<char*>(freelist_head) ) + next_index + 1 );
                // already has the correct size
                return (void*)(((int*)former_head) + 1); 
            }
        }
        else // won't fit in the first freeblock
        {
            int next_index = get_index( freelist_head );
            void* freeblock_prev = nullptr;
            void* freeblock_cur  = freelist_head;

            while( (freeblock_size < object_size) && (next_index != -1) )
            {
                freeblock_prev = freeblock_cur;
                freeblock_cur = &arena[next_index]; //(reinterpret_cast<char*>(&arena)) + next_index;
                freeblock_size = get_size( freeblock_cur );
                next_index = get_index( freeblock_cur ); 
            }

            if( freeblock_size >= object_size )
            {
                if( freeblock_size - object_size >= META_SIZE )
                {
                    *((int*)((char*)freeblock_cur + object_size + 4)) = next_index; // move index
                    *((int*)freeblock_cur) = object_size + 4;
                    
                    //auto former_head = freeblock_cur;
                    //freeblock_cur = ((reinterpret_cast<char*>(freelist_head)) + object_size);
                    *((int*)((char*)freeblock_cur) + object_size + 4) = freeblock_size - object_size - 4;
                    *(((int*)freeblock_prev) + 1 ) = get_index( freeblock_prev ) + object_size;  // update next_index of previous free block
                    return (void*)(((int*)freeblock_cur) + 1); 

                   /*
                    *( ( (char*)freeblock_cur ) + object_size ) = next_index; // move index
                    *((int*)freeblock_cur) = object_size + 4; // set meta(size) of the alloced block
                    *(((int*)freeblock_prev) + 1 ) = get_index( freeblock_prev ) + object_size;  // update next_index of previous free block
                    return (void*)(((int*)freeblock_cur) + 1);
                   */
                }

                else // we have to use the whole chunk
                {
                    *(((int*)freeblock_prev) + 1) = get_index( freeblock_cur );
                    // already has the correct size
                    return (void*)(((int*)freeblock_cur) + 1); 
                }

            }
            else
            {
                std::cerr << "Cannot satisfy allocation in a contiguous block." << std::endl;
                return nullptr;
            }
        }
    }

    void free( void * ptr )
    {
        ptr = (void*)((int*)ptr - 1); // the user is given the ptr past metadata(i.e. size)
        void* f = freelist_head;
        void* f_prev = nullptr;
        if( !f )
            freelist_head = ptr;
        else
        {
            bool extended_prev = false;
            bool extended_post = false;

            f_prev = freelist_head;
            int i;

            while( (i = get_index( f )) != -1 )
            {
                f_prev = f;
                f = &arena[i];
            // can extend immediately preceding block
                int size = get_size( f );
                if( (void*)((char*)f + size + 4) == ptr )//== arena.end() )
                {
                    *((int*)f) += get_size(ptr) + 4;
                    extended_prev = true;
                    break;
                } 
            // can extend immediately following block
                if( (ptr < f) && ((char*)f-(char*)ptr == (get_size(ptr) + 4)) ) // ptr precedes f
                {
                    *((char*)f_prev + 4) = (char*)ptr - (char*)&arena;
                    *((int*)ptr) += get_size( f ) + 4;
                    extended_post = true;
                    break;
                }
            }
            // check if we could have extended on both sides
            // go on iterating through the freelist
            void* extended_ptr = nullptr;
            void* f_prev2;
            if( extended_prev || extended_post )
                extended_ptr = extended_prev? f : ptr;
            if( extended_prev )
            {
                while( (i = get_index( f )) != -1 )
                {
                    f_prev2 = f;
                    f = &arena[i];
                    
                    if( (extended_ptr < f) && ((char*)extended_ptr-(char*)f == (get_size(extended_ptr) + 4)) ) // ptr precedes f
                    {
                        *((char*)f_prev2 + 4) = (char*)extended_ptr - (char*)&arena;
                        *((char*)extended_ptr + 4) = get_index( f );
                        *((int*)ptr) += get_size( f ) + 4;
                        return;
                    }
                }
            }
            else
            {
                if( extended_post )
                {
                    // can we also extend with the immediately preceding chunk?
                    while( (i = get_index( f )) != -1 )
                    {
                        //f_prev = f;
                        f = &arena[i];
        
                        int size = get_size( f );
                        if( (void*)((char*)f + size + 4) == ptr )//== arena.end() )
                        {
                            *((int*)f) += get_size(ptr) + 4;
                            break;
                        }
                        // we have to find the chunk that was pointing to the glued-together post-chunk and make it
                        // point to the next one
                        *((int*)f_prev + 1) = get_index( extended_ptr );
                    } 
                }
                else
                {
                    // haven't extended anything, append as a new free chunk
                    *((int*)f + 1) = (char*)ptr - (char*)&arena;
                }
            }
        }
    }

    memory()
    {
        *(int*)freelist_head = arena_size - META_SIZE;
        *(((int*)freelist_head) + 1) = -1;  
    }

private:
    /* You can use an std::array of char (or uint8_t) to represent the arena.
     * You probably want to store the 'head' of the free list outside of this
     * array of bytes. */

    // first sizeof( int ) bytes are for size of chunk
    static int get_size( void* ptr )
    {
        if( !ptr ) return 0;
        return *((int*)ptr);
    }

    static int get_index( void* ptr )
    {
        return *( ( reinterpret_cast< int * >( ptr ) ) + 1 );
    }

    std::array< uint8_t, arena_size > arena;
    void* freelist_head = &arena; 



    friend void assert_size_eq( void*, int );
    friend void assert_index_eq( void*, int );
    friend void test();
    friend void test_several_allocs();

};



