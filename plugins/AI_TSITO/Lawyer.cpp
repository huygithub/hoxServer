#include	<list>
#include	"Lawyer.h"

#include	"Move.h"
#include	"Board.h"
#include	"Evaluator.h"

/*
 * Lawyer.cpp (c) Noah Roberts 2003-02-24
 * Lawyer class implementation.
 */
using namespace std;
// represents all the places on the board certain pieces can reach from the side of
// the current player at top.
static unsigned char _legalPositions[90] =
  {
    2, 2, 6,10,10,10, 6, 2, 2,
    2, 2, 2,10,10,10, 2, 2, 2,
    6, 2, 2,10,14,10, 2, 2, 6,
    3, 2, 3, 2, 3, 2, 3, 2, 3,
    3, 2, 7, 2, 3, 2, 7, 2, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3
  };
// piece masks to test in above array
static unsigned char _pieceMasks[8] = {0, 1, 2, 2, 2, 4, 8, 8};
// offsets of the knight block square cooresponding to knight offsets
static short _knightBlock[8] = {1, -1, -9, -9, -1, 1, 9, 9};
// same for elephant
static short _elephantEye[8] = {-10, -8, 8, 10, 0, 0, 0, 0};

// index to the bounding box
static short _boundingBoxKey[BOARD_AREA] =
  {
    28, 29, 30, 31, 32, 33, 34, 35, 36,
    41, 42, 43, 44, 45, 46, 47, 48, 49,
    54, 55, 56, 57, 58, 59, 60, 61, 62,
    67, 68, 69, 70, 71, 72, 73, 74, 75,
    80, 81, 82, 83, 84, 85, 86, 87, 88,
    93, 94, 95, 96, 97, 98, 99,100,101,
    106,107,108,109,110,111,112,113,114,
    119,120,121,122,123,124,125,126,127,
    132,133,134,135,136,137,138,139,140,
    145,146,147,148,149,150,151,152,153
  };
/* The bounding box, keeps us from having to check array bounds on moves, just check
 * result for <0. */
static short _boundingBox[13*14] =
  {
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1, 0, 1, 2, 3, 4, 5, 6, 7, 8,-1,-1,
    -1,-1, 9,10,11,12,13,14,15,16,17,-1,-1,
    -1,-1,18,19,20,21,22,23,24,25,26,-1,-1,
    -1,-1,27,28,29,30,31,32,33,34,35,-1,-1,
    -1,-1,36,37,38,39,40,41,42,43,44,-1,-1,
    -1,-1,45,46,47,48,49,50,51,52,53,-1,-1,
    -1,-1,54,55,56,57,58,59,60,61,62,-1,-1,
    -1,-1,63,64,65,66,67,68,69,70,71,-1,-1,
    -1,-1,72,73,74,75,76,77,78,79,80,-1,-1,
    -1,-1,81,82,83,84,85,86,87,88,89,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
  };

/* how the pieces move in the above array */
static short _pieceMoves[8][8] =
  {
    {0,0,0,0,0,0,0,0},/* null */
    {-1,1,13,0,0,0,0,0}, /* pawn */
    {-1,1,-13,13,0,0,0,0}, /* cannon */
    {-1,1,-13,13,0,0,0,0}, /* cart */
    {-11,-15,-25,-27,11,15,25,27}, /* horse */
    {-28,-24,24,28,0,0,0,0}, /* elephant */
    {-12,-14,12,14,0,0,0,0}, /* advisor */
    {-1,-13,1,13,0,0,0,0}    /* king */
  };

Lawyer::Lawyer(Board *brd)
{
  /* once this is done, we should be able to pretend everything is the above arrays. */
  legalPositions = _legalPositions;
  pieceMasks = _pieceMasks;
  knightBlock = _knightBlock;
  elephantEye = _elephantEye;
  boundingBoxKey = _boundingBoxKey;
  boundingBox = _boundingBox;
  pieceMoves = _pieceMoves;

  board = brd;
}

