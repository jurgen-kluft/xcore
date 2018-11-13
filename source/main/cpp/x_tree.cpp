#include "xcore/x_tree.h"

namespace xcore
{
	struct rbnode;

	rbnode			get_root(tree_t* tree);
	rbnode			get_nill(tree_t* tree);

	tree_t::node_t*	getlink(tree_t* tree, tree_t::node_t* self, s32 link);
	void			setlink(tree_t* tree, tree_t::node_t* self, s32 link, tree_t::node_t* linknode);
	u32				getflag(tree_t* tree, tree_t::node_t* self);
	void			setflag(tree_t* tree, tree_t::node_t* self, u32 linkflag);


	struct rbnode
	{
	public:
		enum EColor { RED = 2, BLACK = 0, COLOR_MASK = 2 };
		enum ESide { LEFT = 0, RIGHT = 1, PARENT = 2, SIDE_MASK = 1 };
		tree_t*				m_tree;
		tree_t::node_t*		m_node;

		inline				rbnode(tree_t* tree, tree_t::node_t* node)
			: m_tree(tree)
			, m_node(node)
		{
		}

		inline void			clear() {
			setlink(m_tree, m_node, LEFT, m_node);
			setlink(m_tree, m_node, RIGHT, m_node);
			setlink(m_tree, m_node, PARENT, m_node);
			setflag(m_tree, m_node, 0);
		}

		inline void			set_child(rbnode& node, s32 dir) { setlink(m_tree, m_node, dir, node.m_node); }
		inline rbnode		get_child(s32 dir) const { tree_t::node_t* node = getlink(m_tree, m_node, dir); return rbnode(m_tree, node); }
		inline void			set_right(rbnode& node) { set_child(node, RIGHT); }
		inline rbnode		get_right() const { return get_child(RIGHT); }
		inline void			set_left(rbnode& node) { set_child(node, LEFT); }
		inline rbnode		get_left() const { return get_child(LEFT); }
		inline void			set_parent(rbnode&node) { set_child(node, PARENT); }
		inline rbnode		get_parent() const { return get_child(PARENT); }

		inline bool			is_parent(rbnode& parent) const { return getlink(m_tree, m_node, PARENT) == parent.m_node; }
		inline bool			is_child(s32 dir, rbnode& node) const { return getlink(m_tree, m_node, dir) == node.m_node; }

		inline void			set_flag(u32 flag) { setflag(m_tree, m_node, flag); }
		inline u32			get_flag() const { return getflag(m_tree, m_node); }

		inline void			set_parent_side(s32 side) { ASSERT(side == LEFT || side == RIGHT); u32 flags = get_flag(); flags = (flags & ~SIDE_MASK) | side; set_flag(flags); }
		inline s32			get_parent_side() const { u32 flags = get_flag(); u32 side = (u32)(flags & SIDE_MASK); ASSERT(side == LEFT || side == RIGHT); return side; }

		inline void			set_red() { u32 flags = get_flag(); flags = flags & ~COLOR_MASK | RED; set_flag(flags); }
		inline void			set_black() { u32 flags = get_flag(); flags = flags & ~COLOR_MASK | BLACK; set_flag(flags); }
		inline void			set_color(s32 colr) { if (colr == RED) set_red(); else set_black(); }
		inline s32			get_color() const { s32 const colr = get_flag() & COLOR_MASK; ASSERT(colr == RED || colr == BLACK); return colr; }

		inline bool			is_red() const { return get_color() == RED; }
		inline bool			is_black() const { return get_color() == BLACK; }
		inline bool			is_used() const { return m_node != getlink(m_tree, m_node, RIGHT); }
		inline bool			is_nill() const { return m_node == getlink(m_tree, m_node, LEFT); }

		inline bool			operator == (const rbnode& other) const { return m_node == other.m_node; }
		inline bool			operator != (const rbnode& other) const { return m_node != other.m_node; }
	};

	inline rbnode			get_root(tree_t* tree) { return rbnode(tree, (tree_t::node_t*)&tree->m_root); }
	inline rbnode			get_nill(tree_t* tree) { return rbnode(tree, (tree_t::node_t*)&tree->m_nill); }

