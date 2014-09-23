#include "folEngine.h"
#include <ctime>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;
bool folEngine::make_move(uint32 move)
{
    uint src, dst, src_piece, dst_piece;
    src = move_src(move);
    dst = move_dst(move);
    assert(m_xq.is_legal_move(src, dst));
    src_piece = m_xq.square(src);
    dst_piece = m_xq.square(dst);
    assert(dst_piece != RedKingIndex && dst_piece != BlackKingIndex);

    if (!m_xq.do_move(src, dst))
    {
        return false;
    }

    ++m_ply;
    m_traces[m_ply] = create_trace(m_xq.status(), dst_piece, move);
    if (dst_piece == EmptyIndex)
    {
        m_keys[m_ply] = m_keys[m_ply-1]\
                    ^ piece_key(src_piece, src)
                    ^ piece_key(src_piece, dst);
        m_locks[m_ply] = m_locks[m_ply-1]\
                    ^ piece_lock(src_piece, src)\
                    ^ piece_lock(src_piece, dst);;
        m_values[m_ply] = m_values[m_ply-1]\
                        + piece_value(src_piece, dst)\
                        - piece_value(src_piece, src);
    }
    else
    {
        m_keys[m_ply] = m_keys[m_ply-1]\
                    ^ piece_key(src_piece, src)\
                    ^ piece_key(src_piece, dst)\
                    ^ piece_key(dst_piece, dst);
        m_locks[m_ply] = m_locks[m_ply-1]\
                    ^ piece_lock(src_piece, src)\
                    ^ piece_lock(src_piece, dst)\
                    ^ piece_lock(dst_piece, dst);
        m_values[m_ply] = m_values[m_ply-1]\
                        + piece_value(src_piece, dst)\
                        - piece_value(src_piece, src)\
                        - piece_value(dst_piece, dst);
    }
    return true;
}

void folEngine::unmake_move()
{
    assert (m_ply > 0);
    uint32 trace = m_traces[m_ply--];
    m_xq.undo_move(trace_src(trace), trace_dst(trace), trace_dst_piece(trace));
}

folEngine::folEngine(const XQ& xq, uint hash):m_xq(xq), m_ply(0), m_hash(hash)
{
    m_traces[0] = create_trace(xq.status(), EmptyIndex, 0);
    m_ply = 0;
    m_start_ply = -1;
    m_keys[0] = 0UL;
    m_locks[0] = 0ULL;
    m_values[0] = 0L;

    for (uint i = 0; i < 32; ++i)
    {
        uint32 square = m_xq.piece(i);
        if (square == InvaildSquare)
            continue;
        assert(m_xq.square(square) == i);
        m_keys[0] ^= piece_key(i, square);
        m_locks[0] ^= piece_lock(i, square);
        m_values[0] += piece_value(i, square);
    }
}

