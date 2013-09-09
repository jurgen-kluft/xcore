// x_queue.h - xCore queue
#ifndef __XBASE_QUEUE_H__
#define __XBASE_QUEUE_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_debug.h"
#include "xbase\x_allocator.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	class x_iallocator;

	//------------------------------------------------------------------------------
	// Author:
	//		Virtuos
	// Description:
	//		The queue is using a double linked list. This kind of queue can be very 
	//		fast in both addition and removal. Object cache locality can be high
	//		when using pool allocators.
	//
	//------------------------------------------------------------------------------
	struct xqueuenode
	{
		xqueuenode*		mNext;
		xqueuenode*		mPrev;
	};

	template <typename T>
	class xqueue_heap_strategy;

	template <typename T>
	class xqueue_member_strategy;

	template <typename T>
	class xqueue_derive_strategy;

	template <typename T, typename P = xqueue_heap_strategy<T> >
	class xqueue
	{
	public:
							xqueue(P const& _policy);
							~xqueue();

		u32					size() const;
		bool				empty() const;
		void				clear();

		void				enqueue(T const& item);
		T const&			back() const;
		T const&			front() const;
		void				dequeue();

	private:
		xqueuenode			mSentry;
		u32					mCount;
		P					mStrategy;
	};

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

#include "xbase\private\x_queue_inline.h"

//==============================================================================
// END
//==============================================================================
#endif    /// __XBASE_QUEUE_H__