bool Lawyer::drawn() // Tells us if the game is a draw.
{
  for (int i = 0; i < BOARD_AREA; i++)
    {
      switch (board->pieceAt(i))
        {
        case ZU:
        case PAO:
        case CHE:
        case MA:
          return false; // If we find any of these pieces it is not a draw.
        default: break;
        }
    }
  message = "Insufficient material";
  return true;
}

bool Lawyer::legalMove(Move &theMove) // This method checks to make sure the user has entered a legal move.
{
  bool currentlyInCheck = false;
  message = "";
  if (board->pieceAt(theMove.origin()) == EMPTY ||
      board->colorAt(theMove.origin()) != board->sideToMove() ||
      (board->pieceAt(theMove.destination()) != EMPTY &&
       board->colorAt(theMove.destination()) == board->sideToMove()))
    {
      return false;
    }

  // Very expensive, but very easy...basically generate all possible moves and see if the one we got
  // is in the list.
  list<Move>	moveList;
  bool		found = false;
  generateMoves(moveList);
  //cerr << "Number of moves: " << moveList.size() << endl;
  for (list<Move>::iterator it = moveList.begin(); it != moveList.end(); it++)
    {
      if ((*it).origin() == theMove.origin() && (*it).destination() == theMove.destination())
        found = true;
      //cerr << (*it) << endl;
    }
  if (!found) return false;

  // Now we check to see if we are in check after making the move - if so and we where in check then
  // the message is different than if we are simply moving into check.
  currentlyInCheck = inCheck();
  board->makeMove(theMove);
  board->makeNullMove();
  if (inCheck())
    {
      if (currentlyInCheck) message = "in check";
      else message = "moving into check";
      board->unmakeNullMove();
      board->unmakeMove();
      return false;
    }
  board->unmakeNullMove();
  board->unmakeMove();
  return true;
}

// Return true if the supplied color is in check
bool Lawyer::inCheck(int col)
{
  if (col == -1) col = board->sideToMove();
  // Find king...
  int kLoc = board->king((color)col);
  return underAttack(kLoc); // Is he under attack?
}

void dummy() {}
bool Lawyer::underAttack(int loc, int c)
{
  static int reverseKnightBlocks[] = { -8, -10, -8, -10, 8, 10, 8, 10 }; // Special set of offsets that work backwards.
  color opponent;
  if (c == -1)
      opponent = (board->colorAt(loc) == RED ? BLUE:RED);
  else
      opponent = (color)c;

  
  // linear attackers - R,C,P,K
  for (int i = 0; i < 4; i++)
    {
      int cannon = 0; // Cannons can jump.
      int times = 0;  // How many times we have gone through the loop...
      for (int x = boundingBoxKey[loc] + pieceMoves[CHE][i];
           boundingBox[x] > -1 && cannon < 2; x += pieceMoves[CHE][i])
        {
          piece currentPiece = board->pieceAt(boundingBox[x]);
          if (currentPiece != EMPTY)
            {
              if (board->colorAt(boundingBox[x]) == opponent) // It is an opponents piece
                {
                  if (!cannon) // We haven't jumped.
                    {
                      switch (currentPiece)
                        {
                        case ZU:
                          // If we are on the first loop, the piece under attack is in a legal position for enemy pawns,
                          // and the pawn isn't moving backwards return true.
                          if (times == 0 &&
                              (legalPositions[(opponent == RED ? 89-boundingBox[x]:boundingBox[x])] & pieceMasks[ZU]) &&
                              ((opponent == RED && i != 2) ||
                               (opponent == BLUE && i != 3)))
                            {
                              return true;
                            }
                          break;
                        case JIANG:
                          // If the piece under attack is a king (kings attack kings across the board) or we are on
                          // the first iteration then return true.
                          if (board->pieceAt(loc) == JIANG || i == 0)
                            {
                              return true;
                            }
                        case CHE: // Any time we have hit a cannon here it is an attack.
                          return true;

                        default: break;
                        }
                    }
                  else if (currentPiece == PAO) // We have jumped and hit a cannon - attack
                    {
                      return true; 
                    }
                }
              cannon ++; // We have hit a piece and not returned - incriment cannon so we can check for cannon attacks
            }
          times++; // inc times for pawn and king checks.
        }
    }
  // Knights
  for (int i = 0; i < 8; i++) // Iterate through the offsets and see if there is a non-blocked knight there.
    {
      int origin = boundingBox[boundingBoxKey[loc] + pieceMoves[MA][i]];
      if (origin > -1 && board->pieceAt(origin) == MA &&
          board->colorAt(origin) == opponent && board->pieceAt(loc + reverseKnightBlocks[i]) == EMPTY)
        {
          return true;
        }
    }
  // Diaganal attackers - E, A
  if (legalPositions[loc] & pieceMasks[XIANG]) // Iterate through the corners and look for a non-blocked elephant.
    {
      for (int i = 0; i < 4; i++)
        {
          int origin = boundingBox[boundingBoxKey[loc] + pieceMoves[XIANG][i]];
          if (origin > -1 && board->pieceAt(origin) == XIANG &&
              board->colorAt(origin) == opponent && board->pieceAt(loc + elephantEye[i]) == EMPTY)
            {
              return true;
            }
        }
    }
  if (legalPositions[loc] & pieceMasks[SHI]) // The guard can reach the piece under attack.
    {
      for (int i = 0; i < 4; i++) // iterate throught the corners and look for an enemy guard.
        {
          int origin = boundingBox[boundingBoxKey[loc] + pieceMoves[SHI][i]];
          if (origin > -1 && board->pieceAt(origin) == SHI
              && board->colorAt(origin) == opponent)
            {
              return true;
            }
        }
    }
  return false; // If we haven't returned true it means we haven't found an attacker.
}

