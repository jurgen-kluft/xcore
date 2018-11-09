// x_map.h - xCore map
#ifndef __XCORE_MAP_H__
#define __XCORE_MAP_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"
#include "xbase/private/x_std.h"
#include "xcore/x_tree.h"

namespace xcore
{
	class map_t : public tree_t
	{
	public:
		enum { LEFT = 0, RIGHT = 1, PARENT = 2 };

		map_t(functors fns);
		map_t(void* user, s32(*cmp)(void* user, node_t* na, node_t* nb), void(*get)(void* user, node_t* n, content_t& nc), void(*set)(void* user, node_t* n, content_t& nc));

		bool		insert(node_t* n);
		bool		remove(node_t* n);
		bool		contains(node_t* n);

		functors	m_fns;
		node_t*		m_root;
		s32			m_count;
	};

};


namespace xcore
{
	/*
	This use-case shows how to create your own object that can be managed by the queue.
	In this example next/prev are 32-bit integers and all items are actually part of
	a c-array. This just shows how you can tweak/tune the items you want to add into
	a queue.
	*/
	inline void MapUseCase()
	{
		struct my_item
		{
			enum { nullidx = 0xffffffff };
			inline	my_item() : mItem(0.0f) { mLinks[0] = mLinks[1] = mLinks[2] = nullidx; mFlags = 0; mItem = 0.0f; }
			u32		mLinks[3];
			s32		mFlags;
			f32		mItem;
		};

		struct my_items
		{
			f32						node_to_item(map_t::node_t* n)
			{
				my_item* item = (my_item*)n;
				return item->mItem;
			}

			u32						node_to_idx(map_t::node_t* n)
			{
				if (n == NULL) return my_item::nullidx;
				my_item* item = (my_item*)n;
				u32 idx = (item - items);
				return idx;
			}
			map_t::node_t* 			idx_to_node(u32 idx)
			{
				if (idx == my_item::nullidx) return NULL;
				my_item* item = &items[idx];
				return (map_t::node_t*)item;
			}

			// Interface for map_t
			static s32				cmp(void* user, map_t::node_t* na, map_t::node_t* nb)
			{
				my_items* items = (my_items*)user;
				my_item* itema = (my_item*)na;
				my_item* itemb = (my_item*)nb;
				if (itema->mItem < itemb->mItem)
					return -1;
				else if (itema->mItem > itemb->mItem)
					return 1;
				return 0;
			}
			static void				get(void* user, map_t::node_t* n, map_t::content_t& nc)
			{
				my_items* items = (my_items*)user;
				my_item* item = (my_item*)n;
				nc.node = n;
				nc.links[0] = items->idx_to_node(item->mLinks[0]);
				nc.links[1] = items->idx_to_node(item->mLinks[1]);
				nc.links[2] = items->idx_to_node(item->mLinks[2]);
				nc.flags = item->mFlags;
			}
			static void				set(void* user, map_t::node_t* n, map_t::content_t& nc)
			{
				my_items* items = (my_items*)user;
				my_item* item = (my_item*)n;
				item->mLinks[0] = items->node_to_idx(nc.links[0]);
				item->mLinks[1] = items->node_to_idx(nc.links[1]);
				item->mLinks[2] = items->node_to_idx(nc.links[2]);
				item->mFlags = nc.flags;
			}

			my_item items[100];
		};

		my_items items;
		map_t map(&items, &items.cmp, &items.get, &items.set);

		// Give all items a value
		for (u32 i = 0; i < 100; i++)
		{
			items.items[i].mItem = i * 3.5f;
		}

		// Add all items to the queue
		for (u32 i = 0; i < 100; i++)
		{
			map.insert(items.idx_to_node(i));
		}
		for (u32 i = 0; i < 100; i++)
		{
			map.remove(items.idx_to_node(i));
		}
	}
}



#include "xcore/private/x_map_inline.h"

#endif	///< __XBASE_MAP_H__

