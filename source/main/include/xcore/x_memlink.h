#ifndef __XBASE_MEMLINK_H__
#define __XBASE_MEMLINK_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase\x_memory_std.h"
#include "xcore\x_cmemlink.h"

namespace xcore
{
	class x_iallocator;

	// Author:
	//		Jurgen Kluft
	// Brief:
	//      xmemlink
	//
	// Description:
	//      Wrapper for pointer to block with size.
	//      
	//      Use this class the way you would a pointer to an allocated unstructured block.
	//      The pointer and block size are available through member functions and cast operator.
	//
	// Example usage:
	//<CODE [lang= "c++"]>
	//     void* p = malloc (46721);
	//     xmemlink a, b;
	//     a.link (p, 46721);
	//     ASSERT (a.size() == 46721));
	//     b = a;
	//     ASSERT (b.size() == 46721));
	//     ASSERT (b.begin() + 34 == a.begin + 34);
	//     ASSERT (0 == memcmp (a, b, 12));
	//     a.fill (673, b, 42, 67);
	//     b.erase (87, 12);
	//     free(p);							// Yes you do have to free the memory yourself
	//</CODE>
	//
	class xmemlink : public xcmemlink
	{
	public:
		typedef value_type*					pointer;
		typedef xcmemlink::pointer			const_pointer;
		typedef xcmemlink::const_iterator	const_iterator;
		typedef pointer						iterator;
		typedef const xmemlink&				rcself_t;

	public:
		inline					xmemlink (void) : xcmemlink() {}
		inline					xmemlink (void* p, u32 n) : xcmemlink (p, n) {}
		inline					xmemlink (const void* p, u32 n) : xcmemlink (p, n) {}
		inline					xmemlink (rcself_t l) : xcmemlink (l) {}
		inline explicit			xmemlink (const xcmemlink& l) : xcmemlink (l) {}

		inline pointer			data (void)									{ return (const_cast<pointer>(cdata())); }
		inline iterator			begin (void)								{ return (iterator (data())); }
		inline iterator			iat (u32 i)									{ ASSERT (i <= size()); return (begin() + i); }
		inline iterator			end (void)									{ return (iat (size())); }
		inline const_iterator	begin (void) const							{ return (xcmemlink::begin()); }
		inline const_iterator	end (void) const							{ return (xcmemlink::end()); }
		inline const_iterator	iat (u32 i) const							{ return (xcmemlink::iat (i)); }

		inline void				link (const void* p, u32 n)					{ xcmemlink::link (p, n); }
		inline void				link (void* p, u32 n)						{ xcmemlink::link (p, n); }
		inline void				link (const xcmemlink& l)					{ xcmemlink::link (l); }
		inline void				link (xmemlink& l)							{ xcmemlink::link (l); }
		void					link (const void* first, const void* last);
		void					link (void* first, void* last);
		inline void				relink (const void* p, u32 n)				{ xcmemlink::relink (p, n); }
		inline void				relink (void* p, u32 n)						{ xcmemlink::relink (p, n); }
		inline void				swap (xmemlink& l)							{ xcmemlink::swap (l); }
		void					fill (iterator start, const void* p, u32 elsize, u32 elCount = 1);
		inline void				insert (iterator start, u32 size);
		inline void				erase (iterator start, u32 size);

		inline rcself_t			operator= (const xcmemlink& l)				{ xcmemlink::operator= (l); return (*this); }
		inline rcself_t			operator= (rcself_t l)						{ xcmemlink::operator= (l); return (*this); }
	};

	/// Shifts the data in the linked block from \p start to \p start + \p n.
	/// The contents of the uncovered bytes is undefined.
	inline void xmemlink::insert (iterator start, u32 n)
	{
		ASSERT (data() || !n);
		ASSERT (xcmemlink::begin() || !n);
		ASSERT (start >= begin() && start + n <= end());
		x_memmove(start + n, start, end() - start);
	}

	/// Shifts the data in the linked block from \p start + \p n to \p start.
	/// The contents of the uncovered bytes is undefined.
	inline void xmemlink::erase (iterator start, u32 n)
	{
		ASSERT (data() || !n);
		ASSERT (xcmemlink::begin() || !n);
		ASSERT (start >= begin() && start + n <= end());
		x_memmove(start, start + n, end() - start - n);
	}

}

#endif	///< __XBASE_MEMLINK_H__