void Lawyer::generateMoves(list<Move> &moveList, int loc, bool onlyLegal)
{
  switch (board->pieceAt(loc))
    {
    case ZU:
      generatePawnMoves(loc,moveList, onlyLegal);
      break;
    case PAO:
      generateCanonMoves(loc,moveList, onlyLegal);
      break;
    case CHE:
      generateRookMoves(loc,moveList, onlyLegal);
      break;
    case MA:
      generateKnightMoves(loc,moveList, onlyLegal);
      break;
    case XIANG:
      generateElephantMoves(loc,moveList, onlyLegal);
      break;
    case SHI:
      generateGuardMoves(loc,moveList, onlyLegal);
      break;
    case JIANG:
      generateGeneralMoves(loc,moveList, onlyLegal);
      break;
    default: break;
    }
}
      
void Lawyer::generateMoves(list<Move> &moveList, bool onlyLegal)
{
  for (int i = 1; i < 8; i++) // iterate through piece types
    {
      // Get this colors piece locations for the piece in question
      vector<int> locations = board->pieces(board->sideToMove(), (piece)i);
      for (vector<int>::iterator it = locations.begin(); // iterate through the locations and generate moves for that piece.
           it != locations.end();
           it++)
        {
          switch (i)
            {
            case ZU:
              generatePawnMoves(*it,moveList, onlyLegal);
              break;
            case PAO:
              generateCanonMoves(*it,moveList, onlyLegal);
              break;
            case CHE:
              generateRookMoves(*it,moveList, onlyLegal);
              break;
            case MA:
              generateKnightMoves(*it,moveList, onlyLegal);
              break;
            case XIANG:
              generateElephantMoves(*it,moveList, onlyLegal);
              break;
            case SHI:
              generateGuardMoves(*it,moveList, onlyLegal);
              break;
            case JIANG:
              generateGeneralMoves(*it,moveList, onlyLegal);
              break;
            }
        }
    }
}

