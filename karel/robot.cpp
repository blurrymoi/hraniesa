#include <array>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <cstdlib>
#include <cmath>
#include <cassert>

template< int width, int height >
struct grid
{
    struct Cell{ unsigned tokens = 0; };
    enum direction { d_up = 0, d_right, d_down, d_left };
    struct Robot
    {
        bool has_token = false;
        direction dir = direction::d_up;
        struct position{ unsigned x = 0, y = 0; } pos;
    };

    char get_robot_symbol()
    {
        switch(dk.dir){
            case direction::d_up: return '^';
            case direction::d_down: return 'v';
            case direction::d_left: return '<';
            case direction::d_right: return '>';
	}
    }
    
    typedef std::array< Cell, width > row;
    std::array< row, height > tbl;
    
    void right() { dk.dir = static_cast<direction>((dk.dir + 1) % 4); }
    void left() {
        dk.dir = static_cast<direction>(dk.dir - 1);
        if( dk.dir == -1 )
            dk.dir = static_cast<direction>(3);
    }

    bool down()
    {
        if( dk.dir == direction::d_right )
        {
            right();
            draw();
            bool b = step();
            draw();
            if(!b) return false;
            right();
            draw();
            return true;
        }
        else
        {
            left();
            draw();
            bool b = step();
            draw();
            if(!b) return false;
            left();
            draw();
            return true;
         }
    }


    void add_tokens( unsigned x, unsigned y, unsigned tokens )
    {
        tbl[y][x].tokens += tokens;
    }

    bool step()
    {
        switch (dk.dir) {
            case direction::d_up:
                if(dk.pos.y == 0)
                    return false;
                dk.pos.y--;
                return true;
            case direction::d_down:
                if(dk.pos.y == height - 1)
                    return false;
                dk.pos.y++;
                return true;
            case direction::d_right:
                if(dk.pos.x == width - 1)
                    return false;
                dk.pos.x++;
                return true;
            case direction::d_left:
                if(dk.pos.x == 0)
                    return false;
                dk.pos.x--;
                return true;
        }
    }

    bool holds_token() { return dk.has_token; }
    bool take()
    {
        if(holds_token()) return false;
        if(tbl[dk.pos.y][dk.pos.x].tokens == 0) return false;
        dk.has_token = true;
       	tbl[dk.pos.y][dk.pos.x].tokens--;
        return true;
    }

    bool drop()
    {
        if( !holds_token() )
            return false;
        tbl[dk.pos.y][dk.pos.x].tokens++;
        dk.has_token = false;
        return true;
    }

    void draw_line()
    {
        std::cout << '*';
        for( unsigned i=0; i<width*4; i++ )
            std::cout << '*';
        std::cout << std::endl;
    }

    void draw_window(unsigned h, unsigned r)
    {
        std::cout << '*';
        for( unsigned i=0; i<width*4; i++ )
        {
            if( (i % 4) == 3 ) std::cout << '*';
            else
            {
                if( dk.pos.x == i/4 && dk.pos.y == h && r == 0  )
                {
                    std::cout << ' ' << get_robot_symbol() << (holds_token()? 'o' : ' ');
                    i+=2; continue;
                }
                if( tbl[h][i/4].tokens != 0 && r == 1)
                {
                    std::cout << std::setw(3) << tbl[h][i/4].tokens;
                    i+=2;
                    continue;
                }
                else
                {
                    std::cout << "   ";
                    i+=2;
                    continue;
                }
            }
        }
        std::cout << std::endl;
    }

    void draw()
    {
    #ifdef DRAW
        draw_line();
        for( unsigned i=0; i<height*3; i++ )
        {
            if(i % 3 == 2) draw_line();
            else draw_window(i/3, i%3);
        }
        std::cout << std::endl;
        sleep(1);
    #else
        return;
    #endif
    }

    void turn(direction d)
    {
        while(dk.dir != d)
        {
            right();
            draw();
        }
    }

    void upper_left()
    {
    #ifdef DRAW
        std::cout << "returning home" << std::endl;
    #endif
        turn(direction::d_up);
        draw();
        while(step()) draw();
        turn(direction::d_left);
        draw();
        while(step()) draw();
    }

