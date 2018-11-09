// x_pqueue.h - xCore priority queue
#ifndef __XBASE_PQUEUE_H__
#define __XBASE_PQUEUE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"
#include "xbase/private/x_std.h"

#include "xcore/x_tree.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	class xalloc;

	//------------------------------------------------------------------------------
	// Author:
	//		Virtuos
	// Description:
	//		This priority queue is based on a red-black tree capable of handling
	//		duplicate keys.
	//------------------------------------------------------------------------------
	struct xpqueuenode : public xrbnode_multi
	{
		inline				xpqueuenode() { clear(); }
	};

	template <typename T, typename P>
	class xpqueue_heap_strategy;

	template <typename T, typename P>
	class xpqueue_member_strategy;

	template <typename T, typename P>
	class xpqueue_derive_strategy;

	template <typename T, typename P, typename C = xstd::xcompare<P>, typename S = xpqueue_heap_strategy<T,P> >
	class xpqueue
	{
	public:
							xpqueue(S const& _strategy);
							~xpqueue();

		u32					size() const;
		bool				empty() const;
		void				clear();

		void				enqueue(T const& item, P const& priority);
		
		T const&			min() const;
		T const&			max() const;

		void				dequeue_min();
		void				dequeue_max();

	private:
		u32					mCount;
		S					mStrategy;
		xpqueuenode			mRoot;
	};

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

#include "xcore/private/x_pqueue_inline.h"

//==============================================================================
// END
//==============================================================================
#endif    /// __XBASE_PQUEUE_H__

