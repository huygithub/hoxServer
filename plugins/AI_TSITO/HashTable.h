#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <iostream>

using namespace std;
/*
 * HashTable.h (c) Noah Roberts 2003-03-13
 * HashTable keeps a hash of board positions.
 */

#include	"Board.h"


template<class T>
class HashTable
{
private:
    T*            _content;
    unsigned int  _mask;

public:
    /**
     * @param  size - is a count of bits, not actual size.
     */
    HashTable( unsigned int bitcount )
        : _content( NULL )
    {
        int size = 1 << bitcount;
        _content = new T[size];
        _mask    = size - 1;
    }
  
    ~HashTable()
    {
        delete [] _content;
    }

    void insert( unsigned int key, T &nde )
    {
        _content[key & _mask] = nde;
    }
  
    T& find( unsigned int key )
    {
        return _content[key & _mask];
    }
};

class HashNode
{
 protected:
  unsigned int _key;
  unsigned int _lock;
 public:
  HashNode() { _key = 0; _lock = 0; } // so a hit never occurs on the default.
  HashNode(Board *board) { setKeys(board); }
  unsigned int key() { return _key; }
  unsigned int lock() { return _lock; }
  void setKeys(Board *board)
    {
      _key = board->primaryHash();
      _lock = board->secondaryHash();
    }
  bool operator==(HashNode &nde) { return (_key == nde._key && _lock == nde._lock); }

};


#endif /* __HASHTABLE_H__ */
