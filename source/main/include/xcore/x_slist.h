// x_slist.h - xCore singly linked list
#ifndef __XBASE_SLIST_H__
#define __XBASE_SLIST_H__
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
	/// The singly linked list node
	struct xlistnode
	{
					xlistnode()			{ mNext = this; }
		bool		is_used() const		{ return mNext != this; }
		xlistnode*	mNext;
	};

	//------------------------------------------------------------------------------
	// Author:
	//		Virtuos
	// Description:
	//		The singly linked list requires a policy. This kind of list can be 
	//		very fast in both addition since node and items can be indexed 
	//		when using array's as the source of items and nodes or using the 
	//		'member' of item. Removal is slow since the previous node has to be
	//		searched.
	//		
	//      Object cache locality can be high when using pool allocators.
	//
	//      When using a heap provider the removal is slow since the item has to
	//      be searched and cache locality can be an issue.
	//
	//------------------------------------------------------------------------------

	template <typename T>
	class xslist_heap_strategy;

	template <typename T>
	class xslist_member_strategy;

	template <typename T>
	class xslist_derive_strategy;

	template<typename T, typename P = xslist_heap_strategy<T> >
	class xslist_iterator;

	template<typename T, typename P = xslist_heap_strategy<T> >
	class xslist_const_iterator;

	template <typename T, typename P = xslist_heap_strategy<T> >
	class xslist
	{
	public:
		typedef				xslist_iterator<T, P>		iterator;
		typedef				xslist_const_iterator<T, P>	const_iterator;

							xslist(P const& _policy);

		u32					size() const;
		bool				empty() const;
		void				clear();

		void				push_front(T const& item);

		void				insert_at(u32 index, T const& item);
		void				remove_at(u32 index);

		T const&			front() const;

		void				pop_front();

		iterator			begin();
		iterator			end();
		const_iterator		begin() const;
		const_iterator		end() const;

		const_iterator		find(T const& item) const;

	private:
		xlistnode			sentry;
		u32					count;
		P					policy;
	};


	#define XSLIST_PUSH_HEAD(member_next, head, to_add)							\
							to_add->member_next = head;							\
							head = to_add

	#define XSLIST_PUSH_FRONT(member_next, head, tail, to_add)					\
							to_add->member_next = head;							\
							if (head == NULL) tail = to_add;					\
							head = to_add
		
	#define XSLIST_PUSH_BACK(member_next, head, tail, to_add)					\
							if (tail==NULL) { head = to_add; tail = to_add; }	\
							else { tail->member_next = to_add; }				\
							to_add->member_next=NULL;							\

	#define XSLIST_INSERT_AFTER(member_next, node, to_add)						\
							to_add->member_next = node->member_next;			\
							node->member_next = to_add


	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

#include "xcore\private\x_slist_inline.h"

//==============================================================================
// END
//==============================================================================
#endif    /// __XBASE_SLIST_H__

