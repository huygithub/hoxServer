#ifndef	__OPENINGBOOK_H__
#define	__OPENINGBOOK_H__

/*
 * OpeningBook.h (c) Noah Roberts 2003-03-24
 * In charge of giving the engine some hints on what to play in the beginning of the game.
 * Provides adiquate opening play since the engine is unable to strategize.  Also gives play
 * that is not redundant - ie it won't always respond with the same move every damn time.
 */

class Board;
class Move;

#include <string>
#include <map>
#include <vector>

typedef unsigned short u_int16;

typedef std::vector< u_int16 > BookEntry;
typedef std::map< std::string, BookEntry > Book;

class OpeningBook
{
private:
    Book*  _bookContents;
    bool   _validBook;

    void   _read(std::string filename);

public:
    OpeningBook(std::string filename);
    ~OpeningBook();
    u_int16 getMove(Board *board);

    bool valid() { return _validBook; }
};

#endif	/* __OPENINGBOOK_H__ */
