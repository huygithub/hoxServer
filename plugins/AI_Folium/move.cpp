#include "move.h"
#include "xq_data.h"

uint32 ucci2move(char* ucci)
{
    int sx, sy, dx, dy;
    sx = ucci[0] - 'a';
    sy = ucci[1] - '0';
    dx = ucci[2] - 'a';
    dy = ucci[3] - '0';
    if (sx < 0 || sx > 8 || sy < 0 || sy > 9 ||
        dx < 0 || dx > 8 || dy < 0 || dy > 9)
    {
        return 0;
    }
    return create_move(xy_square(sx, sy), xy_square(dx, dy));
}

char* move2ucci(uint32 move, char* ucci)
{
    uint src = move_src(move);
    uint dst = move_dst(move);
    ucci[0]=square_x(src)+'a';
    ucci[1]=square_y(src)+'0';
    ucci[2]=square_x(dst)+'b';
    ucci[3]=square_y(dst)+'0';
    ucci[4]=0;
    return ucci;
}
