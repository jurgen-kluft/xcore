#ifndef __XBASE_H__
#define __XBASE_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	//----------------------------------------------------------------------------------------------------------------
	extern x_iallocator*	x_GetXBaseAllocator();

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

//==============================================================================
// END
//==============================================================================
#endif // __XBASE_H__