void Lawyer::addMove(list<Move> &moveList, Move &theMove, bool onlylegal) // Add a move to the move list.
{
  if (!onlylegal) // If we are generating pseudo-legal moves then just add the move - don't check a damn thing.
    {
      moveList.push_back(theMove);
      return;
    }

  // Otherwise we need to be sure it doesn't put us in check - doesn't matter why.
  board->makeMove(theMove);
  board->makeNullMove();
  if (!inCheck())
    {
      moveList.push_back(theMove);
    }
  board->unmakeNullMove();
  board->unmakeMove();
}
/*
 * It may be less effecient as far as code sharing goes to split these into several methods,
 * but for debugging purposes it has become insain to try and figure out what is going on
 * when a certain piece is exhibiting strange behavior.  If these are split into seperate
 * methods then the breakpoint can be set for the method for that piece.  Also, this may
 * improve performance as some of the hackery involved in merging these methods may not
 * be particularly good. */
void Lawyer::generatePawnMoves(int from, list<Move> &moveList, bool onlyLegal)
{
  static int offsets[] = { -1, 13, 1 };
  int colorModifier = board->colorAt(from) == RED ? -1:1; // Invert offsets if we are red.
  for (int i = 0; i < 3; i++) // iterate through offsets.
    {
      int dest = boundingBox[boundingBoxKey[from] + (offsets[i] * colorModifier)]; // generate a location
      int legalIndex = colorModifier == -1 ? (89-dest):dest; // make sure it is legal to have a pawn here.
      if (dest > 0 && (legalPositions[legalIndex] & pieceMasks[ZU]))
        {
          if (board->pieceAt(dest) == EMPTY || board->colorAt(dest) != board->colorAt(from))
            {
              // Legal location and the destination is empty or has an enemy piece in it.
              Move move(from, dest);
              addMove(moveList, move, onlyLegal);
            }
        }
    }
}
void Lawyer::generateCanonMoves(int from, list<Move> &moveList, bool onlyLegal)
{
  static int offsets[] = { -1, -13, 13, 1 };
  for (int i = 0; i < 4; i++) // hop through offsets
    {
      int  offset = offsets[i]; // This offset will be incremented.
      bool jumped = false;
      for (int testNum = 0; testNum < 9; testNum++, offset += offsets[i]) // do up to 9 tests and increment offset
        {
          int dest = boundingBox[boundingBoxKey[from] + offset]; // generate a destination
          if (dest < 0) break; // off the board.
          if (board->pieceAt(dest) != EMPTY) // Square is occupied
            {
              if (jumped) // we can only make this move if we have jumped.
                {
                  if (board->colorAt(dest) != board->colorAt(from)) // we can only make this move if taking an enemy.
                    {
                      Move move(from, dest);
                      addMove(moveList, move, onlyLegal);
                    }
                  break;
                }
              else jumped = true;
            }
          else if (!jumped) // Empty squares can only be moved to if we have not jumped.
            {
              Move move(from,dest);
              addMove(moveList, move, onlyLegal);
            }
        }
    }
}
void Lawyer::generateRookMoves(int from, list<Move> &moveList, bool onlyLegal)
{
  static int offsets[] = { -1, -13, 13, 1 };
  for (int i = 0; i < 4; i++) // hop through offsets
    {
      int offset = offsets[i]; // incementable offset
      for (int testNum = 0; testNum < 9; testNum++, offset += offsets[i]) // 9 tests
        {
          int dest = boundingBox[boundingBoxKey[from] + offset]; // generate destination
          if (dest < 0) break; // off the board
          if (board->pieceAt(dest) != EMPTY) // occupied
            {
              if (board->colorAt(from) != board->colorAt(dest)) // enemy
                {
                  Move move(from,dest); // ok.
                  addMove(moveList, move, onlyLegal);
                }
              break;
            }
          else
            {
              Move move(from,dest); // unoccupied - ok.
              addMove(moveList, move, onlyLegal);
            }
        }
    }
}
void Lawyer::generateKnightMoves(int from, list<Move> &moveList, bool onlyLegal)
{
  static int offsets[] = {-11, -15, -25, -27, 11, 15, 25, 27 }; // moves
  static int blocks[]  = {  1,  -1,  -9,  -9, -1,  1,  9,  9 }; // if these are occupied we can't make the move to
								// the corresponding offset.
  for (int i = 0; i < 8; i++)
    {
      int dest = boundingBox[boundingBoxKey[from] + offsets[i]];
      if (dest > 0 && board->pieceAt(from + blocks[i]) == EMPTY) // if on the board and the block square is empty
        {
          if (board->pieceAt(dest) == EMPTY || board->colorAt(dest) != board->colorAt(from)) // unoccupied or enemy.
            {
              Move move(from, dest);
              addMove(moveList, move, onlyLegal);
            }
        }
    }
}
void Lawyer::generateElephantMoves(int from, list<Move> &moveList, bool onlyLegal)
{
  static int offsets[] = { -28, -24, 24, 28 }; // moves
  static int blocks[]  = { -10,  -8,  8, 10 }; // same as per knight.
  for (int i = 0; i < 4; i++)
    {
      int dest = boundingBox[boundingBoxKey[from] + offsets[i]];
      int legalIndex = board->colorAt(from) == RED ? (89-dest):dest;
      if (dest > 0 &&
          board->pieceAt(from + blocks[i]) == EMPTY &&
          (legalPositions[legalIndex] & pieceMasks[XIANG])) // On board, legal location, not blocked.
        {
          if (board->pieceAt(dest) == EMPTY || board->colorAt(from) != board->colorAt(dest)) // unoccupied or enemy
            {
              Move move(from,dest);
              addMove(moveList, move, onlyLegal);
            }
        }
    }
}
void Lawyer::generateGuardMoves(int from, list<Move> &moveList, bool onlyLegal)
{
  static int offsets[] = { -14, -12, 12, 14 }; // moves
  for (int i = 0; i < 4; i++)
    {
      int dest = boundingBox[boundingBoxKey[from] + offsets[i]];
      int legalIndex = board->colorAt(from) == RED ? (89-dest):dest;
      if (dest > 0 && (legalPositions[legalIndex] & pieceMasks[SHI])) // on board and legal location
        {
          if (board->pieceAt(dest) == EMPTY || board->colorAt(from) != board->colorAt(dest)) // unoccupied or enemy
            {
              Move move(from,dest);
              addMove(moveList, move, onlyLegal);
            }
        }
    }
}
void Lawyer::generateGeneralMoves(int from, list<Move> &moveList, bool onlyLegal)
{
  static int offsets[] = { -1, -13, 1, 13 }; // offsets
  int kingLineOffset = board->colorAt(from) == RED ? -13:13; // offset for checking for king-face capture
  for (int i = 0; i < 4; i++)
    {
      int dest = boundingBox[boundingBoxKey[from] + offsets[i]];
      int legalIndex = board->colorAt(from) == RED ? (89-dest):dest; 
      if (dest > 0 && (legalPositions[legalIndex] & pieceMasks[JIANG])) // on board and legal location.
        {
          if (board->pieceAt(dest) == EMPTY || board->colorAt(from) != board->colorAt(dest))
            {
              Move move(from,dest);
              addMove(moveList, move, onlyLegal);
            }
        }
    }

  // Check if we can capture the enemy king - never actually happens but is used during the search to weed out
  // illegal moves
  for (int j = 1; j < 10; j++) // iterate up the file and see if we find a king...
    {
      int dest = boundingBox[boundingBoxKey[from] + (j*kingLineOffset)];
      if (dest < 0) break; // off the board
      if (board->pieceAt(dest) != EMPTY) // occupied
        {
          if (board->pieceAt(dest) == JIANG && board->colorAt(from) != board->colorAt(dest)) // occupied by enemy king!
            {
              Move move(from,dest); // add capture.
              addMove(moveList, move, onlyLegal);
            }
          break; // kings have to directly face across an empty file.
        }
    }
}

