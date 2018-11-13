#ifndef __XCORE_ARRAY_H__
#define __XCORE_ARRAY_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase/x_allocator.h"
#include "xbase/x_integer.h"
#include "xbase/x_memory.h"

namespace xcore
{
	class xalloc;

	//------------------------------------------------------------------------------
	// Author:
	//		Virtuos
	// Description:
	//		STL vector equivalent, but auto resize is OFF and reserve() is not able
	//		to shrink!
	//
	//      Provides a typed array-like interface to a managed memory block, including
	//      element access, iteration, modification, resizing. In this design elements 
	//		frequently undergo bitwise move, so don't put it in here if it doesn't support 
	//		it. This mostly means having no self-pointers.
	//
	class array_t
	{
	public:
		class iterator;
		class const_iterator;

	public:
							array_t ();
							array_t (xalloc*, u32 sizeofelement);
		explicit			array_t (xalloc*, u32 sizeofelement, u32 n);
							array_t (xalloc*, const array_t& other);
							array_t (xalloc*, const_iterator& i1, const_iterator& i2);
                            array_t (const array_t& other);
							~array_t();

        void                set_autoalloc(bool bAuto = true);
		void				relocate(xalloc* a);

		void				reserve (u32 n, bool bExact = true);
		void				resize (u32 n, bool bExact = true);
		u32					capacity (void) const;
		u32					size (void) const;
		u32					max_size (void) const;
		bool				empty (void) const;
		
		iterator			begin (void);
		const_iterator		begin (void) const;
		iterator			end (void);
		const_iterator		end (void) const;
		
		iterator			iat (u32 i);
		const_iterator		iat (u32 i) const;
		void*				at (u32 i);
		void const*			at (u32 i) const;

		iterator			find (void* item);
		const_iterator		find (void* item) const;
		
		void*				front (void);
		void const*			front (void) const;
		void const*			back (void) const;
		s32					push_back (void const* item);
		void				pop_back (void);
		
		void				clear (void);
		void				deallocate (void);
		
		iterator			insert (iterator ip, void* v);
		iterator			insert (iterator ip, const_iterator i1, const_iterator i2);
		iterator			erase (iterator ep);
		iterator			erase (iterator ep1, iterator ep2);
		
		void				sort ();

		array_t&			operator = (const array_t& v);
		bool				operator == (const array_t& v) const;

		void*				operator[] (u32 i);
		void const*			operator[] (u32 i) const;

		class iterator
		{
		public:
			iterator(xbyte* items, u32 itemsize, u32 itemcount);

			void*			operator * ();
			iterator&		operator++ ();

			bool			operator == (const iterator& v) const;
			bool			operator != (const iterator& v) const;

			u32				m_ItemSize;											// Item size
			u32				m_ItemCount;										// Item count
			u32				m_ItemIndex;										// Item index
			xbyte*			m_Data;												// Raw element data, consecutively stored.
		};

		class const_iterator
		{
		public:
			const_iterator(xbyte const* items, u32 itemsize, u32 itemcount);

			void const*		operator * ();
			iterator&		operator++ ();

			bool			operator == (const const_iterator& v) const;
			bool			operator != (const const_iterator& v) const;

			u32				m_ItemSize;											// Item size
			u32				m_ItemCount;										// Item count
			u32				m_ItemIndex;										// Item index
			xbyte const*	m_Data;												// Raw element data, consecutively stored.
		};

	protected:
		iterator			insert_space(iterator& ip, u32 n);
		iterator			erase_space(iterator& ip, u32 n);

	private:
		xalloc *			m_Allocator;
		bool                m_AutoAlloc;										// Flag to indicate whether auto reserve memory when push back
		u32					m_ItemSize;											// Item size, aligned
		u32					m_ItemReserved;										// Number of reserved items
		u32					m_ItemCount;										// Current number of items in the array
		xbyte*				m_Data;												// Raw element data, consecutively stored.
	};

	array_t::array_t()
		: m_Allocator(nullptr)
		, m_AutoAlloc(false)
		, m_ItemSize(0)
		, m_ItemReserved(0)
		, m_ItemCount(0)
		, m_Data(nullptr)
	{

	}

	array_t::array_t(xalloc* allocator, u32 sizeofelement)
		: m_Allocator(allocator)
		, m_AutoAlloc(true)
		, m_ItemSize(sizeofelement)
		, m_ItemReserved(0)
		, m_ItemCount(0)
		, m_Data(nullptr)
	{
	}
	
