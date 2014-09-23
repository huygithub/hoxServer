#include	"Board.h"
#include	"Move.h"

#include	<string>
#include	<cctype>
#include	<cstdlib>
#include	<ctime>
#include	<cstring>

#include	<iostream>
#include	<map>


using namespace std;
/*
static
string defaultPosition("RHEAKAEHR/9/1C5C1/P1P1P1P1P/9/9/p1p1p1p1p/1c5c1/9/rheakaehr r");
*/
static
string defaultPosition("rheakaehr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RHEAKAEHR r");
static
const char pieceChars[] = {'+', 'p','c','r','h','e','a','k' };

// Generates a full int random number...
static u_int32 _rand32()
{
  u_int32 retVal;

  retVal = (rand() << 16) ^ (rand());

  return retVal & 0x7FFFFFFF; // 32nd bit reserved for color.
}

// Static variables...
u_int32 Board::hashValues[2][90][15];
bool Board::hashValuesFilled = false;


Board::Board()
{
  _primaryHash = _secondaryHash = 0;
  if (!hashValuesFilled)
    {
      generateValues();
      hashValuesFilled = true;
    }
  setPosition(defaultPosition);
}
Board::Board(string fen)
{
  _primaryHash = _secondaryHash = 0;
  generateValues();
  setPosition(fen);
}

// Fills hash key values...
void Board::generateValues()
{
  map<u_int32, u_int32> repeatCheckMap;
  srand( (unsigned int) time(NULL) );
  for (int w = 0; w < 2; w++)
    {
      for (int i = 0; i < 90; i++)
        {
          for (int j = 0; j < 15; j++)
            {
              u_int32 x = 0;
              do
                {
                  x = _rand32();
                }
              while (repeatCheckMap.find(x) != repeatCheckMap.end() || x == 0);
              hashValues[w][i][j] = x;
              repeatCheckMap.insert(map<u_int32,u_int32>::value_type(x,x));
            }
        }
    }
}

// Piece management

// Removes piece at loc from the appropriate index...
void Board::removePiece(int loc)
{
  vector<int>::iterator it;
  vector< vector<int> > *pieces = (colorAt(loc)==RED? &rpieces:&bpieces);
  for (it = (*pieces)[pieceAt(loc) - 1].begin();
       it != (*pieces)[pieceAt(loc) - 1].end();
       it++)
    if (*it == loc)
      {
        (*pieces)[pieceAt(loc) - 1].erase(it);
        break;
      }
}

// Adds the piece at loc to the appropriate index...
void Board::addPiece(int loc)
{
  if (colorAt(loc) == RED)
    rpieces[pieceAt(loc) - 1].push_back(loc);
  else
  bpieces[pieceAt(loc) - 1].push_back(loc);
}

// Move the piece at origin to dest in piece indexes.
void Board::movePiece(int origin, int dest)
{
  vector<int>::iterator it;
  vector< vector<int> > *pieces = (colorAt(origin)==RED? &rpieces:&bpieces);
  for (it = (*pieces)[pieceAt(origin) - 1].begin();
       it != (*pieces)[pieceAt(origin) - 1].end();
       it++)
    if (*it == origin)
      {
        *it = dest;
        break;
      }
}

// Return the piece index array for the specified color and piece type...
vector<int> Board::pieces(color c, piece p)
{
  if (c == RED)
    return rpieces[p - 1];
  else
    return bpieces[p - 1];
}

// Set up a piece index vector...
static void setupVector(vector< vector<int> > &v)
{
  for (int i = 0; i < 7; i++) v.push_back(vector<int>());
}

