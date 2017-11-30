// x_stack.h - xCore stack
#ifndef __XBASE_STACK_H__
#define __XBASE_STACK_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"
#include "xcore/x_slist.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	class x_iallocator;
	struct xlistnode;

	template <typename T>
	class xstack_heap_strategy;

	template <typename T>
	class xstack_member_strategy;

	template <typename T>
	class xstack_derive_strategy;

	//------------------------------------------------------------------------------
	// Author:
	//		Jurgen Kluft
	// Description:
	//		The 'c' stack is build using a single linked list. This kind of stack can
	//		be very fast in both addition and removal. Object cache locality can be high
	//		when using pool allocators.
	//
	//------------------------------------------------------------------------------
	template <typename T, typename P = xstack_heap_strategy<T> >
	class xstack
	{
	public:
							xstack();
							xstack(P const& _policy);
							~xstack();

		u32					size() const;
		bool				empty() const;
		void				clear();

		void				push(T const& item);
		T const&			peek() const;
		void				pop();

	private:
		xlistnode			sentry;
		u32					count;
		P					policy;
	};

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

#include "xcore/private/x_stack_inline.h"

//==============================================================================
// END
//==============================================================================
#endif    /// __XBASE_STACK_H__