	array_t::array_t(xalloc* allocator, u32 n, u32 sizeofelement)
		: m_Allocator(allocator)
		, m_AutoAlloc(true)
		, m_ItemSize(sizeofelement)
		, m_ItemReserved(0)
		, m_ItemCount(0)
		, m_Data(nullptr)
	{
		reserve(n);
	}

	array_t::array_t(xalloc* allocator, const array_t& other)
		: m_Allocator(allocator)
		, m_AutoAlloc(other.m_ItemSize)
		, m_ItemSize(other.m_ItemSize)
		, m_ItemReserved(0)
		, m_ItemCount(other.m_ItemCount)
		, m_Data(nullptr)
	{
		reserve(other.m_ItemReserved);
		x_memcpy(m_Data, other.m_Data, m_ItemCount * m_ItemSize);
	}

	array_t::array_t(xalloc* allocator, const_iterator& i1, const_iterator& i2)
		: m_Allocator(allocator)
		, m_AutoAlloc(true)
		, m_ItemSize(i1.m_ItemSize)
		, m_ItemReserved(0)
		, m_ItemCount(0)
		, m_Data(nullptr)
	{
		if (i1.m_Data == i2.m_Data)
		{
			reserve(i2.m_ItemIndex - i1.m_ItemIndex);
			while (i1 != i2)
			{
				push_back(*i1);
				++i1;
			}
		}
	}

	array_t::array_t(const array_t& other)
		: m_Allocator(other.m_Allocator)
		, m_AutoAlloc(other.m_AutoAlloc)
		, m_ItemSize(other.m_ItemSize)
		, m_ItemReserved(0)
		, m_ItemCount(other.m_ItemCount)
		, m_Data(nullptr)
	{
		reserve(other.m_ItemReserved);
		x_memcpy(m_Data, other.m_Data, m_ItemCount * m_ItemSize);
	}

	array_t::~array_t(void)
	{
		if (m_Allocator != NULL)
		{ 
			m_Allocator->deallocate(m_Data);
			m_Data = NULL;
			m_ItemCount = 0;
		}
	}


	void                array_t::set_autoalloc(bool bAuto)
	{
		m_AutoAlloc = bAuto;
	}

	void				array_t::relocate(xalloc* a)
	{
		xbyte* data = (xbyte*)a->allocate(m_ItemReserved * m_ItemSize, X_ALIGNMENT_DEFAULT);
		x_memcpy(data, m_Data, m_ItemCount * m_ItemSize);
		m_Allocator->deallocate(m_Data);
		m_Allocator = a;
	}


	void				array_t::reserve(u32 n, bool bExact)
	{
		xbyte* data = (xbyte*)m_Allocator->allocate(n * m_ItemSize, X_ALIGNMENT_DEFAULT);

	}

	void				array_t::resize(u32 n, bool bExact)
	{
		m_ItemReserved = n;
		xbyte* data = (xbyte*)m_Allocator->allocate(m_ItemReserved * m_ItemSize, X_ALIGNMENT_DEFAULT);
		x_memcpy(data, m_Data, m_ItemCount * m_ItemSize);
		m_Allocator->deallocate(m_Data);
	}

	u32					array_t::capacity(void) const
	{
		return m_ItemReserved;
	}

	u32					array_t::size(void) const
	{
		return m_ItemCount;
	}

	u32					array_t::max_size(void) const
	{
		return m_ItemReserved;
	}

	bool				array_t::empty(void) const
	{
		return m_ItemCount == 0;
	}


	array_t::iterator			array_t::begin(void)
	{
		return iterator(m_Data, m_ItemSize, m_ItemCount);
	}

	array_t::const_iterator		array_t::begin(void) const
	{
		return const_iterator(m_Data, m_ItemSize, m_ItemCount);
	}

	array_t::iterator			array_t::end(void)
	{
		return iterator(m_Data, m_ItemSize, m_ItemCount);
	}

	array_t::const_iterator		array_t::end(void) const
	{
		return const_iterator(m_Data, m_ItemSize, m_ItemCount);
	}


	array_t::iterator		array_t::iat(u32 i)
	{
		iterator iter(m_Data, m_ItemSize, m_ItemCount);
		iter.m_ItemIndex = i;
		return iter;
	}

	array_t::const_iterator		array_t::iat(u32 i) const
	{
		const_iterator iter(m_Data, m_ItemSize, m_ItemCount);
		iter.m_ItemIndex = i;
		return iter;
	}

