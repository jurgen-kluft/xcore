// x_stack.h - xCore stack
#ifndef __XCORE_STACK_H__
#define __XCORE_STACK_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	//------------------------------------------------------------------------------
	// Author:
	//		Virtuos
	// Description:
	//		The queue is using a double linked list. This kind of queue can be very 
	//		fast in both addition and removal. Object cache locality can be high
	//		when using pool allocators.
	//
	//------------------------------------------------------------------------------

	class stack_t
	{
	public:
		struct node_t;
		struct functors
		{
			void*			mUser;
			node_t*			(*getnext)(void* user, node_t* n);
			node_t*			(*getprev)(void* user, node_t* n);
			void			(*setnext)(void* user, node_t* node, node_t* next);
			void			(*setprev)(void* user, node_t* node, node_t* prev);
		};

		stack_t(functors fns);
		stack_t(void* user, node_t*(*getnext)(void* user, node_t* n), node_t*(*getprev)(void* user, node_t* n), void(*setnext)(void* user, node_t* node, node_t* next), void(*setprev)(void* user, node_t* node, node_t* prev));
		~stack_t();

		u32					size() const;
		bool				empty() const;
		void				clear();

		void				push(node_t* item);
		bool				pop(node_t*& item);

		node_t*				top() const;
		node_t*				bottom() const;

	private:
		functors			mFunctors;
		node_t*				mHead;
		node_t*				mTail;
		u32					mCount;
	};

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

#include "xcore/private/x_stack_inline.h"


//==============================================================================
// END
//==============================================================================
#endif    /// __XCORE_STACK_H__