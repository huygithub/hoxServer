#include	"Transposition.h"
#include	"Move.h"
#include	"Board.h"

TNode::TNode(Board *board)
{
  this->_key  = board->primaryHash();
  this->_lock = board->secondaryHash();

  //this->_posText = board->getPosition();

  _flag = NOT_FOUND;
  _score = 0;
  _depth = -1;
  _stale = true;
}

void TNode::move(Move &m)
{
  bestMove = (unsigned short)((m.origin() << 8) | (m.destination() & 0x00FF));
}

Move TNode::move()
{
  return Move((bestMove >> 8), (bestMove & 0x00FF));
}


// The Table...
TranspositionTable::TranspositionTable(int bits)
{
  redTable  = new HashTable<TNode>(bits);
  blueTable = new HashTable<TNode>(bits);

  size = 1<<bits;
}

TranspositionTable::~TranspositionTable()
{
  delete redTable;
  delete blueTable;
}

void TranspositionTable::store(TNode &node)
{
  if (node.key() & COLOR_SWITCH_KEY)
    _store(node, redTable);
  else
    _store(node, blueTable);
}

/* Implementing a DEPTH Transposition table - positions are stored if they where
   searched more turoughly than what is aleady there. */
void TranspositionTable::_store(TNode &node, HashTable<TNode> *in)
{
  TNode there = in->find(node.key());
  if ((there.stale() || there.depth() <= node.depth()))
    {
      in->insert(node.key(), node);
    }
}

void TranspositionTable::find(Board *board, TNode &node)
{
  if (board->sideToMove() == RED)
    _find(board, node, redTable);
  else
    _find(board, node, blueTable);
}

void TranspositionTable::_find(Board *board, TNode &node, HashTable<TNode> *in)
{
  node = in->find(board->primaryHash());
  if (node.key() != board->primaryHash() || node.lock() != board->secondaryHash())
    node = TNode();
  else if (node.stale())
    {
      node.stale(false);
      //node.depth(0); // Because perpetual moves are not detected if save values!
      in->insert(node.key(), node);
    }
}


void TranspositionTable::flush()
{
  TNode x;
  for (int i = 0; i < size; i++)
    {
      x = blueTable->find(i);
      x.stale(true);
      blueTable->insert(i,x);
      x = redTable->find(i);
      x.stale(true);
      redTable->insert(i,x);
    }
}
