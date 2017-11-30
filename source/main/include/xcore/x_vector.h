#ifndef __XBASE_XVECTOR_H__
#define __XBASE_XVECTOR_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase/x_qsort.h"
#include "xcore/x_memblock.h"
#include "xbase/private/x_std.h"

namespace xcore
{
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
	template <typename T>
	class xvector_iterator;

	template <typename T>
	class xvector_const_iterator;


	template <typename T>
	class xvector
	{
	public:
		typedef T										value_type;
		typedef value_type*								pointer;
		typedef const value_type*						const_pointer;
		typedef value_type&								reference;
		typedef const value_type&						const_reference;
		typedef xvector_iterator<T>						iterator;
		typedef xvector_const_iterator<T>				const_iterator;

	public:
							xvector (void);
							xvector (x_iallocator*);
		explicit			xvector (x_iallocator*, u32 n);
							xvector (x_iallocator*, u32 n, const T& v);
							xvector (x_iallocator*, const xvector<T>& v);
							xvector (x_iallocator*, const_iterator& i1, const_iterator& i2);
                            xvector (const xvector<T>& v);

							~xvector (void);

        void                set_autoalloc(bool bAuto = true);
		void				set_allocator(x_iallocator* a);
		void				relocate(x_iallocator* a);
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
		reference			at (u32 i);
		const_reference		at (u32 i) const;

		iterator			find(T const& item);
		const_iterator		find(T const& item) const;
		
		reference			front (void);
		const_reference		front (void) const;
		reference			back (void);
		const_reference		back (void) const;
		s32					push_back (const T& v = T());
		void				pop_back (void);
		
		void				clear (void);
		void				deallocate (void);
		
		void				assign (const_iterator i1,const_iterator i2);
		void				assign (u32 n, const T& v);
		
		void				swap (xvector<T>& v);
		
		iterator			insert (iterator ip, const T& v = T());
		iterator			insert (iterator ip, u32 n, const T& v);
		iterator			insert (iterator ip, const_iterator i1, const_iterator i2);
		iterator			erase (iterator ep, u32 n = 1);
		iterator			erase (iterator ep1, iterator ep2);
		
		void				sort();
		template <typename C>
		void				sort (C cmp);

		void				manage (pointer p, u32 n);
		
		bool				is_linked (void) const;
		void				unlink (void);
		void				copy_link (x_iallocator* allocator);
		
		void				link (x_iallocator*, const_pointer p, u32 n);
		void				link (x_iallocator*, pointer p, u32 n);
		void				link (const xvector<T>& v);
		void				link (xvector<T>& v);
		void				link (x_iallocator*, const_pointer first, const_pointer last);
		void				link (x_iallocator*, pointer first, pointer last);

							operator xcmemlink (void) const;
							operator xcmemlink (void);
							operator xmemlink (void);

		const xvector<T>&	operator = (const xvector<T>& v);
		bool				operator == (const xvector<T>& v) const;

		reference			operator[] (u32 i);
		const_reference		operator[] (u32 i) const;

	protected:
		iterator			insert_space (iterator& ip, u32 n);
	
	private:
		u32					m_ItemSizeAligned;									///< Item size, aligned
		xmemblock			m_Data;												///< Raw element data, consecutively stored.
        bool                m_IsAutoAlloc;                                        ///< Flag to indicate whether auto reserve memory when push back
	};

}

#include "xcore/private/x_vector_inline.h"

#endif	///< __XBASE_XVECTOR_H__