// Read fen notation and distribute pieces on board and indexes...
bool Board::setPosition(string fen)
{
  string::iterator fen_p = fen.begin();

  // Set up temporary board tracking variables to fill in case an error causes
  // us to abort.  That way we will be in a useful state in such a case.
  char tempBoard[BOARD_AREA];
  int board_i = 0, board_j = 0;
  u_int32 t_primaryHash = 0, t_secondaryHash = 0;
  vector< vector<int> > t_bpieces;
  vector< vector<int> > t_rpieces;

  setupVector(t_rpieces); setupVector(t_bpieces);

  // Iterate through the fen string as long as we are still on the board.
  while (fen_p != fen.end() && (board_i) * 9 + board_j < BOARD_AREA)
    {
      int row = (board_i) * 9;
      if (*fen_p == '/') // '/' means next line
        {
          if (board_j != 9) return false;
          board_j = 0;
          board_i++;
        }
      else if (isdigit(*fen_p)) // digits represent a count of sequensial empty squares...
        {
          int num = (*fen_p) - '0';
          if (num + board_j > BOARD_WIDTH) return false;
          for (int i = 0; i < num; i++)
            {
              tempBoard[row + (board_j)] = EMPTY;
              board_j++;
            }
        }
      else
        {
          switch (*fen_p) // Any other character is either a piece or invalid...
            {
            case 'p':
              tempBoard[row+(board_j)] = ZU | BLUE;
              break;
            case 'c':
              tempBoard[row+(board_j)] = PAO | BLUE;
              break;
            case 'r':
              tempBoard[row+(board_j)] = CHE | BLUE;
              break;
            case 'h':
              tempBoard[row+(board_j)] = MA | BLUE;
              break;
            case 'e':
              tempBoard[row+(board_j)] = XIANG | BLUE;
              break;
            case 'a':
              tempBoard[row+(board_j)] = SHI | BLUE;
              break;
            case 'k':
              tempBoard[row+(board_j)] = JIANG | BLUE;
              kings[0]=row+board_j;
              break;
            case 'P':
              tempBoard[row+(board_j)] = ZU | RED;
              break;
            case 'C':
              tempBoard[row+(board_j)] = PAO | RED;
              break;
            case 'R':
              tempBoard[row+(board_j)] = CHE | RED;
              break;
            case 'H':
              tempBoard[row+(board_j)] = MA | RED;
              break;
            case 'E':
              tempBoard[row+(board_j)] = XIANG | RED;
              break;
            case 'A':
              tempBoard[row+(board_j)] = SHI | RED;
              break;
            case 'K':
              tempBoard[row+(board_j)] = JIANG | RED;
              kings[1] = row+board_j;
              break;
            default:
              return false;
            }
          // Add the piece to the hash key
          t_primaryHash ^= hashValues[0][row+board_j][(size_t)tempBoard[row+board_j]];
          t_secondaryHash ^= hashValues[1][row+board_j][(size_t)tempBoard[row+board_j]];

          // Add the piece to the piece index tables...
          if ((tempBoard[row+board_j]&8) == RED)
            t_rpieces[(tempBoard[row+board_j]&7) - 1].push_back(row+board_j);
          else if ((tempBoard[row+board_j]&8) == BLUE)
            t_bpieces[(tempBoard[row+board_j]&7) - 1].push_back(row+board_j);
          
          board_j++;
        }
      fen_p++;
    }
  if (*fen_p != ' ') return false; // Position must be followed by a space and then...
  fen_p++;
  /* FEN notation: 
   *    http://www.wxf.org/xq/computer/fen.pdf
   *    http://www.wxf.org/xq/computer/wxf_notation.html
   */
  if (   *fen_p == 'w'
      || *fen_p == 'r' ) _sideToMove = RED; // A character representing the side to move.
  else if (*fen_p == 'b') _sideToMove = BLUE;
  else return false;

  // Assuming we have not aborted, we now know we have processed a valid fen notation string...
  // Initialize actual variables with contents of temporaries and clear game tracking variables.
  memcpy(board, tempBoard, BOARD_AREA);
  moveHistory.clear();
  _startPos = fen;
  _gameOver = false;
  _primaryHash = t_primaryHash;
  _secondaryHash = t_secondaryHash;
  u_int32 colorSet = (_sideToMove << 28);
  _primaryHash |= colorSet;
  _secondaryHash |= colorSet;
  rpieces.clear(); bpieces.clear();
  for (int i = 0; i < 7; i ++)
    {
      rpieces.push_back(t_rpieces[i]);
      bpieces.push_back(t_bpieces[i]);
    }
  notifyObservers(BOARD_ALTERED);

  return true; // Return true.
}

// Set up default starting position
void Board::resetBoard()
{
  setPosition(defaultPosition);
}

// Retrieve fen notation based on board layout...
string Board::getPosition()
{
  string position("");

  for (int i = 0; i < BOARD_HEIGHT; i++) // Iterate across the board from a9 to i0.
    {
      int spaceCount = 0;
      bool inSpace = false;

      for (int j = 0; j < BOARD_WIDTH; j++)
        {
          unsigned char piece = board[9*(i) + j];
          unsigned char color = piece & 8;
          piece &= 7;

          if (piece == EMPTY) // We count these...
            {
              inSpace = true;
              spaceCount++;
            }
          else
            {
              if (inSpace) // If we where counting spaces we now insert the number of them.
                {
                  position += (char)(spaceCount + '0');
                  inSpace = false;
                  spaceCount = 0;
                }
              if (color == RED) position += toupper(pieceChars[piece]);
              else position += pieceChars[piece];
            }
        }
      if (inSpace) // If we have hit the end of the row and are counting spaces we need to insert that number now.
        {
          position += (char)(spaceCount + '0');
          inSpace = false;
          spaceCount = 0;
        }
      if (i < BOARD_HEIGHT-1)
        position += '/';
    }
  position += ' ';
  position += (char)(_sideToMove == RED ? 'r':'b'); // save side to move.

  return position;
}