	tree_t::node_t*			getlink(tree_t* tree, tree_t::node_t* self, s32 link)
	{
		// Root and Nill nodes are local to this tree
		if (tree->m_root == self)
		{
			return tree->m_root_link[link];
		}
		else if (tree->m_nill == self)
		{
			return tree->m_nill_link[link];
		}
		return tree->m_fns.getlink(tree->m_fns.m_user, self, link);
	}

	void					setlink(tree_t* tree, tree_t::node_t* self, s32 link, tree_t::node_t* linknode)
	{
		// Root and Nill nodes are local to this tree
		if (tree->m_root == self)
		{
			tree->m_root_link[link] = linknode;
		}
		else if (tree->m_nill == self)
		{
			tree->m_nill_link[link] = linknode;
		}
		else
		{
			tree->m_fns.setlink(tree->m_fns.m_user, self, link, linknode);
		}
	}

	u32						getflag(tree_t* tree, tree_t::node_t* self)
	{
		// Root and Nill nodes are local to this tree
		if (tree->m_root == self)
		{
			return tree->m_root_flag;
		}
		else if (tree->m_nill == self)
		{
			return tree->m_nill_flag;
		}
		return tree->m_fns.getflag(tree->m_fns.m_user, self);
	}

	void					setflag(tree_t* tree, tree_t::node_t* self, u32 linkflag)
	{
		// Root and Nill nodes are local to this tree
		if (tree->m_root == self)
		{
			tree->m_root_flag = linkflag;
		}
		else if (tree->m_nill == self)
		{
			tree->m_nill_flag = linkflag;
		}
		else
		{
			tree->m_fns.setflag(tree->m_fns.m_user, self, linkflag);
		}
	}


	typedef	s32(*comparer)(const tree_t::node_t* node, const void* key);

	// Before using this you need to either traverse to the 
	// minimum (far left) or maximum (far right).
	// 0 = successor
	// 1 = predecessor
	inline rbnode	rb_inorder(s32 dir, rbnode& node)
	{
		if (node.is_nill())
			return node;

		rbnode next = node.get_child(1 - dir);
		if (next.is_nill())
		{
			if (node.get_parent_side() != dir)
			{
				while (node.get_parent_side() != dir)
					node = node.get_parent();
				node = node.get_parent();
			}
			else
			{
				node = node.get_parent();
			}
		}
		else
		{
			do {
				node = next;
				next = next.get_child(dir);
			} while (!next.is_nill());
		}
		return node;
	}

#ifdef DEBUG_RBTREE
	inline
		bool			rb_validate_height(rbnode* nill, rbnode* node, u32& height)
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
		bool			rb_validate(rbnode* root, rbnode* nill)
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
		void			rb_test(rbnode* root, rbnode* nill)
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
		void			rb_rotate(rbnode& nill, rbnode& node, s32 s)
	{
		rbnode parent = node.get_parent();
		ASSERT(parent.get_child(node.get_parent_side()) == node);
		s32 o = (1 - s);
		s32 ps = node.get_parent_side();
		rbnode top = node.get_child(o);
		rbnode top_child = top.get_child(s);
		node.set_child(top_child, o);
		if (top_child != nill)
		{
			top_child.set_parent(node);
			top_child.set_parent_side(o);
		}
		top.set_parent(parent);
		top.set_parent_side(ps);
		parent.set_child(top, ps);
		top.set_child(node, s);
		node.set_parent(top);
		node.set_parent_side(s);

		ASSERTS(!nill.is_red(), "tree.nill not red");
	}

	void		rb_insert_fixup(rbnode& nill, rbnode& head, rbnode& node);
	void		rb_erase_fixup(rbnode& nill, rbnode& root, rbnode&node);

	inline void	rb_attach_to(rbnode& _this, rbnode& _parent, s32 _s)
	{
		rbnode links = _parent.get_child(_s);
		_this.set_left(links);
		_this.set_right(links);
		_this.set_parent(_parent);
		_this.set_parent_side(_s);
		_this.set_red();
		_parent.set_child(_this, _s);
	}

	inline
		void			rb_substitute_with(rbnode& _this, rbnode& _child)
	{
		ASSERT(_this != _child);
		s32 ps = _this.get_parent_side();
		rbnode parent = _this.get_parent();
		_child.set_parent(parent);
		_child.set_parent_side(ps);
		parent.set_child(_child, ps);
	}

