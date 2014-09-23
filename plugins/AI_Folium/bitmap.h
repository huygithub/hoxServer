#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <string.h>
#include "defines.h"
#include "xq_data.h"

class Bitmap
{
public:
    Bitmap();
    void do_move(uint, uint);
    void undo_move(uint, uint, uint);

    uint nonempty_up_1(uint)const;
    uint nonempty_down_1(uint)const;
    uint nonempty_left_1(uint)const;
    uint nonempty_right_1(uint)const;
    uint nonempty_up_2(uint)const;
    uint nonempty_down_2(uint)const;
    uint nonempty_left_2(uint)const;
    uint nonempty_right_2(uint)const;

    uint mask(uint, uint)const;
    uint distance(uint, uint)const;
    bool distance_is_0(uint, uint)const;
    bool distance_is_1(uint, uint)const;

    void reset ();
    void setbit(uint);
private:
    uint xinfo(uint, uint)const;
    uint yinfo(uint, uint)const;

    void changebit(uint);

    static uint prev_1(uint info);
    static uint prev_2(uint info);
    static uint next_1(uint info);
    static uint next_2(uint info);
private:
    uint16 m_lines[20];
    static const uint16 s_line_infos[10][1024];
    static const uint8 s_bit_counts[1024];
    static const uint16 s_distance_infos[91][128];
};
//Bitmap
inline uint Bitmap::xinfo(uint x, uint y)const
{
    assert(x < 9UL && y < 10UL);
    return s_line_infos[x][m_lines[y]];
}
inline uint Bitmap::yinfo(uint x, uint y)const
{
    assert(x < 9UL && y < 10UL);
    return s_line_infos[y][m_lines[x+10]];
}
inline void Bitmap::setbit(uint sq)
{
    uint x = square_x(sq);
    uint y = square_y(sq);
    m_lines[y] |= 1UL << x;
    m_lines[x+10UL] |= 1UL << y;
}
inline void Bitmap::changebit(uint sq)
{
    uint x = square_x(sq);
    uint y = square_y(sq);
    m_lines[y] ^= 1UL << x;
    m_lines[x+10UL] ^= 1UL << y;
}
inline uint Bitmap::prev_1(uint info)
{
    return info & 0xf;
}
inline uint Bitmap::prev_2(uint info)
{
    return (info >> 4) & 0xf;
}
inline uint32 Bitmap::next_1(uint info)
{
    return (info >> 8) & 0xf;
}
inline uint Bitmap::next_2(uint info)
{
    return (info >> 12) & 0xf;
}

inline void Bitmap::do_move(uint src, uint dst)
{
    changebit(src);
    setbit(dst);
}
inline void Bitmap::undo_move(uint src, uint dst, uint dst_piece)
{
    if (dst_piece == EmptyIndex)
        changebit(dst);
    changebit(src);
}

inline uint Bitmap::nonempty_up_1(uint sq)const
{
    uint x = square_x(sq);
    uint y = square_y(sq);
    return xy_square(x, Bitmap::next_1(yinfo(x, y)));
}
inline uint Bitmap::nonempty_down_1(uint sq)const
{
    uint x = square_x(sq);
    uint y = square_y(sq);
    return xy_square(x, Bitmap::prev_1(yinfo(x, y)));
}
inline uint Bitmap::nonempty_left_1(uint sq)const
{
    uint x = square_x(sq);
    uint y = square_y(sq);
    return xy_square(Bitmap::prev_1(xinfo(x, y)), y);
}
inline uint Bitmap::nonempty_right_1(uint sq)const
{
    uint x = square_x(sq);
    uint y = square_y(sq);
    return xy_square(Bitmap::next_1(xinfo(x, y)), y);
}
inline uint Bitmap::nonempty_up_2(uint sq)const
{
    uint x = square_x(sq);
    uint y = square_y(sq);
    return xy_square(x, Bitmap::next_2(yinfo(x, y)));
}
inline uint Bitmap::nonempty_down_2(uint sq)const
{
    uint x = square_x(sq);
    uint y = square_y(sq);
    return xy_square(x, Bitmap::prev_2(yinfo(x, y)));
}
inline uint Bitmap::nonempty_left_2(uint sq)const
{
    uint x = square_x(sq);
    uint y = square_y(sq);
    return xy_square(Bitmap::prev_2(xinfo(x, y)), y);
}
inline uint Bitmap::nonempty_right_2(uint sq)const
{
    uint x = square_x(sq);
    uint y = square_y(sq);
    return xy_square(Bitmap::next_2(xinfo(x, y)), y);
}

inline uint Bitmap::mask(uint src, uint dst)const
{
    uint info = s_distance_infos[src][dst];
    assert((info >> 10) < 20);
    return info & m_lines[info >> 10];
}

inline uint Bitmap::distance(uint src, uint dst)const
{
    return s_bit_counts[mask(src, dst)];
}

inline bool Bitmap::distance_is_0(uint src, uint dst)const
{
    return mask(src, dst) == 0;
}

inline bool Bitmap::distance_is_1(uint src, uint dst)const
{
    uint m = mask(src, dst);
    return (m & (m-1)) == 0;
}

inline Bitmap::Bitmap()
{
    reset();
}
inline void Bitmap::reset()
{
    memset(m_lines, 0, 38);
    m_lines[19] = 1023;
}
#endif    //_BITMAP_H_

