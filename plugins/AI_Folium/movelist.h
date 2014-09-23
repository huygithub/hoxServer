#ifndef _MOVELIST_H_
#define _MOVELIST_H_

#include "defines.h"
#include "move.h"

const uint32 MaxMoveNumber = 128;
class MoveList
{
private:
    uint32 movelist[MaxMoveNumber];
    uint length;
public:
    MoveList();
    uint size()const;
    void clear();
    uint32& operator[](uint index);
    const uint32& operator[](uint index)const;
    void push(uint src, uint dst);
    void push(uint32 move);
};
inline MoveList::MoveList():length(0){}
inline uint MoveList::size()const{return length;}
inline void MoveList::clear(){length = 0;}
inline uint32& MoveList::operator[](uint32 index){return movelist[index];}
inline const uint32& MoveList::operator[](uint32 index)const{return movelist[index];}
inline void MoveList::push(uint src, uint dst){movelist[length++] = create_move(src, dst);}
inline void MoveList::push(uint32 move){movelist[length++] = move;}
#endif //_MOVELIST_H_
