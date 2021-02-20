/*
 * Copyright 2014 WonderMedia Technologies, Inc. All Rights Reserved.
 *
 * This PROPRIETARY SOFTWARE is the property of WonderMedia Technologies, Inc.
 * and may contain trade secrets and/or other confidential information of
 * WonderMedia Technologies, Inc. This file shall not be disclosed to any third party,
 * in whole or in part, without prior written consent of WonderMedia.
 *
 * THIS PROPRIETARY SOFTWARE AND ANY RELATED DOCUMENTATION ARE PROVIDED AS IS,
 * WITH ALL FAULTS, AND WITHOUT WARRANTY OF ANY KIND EITHER EXPRESS OR IMPLIED,
 * AND WonderMedia TECHNOLOGIES, INC. DISCLAIMS ALL EXPRESS OR IMPLIED WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 */
#include "DynArray.h"
#include "WAssert.h"
#include <stdlib.h>
#include <string.h>
#include "debug_new.h"

DynArray::DynArray(int eleSize)
{
	WASSERT(eleSize >= 1)(eleSize);

	m_eleSize = eleSize;
	m_data = NULL;
	m_size = 0;
	m_alloced = 0;
}

DynArray::~DynArray()
{
	free(m_data);
}

int DynArray::reserve(int n)
{
	if ( n > m_alloced)
	{
		// round up
		int round = 4;
		if( m_eleSize < 32)
			round = 8;

		n = (n + round) & ~(round - 1);

		char * newData = (char*) realloc(m_data, m_eleSize * n);
		if(!newData)
			return -1;
		m_data = newData;
		m_alloced = n;

		// memset( m_data + m_size * m_eleSize, 0, m_eleSize * m_alloced - m_size);
	}
	return 0;
}

int DynArray::push_front(void * ele)
{
	return insert(0, ele);
}


int DynArray::push_back(void * ele)
{
    return insert(m_size, ele);
}

int DynArray::insert(int pos, void * ele)
{
	WASSERT( pos >= 0 && pos <= m_size)(pos)(m_size);;

	if ( pos < 0 || pos > m_size)
		return -1;

	if( m_size == m_alloced)
	{
		if( 0 != reserve(m_size + 1))
			return -1;
	}

	memmove( getElePtr(pos + 1), getElePtr(pos), (m_size - pos) * m_eleSize);
	memcpy ( getElePtr(pos), ele, m_eleSize);
	m_size++;
	return 0;
}

int DynArray::erase(int index)
{
	WASSERT( index >= 0 && index < m_size)(index)(m_size);

	if( index >= 0 && index < m_size)
	{
		memmove( getElePtr(index), getElePtr(index + 1), (m_size - index - 1) * m_eleSize);
		m_size--;
		return 0;
	}
	return -1;
}


void * DynArray::operator[](int index)
{
	WASSERT( index >= 0 && index < m_size)(index)(m_size);;
	return getElePtr(index);
}

const void * DynArray::operator[](int index) const
{
	WASSERT( index >= 0 && index < m_size)(index)(m_size);;
	return getElePtr(index);
}

void * DynArray::at(int index)
{
	if( index >= 0 && index < m_size)
		return getElePtr(index);
	return NULL;
}

const void * DynArray::at(int index) const
{
	if( index >= 0 && index < m_size)
		return getElePtr(index);
	return NULL;
}

void DynArray::clear()
{
	m_size = 0;
}

