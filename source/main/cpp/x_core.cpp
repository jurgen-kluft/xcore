#include "xbase/x_base.h"
#include "xbase/x_allocator.h"

namespace xcore
{
	static x_iallocator* sXBaseAllocator = NULL;

	void			x_Init(x_iallocator* allocator)
	{
		sXBaseAllocator = allocator;
	}

	x_iallocator*	x_GetXBaseAllocator()
	{
		return sXBaseAllocator;
	}

	void			x_Exit()
	{
		sXBaseAllocator = NULL;
	}
};


