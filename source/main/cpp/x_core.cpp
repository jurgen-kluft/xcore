#include "xbase/x_base.h"
#include "xbase/x_allocator.h"

namespace xcore
{
	static alloc_t* sXBaseAllocator = NULL;

	void			x_Init(alloc_t* allocator)
	{
		sXBaseAllocator = allocator;
	}

	alloc_t*			x_GetXBaseAllocator()
	{
		return sXBaseAllocator;
	}

	void			x_Exit()
	{
		sXBaseAllocator = NULL;
	}
};


