#include "xbase/x_base.h"
#include "xbase/x_allocator.h"

namespace xcore
{
	static xalloc* sXBaseAllocator = NULL;

	void			x_Init(xalloc* allocator)
	{
		sXBaseAllocator = allocator;
	}

	xalloc*			x_GetXBaseAllocator()
	{
		return sXBaseAllocator;
	}

	void			x_Exit()
	{
		sXBaseAllocator = NULL;
	}
};


