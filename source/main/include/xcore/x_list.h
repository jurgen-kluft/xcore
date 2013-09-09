// x_list.h - xCore doubly linked list
#ifndef __XBASE_LIST_H__
#define __XBASE_LIST_H__
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
	//		This doubly linked list requires a policy. This kind of list can be very 
	//      fast in both addition and removal since node and items can be indexed
	//      when using array's as the source of items and nodes or using the 'member'
	//		of item.
	//      Object cache locality can be high when using pool allocators.
	//
	//      When using a heap provider the removal is slow since the item has to
	//      be searched and cache locality can be an issue.
	//
	//------------------------------------------------------------------------------
	struct xdlistnode
	{
		inline				xdlistnode() : mNext(NULL), mPrev(NULL)			{ }
		inline				xdlistnode(xdlistnode* next, xdlistnode* prev) : mNext(next), mPrev(prev) { }
		inline				xdlistnode(xdlistnode const& other) : mNext(other.mNext), mPrev(other.mPrev) { }

		inline bool			is_used() const									{ return mNext!=NULL; }
		inline void			set_unused()									{ mNext=mPrev=NULL; }

		inline xdlistnode*	prev() const									{ return mPrev; }
		inline xdlistnode*	next() const									{ return mNext; }

		inline void			set_prev(xdlistnode* n)							{ mPrev = n; }
		inline void			set_next(xdlistnode* n)							{ mNext = n; }

	protected:
		xdlistnode*			mNext;
		xdlistnode*			mPrev;
	};

	template <typename T>
	class xlist_heap_strategy;

	template <typename T>
	class xlist_member_strategy;

	template <typename T>
	class xlist_derive_strategy;

	template<typename T, typename P = xlist_heap_strategy<T> >
	class xlist_iterator;

	template<typename T, typename P = xlist_heap_strategy<T> >
	class xlist_const_iterator;

	template <typename T, typename P = xlist_heap_strategy<T> >
	class xlist
	{
	public:
		typedef				xlist_iterator<T, P>		iterator;
		typedef				xlist_const_iterator<T, P>	const_iterator;

							xlist(P const& _policy);
							~xlist();

		u32					size() const;
		bool				empty() const;
		void				clear();

		void				push_back(T const& item);
		void				push_front(T const& item);
		
		iterator			iat(u32 index);
		const_iterator		iat(u32 index) const;

		void				insert_at(iterator iter, T const& item);
		void				remove_at(iterator iter);

		T const&			back() const;
		T const&			front() const;

		void				pop_back();
		void				pop_front();

		iterator			begin();
		iterator			end();
		const_iterator		begin() const;
		const_iterator		end() const;

		iterator			find(T const& item);
		const_iterator		find(T const& item) const;

	private:
		xdlistnode			sentry;
		u32					count;
		P					policy;
	};


	
	#define XLIST_PUSH_FRONT(member_next, member_prev, head, tail, to_add)		\
							to_add->member_next = head;							\
							to_add->member_prev = NULL;							\
							if (head == NULL) tail = to_add;					\
							else head->member_prev = to_add;					\
							head = to_add
		
	#define XLIST_PUSH_BACK(member_next, member_prev, head, tail, to_add)		\
							if (tail==NULL) head = to_add;						\
							else { tail->member_next = to_add; }				\
							to_add->member_next=NULL;							\
							to_add->member_prev=tail;							\
							tail = to_add

	#define XLIST_INSERT_AFTER(member_next, member_prev, node, to_add)			\
							to_add->member_next = node->member_next;			\
							if (to_add->member_next!=NULL)						\
								to_add->member_next->member_prev = to_add;		\
							node->member_next = to_add;							\
							to_add->member_prev = node
							
	#define XLIST_INSERT_BEFORE(member_next, member_prev, node, to_add)			\
							to_add->member_next = node;							\
							to_add->member_prev = node->member_prev;			\
							node->member_prev = to_add;							\
							if (to_add->member_prev!=NULL)						\
								to_add->member_prev->member_next = to_add;							

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

#include "xbase\private\x_list_inline.h"

//==============================================================================
// END
//==============================================================================
#endif    /// __XBASE_LIST_H__

