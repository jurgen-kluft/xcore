#include "xbase\x_memlink.h"
#include "xbase\x_debug.h"
#include "xbase\private\x_std.h"

namespace xcore
{
	/// Fills the linked block with the given pattern.
	/// \arg start   Offset at which to start filling the linked block
	/// \arg p       Pointer to the pattern.
	/// \arg elSize  Size of the pattern.
	/// \arg elCount Number of times to write the pattern.
	/// Total number of bytes written is \p elSize * \p elCount.
	///
	void xmemlink::fill (iterator start, const void* p, u32 elSize, u32 elCount)
	{
		ASSERT (data() || !elCount || !elSize);
		ASSERT (start >= begin() && start + elSize * elCount <= end());
		if (elSize == 1)
			xstd::xfill_n (start, elCount, *reinterpret_cast<const u8*>(p));
		else while (elCount--)
			start = xstd::xcopy_n (const_iterator(p), elSize, start);
	}

	void	xmemlink::link (const void* first, const void* last)
	{
		xbyte const* f = (xbyte const*)first;
		xbyte const* l = (xbyte const*)last;
		ASSERT(f < l);
		link (first, (u32)(l - f));
	}

	void	xmemlink::link (void* first, void* last)
	{
		xbyte* f = (xbyte*)first;
		xbyte* l = (xbyte*)last;
		ASSERT(f < l);
		link (first, (u32)(l - f));
	}


} // namespace ustl
