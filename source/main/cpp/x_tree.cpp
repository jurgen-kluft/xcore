#include "xcore/x_tree.h"

namespace xcore
{
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

	void			xrbtree::init()
	{
		root = &nodes[0];
		nill = &nodes[1];
		nill->clear();
		root->clear();
		root->set_parent(nill);
		root->set_left(nill);
		root->set_right(nill);
	}

	bool			xrbtree::insert(xrbnode * node_to_insert, void* key, comparer cmpfunc)
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
		rb_insert_fixup(nill, *root, node);

#ifdef DEBUG_RBTREE
		rb_validate(root, nill);
#endif
		return xTRUE;
	}

	bool		xrbtree::remove(xrbnode*& found_node, void* key, comparer cmpfunc)
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

	bool		xrbtree::find(xrbnode*& found_node, void* key, comparer cmpfunc)
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

	xrbnode*	xrbtree::clear(xrbnode*& save)
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