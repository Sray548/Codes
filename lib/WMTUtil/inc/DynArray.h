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
	 * \brief 构造函数
	 * \param eleSize 元素的大小
	 */
	DynArray(int eleSize);
	~DynArray();

	/**
	 * \brief 尾部增加一个元素
	 *
	 * \param ele  element对象地址
	 * \return 0 成功， -1 失败
	 */
	int push_back(void * ele);


	/**
	 * \brief 头部增加一个元素
	 *
	 * \param ele  element对象地址
	 * \return 0 成功， -1 失败
	 */
	int push_front(void * ele);

    /**
	 * \brief 删除最后一个元素
	 *
	 * \return 0 成功， -1 失败
	 */
	int pop_back();

    /**
	 * \brief 删除第一个元素
	 *
	 * \return 0 成功， -1 失败
	 */
	int pop_front();


	/**
	 * \brief 删除某个元素
	 *
	 * \return 0 成功， -1 失败
	 */
	int erase(int index);


	/**
	 * \brief  清除所有元素
	 *
	 * \note  调用后 size() == 0
	 */
	void clear();

	/**
	 * \brief 指定位置插入一个元素
	 *
	 * \return 0 成功， -1 失败
	 */
	int insert(int pos, void * ele);

	/**
	 * \brief 元素个数
	 */
	int size() const { return m_size; }

    /**
	 * \brief 返回第一个元素
	 */
    void * front() { return at(0); }
    const void* front() const { return at(0); }

    /**
	 * \brief 返回最后一个元素
	 */
    void * back() { return at( m_size - 1); }
    const void * back() const { return at( m_size - 1); }

	/**
	 * \brief 获取某个元素的数据
	 *
	 * \note 不校验index是否有效，用户必须保证index位于0 到 size() - 1 之间
	 */
	void * operator[](int index);
	const void * operator[](int index) const;

	/**
	 * \brief 获取某个元素的数据
	 *
	 * \note 校验index是否有效，失败返回NULL
	 */
	void * at(int index);
	const void * at(int index) const;

	/**
	 * \brief 保留空间，避免频繁内存分配
	 *
	 * \param n 要求至少能够容纳n个元素
	 * \return 0 成功， -1 失败
	 */
	int reserve(int n);

	/**
	 * \brief 当前不重新分配内存的情况下可能容纳多少个元素
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
 * \brief 模板实现
 *
 * 该动态数组仅仅将T所在的内存按照bit-wise进行拷贝
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

