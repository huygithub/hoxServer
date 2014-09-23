#include <cstring>
#include <vector>
using namespace std;
#include "xq.h"

inline uint char_type(sint32 c)
{
    switch(c)
    {
    case 'K':
        return RedKing;
    case 'k':
        return BlackKing;
    case 'G':
    case 'A':
        return RedAdvisor;
    case 'g':
    case 'a':
        return BlackAdvisor;
    case 'B':
    case 'E':
        return RedBishop;
    case 'b':
    case 'e':
        return BlackBishop;
    case 'R':
        return RedRook;
    case 'r':
        return BlackRook;
    case 'H':
    case 'N':
        return RedKnight;
    case 'h':
    case 'n':
        return BlackKnight;
    case 'C':
        return RedCannon;
    case 'c':
        return BlackCannon;
    case 'P':
        return RedPawn;
    case 'p':
        return BlackPawn;
    default:
        return InvaildPiece;
    }
}
inline sint type_char(uint32 type)
{
    switch(type)
    {
    case RedKing:
        return 'K';
    case BlackKing:
        return 'k';
    case RedAdvisor:
        return 'A';
    case BlackAdvisor:
        return 'a';
    case RedBishop:
        return 'B';
    case BlackBishop:
        return 'b';
    case RedRook:
        return 'R';
    case BlackRook:
        return 'r';
    case RedKnight:
        return 'N';
    case BlackKnight:
        return 'n';
    case RedCannon:
        return 'C';
    case BlackCannon:
        return 'c';
    case RedPawn:
        return 'P';
    case BlackPawn:
        return 'p';
    default:
        return 0;
    }
}
inline sint piece_char(uint piece)
{
    return type_char(piece_type(piece));
}

void XQ::clear()
{
    m_bitmap.reset();
    memset(m_pieces, InvaildSquare, 34);
    memset(m_squares, EmptyIndex, 90);
    m_squares[90] = InvaildIndex;
    m_player = Empty;
}
XQ::XQ(string const &fen)
{
    clear();
    uint idx = 0UL, len = (uint)fen.size();
    uint y = 9UL, x = 0UL;
    while(idx < len)
    {
        sint32 c = fen[idx++];
        uint32 type = char_type(c);
        if (type != InvaildPiece)
        {
            if (y >= 10UL || x >= 9UL)
            {
                //error
                clear();
                return;
            }
            uint32 begin = type_begin(type);
            uint32 end = type_end(type);
            while (end >= begin)
            {
                if (piece(begin) == InvaildSquare)
                {
                    break;
                }
                ++begin;
            }
            if (begin > end)
            {
                //error
                clear();
                return;
            }
            uint32 sq = xy_square(x++, y);
            m_squares[sq] = begin;
            m_pieces[begin] = sq;
        }
        else if (c == ' ')
        {
            if (y || x != 9UL || (idx == len))
            {
                //error
                clear();
                return;
            }
            c = fen[idx];
            m_player = ((c == 'b') ? Black : Red);
            for (int i = 0; i < 32; i++)
            {
                uint32 sq = piece(i);
                if (sq != InvaildSquare)
                {
                    if (!(square_flag(sq) & piece_flag(i)))
                    {
                        //error
                        clear();
                        return;
                    }
                    m_bitmap.setbit(sq);
                }
            }
            //ok
            return;
        }
        else if (c == '/')
        {
            if (!y || x != 9UL)
            {
                //error
                clear();
                return;
            }
            --y;
            x = 0UL;
        }
        else if (c >= '1' && c <= '9')
        {
            x += c - '0';
        }
        else
        {
            //error
            clear();
            return;
        }
    }
    //error
    clear();
}
XQ& XQ::operator=(const XQ &xq)
{
    m_bitmap = xq.m_bitmap;
    memcpy(m_pieces, xq.m_pieces, 34);
    memcpy(m_squares, xq.m_squares, 91);
    m_player = xq.m_player;
    return *this;
}
XQ::operator string() const
{
    if (m_player == Empty)
        return string();
    vector<char> chars;
    for (sint y = 9; y >= 0; --y)
    {
        if(y != 9)
            chars.push_back('/');
        sint32 empty_count = 0;
        for (uint x = 0; x < 9; ++x)
        {
            uint sq = xy_square(x,y);
            sint c = piece_char(square(sq));
            if (c)
            {
                if (empty_count)
                {
                    chars.push_back('0' + empty_count);
                    empty_count = 0;
                }
                chars.push_back(c);
            }
            else
            {
                ++empty_count;
            }
        }
        if (empty_count)
        {
            chars.push_back('0' + empty_count);
        }
    }
    chars.push_back(' ');
    chars.push_back(m_player == Red ? 'r' : 'b');
    return string(chars.begin(), chars.end());
}

