#ifndef _ENGINE_DATA_H_
#define _ENGINE_DATA_H_

#include <cstring>

extern const uint64 * const g_type_locks[14];
extern const uint32 * const g_type_keys[14];
extern const sint32 * const g_type_values[14];
extern const uint64 * const g_piece_locks[32];
extern const uint32 * const g_piece_keys[32];
extern const sint32 * const g_piece_values[32];

inline const uint64& piece_lock(uint piece, uint square)
{
    assert (piece < 32UL);
    assert (square < 91UL);
    return g_piece_locks[piece][square];
}
inline const uint32& piece_key(uint piece, uint square)
{
    assert (piece < 32UL);
    assert (square < 91UL);
    return g_piece_keys[piece][square];
}
inline const sint32& piece_value(uint piece, uint square)
{
    assert (piece < 32UL);
    assert (square < 91UL);
    return g_piece_values[piece][square];
}

inline const uint64& type_lock(uint type, uint square)
{
    assert (type < 14UL);
    assert (square < 91UL);
    return g_type_locks[type][square];
}
inline const uint32& type_key(uint type, uint square)
{
    assert (type < 14UL);
    assert (square < 91UL);
    return g_type_keys[type][square];
}
inline const sint32& type_value(uint type, uint square)
{
    assert (type < 14UL);
    assert (square < 91UL);
    return g_type_values[type][square];
}

#endif //_ENGINE_DATA_H_
