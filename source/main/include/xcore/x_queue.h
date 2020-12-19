// x_queue.h - xCore queue
#ifndef __XBASE_QUEUE_H__
#define __XBASE_QUEUE_H__
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

	class queue_t
	{
	public:
		struct node_t;
		struct cache_t
		{
			node_t*			next;
			node_t*			prev;
		};
		struct functors
		{
			void*			mUser;
			void			(*get)(void* user, node_t* n, cache_t& c);
			void			(*set)(void* user, node_t* node, cache_t& c);
		};

		queue_t(functors fns);
		queue_t(void* user, void(*get)(void* user, node_t* n, queue_t::cache_t& c), void(*set)(void* user, node_t* n, queue_t::cache_t& c));
		~queue_t();

		u32					size() const;
		bool				empty() const;
		void				clear();

		void				enqueue(node_t* item);
		bool				dequeue(node_t*& item);

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

#include "xcore/private/x_queue_inline.h"


namespace xcore
{
	/*
	This use-case shows how to create your own object that can be managed by the queue.
	In this example next/prev are 32-bit integers and all items are actually part of
	a c-array. This just shows how you can tweak/tune the items you want to add into
	a queue.
	*/
	inline void QueueUseCase1()
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
			f32						node_to_item(queue_t::node_t* n)
			{
				my_item* item = (my_item*)n;
				return item->mItem;
			}

			u32						node_to_idx(queue_t::node_t* n)
			{
				if (n == NULL) return my_item::NULL_IDX;
				my_item* item = (my_item*)n;
				u32 idx = (item - items);
				return idx;
			}
			queue_t::node_t* 		idx_to_node(u32 idx)
			{
				if (idx == my_item::NULL_IDX) return NULL;
				my_item* item = &items[idx];
				return (queue_t::node_t*)item;
			}

			// Interface for a double linked list node used by queue_t
			static void				get(void* user, queue_t::node_t* n, queue_t::cache_t& c)
			{
				if (n == NULL) return;
				my_items* items = (my_items*)user;
				my_item* item = (my_item*)n;
				c.next = items->idx_to_node(item->mNext);
				c.prev = items->idx_to_node(item->mPrev);
			}
			static void				set(void* user, queue_t::node_t* node, queue_t::cache_t& c)
			{
				my_items* items = (my_items*)user;
				my_item* item = (my_item*)node;
				item->mNext = items->node_to_idx(c.next);
				item->mPrev = items->node_to_idx(c.prev);
			}

			my_item items[100];
		};

		my_items items;
		queue_t queue(&items, &my_items::get, &my_items::set);

		// Give all items a value
		for (u32 i = 0; i < 100; i++)
		{
			items.items[i].mItem = i * 3.5f;
		}

		// Add all items to the queue
		for (u32 i = 0; i < 100; i++)
		{
			queue.enqueue(items.idx_to_node(i));
		}

		queue_t::node_t* node;
		while (queue.dequeue(node))
		{

		}
	}

	inline void QueueUseCase2()
	{
		struct mynode
		{
			mynode*			m_next;
			mynode*			m_prev;
			f32				m_item;

			static void		get(void* user, queue_t::node_t* n, queue_t::cache_t& c)
			{
				if (n == NULL) return;
				mynode* mn = (mynode*)n;
				c.next = (queue_t::node_t*)mn->m_next;
				c.prev = (queue_t::node_t*)mn->m_prev;
			}
			static void		set(void* user, queue_t::node_t* node, queue_t::cache_t& c)
			{
				mynode* mn = (mynode*)node;
				mn->m_next = (mynode*)c.next;
				mn->m_prev = (mynode*)c.prev;
			}
		};

		const s32 cNumNodes = 20;
		mynode nodes[cNumNodes];

		queue_t queue(nullptr, &mynode::get, &mynode::set);

		for (u32 i = 0; i < cNumNodes; i++)
		{
			nodes[i].m_item = (f32)i;
			queue.enqueue((queue_t::node_t*)&nodes[i]);
		}

		for (u32 i = 0; i < cNumNodes; i++)
		{
			queue_t::node_t* n;
			queue.dequeue(n);
		}
	}

}








namespace xcore
{
	namespace queue_t_private
	{
		inline void	enqueue(queue_t::functors& fns, queue_t::node_t*& front, queue_t::node_t*& back, queue_t::node_t* node)
		{
			if (back == NULL)
			{
				queue_t::cache_t node_cache{ nullptr, nullptr };
				fns.set(fns.mUser, node, node_cache);
				back = node;
				front = node;
			}
			else
			{
				queue_t::cache_t back_cache;
				fns.get(fns.mUser, back, back_cache);
				back_cache.next = node;
				fns.set(fns.mUser, back, back_cache);
				queue_t::cache_t node_cache{ nullptr, back };
				fns.set(fns.mUser, node, node_cache);
			}
		}

		inline queue_t::node_t*	dequeue(queue_t::functors& fns, queue_t::node_t*& front, queue_t::node_t*& back)
		{
			queue_t::node_t* node = front;
			queue_t::cache_t cache;
			fns.get(fns.mUser, front, cache);
			front = cache.next;
			if (front == NULL)
			{
				back = NULL;
			}
			else
			{
				fns.get(fns.mUser, front, cache);
				cache.prev = NULL;
				fns.set(fns.mUser, front, cache);
			}
			cache.next = cache.prev = NULL;
			fns.set(fns.mUser, node, cache);
			return node;
		}
	}

	inline				queue_t::queue_t(queue_t::functors fns)
		: mFunctors(fns)
		, mHead(NULL)
		, mTail(NULL)
		, mCount(0)
	{
	}

	inline				queue_t::queue_t(void* user, void(*get)(void* user, node_t* n, queue_t::cache_t& c), void(*set)(void* user, node_t* node, queue_t::cache_t& c))
		: mHead(NULL)
		, mTail(NULL)
		, mCount(0)
	{
		mFunctors.mUser = user;
		mFunctors.get= get;
		mFunctors.set= set;
	}

	inline				queue_t::~queue_t()
	{
		clear();
	}

	inline u32			queue_t::size() const
	{
		return mCount;
	}

	inline bool			queue_t::empty() const
	{
		return mCount == 0;
	}

	inline void			queue_t::clear()
	{
		while (mCount > 0)
		{
			queue_t::node_t* item;
			dequeue(item);
		}
	}

	inline void			queue_t::enqueue(queue_t::node_t* item)
	{
		queue_t_private::enqueue(mFunctors, mHead, mTail, item);
		++mCount;
	}

	inline queue_t::node_t*		queue_t::front() const
	{
		return mHead;
	}

	inline queue_t::node_t*		queue_t::back() const
	{
		return mTail;
	}

	inline bool					queue_t::dequeue(queue_t::node_t*& node)
	{
		if (mCount > 0)
		{
			node = queue_t_private::dequeue(mFunctors, mHead, mTail);
			--mCount;
			return true;
		}
		node = NULL;
		return false;
	}
}


//==============================================================================
// END
//==============================================================================
#endif    /// __XBASE_QUEUE_H__

