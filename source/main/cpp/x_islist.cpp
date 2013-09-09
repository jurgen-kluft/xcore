// x_islist.h - xCore intrusive singly linked list requiring inheritance
#include "xbase\x_islist.h"

namespace xcore
{
	// Note:
	// - this is part of the list
	// - this->mNext is pointing to the HEAD
	// - The TAIL->mNext is pointing to this
	// 
	// By doing the above we can make all of the below logic branch-less.
	//

	void			xislist::push_front(xislistnode* node)
	{
		ASSERT(node->is_used() == false);

		node->mNext = mNext;
		mNext = node;

		++mCount;
	}

	void			xislist::push_back(xislistnode* node)
	{
		ASSERT(node->is_used() == false);

		node->mNext = this;

		mTail->mNext = node;
		mTail = node;

		++mCount;
	}


	void			xislist::insert_after(xislistnode* node, xislistnode* new_node)
	{
		ASSERT(node->is_used() == true);
		ASSERT(new_node->is_used() == false);
		new_node->mNext = node->mNext;
		node->mNext = new_node;
	}


	xislistnode*		xislist::pop_front()
	{
		ASSERT(mNext != this);
		xislistnode* node = mNext;

		mNext = mNext->mNext;

		node->mNext = node;
		--mCount;
		return node;
	}

	void			xislist::remove(xislistnode* node)
	{
		ASSERT(mCount != 0);
		ASSERT(node != this);
		ASSERT(node->is_used() == true);

		// We have to search the node before 'node'
		xislistnode* prev = node;
		while (prev->mNext != node)
			prev = prev->mNext;

		prev->mNext = node->mNext;

		node->mNext = node;
		--mCount;
	}


	void			xislist::clear()
	{
		while (mCount != 0)
		{
			xislistnode* node = mNext;
			ASSERT(node != this);
			mNext = mNext->mNext;
			
			node->mNext = node;
			--mCount;
		}
	}




	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

//==============================================================================
// END
//==============================================================================

