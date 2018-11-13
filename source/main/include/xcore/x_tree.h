// x_tree.h - XCORE tree
#ifndef __XCORE_REDBLACK_TREE_H__
#define __XCORE_REDBLACK_TREE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"

namespace xcore
{
	class tree_t
	{
	public:
		struct node_t;

		struct functors
		{
			void*			m_user;
			s32				(*cmp)(void* user, node_t* a, node_t* n);
			enum { LEFT = 0, RIGHT = 1, PARENT = 2 };
			node_t*			(*getlink)(void* user, node_t* self, s32 link);
			void			(*setlink)(void* user, node_t* self, s32 link, node_t* linknode);
			u32				(*getflag)(void* user, node_t* self);
			void			(*setflag)(void* user, node_t* self, u32 linkflag);
		};

		tree_t(functors fns);
		tree_t(void* user, s32(*cmp)(void* user, node_t* a, node_t* n), node_t*(*getlink)(void* user, node_t* self, s32 link), void(*setlink)(void* user, node_t* self, s32 link, node_t* linknode), u32(*getflag)(void* user, node_t* self), void(*setflag)(void* user, node_t* self, u32 linkflag));

		bool			insert(node_t*);
		bool			find(node_t*);
		bool			remove(node_t*);

		functors		m_fns;
		
		node_t*			m_nill;
		node_t*			m_nill_link[3];
		u32				m_nill_flag;

		node_t* 		m_root;				// Top of the tree
		node_t*			m_root_link[3];
		u32				m_root_flag;

		u32				m_size;				// Number of items
	};
}


namespace xcore
{
	/*
	This use-case shows how to create your own object that can be managed by the tree.
	In this example child/parent are 32-bit integers and all items are actually part of
	a c-array. This just shows how you can tweak/tune the items you want to add into
	a tree.
	*/
	inline void TreeUseCase1()
	{
		struct my_item
		{
			enum { NULL_IDX = 0xffffffff };
			inline	my_item() : mFlags(0), mItem(0.0f) { mLinks[0] = mLinks[1] = mLinks[2] = NULL_IDX; }
			u32		mLinks[3];
			u32		mFlags;
			f32		mItem;
		};

		struct my_items
		{
			f32						node_to_item(tree_t::node_t* n)
			{
				my_item* item = (my_item*)n;
				return item->mItem;
			}

			u32						node_to_idx(tree_t::node_t* n)
			{
				if (n == NULL) return my_item::NULL_IDX;
				my_item* item = (my_item*)n;
				u32 idx = (item - items);
				return idx;
			}
			tree_t::node_t* 		idx_to_node(u32 idx)
			{
				if (idx == my_item::NULL_IDX) return NULL;
				my_item* item = &items[idx];
				return (tree_t::node_t*)item;
			}

			// Interface for a double linked list node used by queue_t
			static s32				cmp(void* user, tree_t::node_t* a, tree_t::node_t* b)
			{
				my_item* itema = (my_item*)a;
				my_item* itemb = (my_item*)b;
				if (itema->mItem < itemb->mItem)
					return -1;
				if (itema->mItem > itemb->mItem)
					return 1;
				return 0;
			}
			static tree_t::node_t*	getlink(void* user, tree_t::node_t* n, s32 link)
			{
				if (n == NULL) return NULL;
				my_items* items = (my_items*)user;
				my_item* item = (my_item*)n;
				return items->idx_to_node(item->mLinks[link]);
			}
			static void				setlink(void* user, tree_t::node_t* n, s32 link, tree_t::node_t* ln)
			{
				if (n == NULL) return;
				my_items* items = (my_items*)user;
				my_item* item = (my_item*)n;
				item->mLinks[link] = items->node_to_idx(ln);
			}
			static u32				getflag(void* user, tree_t::node_t* n)
			{
				if (n == NULL) return 0;
				my_item* item = (my_item*)n;
				return item->mFlags;
			}
			static void				setflag(void* user, tree_t::node_t* n, u32 flag)
			{
				if (n == NULL) return;
				my_item* item = (my_item*)n;
				item->mFlags = flag;
			}

			my_item items[100];
		};

		my_items items;
		tree_t tree(&items, &my_items::cmp, &my_items::getlink, &my_items::setlink, &my_items::getflag, &my_items::setflag);

		// Give all items a value
		for (u32 i = 0; i < 100; i++)
		{
			items.items[i].mItem = i * 3.5f;
		}

		// Add all items to the queue
		for (u32 i = 0; i < 100; i++)
		{
			tree.insert(items.idx_to_node(i));
		}
		for (u32 i = 0; i < 100; i++)
		{
			tree.remove(items.idx_to_node(i));
		}
	}
}

//==============================================================================
// END
//==============================================================================
#endif    // __XCORE_REDBLACK_TREE_H__