void folEngine::reset(const XQ& xq)
{
    m_xq = xq;

    m_traces[0] = create_trace(xq.status(), EmptyIndex, 0);
    m_ply = 0;
    m_start_ply = -1;
    m_keys[0] = 0UL;
    m_locks[0] = 0ULL;
    m_values[0] = 0L;

    for (uint i = 0; i < 32; ++i)
    {
        uint32 square = m_xq.piece(i);
        if (square == InvaildSquare)
            continue;
        assert(m_xq.square(square) == i);
        m_keys[0] ^= piece_key(i, square);
        m_locks[0] ^= piece_lock(i, square);
        m_values[0] += piece_value(i, square);
    }
}
static vector<uint> generate_moves(const XQ& xq)
{
    vector<uint> ml;
    uint own = xq.player();
    uint opp = 1 - own;
    uint idx;
    uint src, dst;
    if (own == Red)
    {
        idx = RedKingIndex;
        //red king
        src = xq.piece(RedKingIndex);
        const uint8 *pm = g_red_king_pawn_moves[src];
        dst = *pm++;
        while (dst != InvaildSquare)
        {
            if (xq.square_color(dst) != own)
                ml.push_back(create_move(src, dst));
            dst = *pm++;
        }
        //red pawn
        for (uint i = RedPawnIndex1; i <= RedPawnIndex5; ++i)
        {
            src = xq.piece(i);
            if (src == InvaildSquare)
                continue;
            pm = g_red_king_pawn_moves[src];
            dst = *pm++;
            while (dst != InvaildSquare)
            {
                if (xq.square_color(dst) != own)
                    ml.push_back(create_move(src, dst));
                dst = *pm++;
            }
        }
    }
    else
    {
        idx = BlackKingIndex;
        //black king
        src = xq.piece(BlackKingIndex);
        const uint8 *pm = g_black_king_pawn_moves[src];
        dst = *pm++;
        while (dst != InvaildSquare)
        {
            if (xq.square_color(dst) != own)
                ml.push_back(create_move(src, dst));
            dst = *pm++;
        }
        //black pawn
        for (uint i = BlackPawnIndex1; i <= BlackPawnIndex5; ++i)
        {
            src = xq.piece(i);
            if (src == InvaildSquare)
                continue;
            pm = g_black_king_pawn_moves[src];
            dst = *pm++;
            while (dst != InvaildSquare)
            {
                if (xq.square_color(dst) != own)
                    ml.push_back(create_move(src, dst));
                dst = *pm++;
            }
        }
    }
    //advisor
    for(uint i = 0; i < 2; ++i)
    {
        ++idx;
        src = xq.piece(idx);
        if (src == InvaildSquare)
            continue;
        const uint8 *pm = g_advisor_bishop_moves[src];
        dst = *pm++;
        while (dst != InvaildSquare)
        {
            if (xq.square_color(dst) != own)
                ml.push_back(create_move(src, dst));
            dst = *pm++;
        }
    }
    //bishop
    for(uint i = 0; i < 2; ++i)
    {
        ++idx;
        src = xq.piece(idx);
        if (src == InvaildSquare)
            continue;
        const uint8 *pm = g_advisor_bishop_moves[src];
        dst = *pm++;
        while (dst != InvaildSquare)
        {
            if (xq.square_color(dst) != own && xq.square((dst + src) >> 1) == EmptyIndex)
                ml.push_back(create_move(src, dst));
            dst = *pm++;
        }
    }
    //rook
    for(uint i = 0; i < 2; ++i)
    {
        uint dst;
        src = xq.piece(++idx);
        if (src == InvaildSquare)
            continue;
        dst = xq.nonempty_left_1(src);
        if (xq.square_color(dst) == opp)
            ml.push_back(create_move(src, dst));
        dst = xq.nonempty_right_1(src);
        if (xq.square_color(dst) == opp)
            ml.push_back(create_move(src, dst));
        dst = xq.nonempty_down_1(src);
        if (xq.square_color(dst) == opp)
            ml.push_back(create_move(src, dst));
        dst = xq.nonempty_up_1(src);
        if (xq.square_color(dst) == opp)
            ml.push_back(create_move(src, dst));
        for (uint tmp = xq.nonempty_left_1(src), dst = square_left(src);
            dst != tmp;
            dst = square_left(dst))
            ml.push_back(create_move(src, dst));
        for (uint tmp = xq.nonempty_right_1(src), dst = square_right(src);
            dst != tmp;
            dst = square_right(dst))
            ml.push_back(create_move(src, dst));
        for (uint tmp = xq.nonempty_down_1(src), dst = square_down(src);
            dst != tmp;
            dst = square_down(dst))
            ml.push_back(create_move(src, dst));
        for (uint tmp = xq.nonempty_up_1(src), dst = square_up(src);
            dst != tmp;
            dst = square_up(dst))
            ml.push_back(create_move(src, dst));
    }
    //knight
    for(uint i = 0; i < 2; ++i)
    {
        ++idx;
        src = xq.piece(idx);
        if (src == InvaildSquare)
            continue;
        const uint16 *pm = g_kinght_moves[src];
        dst = *pm++;
        //23130 = ((InvaildSquare << 8) | InvaildSquare)
        while (dst != 23130UL)
        {
            uint leg = (dst & 0xff00) >> 8;
            dst &= 0xff;
            if (xq.square(leg) == EmptyIndex && xq.square_color(dst) != own)
                ml.push_back(create_move(src, dst));
            dst = *pm++;
        }
    }
    //cannon
    for(uint i = 0; i < 2; ++i)
    {
        uint dst;
        src = xq.piece(++idx);
        if (src == InvaildSquare)
            continue;
        dst = xq.nonempty_left_2(src);
        if (xq.square_color(dst) == opp)
            ml.push_back(create_move(src, dst));
        dst = xq.nonempty_right_2(src);
        if (xq.square_color(dst) == opp)
            ml.push_back(create_move(src, dst));
        dst = xq.nonempty_down_2(src);
        if (xq.square_color(dst) == opp)
            ml.push_back(create_move(src, dst));
        dst = xq.nonempty_up_2(src);
        if (xq.square_color(dst) == opp)
            ml.push_back(create_move(src, dst));
        for (uint tmp = xq.nonempty_left_1(src), dst = square_left(src);
            dst != tmp;
            dst = square_left(dst))
            ml.push_back(create_move(src, dst));
        for (uint tmp = xq.nonempty_right_1(src), dst = square_right(src);
            dst != tmp;
            dst = square_right(dst))
            ml.push_back(create_move(src, dst));
        for (uint tmp = xq.nonempty_down_1(src), dst = square_down(src);
            dst != tmp;
            dst = square_down(dst))
            ml.push_back(create_move(src, dst));
        for (uint tmp = xq.nonempty_up_1(src), dst = square_up(src);
            dst != tmp;
            dst = square_up(dst))
            ml.push_back(create_move(src, dst));
    }
    return ml;
}