	void*				array_t::at(u32 i)
	{
		if (i < m_ItemCount)
			return &m_Data[i];
		return nullptr;
	}

	void const*			array_t::at(u32 i) const
	{
		if (i < m_ItemCount)
			return &m_Data[i];
		return nullptr;
	}


	array_t::iterator	array_t::find(void* item)
	{
		for (u32 i = 0; i < m_ItemCount; ++i)
		{
			if (x_memcmp(item, &m_Data[i], m_ItemSize) == 0)
			{
				return iat(i);
			}
		}
		return end();
	}

	array_t::const_iterator	array_t::find(void* item) const
	{
		for (u32 i = 0; i < m_ItemCount; ++i)
		{
			if (x_memcmp(item, &m_Data[i], m_ItemSize) == 0)
			{
				return iat(i);
			}
		}
		return end();
	}


	void*				array_t::front(void)
	{
		if (m_ItemCount > 0)
			return &m_Data[0 * m_ItemSize];
		return nullptr;
	}

	void const*			array_t::front(void) const
	{
		if (m_ItemCount > 0)
			return &m_Data[0 * m_ItemSize];
		return nullptr;
	}

	void const*			array_t::back(void) const
	{
		return &m_Data[m_ItemCount * m_ItemSize];
	}

	s32					array_t::push_back(void const* item)
	{
		if (m_ItemCount < m_ItemReserved)
		{
			xbyte* itemData = &m_Data[m_ItemCount * m_ItemSize];
			x_memcpy(itemData, item, m_ItemSize);
			m_ItemCount++;
			return m_ItemCount - 1;
		}
		return -1;
	}

	void				array_t::pop_back(void)
	{
		if (m_ItemCount > 0)
		{
			m_ItemCount -= 1;
		}
	}

	void				array_t::clear(void)
	{
		m_ItemCount = 0;
	}

	void				array_t::deallocate(void)
	{
		if (m_Data != NULL)
		{
			m_ItemCount = 0;
			m_Allocator->deallocate(m_Data);
		}
	}

	array_t::iterator	array_t::insert(iterator ip, void* v)
	{
		if (ip.m_ItemIndex == m_ItemCount)
		{
			push_back(v);
			return end();
		}
		else
		{
			iterator after = insert_space(ip, 1);
			x_memcpy(&m_Data[ip.m_ItemIndex * m_ItemSize], v, m_ItemSize);
			return after;
		}
	}

	array_t::iterator	array_t::insert(iterator ip, const_iterator i1, const_iterator i2)
	{
		if (ip.m_Data == m_Data && i1.m_ItemSize == m_ItemSize && i1.m_Data == i2.m_Data)
		{
			s32 n = i2.m_ItemIndex - i1.m_ItemIndex;
			iterator after = insert_space(ip, n);
			x_memcpy(&m_Data[ip.m_ItemIndex * ip.m_ItemSize], &i1.m_Data[i1.m_ItemIndex * i1.m_ItemSize], n * m_ItemSize);
			return after;
		}
		return end();
	}

	array_t::iterator	array_t::erase(iterator ep)
	{
		if (ep.m_ItemIndex < m_ItemCount) {
			ep = erase_space(ep, 1);
			return ep;
		}
		return end();
	}

	array_t::iterator	array_t::erase(iterator ep1, iterator ep2)
	{
		if (ep1.m_ItemIndex < m_ItemCount && ep2.m_ItemIndex < m_ItemCount) {
			s32 n = ep2.m_ItemIndex - ep1.m_ItemIndex;
			ep1 = erase_space(ep1, n);
			return ep1;
		}
		return end();
	}


	array_t&			array_t::operator = (const array_t& v)
	{
		return *this;
	}

	bool				array_t::operator == (const array_t& v) const
	{
		return false;
	}


	void*				array_t::operator[] (u32 i)
	{
		if (i < m_ItemCount)
			return &m_Data[i];
		return nullptr;
	}

	void const*			array_t::operator[] (u32 i) const
	{
		if (i < m_ItemCount)
			return &m_Data[i];
		return nullptr;
	}

	array_t::iterator	array_t::insert_space(iterator& ip, u32 n)
	{
		return ip;
	}

	array_t::iterator	array_t::erase_space(iterator& ip, u32 n)
	{
		return ip;
	}
}

//#include "xcore/private/x_array_inline.h"

#endif	///< __XCORE_ARRAY_H__
