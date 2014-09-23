#ifndef __MOVE_H__
#define __MOVE_H__

/*
 * Move.h (c) Noah Roberts 2003-02-23
 * Class Move: holds all data necisary to represent a single move in XiangQi.
 */

#include <iostream>
#include <string>

class Move
{
private:
    int            _origin;
    int            _destination;
    unsigned char  _capturedPiece;

    static Move    _null;

public:
    Move();
    Move(int o, int d, unsigned char cP = 0);
    Move( const std::string& moveText );

    bool isCapture() const { return _capturedPiece != 0; } 
    int origin() const { return _origin; }
    int destination() const { return _destination; }
    unsigned char capturedPiece() const { return _capturedPiece; }

    void origin(int o) { _origin = o; }
    void destination(int d) { _destination = d; }
    void capturedPiece(unsigned char cP) { _capturedPiece = cP; }

    std::string getText() const;
    friend std::ostream& operator<<(std::ostream& out, const Move &move);
    
    bool operator==(const Move &m2) const
    {
        return (   _origin       == m2._origin
                 && _destination == m2._destination );
    }

public:
    static Move& nullMove() { return _null; } // The null move.
};

#endif /* __MOVE_H__ */
