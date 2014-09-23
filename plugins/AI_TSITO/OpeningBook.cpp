#include "OpeningBook.h"
#include "Board.h"

#include <fstream>
#include <iostream>
#include <sstream>  // ... istringstream
#include <ctime>
#include <cstdlib>

using namespace std;

OpeningBook::OpeningBook(std::string filename)
    : _bookContents( new Book )
{ 
    _read(filename);
    srand( (unsigned int)time(NULL) );
}

OpeningBook::~OpeningBook()
{
    delete _bookContents;
}

void
OpeningBook::_read(std::string filename)
{
    ifstream	bookFile(filename.c_str(), ios::in);

    if (!bookFile)
    {
        cerr << "Can't open " << filename << endl;
        _validBook = false;
        return;
    }

    int nline = 0;
    char buffer[256];
    bookFile.getline(buffer,255);
    while (!bookFile.eof())
    {
        nline++;
        string line(buffer);

        size_t indexOfColon = line.find(':');
        if (indexOfColon == string::npos)
        {
            cerr << "Illegal book entry at line " << nline << endl;
        }
        else
        {
            string position = line.substr(0,indexOfColon);
            string moveTexts = line.substr(indexOfColon+1);
            moveTexts += " ";
            std::istringstream movesStream(moveTexts);

            string move;
            vector<u_int16> moves;
            movesStream >> move;
            while (!movesStream.eof())
            {
                u_int16	origin = 0, dest = 0;

                Move m(move);
                origin = (u_int16)m.origin(); dest = (u_int16)m.destination();
                if (origin == 0 && dest == 0);
                else
                {
                    moves.push_back((origin << 8) | dest);
                }
                movesStream >> move;
            }
            _bookContents->insert(Book::value_type(position, moves));
        }
        bookFile.getline(buffer,255);
    }

    _validBook = true;
}



u_int16 OpeningBook::getMove(Board *board)
{
  u_int16 move = 0;

  Book::iterator it = _bookContents->find(board->getPosition());

  if (it != _bookContents->end()) move = it->second[rand() % it->second.size()];
  return move;
}
