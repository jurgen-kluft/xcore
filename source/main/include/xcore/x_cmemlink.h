#ifndef __XBASE_CMEMLINK_H__
#define __XBASE_CMEMLINK_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase\x_debug.h"

namespace xcore
{
	class x_iallocator;

	// Author:
	//		Jurgen Kluft
	// Brief:
	//      xcmemlink
	//
	// Description:
	//      A read-only pointer to a sized block of memory.
	//      Use this class the way you would a const pointer to an allocated unstructured block.
	//      The pointer and block size are available through member functions and cast operator.
	//
	// Example usage:
	//
	//<CODE [lang= "c++"]>
	//     void* p = malloc (46721);
	//     xcmemlink a, b;
	//     a.link (p, 46721);
	//     assert (a.size() == 46721));
	//     b = a;
	//     assert (b.size() == 46721));
	//     assert (b.DataAt(34) == a.DataAt(34));
	//     assert (0 == memcmp (a, b, 12));
	//     free(p);	// Yes you do have to free the memory yourself
	//</CODE>
	//
	class xcmemlink
	{
	public:
		typedef xbyte				value_type;
		typedef const value_type*	pointer;
		typedef const value_type*	const_pointer;
		typedef value_type			reference;
		typedef value_type			const_reference;
		typedef u32					written_size_type;
		typedef s32					difference_type;
		typedef const_pointer		const_iterator;
		typedef const_iterator		iterator;
		typedef const xcmemlink&	rcself_t;

	public:
		inline					xcmemlink (void) : m_Capacity (0), m_Data (NULL), m_Size (0) { }
		inline					xcmemlink (const void* p, u32 n) : m_Capacity (0), m_Data (const_pointer(p)), m_Size (n) { ASSERT(p || !n); }
		inline					xcmemlink (const xcmemlink& l) : m_Capacity (0), m_Data (l.m_Data), m_Size (l.m_Size) { }
		inline					~xcmemlink (void)							{ }

		void					link (const void* p, u32 n);
		inline void				link (const xcmemlink& l)					{ link (l.begin(), l.size()); }
		void					link (const void* first, const void* last);

		inline void				relink (const void* p, u32 n);
		void					unlink (void);

		void					swap (xcmemlink& l);
		inline u32				size (void) const							{ return (m_Size); }
		inline u32				max_size (void) const						{ return (size()); }
		inline bool				empty (void) const							{ return (!size()); }

		inline const_pointer	cdata (void) const							{ return (m_Data); }
		inline iterator			begin (void) const							{ return (iterator (cdata())); }
		inline iterator			iat (u32 i) const							{ ASSERT (i <= size()); return (begin() + i); }
		inline iterator			end (void) const							{ return (iat (size())); }
		inline void				resize (u32 n)								{ m_Size = n; }

		inline rcself_t			operator= (const xcmemlink& l)				{ link (l); return (*this); }
		bool					operator== (const xcmemlink& l) const;

	protected:
		///< Number of bytes allocated by Resize. If m_Capacity==0 but we are managing a block of memory with
		///< a certain size than this means that we are a link. We can make a copy of that data with copy_link().
		u32					m_Capacity;

	private:
		const_pointer		m_Data;										///< Pointer to the data block (const)
		u32					m_Size;										///< size of the data block
	};

	//----------------------------------------------------------------------

	/// A fast alternative to link which can be used when relinking to the same block (i.e. when it is resized)
	inline void xcmemlink::relink (const void* p, u32 n)
	{
		m_Capacity = 0;
		m_Data = reinterpret_cast<const_pointer>(p);
		m_Size = n;
	}

}

#endif	///< __XBASE_CMEMLINK_H__
