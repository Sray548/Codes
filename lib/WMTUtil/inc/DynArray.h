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

#ifndef WMT_DYNARRAY_HPP_INCLUDED
#define WMT_DYNARRAY_HPP_INCLUDED


/**
 * a dynamic array class
 */
class DynArray
{
public:
	/**
	 * \brief ���캯��
	 * \param eleSize Ԫ�صĴ�С
	 */
	DynArray(int eleSize);
	~DynArray();

	/**
	 * \brief β������һ��Ԫ��
	 *
	 * \param ele  element�����ַ
	 * \return 0 �ɹ��� -1 ʧ��
	 */
	int push_back(void * ele);


	/**
	 * \brief ͷ������һ��Ԫ��
	 *
	 * \param ele  element�����ַ
	 * \return 0 �ɹ��� -1 ʧ��
	 */
	int push_front(void * ele);

    /**
	 * \brief ɾ�����һ��Ԫ��
	 *
	 * \return 0 �ɹ��� -1 ʧ��
	 */
	int pop_back();

    /**
	 * \brief ɾ����һ��Ԫ��
	 *
	 * \return 0 �ɹ��� -1 ʧ��
	 */
	int pop_front();


	/**
	 * \brief ɾ��ĳ��Ԫ��
	 *
	 * \return 0 �ɹ��� -1 ʧ��
	 */
	int erase(int index);


	/**
	 * \brief  �������Ԫ��
	 *
	 * \note  ���ú� size() == 0
	 */
	void clear();

	/**
	 * \brief ָ��λ�ò���һ��Ԫ��
	 *
	 * \return 0 �ɹ��� -1 ʧ��
	 */
	int insert(int pos, void * ele);

	/**
	 * \brief Ԫ�ظ���
	 */
	int size() const { return m_size; }

    /**
	 * \brief ���ص�һ��Ԫ��
	 */
    void * front() { return at(0); }
    const void* front() const { return at(0); }

    /**
	 * \brief �������һ��Ԫ��
	 */
    void * back() { return at( m_size - 1); }
    const void * back() const { return at( m_size - 1); }

	/**
	 * \brief ��ȡĳ��Ԫ�ص�����
	 *
	 * \note ��У��index�Ƿ���Ч���û����뱣֤indexλ��0 �� size() - 1 ֮��
	 */
	void * operator[](int index);
	const void * operator[](int index) const;

	/**
	 * \brief ��ȡĳ��Ԫ�ص�����
	 *
	 * \note У��index�Ƿ���Ч��ʧ�ܷ���NULL
	 */
	void * at(int index);
	const void * at(int index) const;

	/**
	 * \brief �����ռ䣬����Ƶ���ڴ����
	 *
	 * \param n Ҫ�������ܹ�����n��Ԫ��
	 * \return 0 �ɹ��� -1 ʧ��
	 */
	int reserve(int n);

	/**
	 * \brief ��ǰ�����·����ڴ������¿������ɶ��ٸ�Ԫ��
	 */
	int capacity() const;
private:
	// noncopyable
	DynArray& operator=(const DynArray&);
	DynArray(const DynArray&);

	void * getElePtr(int index) const;

	char *  m_data;
	int		m_size;
	int		m_alloced;
	int     m_eleSize;
};

inline void * DynArray::getElePtr(int index) const
{
	return m_data + index * m_eleSize;
}

inline int DynArray::capacity() const
{
	return m_alloced;
}

inline int DynArray::pop_back()
{
    return erase(m_size - 1);
}

inline int DynArray::pop_front()
{
    return erase(0);
}



/**
 * \brief ģ��ʵ��
 *
 * �ö�̬���������T���ڵ��ڴ水��bit-wise���п���
 */
template<class T>
class TDynArray : private DynArray
{
public:
    using DynArray::erase;
    using DynArray::clear;
    using DynArray::size;
    using DynArray::reserve;
    using DynArray::capacity;
    using DynArray::pop_back;
    using DynArray::pop_front;

	TDynArray()
        : DynArray(sizeof(T))
    {
    }

    int push_back( const T& t) {
        return DynArray::push_back((void *)&t);
    }

    int push_front(const T& t) {
        return DynArray::push_front((void *)&t);
    }

    int insert(int pos, const T& t) {
        return DynArray::insert(pos, (void*)&t);
    }


    T&  operator[](int index) {
        return *(T*)DynArray::operator[](index);
    }

    const T& operator[](int index) const{
        return *(const T*)DynArray::operator[](index);
    }

    T& at(int index) {
        return *(T*)DynArray::at(index);
    }
    const T& at(int index) const {
        return *(const T*)DynArray::at(index);
    }

    T& front() {
        return at(0);
    }

    const T& front() const {
        return at(0);
    }

    T& back() {
        return at(size() - 1);
    }

    const T& back() const {
        return at(size() - 1);
    }
};

#endif // WMT_DYNARRAY_HPP_INCLUDED

