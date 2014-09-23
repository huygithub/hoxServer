#include	"Move.h"

/*
 * Move.cpp (c) Noah Roberts 2003-02-27
 * << operator and string parser for Move class.
 */


#define	ERROR_OUT { _origin = 0; _destination = 0; _capturedPiece = 0; return; }

Move Move::_null; // The null move.

std::ostream& operator<<(std::ostream& out, const Move &move)
{
  return out << move.getText();
}

// ----------------------------------------------------------------
//               Move                                             
// ----------------------------------------------------------------

Move::Move()
    : _origin( 0 )
    , _destination( 0 )
    , _capturedPiece( 0 )
{
}

Move::Move(int o, int d, unsigned char cP /* = 0 */ )
    : _origin( o )
    , _destination( d )
    , _capturedPiece( cP )
{
}

/**
 * Scan and create a move out of a string.  Depends on ASCII layout...
 */
Move::Move( const std::string& moveText)
{
    std::string::const_iterator it = moveText.begin();

    // ---- origin

    // file
    if (moveText.length() != 4) ERROR_OUT;
    if (*it >= 'a' && *it <= 'i') // between a and i
        _origin = *it++ - 'a';
    else if (*it >= 'A' && *it <= 'I') // capital version
        _origin = *it++ - 'A';
    else ERROR_OUT;

    // rank
    if (*it < '0' || *it > '9') ERROR_OUT;
    _origin += (9 - (*it++ - '0')) * 9;

    // ---- destination

    // file
    if (*it >= 'a' && *it <= 'i')
        _destination = *it++ - 'a';
    else if (*it >= 'A' && *it <= 'I')
        _destination = *it++ - 'A';
    else ERROR_OUT;
  
    // rank
    if (*it < '0' || *it > '9') ERROR_OUT;
    _destination += (9 - (*it++ - '0')) * 9;
}

std::string
Move::getText() const // Translate into algebraic notation.
{
    std::string moveText;

    moveText += (char)(_origin % 9 + 'a');
    moveText += (char)(9 - (_origin / 9) + '0');
    moveText += (char)(_destination % 9 + 'a');
    moveText += (char)(9 - (_destination / 9) + '0');

    return moveText;
}

////////////////////////////////// END OF FILE ////////////////////////////