	void			rb_switch_with(rbnode& _this, rbnode& _node);
	void			rb_insert_fixup(rbnode& nill, rbnode& head, rbnode& node)
	{
		rbnode cur = node;
		rbnode p = cur.get_parent();
		while (p.is_red())
		{
			rbnode pp = p.get_parent();
			ASSERT(pp != head);
			s32 s = p.get_parent_side();
			s32 o = (1 - s);
			rbnode pp_right = pp.get_child(o);
			if (pp_right.is_red())
			{
				p.set_black();
				pp_right.set_black();
				pp.set_red();
				cur = pp;
				p = cur.get_parent();
			}
			else
			{
				if (cur == p.get_child(o))
				{
					cur = p;
					rb_rotate(nill, cur, s);
					p = cur.get_parent();
				}
				p.set_black();
				pp.set_red();
				rb_rotate(nill, pp, o);
			}
		}

		rbnode head_left = head.get_left();
		head_left.set_black();
	}

	void		rb_erase_fixup(rbnode& nill, rbnode& root, rbnode& node)
	{
		rbnode head = root;
		rbnode cur = node;
		while (!cur.is_red() && cur != head.get_left())
		{
			rbnode p = cur.get_parent();
			ASSERT(p != head);
			s32 const s = cur.get_parent_side();
			s32 const o = (1 - s);
			rbnode w = p.get_child(o);
			ASSERT(w != head);
			if (w.is_red())
			{
				ASSERT(w.get_left().is_black() && w.get_right().is_black());
				w.set_black();
				p.set_red();
				w = w.get_child(s);
				rb_rotate(nill, p, s);
			}
			ASSERT(w != head);
			rbnode w_o_child = w.get_child(o);
			rbnode w_s_child = w.get_child(s);
			if (w_o_child.is_black() && w_s_child.is_black())
			{
				w.set_red();
				cur = p;
			}
			else
			{
				if (w_o_child.is_black())
				{
					w_s_child.set_black();
					w.set_red();
					rb_rotate(nill, w, o);
					w = w_s_child;
					w_o_child = w.get_child(o);
					ASSERT(w != head);
				}
				ASSERT(w_o_child.is_red());
				w.set_color(p.get_color());
				p.set_black();
				w_o_child.set_black();
				rb_rotate(nill, p, s);
				cur = head.get_left();
			}
		}
		cur.set_black();
	}

	void			rb_switch_with(rbnode& _this, rbnode& _node)
	{
		ASSERT(_this != _node);

		// Copy pointers (includes color and parent side)
		rbnode parent = _node.get_parent();
		_this.set_parent(parent);
		rbnode left = _node.get_left();
		_this.set_left(left);
		rbnode right = _node.get_right();
		_this.set_right(right);

		_this.set_color(_node.get_color());
		s32 const ps = _node.get_parent_side();
		_this.set_parent_side(ps);

		left.set_parent(_this);
		left.set_parent_side(rbnode::LEFT);
		right.set_parent(_this);
		right.set_parent_side(rbnode::RIGHT);

		// Set ourselves as the new links of our parent
		parent.set_child(_this, ps);
	}


	bool			tree_insert(rbnode& root, tree_t::node_t* node_to_insert, tree_t::functors* fns)
	{
		ASSERT(node_to_insert != 0);

		rbnode endNode = root;
		rbnode lastNode = root;
		rbnode curNode = root.get_left();
		s32 s = rbnode::LEFT;
		while (curNode != endNode)
		{
			lastNode = curNode;

			s32 c = fns->cmp(fns->m_user, curNode.m_node, node_to_insert);
			if (c == 0)
				return xFALSE;	// Duplicate
			s = (c + 1) >> 1;
			curNode = curNode.get_child(s);
		}

		rbnode node(root.m_tree, node_to_insert);

		rb_attach_to(node, lastNode, s);
		rb_insert_fixup(root, root, node);

#ifdef DEBUG_RBTREE
		rb_validate(root, root);
#endif
		return xTRUE;
	}

