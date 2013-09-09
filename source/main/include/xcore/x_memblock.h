#ifndef __XBASE_MEMBLOCK_H__
#define __XBASE_MEMBLOCK_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xcore\x_memlink.h"

namespace xcore
{
	class x_iallocator;
	class cmemlink;

	// Author:
	//		Jurgen Kluft
	// Brief:
	//      xmemblock
	//
	// Description:
	//      Adds memory management capabilities to xmemlink. Uses allocate and reallocate to
	//      maintain the internal pointer, but only if allocated using members of this class,
	//      or if linked to using the manage() member function. Managed memory is automatically
	//      freed in the destructor.
	//
	//<CODE [lang= "c++"]>
	//     x_iallocator* al;					// A pointer to a known instance
	//     xmemblock a(al, 46721);				// Will allocate memory by using the allocator
	//     ASSERT (a.size() == 46721));
	//     xmemblock b;
	//     b = a;								// 'b' will allocate its own memory and copy content from 'a'
	//     ASSERT (b.size() == 46721));
	//     ASSERT (b.begin()+34 != a.begin+34);
	//     ASSERT (0 == memcmp (a, b, 12));
	//		a.clear();
	//     ASSERT (a.size() == 0));
	//     ASSERT (b.size() == 46721));
	//		a.clear();
	//     ASSERT (b.size() == 0));
	//</CODE>
	//		
	class xmemblock : public xmemlink
	{
	public:
								xmemblock (void);
								xmemblock (x_iallocator*);
								xmemblock (x_iallocator*, const void* p, u32 n);
		explicit				xmemblock (x_iallocator*, u32 n);
		explicit				xmemblock (x_iallocator*, const xcmemlink& b);
		explicit				xmemblock (x_iallocator*, const xmemlink& b);
								xmemblock (x_iallocator*, const xmemblock& b);
                                xmemblock (const xmemblock& b);

								~xmemblock (void);

		void					bind (x_iallocator* a);
		void					set_allocator (x_iallocator* a);

		void					assign (const void* p, u32 n);
		inline void				assign (const xcmemlink& l)					{ assign (l.cdata(), l.size()); }

		void					swap (xmemblock& l);
		void					reserve (u32 newSize, bool bExact = true);
		void					resize (u32 newSize, bool bExact = true);
		iterator				insert (iterator start, u32 size);
		iterator				erase (iterator start, u32 size);
		inline void				clear (void)								{ resize (0); }
		inline u32				capacity (void) const						{ return (m_Capacity); }
		inline bool				is_linked (void) const						{ return (!capacity()); }
		inline u32				max_size (void) const						{ return (is_linked() ? xmemlink::max_size() : capacity()); }
		inline void				manage (x_iallocator* a, xmemlink& l)		{ manage (a, l.begin(), l.size()); }
		void					manage (x_iallocator* a, void* p, u32 n);
		void					deallocate (void);
		void					copy_link (x_iallocator* a);
	//	void					copy_link (); // use the existing member allocator

		inline const xmemblock&	operator= (const xcmemlink& l)				{ assign (l); return (*this); }
		inline const xmemblock&	operator= (const xmemlink& l)				{ assign (l); return (*this); }
		inline const xmemblock&	operator= (const xmemblock& l)				{ m_Allocator = l.m_Allocator; assign (l); return (*this); }

	private:
		x_iallocator*			m_Allocator;
	};

}

#endif	///< __XBASE_MEMBLOCK_H__
