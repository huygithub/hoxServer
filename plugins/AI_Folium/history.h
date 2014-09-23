#ifndef _HISTORY_H
#define _HISTORY_H

#include <cstring>

#include "defines.h"
#include "move.h"

extern const uint32 g_capture_scores[33][32];

class History
{
public:
    void clear();
    void update_history(uint32, uint);
    uint32 move(uint, uint)const;
    uint32 move(uint, uint, uint, uint)const;
    static uint32 update_capture_move(uint32, uint, uint);
private:
    uint32 m_scores[0x4000];
};

inline void History::clear()
{
    memset(m_scores, 0, 0x10000);
}

inline void History::update_history(uint32 move, uint depth)
{
    uint32 &score = m_scores[move & 0x3fff];
    score += depth * depth;
    if (score >= 131072)//131072 == 1 << 17
    {
        for(uint i = 0; i < 0x4000; ++i)
            m_scores[i] >>= 1;
    }
}
inline uint32 History::move(uint src, uint dst) const
{
    uint m = create_move(src, dst);
    return static_cast<uint32>((m_scores[m] << 14)) | m;
}
inline uint32 History::move(uint src, uint dst, uint src_piece, uint dst_piece) const
{
    uint m = create_move(src, dst);
    return static_cast<uint32>((g_capture_scores[dst_piece][src_piece]) << 14) | m;
}

#endif  //_HISTORY_H