static vector<uint> generate_root_move(XQ& xq, const set<uint>& ban)
{
    vector<uint> r;
    vector<uint> ml = generate_moves(xq);
    for (uint i = 0; i < ml.size(); ++i)
    {
        uint move = ml[i];
        uint dst_piece = xq.square(move_dst(move));
        if (dst_piece == RedKingIndex || dst_piece == BlackKingIndex)
        {
            r.clear();
            r.push_back(move);
            return r;
        }
        if (ban.find(move) != ban.end())
            continue;
        if (xq.do_move(move_src(move), move_dst(move)))
        {
            r.push_back(move);
            xq.undo_move(move_src(move), move_dst(move), dst_piece);
        }
    }
    return r;
}
uint32 folEngine::search(int depth, set<uint> ban)
{
    m_stop = false;
    m_tree_nodes = 0;
    m_leaf_nodes = 0;
    m_quiet_nodes = 0;
    m_hash_hit_nodes = 0;
    m_hash_move_cuts = 0;
    m_kill_cuts_1 = 0;
    m_kill_cuts_2 = 0;
    m_null_nodes = 0;
    m_null_cuts = 0;

    m_history.clear();
    m_hash.clear();

    m_null_ply = m_start_ply = m_ply;

    int best_value;
    vector<uint> ml = generate_root_move(m_xq, ban);
    vector<uint> bests;
    clock_t start;
    start = clock();
    for (sint i = 1;
        i <= depth  || (i <= depth*2 && float(clock()-start)/CLOCKS_PER_SEC < 2.0);
        ++i)
    {
        if (m_stop)
            break;
        if (ml.size() == 1)
        {
            bests.clear();
            bests.push_back(ml[0]);
            break;
        }
        if (ml.size() == 0)
        {
            break;
        }
        best_value = -WINSCORE;
        vector<uint> olds;
        bests.swap(olds);
        for (uint j = 0; j < olds.size(); ++j)
        {
            if (m_stop)
                break;
            int score;
            uint32 move = olds[j];
            make_move(move);
            if (best_value != -WINSCORE)
            {
                score = - full(i, -1-best_value, 1-best_value);
                if (score > best_value)
                    score = - full(i, -WINSCORE, -1-best_value);
            }
            else
            {
                score = - full(i, -WINSCORE, WINSCORE);
            }
            unmake_move();
            if (score >= best_value)
            {
                if (score > best_value)
                    bests.clear();
                if (find(bests.begin(), bests.end(), move) == bests.end())
                {
                    bests.push_back(move);
                }
                best_value = score;
            }
        }
        for (uint j = 0; j < ml.size(); ++j)
        {
            if (m_stop)
                break;
            uint32 move = ml[j];
            if (!make_move(move))
            {
                ml[j] = 0;
                continue;
            }
            int score;
            if (best_value != -WINSCORE)
            {
                score = - full(i, -1-best_value, -best_value);
                if (score > best_value)
                    score = - full(i, -WINSCORE, -best_value);
            }
            else
            {
                score = - full(i, -WINSCORE, WINSCORE);
            }
            unmake_move();
            if (score > best_value)
            {
                if (score > best_value)
                    bests.clear();
                if (find(bests.begin(), bests.end(), move) == bests.end())
                {
                    bests.push_back(move);
                }
                best_value = score;
            }
            else if (score < -MATEVALUE)
            {
                ml[j] = 0;
            }
        }

        //float t = float(clock()-start)/CLOCKS_PER_SEC;

        if (best_value > MATEVALUE || best_value < -MATEVALUE)
            break;
        ml.erase(remove(ml.begin(), ml.end(), (uint)0), ml.end());
    }
    if (bests.empty())
        return 0;
    return bests[clock()%bests.size()];
}