	bool		tree_remove(rbnode& root, tree_t::node_t*& remove_node, tree_t::functors* fns)
	{
		rbnode nill = root;
		rbnode it = root.get_left();
		while (it != nill)
		{
			s32 cmp = fns->cmp(fns->m_user, it.m_node, remove_node);
			if (cmp == 0)
				break;
			s32 const s = (cmp + 1) >> 1;
			it = it.get_child(s);
		}

		// Did we find it? If not we have don't have to continue.
		if (it == nill)
		{
			remove_node = nullptr;
			return false;
		}

		// The node to remove
		rbnode node = it;
		ASSERT(node != root);

		rbnode endNode = root;
		rbnode repl = node;
		s32 s = rbnode::LEFT;
		if (node.get_right() != endNode)
		{
			if (node.get_left() != endNode)
			{
				repl = node.get_right();
				while (repl.get_left() != endNode)
					repl = repl.get_left();
			}
			s = rbnode::RIGHT;
		}
		ASSERT(repl.get_child(1 - s) == endNode);
		bool red = repl.is_red();
		rbnode replChild = repl.get_child(s);

		rb_substitute_with(repl, replChild);
		ASSERT(endNode.is_black());

		if (repl != node)
			rb_switch_with(repl, node);

		ASSERT(endNode.is_black());

		if (!red)
			rb_erase_fixup(nill, root, replChild);

		node.clear();

#ifdef DEBUG_RBTREE
		rb_validate(root, nill);
#endif
		remove_node = node.m_node;
		return true;
	}

	bool		tree_find(rbnode& root, tree_t::node_t*& found_node, tree_t::functors* fns)
	{
		rbnode nill = root;
		rbnode it = root.get_left();
		while (it != nill)
		{
			s32 const cmp = fns->cmp(fns->m_user, it.m_node, found_node);
			if (cmp == 0)
				break;
			s32 const s = (cmp + 1) >> 1;
			it = it.get_child(s);
		}
		found_node = (it == nill) ? NULL : it.m_node;
		return found_node != NULL;
	}

	rbnode	tree_clear(rbnode& root, rbnode& save)
	{
		//	Rotate away the left links so that
		//	we can treat this like the destruction
		//	of a linked list
		if (save.m_node == NULL)
			save = root.get_left();

		rbnode it = save;
		save.m_node = NULL;

		while (it != root)
		{
			if (it.get_left() == root)
			{
				/* No left links, just kill the node and move on */
				save = it.get_right();
				if (it != root)
				{
					it.clear();
					return it;
				}
			}
			else
			{
				/* Rotate away the left link and check again */
				save = it.get_left();
				it.set_left(save.get_right());
				save.set_right(it);
			}

			it = save;
			save.m_node = NULL;
		}
		root.clear();

		return rbnode(nullptr, nullptr);
	}

	tree_t::tree_t(functors fns)
	{
		m_fns = fns;

		m_nill = (node_t*)&m_nill;
		m_nill_link[0] = m_nill_link[1] = m_nill_link[2] = NULL;
		m_nill_flag = 0;

		m_root = (node_t*)&m_root;;
		m_root_link[0] = m_root_link[1] = m_root_link[2] = NULL;
		m_root_flag = 0;
		m_size = 0;
	}

	tree_t::tree_t(void* user, s32(*cmp)(void* user, node_t* a, node_t* n), node_t*(*getlink)(void* user, node_t* self, s32 link), void(*setlink)(void* user, node_t* self, s32 link, node_t* linknode), u32(*getflag)(void* user, node_t* self), void(*setflag)(void* user, node_t* self, u32 linkflag))
	{
		m_fns.m_user = user;
		m_fns.cmp = cmp;
		m_fns.getlink = getlink;
		m_fns.setlink = setlink;
		m_fns.getflag = getflag;
		m_fns.setflag = setflag;

		m_nill = (node_t*)&m_nill;
		m_nill_link[0] = m_nill_link[1] = m_nill_link[2] = NULL;
		m_nill_flag = 0;

		m_root = (node_t*)&m_root;;
		m_root_link[0] = m_root_link[1] = m_root_link[2] = NULL;
		m_root_flag = 0;
		m_size = 0;
	}

	bool			tree_t::insert(node_t* n)
	{
		rbnode root(this, (tree_t::node_t*)&m_root);
		rbnode nill(this, (tree_t::node_t*)&m_nill);
		return tree_insert(root, n, &m_fns);
	}

	bool			tree_t::find(node_t* n)
	{
		rbnode root(this, (tree_t::node_t*)&m_root);
		return tree_find(root, n, &m_fns);
	}

	bool			tree_t::remove(node_t* n)
	{
		rbnode root(this, (tree_t::node_t*)&m_root);
		return tree_remove(root, n, &m_fns);
	}



}