// Display the board in ascii characters...
ostream& operator<<(ostream& out, Board& theBoard)
{
  unsigned char location = 0;
  for (int i = 0; i < BOARD_HEIGHT; i++)
    {
      for (int j = 0; j < BOARD_WIDTH; j++)
        {
          location = theBoard.board[i*9 + j];
          if (j)
            {
              out << "---";
            }
          else
            {
              out << " " << BOARD_HEIGHT - (i+1) << " ";
            }
          if (location & 8) out << (char)toupper(pieceChars[location&7]);
          else out << pieceChars[location&7];
        }
      out << endl;
      if (i < 9)
        {
          for (int k = 0; k < BOARD_WIDTH; k++)
            {
              if (k)
                {
                  out << ' ';
                  if (k == 4)
                    {
                      if (i == 0 || i == 7)
                        out << '\\';
                      else if (i == 1 || i == 8)
                        out << '/';
                      else out << ' ';
                    }
                  else if (k == 5)
                    {
                      if (i == 0 || i == 7)
                        out << '/';
                      else if (i == 1 || i == 8)
                        out << '\\';
                      else out << ' ';
                    }
                  else
                    out << ' ';
                  out << ' ';
                }
              else out << "   ";
              if (i == 4)
                {
                  if (k == 0 || k == 8)
                    out << '|';
                  else out << ' ';
                }
              else
                out << '|';
            }
          out << endl;
        }
      else
        {
          for (int j = 0; j < BOARD_WIDTH; j++)
            out << "   " << (char)('a' + j);
          out << endl;
        }
    }
  return out;
}

void Board::makeMove(Move &theMove)
{
  if (!(theMove.origin() == theMove.destination()))
    {
      //std::cerr << "Making move " << theMove << "\n";
      alterHashes(theMove.origin());
      if (board[theMove.destination()])
        {
          alterHashes(theMove.destination());
          removePiece(theMove.destination());
        }
      if (pieceAt(theMove.origin()) == JIANG)
        kings[colorAt(theMove.origin())==RED?1:0] = theMove.destination();
      //else
      movePiece(theMove.origin(), theMove.destination());
      theMove.capturedPiece(board[theMove.destination()]);
      board[theMove.destination()] = board[theMove.origin()];
      board[theMove.origin()] = 0;
      //alterHashes(theMove.origin());
      alterHashes(theMove.destination());
    }
  moveHistory.push_back(theMove);
  
  
  _sideToMove    ^= RED;
  _primaryHash   ^= COLOR_SWITCH_KEY;
  _secondaryHash ^= COLOR_SWITCH_KEY;
  notifyObservers(MOVE_MADE);
}

void Board::unmakeMove() // unmakes the top move in moveHistory.
{
  Move theMove = moveHistory.back();
  moveHistory.pop_back();
  if (!(theMove.origin() == theMove.destination()))
    {
      //std::cerr << "Unmaking move " << theMove << "\n";
      //alterHashes(theMove.origin());
      alterHashes(theMove.destination());
      if (pieceAt(theMove.destination()) == JIANG)
        kings[colorAt(theMove.destination())==RED?1:0] = theMove.origin();
      //else
      movePiece(theMove.destination(), theMove.origin());
      board[theMove.origin()] = board[theMove.destination()];
      board[theMove.destination()] = theMove.capturedPiece();
      alterHashes(theMove.origin());
      if (board[theMove.destination()])
        {
          alterHashes(theMove.destination());
          addPiece(theMove.destination());
        }
    }
  
  _sideToMove    ^= RED;
  _primaryHash   ^= COLOR_SWITCH_KEY;
  _secondaryHash ^= COLOR_SWITCH_KEY;
  notifyObservers(MOVE_UNDONE);
}

void Board::notifyObservers(int message)
{
  for (vector<BoardObserver*>::iterator it = observers.begin();
       it != observers.end();
       it++)
    (*it)->boardChanged(this, message);
}
