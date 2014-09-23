#ifndef	__LAWYER_H__
#define	__LAWYER_H__

/*
 * Lawyer.h (c) Noah Roberts 2003-02-23
 * Lawyer class: contains tools for checking move legality.
 * Lets make this class responsible for generating moves also - reasoning: if we decide to
 * allow different styles of this game to be played with this engine, it would be nice to
 * be able to replace a single class.  For instance, Korean rules are very different, but
 * use the same board; just replace this class to be able to play by Korean rules.
 */

/*
 * Notes:
 *	generateMoves should still check for king-facing problems - right now it doesn't
 *
 */

#include	<list>
#include	<string>
#include	<utility>
#include	"HashTable.h"

class Move;
#include	"Board.h"

typedef unsigned long ulong;
typedef std::pair< ulong, ulong > positionHash;

class Lawyer : public BoardObserver
{
 private:
  unsigned char *legalPositions;
  unsigned char *pieceMasks;
  short *knightBlock;
  short *elephantEye;

  short *boundingBoxKey;
  short *boundingBox;
  
  /* how the pieces move in the above array */
  short (*pieceMoves)[8];

  Board	*board;

  std::string message;
  std::vector< positionHash > positionalHistory;


  void addMove(std::list<Move> &moveList, Move &theMove, bool onlylegal = false);
 public:
  Lawyer(Board *brd);
  bool legalMove(Move &theMove);
  bool inCheck(int col = -1);
  bool drawn();
  std::string getMessage() { return message; }

  bool underAttack(int location, int c = -1);

  /* These are used at different times for move ordering and a future quiescence search.
   * capture moves are more likely to alter the dynamics of the game, so they are searched
   * first.  Also after the iterative deepening has gone as deep as it supposed to the
   * engine can begin looking at a few extra moves to try and avoid horizon mistakes (a good
   * line may actually be devistating if you just looked one move deeper).  When the king
   * is in check we want to only look at legal moves (ones that do not result in a king
   * capture).  Checking for this is rather expensive so there will be many times when
   * it is simply cheaper to search the illegal trees.  Must make sure that the program never
   * rates loosing a king better than loosing a rook and then the king.
   */
  void generateMoves(std::list<Move> &moves, bool onlyLegal = false);
  void generateMoves(std::list<Move> &moves, int location, bool legalonly = true);
  //void generateMoves(int location, std::list<Move> &moveList, bool onlyLegal = false);
  void setBoard(Board *brd) { board = brd; }

  // generation functions - one for each piece.
  void generatePawnMoves(int location, std::list<Move> &moveList, bool onlyLegal = false);
  void generateCanonMoves(int location, std::list<Move> &moveList, bool onlyLegal = false);
  void generateRookMoves(int location, std::list<Move> &moveList, bool onlyLegal = false);
  void generateKnightMoves(int location, std::list<Move> &moveList, bool onlyLegal = false);
  void generateElephantMoves(int location, std::list<Move> &moveList, bool onlyLegal = false);
  void generateGuardMoves(int location, std::list<Move> &moveList, bool onlyLegal = false);
  void generateGeneralMoves(int location, std::list<Move> &moveList, bool onlyLegal = false);

  // returns color that won.
  int gameWonByChase();
  int gameWonByPCheck();
  bool gameDrawn();

  void boardChanged(Board* brd, int msg);
};

#endif /* __LAWYER_H__ */
