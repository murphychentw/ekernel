// File: Array.h

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
1998/12/14 : Created by Murphy Cheng-Che Chen
	Currently, SetSize() should be called only once, and before any
	operations on an Array object.
	void SetSize(int nSize);
	int GetSize() const;
	void SetAt(int nIndex, Type& tElement);
	Type& GetAt(int nIndex) const;
	BOOL CheckValid(int nIndex);
	int FindEmptySlot();
	int Add(Type& tElement);
1999/01/27: Murphy
	void InvalidateAt(int nIndex);
1999/03/02: Murphy
	Made m_nSize protected instead of private.
*/

#ifndef __EKERNEL_ARRAY_H_
#define __EKERNEL_ARRAY_H_

#include "Types.h"
#include "LibC\assert\assert.h"
#include "LibC\stdio\stdio.h"

template <class Type>
class Array {
private:
	Type *m_pData;
	BOOL *m_pValidData;

protected:
	int m_nSize;

public:
	inline Array() { m_pData=0; m_pValidData=0; };

	void SetSize(int nSize);
	int GetSize() const;

	void SetAt(int nIndex, Type& tElement);
	Type& GetAt(int nIndex) const;
	void InvalidateAt(int nIndex);	// Invalidate the element at nIndex

	int FindEmptySlot();
	BOOL CheckValid(int nIndex);

	int Add(Type& tElement);

//	Type& operator []( int nIndex );
//	Type operator []( int nIndex ) const;
};



template<class Type>
inline void Array<Type>::SetSize(int nSize)
{
	assert(m_pData==0);
	m_pData = (Type*) new Type[nSize];
	assert(m_pData);

	assert(m_pValidData==0);
	m_pValidData = (BOOL*) new BOOL[nSize];
	assert(m_pValidData);

	for(int i = 0; i < nSize; i++)
		m_pValidData[i] = FALSE;

	m_nSize=nSize;
}



template<class Type>
inline int Array<Type>::GetSize() const
{
	return m_nSize;
}



template<class Type>
inline void Array<Type>::SetAt(int nIndex, Type& tElement)
{
//	printf("SetAt()\n");
//	printf("nIndex=%d, m_pData=%08x\n", nIndex, m_pData);

	assert(nIndex >= 0 && nIndex < m_nSize);
	m_pData[nIndex] = tElement;
	m_pValidData[nIndex] = TRUE;
}



template<class Type>
inline Type& Array<Type>::GetAt(int nIndex) const
{
/*	printf("GetAt():\n");
	printf("m_pData = %x\n", m_pData);*/
	if(nIndex<0 || nIndex>=m_nSize)
	{
		printf("nIndex=%x\n", nIndex);
		printf("m_nSize=%d\n", m_nSize);
	}
	//printf("m_pValidData[nIndex]=%d\n", m_pValidData[nIndex]);
	assert(nIndex >= 0 && nIndex < m_nSize);
	assert(m_pValidData[nIndex]==TRUE);
	return m_pData[nIndex];	
}



template<class Type>
inline void Array<Type>::InvalidateAt(int nIndex)
{
	assert(nIndex >= 0 && nIndex < m_nSize);
	m_pValidData[nIndex]=FALSE;
}



template<class Type>
inline BOOL Array<Type>::CheckValid(int nIndex)
{
	if((int)nIndex<0 || nIndex>=m_nSize)
		return FALSE;

	return m_pValidData[nIndex];
}



template<class Type>
inline int Array<Type>::FindEmptySlot()
{
	int slot=-1;
	for(int i=0; i<m_nSize; i++)
	{
		if( m_pValidData[i] == FALSE )
		{
			slot=i;
			break;
		}
	}
	return slot;
}



template<class Type>
inline int Array<Type>::Add(Type& tElement)
{
	int slot;

	// Find an empty slot
	slot = FindEmptySlot();

	if(slot!=-1)
	{
		assert(slot>=0 && slot<m_nSize);
		SetAt(slot, tElement);
	}

	return slot;
}

#endif
