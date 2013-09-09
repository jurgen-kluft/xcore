#ifndef __XBASE_H__
#define __XBASE_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_debug.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	// Forward declarations
	class x_iallocator;

	//----------------------------------------------------------------------------------------------------------------
	// Init / Exit

	extern void		x_Init(x_iallocator* allocator);
	extern void		x_Exit();

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

//==============================================================================
// END
//==============================================================================
#endif // __XBASE_H__