    void collect()
    {
    #ifdef DRAW
        std::cout << "COLLECTING" << std::endl;
    #endif
        upper_left();
        drop();

        turn(direction::d_right);
        draw();
        //bool b = step();
        if( !step() )
            if(!down()) return;
 
        draw();
        while( !take() )
        {
            bool b = step(); draw();
            if( !b )
            {
                bool d = down(); draw();
                if( !d ) return;
            }
        }
        if( holds_token() )
            collect();
    }

    void get_to_position(Robot& ghost)
    {
    #ifdef DRAW
        std::cout << "returning to last position" << std::endl;
    #endif
        turn(direction::d_right);
        while( dk.pos.x != ghost.pos.x )
            step();
        turn(direction::d_down);
        while( dk.pos.y != ghost.pos.y )
            step();
        turn(ghost.dir);
    }

    bool move_to_next_black()
    {
        bool b = step(); draw();
        if(!b)
        {
            bool d = down(); draw();
            if(!d) return false;
        }
        // did step, we are on white
        b = step(); draw();
        if(!b)
        {
            bool d = down(); draw();
            if(!d) return false;
        }
        //black
        return true;
    }


    void simple_checkers()
    {
        Robot ghost_karel;
        
        collect();
        upper_left();
        take();
        turn(direction::d_right);
        
        while(move_to_next_black())
        {
            drop();
            ghost_karel.pos.x = dk.pos.x;
            ghost_karel.pos.y = dk.pos.y;
            ghost_karel.dir = dk.dir;
            upper_left();
            take();
            get_to_position(ghost_karel);
        }
        upper_left();
        drop();
        draw();
    }

    void checkers() { simple_checkers(); }
        

//  private:
    Robot dk;
};

template< int n, int m >
grid< n, m > random_grid( int min_tokens, int max_tokens )
{
    grid< n,m > g;
    int r = rand() % (max_tokens - min_tokens + 1);
    r += min_tokens;
    std::cout << "tokens: " << r << std::endl;
//    std::cout << "min_tokens: " << min_tokens << std::endl;

    for(auto i=0; i<r; i++)
        g.add_tokens( rand() % n, rand() % m, 1 );
    g.dk.pos.x = rand() % n;
    g.dk.pos.y = rand() % m;
    g.dk.dir = static_cast< typename grid< n,m >::direction >( rand() % 4 );
    return g;
}

template< int n, int m >
int count_tokens( grid< n,m > g )
{
    assert( !g.holds_token() );
    unsigned sum = 0;
    for( int x = 0; x < n; x++ )
        for( int y = 0; y < m; y++ )
            sum += g.tbl[y][x].tokens;
    return sum;
}

template < int n, int m >
void test_collect_one( grid< n,m > g )
{
    g.collect();
    assert( g.tbl[0][0].tokens == count_tokens( g ) );
    for( int x = 0; x < n; x++ )
    {
        for( int y = 0; y < m; y++ )
            if( x != 0 and y != 0 )
                assert( g.tbl[y][x].tokens == 0 );
    }
}

template < int n, int m >
void test_checkers_one( grid< n,m > g )
{
    g.simple_checkers();
    unsigned sum = 0;
    for( int x = 0; x < n; x++ )
        for( int y = 0; y < m; y++ )
        {
            int t = g.tbl[y][x].tokens;
            if((  x + y ) % 2 == 0 )
            {
                if ( t == 0 ) g.draw();
                assert( t > 0 );
            }
            sum += t;
        }
    assert( sum == count_tokens( g ) );
}

template< int n, int m >
void test()
{
    std::cout << "grid size: " << n << " x " << m << std::endl;
    for(auto i=0; i < 250; i++)
    {
        int r = rand() % 20;
        int min = ceil(((float(n) * m)/2));
        auto g1 = random_grid< n, m >( min, min + r );
        //g1.draw();
        std::cout << "collecting" << std::endl;
        test_collect_one( g1 );
        std::cout << "checkers" << std::endl;
        test_checkers_one( g1 );
    }
}

int main()
{

    //auto g = random_grid<1,3>( 2, 2 );
    //g.collect();
    //g.checkers();

    test< 10, 10 >();
    test< 7, 7 >();
    test< 5, 8 >();
    test< 1, 9 >();
    test< 1, 1 >();
    test< 2, 2 >();

/*
    constexpr std::array< std::pair<int,int>, 5 > sizes = {
        {{204,204}},
        {{95,95}},
        {{103,28}},
        {{1,19}},
        {{68,34}}
    };
*/
}

