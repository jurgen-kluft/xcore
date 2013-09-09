#include "xbase\x_cmemlink.h"
#include "xbase\x_memory_std.h"
#include "xbase\private\x_std.h"

namespace xcore
{
	void xcmemlink::swap (xcmemlink& l)
	{
		xstd::xswap(m_Capacity,l .m_Capacity); 
		xstd::xswap(m_Data, l.m_Data); 
		xstd::xswap(m_Size, l.m_Size); 
	}

	void xcmemlink::link (const void* first, const void* last)
	{ 
		link (first, (u32)((xbyte*)last - (xbyte*)first));
	}


	/// \brief Attaches the object to pointer \p p of size \p n.
	///
	/// If \p p is NULL and \p n is non-zero, bad_alloc is thrown and current
	/// state remains unchanged.
	///
	void xcmemlink::link (const void* p, u32 n)
	{
		if (!p && n)
		{
			ASSERT(false);
			return;
		}

		unlink();
		relink (p, n);
	}

	void xcmemlink::unlink (void)
	{
		m_Capacity = 0;
		m_Data = NULL; m_Size = 0;
	}

	/// Compares to memory block pointed by l. Size is compared first.
	bool xcmemlink::operator == (const xcmemlink& l) const
	{
		return (l.m_Size == m_Size && (l.m_Data == m_Data || 0 == x_memcmp (l.m_Data, m_Data, m_Size)));
	}

} // namespace ustl
