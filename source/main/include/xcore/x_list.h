// x_list.h - xCore doubly linked list
#ifndef __XCORE_LIST_H__
#define __XCORE_LIST_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	class alloc_t;

	//------------------------------------------------------------------------------
	// Author:
	//		Virtuos
	// Description:
	//		The queue is using a double linked list. This kind of queue can be very 
	//		fast in both addition and removal. Object cache locality can be high
	//		when using pool allocators.
	//
	//------------------------------------------------------------------------------

	class list_t
	{
	public:
		struct node_t;
		struct functors
		{
			void*			mUser;
			node_t*			(*getnext)(void* user, node_t* n);
			node_t*			(*getprev)(void* user, node_t* n);
			void(*setnext)(void* user, node_t* node, node_t* next);
			void(*setprev)(void* user, node_t* node, node_t* prev);
		};

		list_t(functors fns);
		list_t(void* user, node_t*(*getnext)(void* user, node_t* n), node_t*(*getprev)(void* user, node_t* n), void(*setnext)(void* user, node_t* node, node_t* next), void(*setprev)(void* user, node_t* node, node_t* prev));
		~list_t();

		u32					size() const;
		bool				empty() const;
		void				clear();

		void				push_front(node_t* item);
		void				push_back(node_t* item);
		bool				pop_front(node_t*& item);
		bool				pop_back(node_t*& item);

		node_t*				front() const;
		node_t*				back() const;

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

#include "xcore/private/x_list_inline.h"


namespace xcore
{
	/*
	This use-case shows how to create your own object that can be managed by the queue.
	In this example next/prev are 32-bit integers and all items are actually part of
	a c-array. This just shows how you can tweak/tune the items you want to add into
	a queue.
	*/
	inline void ListUseCase()
	{
		struct my_item
		{
			enum { NULL_IDX = 0xffffffff };
			inline	my_item() : mNext(NULL_IDX), mPrev(NULL_IDX), mItem(0.0f) {}
			u32		mNext;
			u32		mPrev;
			f32		mItem;
		};

		struct my_items
		{
			f32						node_to_item(list_t::node_t* n)
			{
				my_item* item = (my_item*)n;
				return item->mItem;
			}

			u32						node_to_idx(list_t::node_t* n)
			{
				if (n == NULL) return my_item::NULL_IDX;
				my_item* item = (my_item*)n;
				u32 idx = (item - items);
				return idx;
			}
			list_t::node_t* 		idx_to_node(u32 idx)
			{
				if (idx == my_item::NULL_IDX) return NULL;
				my_item* item = &items[idx];
				return (list_t::node_t*)item;
			}

			// Interface for a double linked list node used by list_t
			static list_t::node_t*		getnext(void* user, list_t::node_t* n)
			{
				if (n == NULL) return NULL;
				my_items* items = (my_items*)user;
				my_item* item = (my_item*)n;
				u32 idx = item->mNext;
				return items->idx_to_node(idx);
			}
			static list_t::node_t*		getprev(void* user, list_t::node_t* n)
			{
				if (n == NULL) return NULL;
				my_items* items = (my_items*)user;
				my_item* item = (my_item*)n;
				u32 idx = item->mPrev;
				return items->idx_to_node(idx);
			}
			static void					setnext(void* user, list_t::node_t* node, list_t::node_t* next)
			{
				my_items* items = (my_items*)user;
				my_item* item = (my_item*)node;
				item->mNext = items->node_to_idx(next);
			}
			static void					setprev(void* user, list_t::node_t* node, list_t::node_t* prev)
			{
				my_items* items = (my_items*)user;
				my_item* item = (my_item*)node;
				item->mPrev = items->node_to_idx(prev);
			}

			my_item items[100];
		};

		my_items items;
		list_t list(&items, &items.getnext, &items.getprev, &items.setnext, &items.setprev);

		// Give all items a value
		for (u32 i = 0; i < 100; i++)
		{
			items.items[i].mItem = i * 3.5f;
		}

		// Add all items to the queue
		for (u32 i = 0; i < 100; i++)
		{
			list.push_back(items.idx_to_node(i));
		}

		list_t::node_t* node;
		while (list.pop_back(node))
		{

		}
	}
}



//==============================================================================
// END
//==============================================================================
#endif    /// __XCORE_LIST_H__