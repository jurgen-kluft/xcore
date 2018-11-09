#include "xcore/x_tree.h"

namespace xcore
{
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
		xrbnode *			parent;
		xrbnode *			child[2];
	};

	typedef	s32(*comparer)(const xrbnode* node, const void* key);

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
					if (root->get_left() == nill || root->get_left()->is_black())
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
	void			rb_insert_fixup(xrbnode* nill, xrbnode& head, xrbnode* node)
	{
		xrbnode* cur = node;
		xrbnode* p = cur->get_parent();
		while (p->is_red())
		{
			xrbnode* pp = p->get_parent();
			ASSERT(pp != &head);
			s32 s = p->get_parent_side();
			s32 o = (1 - s);
			xrbnode* pp_right = pp->get_child(o);
			if (pp_right->is_red())
			{
				p->set_black();
				pp_right->set_black();
				pp->set_red();
				cur = pp;
				p = cur->get_parent();
			}
			else
			{
				if (cur == p->get_child(o))
				{
					cur = p;
					rb_rotate(nill, cur, s);
					p = cur->get_parent();
				}
				p->set_black();
				pp->set_red();
				rb_rotate(nill, pp, o);
			}
		}
		head.get_left()->set_black();
	}

	void		rb_erase_fixup(xrbnode* nill, xrbnode* root, xrbnode* node)
	{
		xrbnode* head = root;

		xrbnode* cur = node;
		while (!cur->is_red() && cur != head->get_left())
		{
			xrbnode* p = cur->get_parent();
			ASSERT(p != head);
			s32 const s = cur->get_parent_side();
			s32 const o = (1 - s);
			xrbnode* w = p->get_child(o);
			ASSERT(w != head);
			if (w->is_red())
			{
				ASSERT(w->get_left()->is_black() && w->get_right()->is_black());
				w->set_black();
				p->set_red();
				w = w->get_child(s);
				rb_rotate(nill, p, s);
			}
			ASSERT(w != head);
			if (w->get_left()->is_black() && w->get_right()->is_black())
			{
				w->set_red();
				cur = p;
			}
			else
			{
				if (w->get_child(o)->is_black())
				{
					xrbnode* c = w->get_child(s);
					c->set_black();
					w->set_red();
					rb_rotate(nill, w, o);
					w = c;
					ASSERT(w != head);
				}
				ASSERT(w->get_child(o)->is_red());
				w->set_color(p->get_color());
				p->set_black();
				w->get_child(o)->set_black();
				rb_rotate(nill, p, s);
				cur = head->get_left();
			}
		}
		cur->set_black();
	}

	void			rb_switch_with(xrbnode* _this, xrbnode* _node)
	{
		ASSERT(_this != _node);

		// Copy pointers (includes color and parent side)
		xrbnode* parent = _node->get_parent();
		_this->set_parent(parent);
		xrbnode* left = _node->get_left();
		_this->set_left(left);
		xrbnode* right = _node->get_right();
		_this->set_right(right);

		_this->set_color(_node->get_color());
		s32 const ps = _node->get_parent_side();
		_this->set_parent_side(ps);

		left->set_parent(_this);
		left->set_parent_side(xrbnode::LEFT);
		right->set_parent(_this);
		right->set_parent_side(xrbnode::RIGHT);

		// Set ourselves as the new child of our parent
		parent->set_child(_this, ps);
	}


	bool			tree_insert(xrbnode* root, xrbnode * node_to_insert, void* key, comparer cmpfunc)
	{
		ASSERT(node_to_insert != 0);

		xrbnode* endNode = root;
		xrbnode* lastNode = root;
		xrbnode* curNode = (xrbnode*)root->get_left();
		s32 s = xrbnode::LEFT;
		while (curNode != endNode)
		{
			lastNode = curNode;

			s32 c = cmpfunc(curNode, key);
			if (c == 0)
				return xFALSE;	// Duplicate
			s = (c + 1) >> 1;
			curNode = (xrbnode*)curNode->get_child(s);
		}

		xrbnode* node = node_to_insert;

		rb_attach_to(node, lastNode, s);
		rb_insert_fixup(root, *root, node);

#ifdef DEBUG_RBTREE
		rb_validate(root, root);
#endif
		return xTRUE;
	}

	bool		tree_remove(xrbnode* root, xrbnode*& found_node, void* key, comparer cmpfunc)
	{
		xrbnode* nill = root;
		xrbnode* it = (xrbnode*)root->get_left();
		while (it != nill)
		{
			s32 cmp = cmpfunc(it, key);
			if (cmp == 0)
				break;
			s32 const s = (cmp + 1) >> 1;
			it = (xrbnode*)it->get_child(s);
		}

		// Did we find it? If not we have don't have to continue.
		if (it == nill)
		{
			found_node = NULL;
			return false;
		}

		// The node to remove
		xrbnode* node = it;
		ASSERT(node != root);

		xrbnode* endNode = root;
		xrbnode* repl = node;
		s32 s = xrbnode::LEFT;
		if (node->get_right() != endNode)
		{
			if (node->get_left() != endNode)
			{
				repl = (xrbnode*)node->get_right();
				while (repl->get_left() != endNode)
					repl = (xrbnode*)repl->get_left();
			}
			s = xrbnode::RIGHT;
		}
		ASSERT(repl->get_child(1 - s) == endNode);
		bool red = repl->is_red();
		xrbnode* replChild = (xrbnode*)repl->get_child(s);

		rb_substitute_with(repl, replChild);
		ASSERT(endNode->is_black());

		if (repl != node)
			rb_switch_with(repl, node);

		ASSERT(endNode->is_black());

		if (!red)
			rb_erase_fixup(nill, root, replChild);

		node->clear();

#ifdef DEBUG_RBTREE
		rb_validate(root, nill);
#endif
		found_node = node;
		return true;
	}

	bool		tree_find(xrbnode* root, xrbnode*& found_node, void* key, comparer cmpfunc)
	{
		xrbnode* nill = root;
		xrbnode* it = (xrbnode*)root->get_left();
		while (it != nill)
		{
			s32 const cmp = cmpfunc(it, key);
			if (cmp == 0)
				break;
			s32 const s = (cmp + 1) >> 1;
			it = (xrbnode*)it->get_child(s);
		}
		found_node = (it == nill) ? NULL : it;
		return found_node != NULL;
	}

	xrbnode*	tree_clear(xrbnode* root, xrbnode*& save)
	{
		//	Rotate away the left links so that
		//	we can treat this like the destruction
		//	of a linked list
		if (save == NULL)
			save = (xrbnode*)root->get_left();

		xrbnode* it = save;
		save = NULL;

		while (it != root)
		{
			if (it->get_left() == root)
			{
				/* No left links, just kill the node and move on */
				save = (xrbnode*)it->get_right();
				if (it != root)
				{
					it->clear();
					return it;
				}
			}
			else
			{
				/* Rotate away the left link and check again */
				save = (xrbnode*)it->get_left();
				it->set_left(save->get_right());
				save->set_right(it);
			}

			it = save;
			save = NULL;
		}
		root->clear();
		return NULL;
	}
}