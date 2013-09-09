#include "xbase\x_memblock.h"
#include "xbase\x_allocator.h"
#include "xbase\x_integer.h"
#include "xbase\private\x_std.h"

namespace xcore
{

	xmemblock::xmemblock (void) 
		: xmemlink ()
		, m_Allocator(NULL)
	{

	}

	xmemblock::xmemblock (x_iallocator* a) 
		: xmemlink ()
		, m_Allocator(a)
	{

	}

	xmemblock::xmemblock (x_iallocator* a, const void* p, u32 n) 
		: xmemlink ()
		, m_Allocator(a)
	{ 
		assign (p, n); 
	}

	xmemblock::xmemblock (x_iallocator* a, u32 n) 
		: xmemlink ()
		, m_Allocator(a)
	{ 
		resize (n);
	}

	xmemblock::xmemblock (x_iallocator* a, const xcmemlink& b) 
		: xmemlink ()
		, m_Allocator(a)
	{ 
		assign (b);
	}

	xmemblock::xmemblock (x_iallocator* a, const xmemlink& b) 
		: xmemlink ()
		, m_Allocator(a)
	{
		assign (b); 
	}

	xmemblock::xmemblock (x_iallocator* a, const xmemblock& b) 
		: xmemlink ()
		, m_Allocator(a)
	{
		assign (b); 
	}

    xmemblock::xmemblock (const xmemblock& b)
        : xmemlink()
        , m_Allocator(b.m_Allocator)
    {
        assign(b);
    }

	xmemblock::~xmemblock (void)
	{
		deallocate(); 
	}

	/// resizes the block to \p newSize bytes, reallocating if necessary.
	void xmemblock::resize (u32 newSize, bool bExact)
	{
		if (m_Capacity < newSize)
			reserve (newSize, bExact);
		xmemlink::resize (newSize);
	}

	/// Frees internal data.
	void xmemblock::deallocate (void)
	{
		if (m_Capacity!=0 && m_Allocator!=NULL) 
		{
			ASSERTS(cdata(), "Internal error: space allocated, but the pointer is NULL");
			ASSERTS(data(), "Internal error: read-only block is marked as allocated space");
			m_Allocator->deallocate(data());
		}
		unlink();
	}

	/// Assumes control of the memory block \p p of size \p n.
	/// The block assigned using this function will be freed in the destructor.
	void xmemblock::manage (x_iallocator* a, void* p, u32 n)
	{
		ASSERT (p || !n);
		ASSERTS(!m_Capacity, "Already managing something. deallocate or unlink first.");
		link (p, n);
		m_Allocator = a;
		m_Capacity = n;
	}

	/// "Instantiate" a linked block by allocating and copying the linked data.
	void xmemblock::copy_link (x_iallocator* a)
	{
		ASSERT (is_linked());
		const pointer p (begin());
		const u32 sz (size());
		unlink();
		m_Allocator = a;
		assign (p, sz);
	}

	/*
	void xmemblock::copy_link(void)
	{
		copy_link(m_Allocator);
	}
	*/

	/// Copies data from \p p, \p n.
	void xmemblock::assign (const void* p, u32 n)
	{
		ASSERTS((p != (const void*) cdata() || size() == n), "Self-assignment can not resize");
		resize (n);
		xstd::xcopy_n (pointer(p), n, begin());
	}

	void xmemblock::swap (xmemblock& l)
	{
		xmemlink::swap (l);
		xstd::xswap(m_Allocator, l.m_Allocator);
	}

	/// \brief Reallocates internal block to hold at least \p newSize bytes.
	///
	/// Additional memory may be allocated, but for efficiency it is a very
	/// good idea to call reserve before doing byte-by-byte edit operations.
	/// The block size as returned by size() is not altered. reserve will not
	/// reduce allocated memory. If you think you are wasting space, call
	/// deallocate and start over. To avoid wasting space, use the block for
	/// only one purpose, and try to get that purpose to use similar amounts
	/// of memory on each iteration.
	///
	void xmemblock::reserve (u32 newSize, bool bExact)
	{
		if (newSize <= m_Capacity)
			return;

		pointer oldBlock (is_linked() ? NULL : data());

		const u32 alignedSize (x_intu::alignUp(newSize, 32));
		if (!bExact)
			newSize = alignedSize;

		u32 newAlignment = 4;
		pointer newBlock = (pointer) m_Allocator->reallocate(oldBlock, newSize, newAlignment);
		if (!newBlock)
		{
			ASSERTS(false, "Error; bad allocation");
			return;
		}

		if (!oldBlock & (cdata() != NULL))
			xstd::xcopy_n (cdata(), x_intu::min(size() + 1, newSize), newBlock);

		link (newBlock, size());
		m_Capacity = newSize;
	}

	/// Shifts the data in the linked block from \p start to \p start + \p n.
	xmemblock::iterator xmemblock::insert (iterator start, u32 n)
	{
		const u32 ip = start - begin();
		ASSERT (ip <= size());
		resize (size() + n, false);
		xmemlink::insert (iat(ip), n);
		return (iat (ip));
	}

	/// Shifts the data in the linked block from \p start + \p n to \p start.
	xmemblock::iterator xmemblock::erase (iterator start, u32 n)
	{
		const u32 ep = start - begin();
		ASSERT (ep + n <= size());
		xmemlink::erase (start, n);
		xmemlink::resize (size() - n);
		return (iat (ep));
	}

	void xmemblock::set_allocator (x_iallocator* a)
	{

		ASSERT(m_Allocator == NULL); // verify that the allocator is not set already..
		
		m_Allocator = a;
	}

} // namespace ustl
