#include "xbase/x_qsort.h"
#include "xcore/x_array.h"

namespace xcore
{
	s32			cmp_memcmp(void const* a, void const* b, void* data)
	{
		u32 const itemSize = (u32 const)data;
		return x_memcmp(a, b, itemSize);
	}
	void		array_t::sort()
	{
		xqsort(m_Data, m_ItemCount, m_ItemSize, cmp_memcmp, (void*)m_ItemSize);
	}

}
