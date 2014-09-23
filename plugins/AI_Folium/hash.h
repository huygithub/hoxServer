#ifndef _HASH_H_
#define _HASH_H_

#include "defines.h"
#include "xq.h"

const int HALPHA = 1;

const int WINSCORE = 1000;
const int MATEVALUE = 900;
const int INVAILDVALUE = 2000;

const int ALPHA = 1;
const int BETA = 2;
const int PV = 3;

class Record
{
public:
    Record();
    void clear();
    int probe(XQ& xq, int depth, int ply, int alpha, int beta, uint32& move, const uint64& lock);
    void store_beta(int depth, int ply, int score, uint32 move, const uint64 &lock);
    void store_alpha(int depth, int ply, int score, uint32 move, const uint64 &lock);
    void store_pv(int depth, int ply, int score, uint32 move, const uint64 &lock);
private:
    uint64 m_lock;
    sint8 m_depth;
    sint8 m_flag;
    uint16 m_move;
    sint32 m_score;

};
inline Record::Record():m_flag(0)
{
}
inline void Record::clear()
{
    m_flag = m_flag > PV ? m_flag & PV : 0;
}
inline int Record::probe(XQ& xq, int depth, int ply, int alpha, int beta, uint32& move, const uint64& lock)
{
    if ((m_flag & PV) != 0 && m_lock == lock && xq.is_legal_move(m_move))
    {
        move = m_move;
        if (m_score == INVAILDVALUE)
        {
            return INVAILDVALUE;
        }
        if (m_score > MATEVALUE)
        {
            return m_score - ply;
        }
        if (m_score < -MATEVALUE)
        {
            return m_score + ply;
        }
        if (m_depth >= depth)
        {
            switch(m_flag & PV)
            {
            case ALPHA:
                if (m_score <= alpha)
                {
                    return m_score;
                }
                break;
            case BETA:
                if (m_score >= beta)
                {
                    return m_score;
                }
                break;
            case PV:
                return m_score;
            }
        }
    }
    else
    {
        move = 0;
    }
    return INVAILDVALUE;
}
inline void Record::store_alpha(int depth, int ply, int score, uint32 move, const uint64 &lock)
{
    if ((m_flag > PV) && m_depth > depth)
    {
        return;
    }
    if (score > MATEVALUE || score < -WINSCORE)
    {
        score = INVAILDVALUE;
    }
    else if (score < -MATEVALUE)
    {
        score -= ply;
    }
    m_lock = lock;
    m_depth = depth;
    m_flag = ALPHA | 4;
    m_move = (uint16) move;
    m_score = score;
}
inline void Record::store_beta(int depth, int ply, int score, uint32 move, const uint64 &lock)
{
    if ((m_flag > PV) && m_depth > depth)
    {
        return;
    }
    if (score > WINSCORE  || score < -MATEVALUE)
    {
        score = INVAILDVALUE;
    }
    else if (score > MATEVALUE)
    {
        score += ply;
    }
    m_lock = lock;
    m_depth = depth;
    m_flag = BETA | 4;
    m_move = (uint16) move;
    m_score = score;
}
inline void Record::store_pv(int depth, int ply, int score, uint32 move, const uint64 &lock)
{
    if ((m_flag > PV) && m_depth > depth)
    {
        return;
    }
    if (score > WINSCORE  || score < -WINSCORE)
    {
        score = INVAILDVALUE;
    }
    else if (score > MATEVALUE)
    {
        score += ply;
    }
    else if (score < -MATEVALUE)
    {
        score -= ply;
    }
    m_lock = lock;
    m_depth = depth;
    m_flag = PV | 4;
    m_move = (uint16) move;
    m_score = score;
}
class HashTable
{
public:
    HashTable(uint32 power=22);
    ~HashTable();
    void clear();
    Record& record(const uint32 &key, uint player);
private:
    uint32 m_size;
    uint32 m_mask;
    Record* m_records[2];
};

inline Record& HashTable::record(const uint32 &key, uint player)
{
    return m_records[player][key & m_mask];
}

#endif //_HASH_H_