int Lawyer::gameWonByChase()
{
  vector< Move > moveHistory = board->history();
  vector< Move > undoHistory;
  int size = positionalHistory.size();
  if (size < 5) return NOCOLOR;
  bool possible = false;

  // First look for perpetual chase because it is easier to look for...
  /*
   * Rules: If Player X attacks Y's piece at location A using its piece at location C, and player Y runs to location B,
   *        and player X chases Y's piece at B by moving from C to D, and player Y runs back to A, and player X
   *        again chases player Y's piece at A by moving back to location C...and player Y's piece is not protected
   *        in neither A nor B, then player X looses by moving back to C.
   */
  if (positionalHistory[size - 1] == positionalHistory[size - 5] && // current position matches 4 past...possible problem.
      moveHistory[size-1].origin() == moveHistory[size - 3].destination() &&
      moveHistory[size-3].origin() == moveHistory[size - 5].destination() &&
      moveHistory[size-5].destination() == moveHistory[size-1].destination() &&
      moveHistory[size-2].origin() == moveHistory[size - 4].destination())
    {
      list<Move> atks;
      board->makeNullMove();
      generateMoves(atks, moveHistory[size-1].destination());
      board->unmakeNullMove();
      int locA = -1;
      int locB = -1;
      possible = true;

      // The last piece we moved is being attacked by the last piece the opponent moved.
      locA = moveHistory[size-2].destination();
      locB = moveHistory[size-2].origin();
      possible = false;
      for (list<Move>::iterator it = atks.begin(); it != atks.end(); it++)
        if (it->destination() == locA) { possible = true; break; }
      if (possible == true)
        {
          // Check if we are protected by any piece...
          if (underAttack(locA, board->colorAt(locA)))
            {
              possible = false;
            }
          if (possible == true)
            {
              // Check if the same piece was being attacked by the same piece in the previous turn.
              undoHistory.push_back(moveHistory[size-1]); undoHistory.push_back(moveHistory[size-2]);
              board->unmakeMove(); board->unmakeMove();
              
              atks.clear();
              board->makeNullMove();
              generateMoves(atks, board->history()[size-3].destination());
              board->unmakeNullMove();
              possible = false;
              for (list<Move>::iterator it = atks.begin(); it != atks.end(); it++)
                if (it->destination() == locB) { possible = true; break; }
              if (possible == true)
                {
                  if (underAttack(locB, board->colorAt(locB)))
                    {
                      possible = false;
                    }
                }
            }
        }
    }
  // cleanup...
  for (vector<Move>::reverse_iterator it = undoHistory.rbegin(); it != undoHistory.rend(); it++)
    board->makeMove(*it);
  if (possible) { return board->sideToMove();}
 
  return NOCOLOR;
}

