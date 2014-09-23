#ifndef MOVE_H
#define MOVE_H

#include <cassert>

#include "int.h"

inline uint move_src(uint32 move) {return move & 0x7f;}
inline uint move_dst(uint32 move) {return (move >> 7) & 0x7f;}
inline uint32 create_move(uint src, uint dst)
{
    assert(src < 90);
    assert(dst < 90);
    return src | (dst << 7);
}

#define trace_src move_src
#define trace_dst move_dst
inline uint trace_dst_piece(uint32 trace){return (trace >> 14) & 0x3f;}
inline uint trace_flag(uint32 trace){return trace >> 20;}
inline uint32 trace_move(uint32 trace){return trace & 0x3fff;}
inline uint32 create_trace(uint flag, uint dst_piece, uint32 move)
{
    assert (flag < 16);//4bits
    assert (dst_piece < 33);//6bits
    return (flag << 20) | (dst_piece << 14) | (move & 0x3fff);
}

extern uint32 ucci2move(char* iccs);
extern char* move2ucci(uint32 move, char* iccs);

#endif //MOVE_H
