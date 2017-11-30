// x_tree.h - xCore red-black tree
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
#include "xbase/x_allocator.h"

namespace xcore
{
//#ifdef TARGET_TEST
	#define DEBUG_RBTREE
//#endif

	//------------------------------------------------------------------------------
	// Author:
	//		Jurgen Kluft
	// Description:
	//
	//      Red-black tree structure and helper-functions
	// 
	//<CODE [lang= "c++"]>
	// 		
	// 	Need an example here
	// 		
	//</CODE>
	//     
	//------------------------------------------------------------------------------
	struct xrbnode
	{
	protected:
		enum EColor { RED = 2, BLACK = 0, COLOR_MASK = 2 };

	public:
		enum ESide { LEFT = 0, RIGHT = 1, SIDE_MASK = 1 };
		enum EMask { MASK = SIDE_MASK | COLOR_MASK };

		inline void			clear() { parent = child[LEFT] = child[RIGHT] = this; }

		inline void			set_child(xrbnode* node, s32 dir) { child[dir] = node; }
		inline xrbnode*		get_child(s32 dir) const { return child[dir]; }
		inline xrbnode*		get_non_null_child() const { s32 d = child[LEFT]->is_nill(); return child[d]; }

		inline void			set_right(xrbnode* node) { child[RIGHT] = node; }
		inline xrbnode*		get_right() const { return child[RIGHT]; }

		inline void			set_left(xrbnode* node) { child[LEFT] = node; }
		inline xrbnode*		get_left() const { return child[LEFT]; }

		inline void			set_parent(xrbnode* node) { u32 const flags = (u32)((uptr)parent & MASK); parent = (xrbnode*)((uptr)node | (uptr)flags); }
		inline xrbnode*		get_parent() const { return (xrbnode*)((uptr)parent & ~(uptr)MASK); }

		inline void			set_parent_side(s32 side) { ASSERT(side == LEFT || side == RIGHT); if (side == RIGHT) { parent = (xrbnode*)((uptr)parent | (uptr)RIGHT); } else { parent = (xrbnode*)((uptr)parent & (uptr)~SIDE_MASK); } }
		inline s32			get_parent_side() const { s32 const side = (s32)((uptr)parent & SIDE_MASK); ASSERT(side == LEFT || side == RIGHT); return side; }

		inline void			set_red() { parent = (xrbnode*)(((uptr)parent & (uptr)~COLOR_MASK) | (uptr)RED); } 
		inline void			set_black() { parent = (xrbnode*)(((uptr)parent & (uptr)~COLOR_MASK) | (uptr)BLACK); }
		inline void			set_color(s32 colr) { if (colr == RED) set_red(); else set_black(); }
		inline s32			get_color() const { s32 const colr = (s32)((uptr)parent & COLOR_MASK); ASSERT(colr == RED || colr == BLACK); return colr; }

		inline bool			is_red() const { return ((u32)((uptr)parent & COLOR_MASK)) == RED; }
		inline bool			is_black() const { return ((u32)((uptr)parent & COLOR_MASK)) == BLACK; }
		inline bool			is_used() const { return this != child[RIGHT]; }
		inline bool			is_nill() const { return this == child[RIGHT]; }

	protected:
		xrbnode*			parent;
		xrbnode*			child[2];
	};

	// @note	If the key is greater than the key of node then return 1. If the key is less than the key of node then return -1 otherwise return 0
	struct xrbtree
	{
		typedef	s32(*comparer)(const xrbnode* node, const void* key);

		void			init();

		bool			insert(xrbnode * node, void* key, comparer c);
		bool			remove(xrbnode*& node, void* key, comparer c);
		bool			find(xrbnode*& node, void* key, comparer c);

		// @note	Initialize 'iterator' to NULL, iterate until NULL is returned.
		//			The returned node can be destroyed and it is already detached from the tree.
		xrbnode*		clear(xrbnode*& iterator);

#ifdef DEBUG_RBTREE
		bool			test(const char*& result);
#endif

		xrbnode*		root;
		xrbnode			nodes[1];
	};

	struct xrbnode_multi : public xrbnode
	{
		void				clear() { siblings = NULL; xrbnode::clear(); }

		xrbnode_multi*		get_siblings() const
		{
			return siblings;
		}

		void				insert_sibling(xrbnode_multi* sib)
		{
			if (siblings == NULL) siblings = sib;
			else
			{
				xrbnode_multi* next = siblings;
				xrbnode_multi* prev = (xrbnode_multi*)next->get_child(LEFT);
				sib->set_child(prev, LEFT);
				sib->set_child(next, RIGHT);
				prev->set_child(sib, RIGHT);
				next->set_child(sib, LEFT);
			}
		}