int Lawyer::gameWonByPCheck()
{
  /* RULE: If we have been put into check by the opponent moving the same piece 3 times in a row, and the material is the
           same - we win. */
  vector<Move> hist = board->history();
  int size = hist.size();
  if (size < 6) return NOCOLOR;

  Evaluator *eval = Evaluator::defaultEvaluator();
  long material = eval->evaluateMaterial(*board);

  vector<Move> undoHist;
  if (inCheck())
    {
      if (hist[size-1].origin() == hist[size-3].destination() &&
          hist[size-3].origin() == hist[size-5].destination())
        {
          bool possible = true;
          for (int i = 0; i < 3 && possible; i++)
            {
              undoHist.push_back(board->history()[board->history().size() - 1]);
              board->unmakeMove();
              undoHist.push_back(board->history()[board->history().size() - 1]);
              board->unmakeMove();

              if (eval->evaluateMaterial(*board) != material ||
                  !inCheck())
                possible = false;
            }
          for (vector<Move>::reverse_iterator it = undoHist.rbegin(); it != undoHist.rend(); it++)
            {
              board->makeMove(*it);
            }
          if (possible) return board->sideToMove();
        }
    }
  return NOCOLOR;
}

void Lawyer::boardChanged(Board *brd, int message)
{
  switch(message)
    {
    case BOARD_ALTERED:
      positionalHistory.clear();
      break;
    case MOVE_MADE:
      positionalHistory.push_back(make_pair(brd->primaryHash(), brd->secondaryHash()));
      break;
    case MOVE_UNDONE:
      positionalHistory.pop_back();
      break;
    }
}
