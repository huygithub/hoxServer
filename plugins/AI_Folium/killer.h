#ifndef _KILLER_H_
#define _KILLER_H_

class Killer
{
public:
    void clear()
    {
        moves[0] = moves[1] = 0;
    }
    void push(uint move)
    {
        assert(move < 0x4000);
        if (move==moves[0])
            return;
        moves[1] = moves[0];
        moves[0] = move;
    }
    uint killer(uint i)
    {
        return moves[i];
    }
private:
    uint16 moves[2];
};

#endif //_KILLER_H_
