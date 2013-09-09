#include "xbase\x_base.h"
#include "xbase\x_allocator.h"
#include "xcore\x_string.h"

namespace xcore
{
	static x_iallocator* sXBaseAllocator = NULL;

	void			x_Init(x_iallocator* allocator)
	{
		sXBaseAllocator = allocator;
		
		// Initialize the string classes with this allocator.
#ifndef SPU
		xstring::sSetAllocator(allocator);
		xstring_tmp::sSetAllocator(allocator);
#endif
	}

	x_iallocator*	x_GetXBaseAllocator()
	{
		return sXBaseAllocator;
	}

	void			x_Exit()
	{
		// Unbind the allocator from the string classes
#ifndef SPU
		xstring::sSetAllocator(NULL);
		xstring_tmp::sSetAllocator(NULL);
#endif

		sXBaseAllocator = NULL;
	}
};


