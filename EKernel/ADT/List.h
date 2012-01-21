// File: List.h

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
 1998/10/01: Modified from <<Data Structures and Algorithm Analysis in C++>>
		by Mark Allen Weiss
 1998/10/08: Modified by Murphy Cheng-Che Chen
		Merge the concepts of cursor and list.
		Constructors, member variables, member functions.
		Also make it a doubly linked list.
 1998/12/07: Murphy
		Redesign the List class.
		Base on the codes from <<Fudamentals of Data Structure in C++>>
 1999/01/31: Murphy
		Compeletely redesign the List class.
		Abandon the concept of iterator class.
 1999/02/03: Murphy
		Modify AddHead, AddTail, RemoveAt by the principle of circular
			doubly linked list.
		Type& GetHead(void);
		void RemoveHead(void);
*/

#ifndef __EKERNEL_LIST_H_
#define __EKERNEL_LIST_H_


#include "LibC\assert\assert.h"
#include "LibC\malloc\malloc.h"

template<class Type>
class List {
private:
	struct Node
	{
		Node* pNext;
		Node* pPrev;
		Type data;
	};

	Node* m_pNodeHead;
	Node* m_pNodeTail;
	int m_nCount;

	Node* NewNode(Node*, Node*);
	void FreeNode(Node*);


public:
	List();

	inline int GetCount() const { return m_nCount; }
	BOOL IsEmpty() const { return m_nCount == 0; }

	void* AddHead(Type tElement);
	void* AddTail(Type tElement);

	Type GetAt(void* position);
	inline Type GetHead(void) const { assert(m_pNodeHead!=0); return m_pNodeHead->data; }
	void RemoveHead(void);
	void RemoveAt(void* pPosition);

	// Iteration operators.
	void* GetHeadPosition() const { assert(m_nCount>0); return (void*) m_pNodeHead; }
	void* GetTailPosition() const { assert(m_nCount>0); return (void*) m_pNodeTail; }
	Type& GetNext(void* &pPosition) const;
	Type& GetPrev(void* &pPosition) const;
};



template<class Type>
List<Type>::List()
{
	m_nCount = 0;
	m_pNodeHead = m_pNodeTail = 0;
}



template<class Type>
void* List<Type>::AddHead(Type tElement)
{
	assert(this);

	Node* pNewNode = NewNode(m_pNodeTail, m_pNodeHead);
	pNewNode->data = tElement;
	if (m_pNodeHead != 0)
	{
		m_pNodeHead->pPrev = pNewNode;
		m_pNodeTail->pNext = pNewNode;
	}
	else
		m_pNodeTail = pNewNode;
	m_pNodeHead = pNewNode;
	return (void*)pNewNode;
}



template<class Type>
void* List<Type>::AddTail(Type tElement)
{
	assert(this);

	Node *pNewNode = NewNode(m_pNodeTail, m_pNodeHead);
	pNewNode->data = tElement;
	if (m_pNodeTail != 0)
	{
		m_pNodeHead->pPrev = pNewNode;
		m_pNodeTail->pNext = pNewNode;
	}
	else
	{
		pNewNode->pPrev=pNewNode;
		pNewNode->pNext=pNewNode;
		m_pNodeHead = pNewNode;
	}
	m_pNodeTail = pNewNode;

	return (void*) pNewNode;
}



template<class Type>
inline Type List<Type>::GetAt(void* pPosition)
{
	Node* pNode = (Node*) pPosition;
	//assert(IsValidAddress(pNode, sizeof(CNode)));
	return pNode->data;
}



template<class Type>
void List<Type>::RemoveAt(void* pPosition)
{
	assert(this);

	Node* pOldNode = (Node*) pPosition;
//	assert(IsValidAddress(pOldNode, sizeof(CNode)));

	// remove pOldNode from list
	if (pOldNode == m_pNodeHead)
	{
		m_pNodeHead = pOldNode->pNext;
		m_pNodeTail->pNext = m_pNodeHead;
	}
	else
	{
//		assert(IsValidAddress(pOldNode->pPrev, sizeof(CNode)));
		pOldNode->pPrev->pNext = pOldNode->pNext;
	}

	if (pOldNode == m_pNodeTail)
	{
		m_pNodeTail = pOldNode->pPrev;
		m_pNodeHead->pPrev = m_pNodeTail;
	}
	else
	{
//		assert(IsValidAddress(pOldNode->pNext, sizeof(CNode)));
		pOldNode->pNext->pPrev = pOldNode->pPrev;
	}

	FreeNode(pOldNode);
}


template<class Type>
void List<Type>::RemoveHead(void)
{
	if(m_pNodeHead!=0)
	{
		RemoveAt(m_pNodeHead);
	}
}



template<class Type>
inline Type& List<Type>::GetNext(void* &pPosition) const
{
	Node* pNode = (Node*) pPosition;
//	ASSERT(IsValidAddress(pNode, sizeof(CNode)));
	pPosition = (void*) pNode->pNext;
	//printf("(GetNext) next=%x\n", pPosition);
	return pNode->data;
}



template<class Type>
inline Type& List<Type>::GetPrev(void* &pPosition) const
{
	Node* pNode = (Node*) pPosition;
//	ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
	pPosition = (void*) pNode->pPrev;
	return pNode->data;
}



template<class Type>
List<Type>::Node* List<Type>::NewNode(List::Node* pPrev, List::Node* pNext)
{
	List::Node* pNode = (List::Node*)malloc(sizeof(List::Node));
//	printf("New Node Address:%x prev=%x next=%x\n", pNode, pPrev, pNext);
	pNode->pPrev = pPrev;
	pNode->pNext = pNext;
	m_nCount++;
	assert(m_nCount > 0);	// make sure we don't overflow

	// Call constructor for node's data
//	((Type*)(&pNode->data))->Type();

	return pNode;
}



template<class Type>
void List<Type>::FreeNode(List::Node* pNode)
{
	// Call destructor for node's data
//	((Type*)(&pNode->data))->~Type();
	free(pNode);

	m_nCount--;
	assert(m_nCount >= 0);	// make sure we don't underflow

	// if no more elements, cleanup completely
	if (m_nCount == 0)
	{
		m_pNodeHead = m_pNodeTail = 0;
	}
}



#endif
