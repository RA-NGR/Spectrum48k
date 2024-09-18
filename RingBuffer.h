#pragma once
#include "Common.h"

template<uint16_t SIZE, class DATA> 
class RingBuffer
{
	DATA m_dataBuffer[SIZE];
	uint16_t m_readIndex = 0;
	uint16_t m_writeIndex = 0;
	uint16_t m_storedIndex = 0;
public:
	inline bool isEmpty() { return m_readIndex == m_writeIndex; };
	bool completeGet()
	{
		if (isEmpty()) return false;
		m_readIndex = (++m_readIndex) & (SIZE - 1);
		return true;
	};
	bool getData(DATA& value)
	{
		if (isEmpty()) return false;
		value = m_dataBuffer[m_readIndex];
		return true;
	};
	void updateData(DATA value)	{ m_dataBuffer[m_readIndex] = value; };
	void putData(DATA value) { m_dataBuffer[m_writeIndex] = value; m_writeIndex = (++m_writeIndex) & (SIZE - 1); };
	void resetBuffer() { m_writeIndex = m_readIndex = 0; };
	void store() { m_storedIndex = m_readIndex; };
	void restore() { m_readIndex = m_storedIndex; };
};

