#ifndef	__TRANSPOSITION_H__
#define	__TRANSPOSITION_H__

/* Transposition.h (c) Noah Roberts 2003-03-28
 * Table and node classes responsible for keeping track of game positions that have
 * already been searched under a different line or in an earlier iteration.
 */

#include	"HashTable.h"

class Board;
class Move;

#include	<string>

enum { NOT_FOUND = 0, EXACT_SCORE = 1, UPPER_BOUND = 2, LOWER_BOUND = 3 };

class TNode : public HashNode
{
  unsigned short	bestMove;
  long			_score;
  unsigned char		_flag;
  int			_depth;
  bool			_stale;
  std::string		_posText;

 public:
  TNode() { _flag = NOT_FOUND; _depth = -1; _score = 0; bestMove = 0;_stale=true;}
  TNode(Board *brd);
  void move(Move &m);
  void score(long s)         { _score = s; }
  void flag(unsigned char f)  { _flag = f; }
  void depth(int d) { _depth = d; }
  void stale(bool s) { _stale = s; }

  Move move();
  long score()         { return _score; }
  unsigned char flag()  { return _flag; }
  int depth() { return _depth; }
  bool stale() { return _stale; }
  std::string pos() { return _posText; }
};

class TranspositionTable
{
  HashTable<TNode>*  redTable;
  HashTable<TNode>*  blueTable;

  int size;

  void _find(Board *board, TNode &node, HashTable<TNode> *in);
  void _store(TNode &node, HashTable<TNode> *in);

 public:
  TranspositionTable(int bits);
  ~TranspositionTable();

  void store(TNode &node);
  void find(Board *board, TNode &node);

  void flush();
};

#endif /* __TRANSPOSITION_H__ */
