// File: Queue.h

/*
 * Copyright (c) 1998-1999 Murphy Cheng-Che Chen <murphychen@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
*/

/*
1999/03/02: Created by Murphy
	Queue();
	BOOL Add(Type& tElement);
	BOOL Remove(Type& tElement);
	BOOL IsEmpty();
	BOOL IsFull();
*/



#ifndef __EKERNEL_QUEUE_H_
#define __EKERNEL_QUEUE_H_



#include "ADT\Array.h"
#include "LibC\assert\assert.h"
#include "LibC\stdio\stdio.h"



template <class Type>
class Queue : public Array<Type> {
private:
	int m_nFront;	// the front node of the queue
	int m_nTail;	// the tail node of the queue
	int m_nCount;	// the number of elements in the queue
public:
	Queue();

	BOOL Add(Type& tElement);
	BOOL Remove(Type& tElement);
	BOOL IsEmpty() { return m_nCount==0 ? TRUE : FALSE; }
	BOOL IsFull() { return m_nCount==m_nSize ? TRUE : FALSE; }
};



template<class Type>
inline Queue<Type>::Queue() : m_nFront(0), m_nTail(0) , m_nCount(0)
{
}



template<class Type>
inline BOOL Queue<Type>::Add(Type& tElement)
{
	// If the queue is full.
	if(m_nCount==m_nSize)
		return FALSE;

	SetAt(m_nTail, tElement);

	m_nTail++;
	if(m_nTail==m_nSize)
		m_nTail=0;

	m_nCount++;

	return TRUE;
}



template<class Type>
inline BOOL Queue<Type>::Remove(Type& tElement)
{
	// If the queue is empty.
	if(m_nCount==0)
		return FALSE;

	tElement = GetAt(m_nFront);

	m_nFront++;
	if(m_nFront==m_nSize)
		m_nFront=0;

	m_nCount--;

	return TRUE;
}



#endif