bool XQ::is_legal_move(uint32 src, uint32 dst) const
{
    if (src > 90 || dst > 90)
        return false;
    uint32 src_piece = square(src);
    if (piece_color(src_piece) == square_color(dst))
        return false;
    if (!(g_move_flags[dst][src] & piece_flag(src_piece)))
        return false;
    switch (piece_type(src_piece))
    {
    case RedKing:
        if (g_move_flags[dst][src] & RedPawnFlag)
            return true;
        return (square_flag(dst) & BlackKingFlag) && distance(src, dst) == 0;
    case BlackKing:
        if (g_move_flags[dst][src] & BlackPawnFlag)
            return true;
        return (square_flag(dst) & RedKingFlag) && distance(src, dst) == 0;
    case RedAdvisor:
    case BlackAdvisor:
        return true;
    case RedBishop:
    case BlackBishop:
        return square(bishop_eye(src, dst)) == EmptyIndex;
    case RedRook:
    case BlackRook:
        return distance_is_0(src, dst);
    case RedKnight:
    case BlackKnight:
        return square(knight_leg(src, dst)) == EmptyIndex;
    case RedCannon:
    case BlackCannon:
        return (distance(src, dst) + (square(dst) >> 5)) == 1;
    case RedPawn:
    case BlackPawn:
        return true;
    default:
        return false;
    }
}
uint XQ::status() const
{
    if (player_in_check(player()))
        return 1;
    return 0;
}
uint XQ::player_in_check(uint player) const
{
    if (player == Red)
    {
        uint kp = piece(RedKingIndex);
        assert(kp != InvaildSquare);
        return ((square_flag(square_up(kp))
            | square_flag(square_left(kp))
            | square_flag(square_right(kp)))
            & BlackPawnFlag)//pawn
            | ((square_flag(nonempty_down_1(kp))
            | square_flag(nonempty_up_1(kp))
            | square_flag(nonempty_left_1(kp))
            | square_flag(nonempty_right_1(kp)))
            & (BlackRookFlag | BlackKingFlag))//rook
            | ((square_flag(nonempty_down_2(kp))
            | square_flag(nonempty_up_2(kp))
            | square_flag(nonempty_left_2(kp))
            | square_flag(nonempty_right_2(kp)))
            & BlackCannonFlag)//cannon
            | ((square_flag(knight_leg(piece(BlackKnightIndex1), kp))
            | square_flag(knight_leg(piece(BlackKnightIndex2), kp)))
            & EmptyFlag);//knight
    }
    else
    {
        uint kp = piece(BlackKingIndex);
        assert(kp != InvaildSquare);
        return ((square_flag(square_down(kp))
            | square_flag(square_left(kp))
            | square_flag(square_right(kp)))
            & RedPawnFlag)//pawn
            | ((square_flag(nonempty_down_1(kp))
            | square_flag(nonempty_up_1(kp))
            | square_flag(nonempty_left_1(kp))
            | square_flag(nonempty_right_1(kp)))
            & (RedRookFlag | RedKingFlag))//rook
            | ((square_flag(nonempty_down_2(kp))
            | square_flag(nonempty_up_2(kp))
            | square_flag(nonempty_left_2(kp))
            | square_flag(nonempty_right_2(kp)))
            & RedCannonFlag)//cannon
            | ((square_flag(knight_leg(piece(RedKnightIndex1), kp))
            | square_flag(knight_leg(piece(RedKnightIndex2), kp)))
            & EmptyFlag);//knight
    }
}
bool XQ::is_good_cap(uint move)const
{
    uint dst = move_dst(move);
    uint dst_piece = square(dst);
    if (dst_piece == EmptyIndex)
        return false;
    uint src_piece = square(move_src(move));
    if (g_simple_values[src_piece] < g_simple_values[dst_piece])
        return true;
    if (player() == Red)
    {
        //king
        {
            uint src = piece(BlackKingIndex);
            if (g_move_flags[dst][src] & BlackPawnFlag)
                return false;
        }
        //advisor
        for (uint idx = BlackAdvisorIndex1; idx <= BlackAdvisorIndex2; ++idx)
        {
            uint src = piece(idx);
            if (g_move_flags[dst][src] & BlackAdvisorFlag)
                return false;
        }
        //bishop
        for (uint idx = BlackBishopIndex1; idx <= BlackBishopIndex2; ++idx)
        {
            uint src = piece(idx);
            if (g_move_flags[dst][src] & BlackBishopFlag)
                if (square(bishop_eye(src, dst)) == EmptyIndex)
                    return false;
        }
        //rook
        for (uint idx = BlackRookIndex1; idx <= BlackRookIndex2; ++idx)
        {
            uint src = piece(idx);
            if (g_move_flags[dst][src] & BlackRookFlag)
            {
                switch(distance(src, dst))
                {
                case 0:
                    return false;
                case 1:
                    if (distance(src, move_src(move)) == 0)
                        return false;
                }
            }
        }
        //knight
        for (uint idx = BlackKnightIndex1; idx <= BlackKnightIndex2; ++idx)
        {
            uint src = piece(idx);
            if (g_move_flags[dst][src] & BlackKnightFlag)
                if (square(knight_leg(src, dst)) == EmptyIndex)
                    return false;
        }
        //cannon
        for (uint idx = BlackCannonIndex1; idx <= BlackCannonIndex2; ++idx)
        {
            uint src = piece(idx);
            if (g_move_flags[dst][src] & BlackCannonFlag)
            {
                switch(distance(src, dst))
                {
                case 1:
                    if (distance(src, move_src(move)) != 0)
                        return false;
                    break;
                case 2:
                    if (distance(src, move_src(move)) < 2)
                        return false;
                }
            }
        }
        //pawn
        for (uint idx = BlackPawnIndex1; idx <= BlackPawnIndex5; ++idx)
        {
            uint src = piece(idx);
            if (g_move_flags[dst][src] & BlackPawnFlag)
                return false;
        }
    }
    else
    {
        //king
        {
            uint src = piece(RedKingIndex);
            if (g_move_flags[dst][src] & RedPawnFlag)
                return false;
        }
        //advisor
        for (uint idx = RedAdvisorIndex1; idx <= RedAdvisorIndex2; ++idx)
        {
            uint src = piece(idx);
            if (g_move_flags[dst][src] & RedAdvisorFlag)
                return false;
        }
        //bishop
        for (uint idx = RedBishopIndex1; idx <= RedBishopIndex2; ++idx)
        {
            uint src = piece(idx);
            if (g_move_flags[dst][src] & RedBishopFlag)
                if (square(bishop_eye(src, dst)) == EmptyIndex)
                    return false;
        }
        //rook
        for (uint idx = RedRookIndex1; idx <= RedRookIndex2; ++idx)
        {
            uint src = piece(idx);
            if (g_move_flags[dst][src] & RedRookFlag)
            {
                switch(distance(src, dst))
                {
                case 0:
                    return false;
                case 1:
                    if (distance(src, move_src(move)) == 0)
                        return false;
                }
            }
        }
        //knight
        for (uint idx = RedKnightIndex1; idx <= RedKnightIndex2; ++idx)
        {
            uint src = piece(idx);
            if (g_move_flags[dst][src] & RedKnightFlag)
                if (square(knight_leg(src, dst)) == EmptyIndex)
                    return false;
        }
        //cannon
        for (uint idx = RedCannonIndex1; idx <= RedCannonIndex2; ++idx)
        {
            uint src = piece(idx);
            if (g_move_flags[dst][src] & RedCannonFlag)
            {
                switch(distance(src, dst))
                {
                case 1:
                    if (distance(src, move_src(move)) != 0)
                        return false;
                    break;
                case 2:
                    if (distance(src, move_src(move)) < 2)
                        return false;
                }
            }
        }
        //pawn
        for (uint idx = RedPawnIndex1; idx <= RedPawnIndex5; ++idx)
        {
            uint src = piece(idx);
            if (g_move_flags[dst][src] & RedPawnFlag)
                return false;
        }
    }
    return true;
}
