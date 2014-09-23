#include "hash.h"
HashTable::HashTable(uint32 power)
{
    m_size = 1 << (power -  1);
    m_mask = m_size - 1;
    m_records[0] = new Record[m_size];
    m_records[1] = new Record[m_size];
}

HashTable::~HashTable()
{
    delete[] m_records[0];
    delete[] m_records[1];
}

void HashTable::clear()
{
    for (uint i = 0; i < 2; ++i)
        for (uint j = 0; j < m_size; ++j)
            m_records[i][j].clear();
}
