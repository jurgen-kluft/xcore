// x_ilist.h - xCore intrusive doubly linked list requiring inheritance
#include "xcore/x_ilist.h"

namespace xcore
{
	// Note:
	// - this is part of the list
	// - this->mPrev is pointing to the TAIL
	// - this->mNext is pointing to the HEAD
	// - The TAIL->mNext is pointing to this
	// - The HEAD->mPrev is pointing to this
	// 
	// By doing the above we can make all of the below logic branch-less.
	//

	void			xilist::push_front(xdlistnode* node)
	{
		ASSERT(node->is_used() == false);

		node->set_next(mNext);
		node->set_prev( this);
		
		mNext->set_prev(node);
		mNext = node;

		++mCount;
	}

	void			xilist::push_back(xdlistnode* node)
	{
		ASSERT(node->is_used() == false);

		node->set_next( this);
		node->set_prev(mPrev);

		mPrev->set_next(node);
		mPrev = node;

		++mCount;
	}


	void			xilist::insert_before(xdlistnode* node, xdlistnode* new_node)
	{
		ASSERT(node->is_used() == true);
		ASSERT(new_node->is_used() == false);
		new_node->set_next(node);
		new_node->set_prev(node->prev());
		node->set_prev(new_node);
		++mCount;
	}

	void			xilist::insert_after(xdlistnode* node, xdlistnode* new_node)
	{
		ASSERT(node->is_used() == true);
		ASSERT(new_node->is_used() == false);
		new_node->set_prev(node);
		new_node->set_next(node->next());
		node->set_next(new_node);
		++mCount;
	}


	xdlistnode*		xilist::pop_front()
	{
		ASSERT(mNext != this);
		xdlistnode* node = mNext;

		mNext = mNext->next();
		mNext->set_prev(this);

		node->set_unused();

		--mCount;
		return node;
	}

	xdlistnode*		xilist::pop_back()
	{
		ASSERT(mPrev != this);
		xdlistnode* node = mPrev;

		mPrev = mPrev->prev();
		mPrev->set_next(this);

		node->set_unused();
		--mCount;
		return node;
	}


	void			xilist::remove(xdlistnode* node)
	{
		ASSERT(mCount != 0);
		ASSERT(node != this);
		ASSERT(node->is_used() == true);

		node->prev()->set_next(node->next());
		node->next()->set_prev(node->prev());

		node->set_unused();
		--mCount;
	}

	void			xilist::clear()
	{
		xdlistnode* node = mNext;
		while (node!=this)
		{
			xdlistnode* next = node->next();
			node->set_unused();
			node = next;
			--mCount;
		}

		mCount = 0;
		mNext = this;
		mPrev = this;
	}

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

//==============================================================================
// END
//==============================================================================