		void				remove_sibling(xrbnode_multi* sib)
		{
			xrbnode_multi* next = (xrbnode_multi*)sib->get_child(RIGHT);
			xrbnode_multi* prev = (xrbnode_multi*)sib->get_child(LEFT);
			next->set_child(prev, LEFT);
			prev->set_child(next, RIGHT);
			if (siblings == sib && next != sib)
				siblings = next;
			else
				siblings = NULL;
		}

	protected:
		xrbnode_multi*		siblings;
	};


	// Before using this you need to either traverse to the 
	// minimum (far left) or maximum (far right).
	// 0 = successor
	// 1 = predecessor
	inline xrbnode*	rb_inorder(s32 dir, xrbnode* node)
	{
		if (node->is_nill())
			return node;

		xrbnode* next = node->get_child(1 - dir);
		if (next->is_nill())
		{
			if (node->get_parent_side() != dir)
			{
				while (node->get_parent_side() != dir)
					node = node->get_parent();
				node = node->get_parent();
			}
			else
			{
				node = node->get_parent();
			}
		}
		else
		{
			do {
				node = next;
				next = next->get_child(dir);
			} while (!next->is_nill());
		}
		return node;
	}

#ifdef DEBUG_RBTREE
	inline
		bool			rb_validate_height(xrbnode* nill, xrbnode* node, u32& height)
	{
		if (node == nill)
		{
			height = 0;
			return true;
		}

		if (node->is_black())
		{
			u32 lh, rh;
			if (rb_validate_height(nill, node->get_left(), lh) && rb_validate_height(nill, node->get_right(), rh))
			{
				height = lh + rh + 1;
				return true;
			}
		} 
		else if (node->get_left()->is_black() && node->get_right()->is_black())
		{
			u32 lh, rh;
			if (rb_validate_height(nill, node->get_left(), lh) && rb_validate_height(nill, node->get_right(), rh))
			{
				if (lh == rh)
				{
					height = lh;
					return true;
				}
			}
		}
		return false;
	}

	inline
		bool			rb_validate(xrbnode* root, xrbnode* nill)
	{
		if (nill->is_nill())
		{
			if (root->is_black())
			{
				if (root->get_right() == nill)
				{
					if (root->get_left() == nill  || root->get_left()->is_black())
					{
						u32 height;
						return rb_validate_height(nill, root->get_left(), height);
					}
				}
			}
		}
		return false;
	}

	inline
		void			rb_test(xrbnode* root, xrbnode* nill)
	{
		ASSERT(rb_validate(root, nill));
	}

	inline
		bool			xrbtree::test(const char*& result)
	{
		if (!rb_validate(root, root))
		{
			result = "invalid tree";
			return false;
		}
		return true;
	}

#endif

	//////////////////////////////////////////////////////////////////////////
	inline
	void			rb_rotate(xrbnode* nill, xrbnode* node, s32 s)
	{
		ASSERT(node->get_parent()->get_child(node->get_parent_side()) == node);
		s32 o = (1 - s);
		s32 ps = node->get_parent_side();
		xrbnode* top = node->get_child(o);
		xrbnode* top_child = top->get_child(s);
		node->set_child(top_child, o);
		if (top_child != nill)
		{
			top_child->set_parent(node);
			top_child->set_parent_side(o);
		}
		top->set_parent(node->get_parent());
		top->set_parent_side(ps);
		node->get_parent()->set_child(top, ps);
		top->set_child(node, s);
		node->set_parent(top);
		node->set_parent_side(s);
		ASSERTS(!nill->is_red(), "tree->nil not red");
	}

	void		rb_insert_fixup(xrbnode* nill, xrbnode& head, xrbnode* node);
	void		rb_erase_fixup(xrbnode* nill, xrbnode* root, xrbnode* node);

	inline
	void			rb_attach_to(xrbnode* _this, xrbnode* _parent, s32 _s)
	{
		xrbnode* child = _parent->get_child(_s);
		_this->set_left(child);
		_this->set_right(child);
		_this->set_parent(_parent);
		_this->set_parent_side(_s);
		_this->set_red();
		_parent->set_child(_this, _s);
	}

	inline
	void			rb_substitute_with(xrbnode* _this, xrbnode* _child)
	{
		ASSERT(_this != _child);
		s32 ps = _this->get_parent_side();
		xrbnode* parent = _this->get_parent();
		_child->set_parent(parent);
		_child->set_parent_side(ps);
		parent->set_child(_child, ps);
	}

	void		rb_switch_with(xrbnode* _this, xrbnode* _node);

}


//==============================================================================
// END
//==============================================================================
#endif    // __XCORE_REDBLACK_TREE_H__