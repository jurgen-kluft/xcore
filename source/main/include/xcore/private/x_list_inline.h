namespace xcore
{
	namespace list_private
	{
		inline void	push_front(list_t::functors& fns, list_t::node_t*& front, list_t::node_t*& back, list_t::node_t* node)
		{
			fns.setprev(fns.mUser, node, NULL);
			fns.setnext(fns.mUser, node, front);
			if (front == NULL)
			{
				back = node;
			}
			else
			{
				fns.setprev(fns.mUser, front, node);
			}
			front = node;
		}
		inline void	push_back(list_t::functors& fns, list_t::node_t*& front, list_t::node_t*& back, list_t::node_t* node)
		{
			fns.setprev(fns.mUser, node, back);
			fns.setnext(fns.mUser, node, NULL);
			if (back == NULL)
			{
				front = node;
			}
			else
			{
				fns.setnext(fns.mUser, back, node);
			}
			back = node;
		}

		inline list_t::node_t*	pop_front(list_t::functors& fns, list_t::node_t*& front, list_t::node_t*& back)
		{
			list_t::node_t* node = front;
			front = fns.getnext(fns.mUser, front);
			if (front == NULL)
			{
				back = NULL;
			}
			else
			{
				fns.setprev(fns.mUser, front, NULL);
			}
			// Fully unlink popped node
			fns.setnext(fns.mUser, node, NULL);
			fns.setprev(fns.mUser, node, NULL);
			return node;
		}
		inline list_t::node_t*	pop_back(list_t::functors& fns, list_t::node_t*& front, list_t::node_t*& back)
		{
			list_t::node_t* node = back;
			back = fns.getprev(fns.mUser, back);
			if (back == NULL)
			{
				front = NULL;
			}
			else
			{
				fns.setnext(fns.mUser, back, NULL);
			}
			// Fully unlink popped node
			fns.setnext(fns.mUser, node, NULL);
			fns.setprev(fns.mUser, node, NULL);
			return node;
		}
	}

	inline				list_t::list_t(list_t::functors fns)
		: mFunctors(fns)
		, mHead(NULL)
		, mTail(NULL)
		, mCount(0)
	{
	}

	inline				list_t::list_t(void* user, node_t*(*getnext)(void* user, node_t* n), node_t*(*getprev)(void* user, node_t* n), void(*setnext)(void* user, node_t* node, node_t* next), void(*setprev)(void* user, node_t* node, node_t* prev))
		: mHead(NULL)
		, mTail(NULL)
		, mCount(0)
	{
		mFunctors.mUser = user;
		mFunctors.getnext = getnext;
		mFunctors.getprev = getprev;
		mFunctors.setnext = setnext;
		mFunctors.setprev = setprev;
	}

	inline				list_t::~list_t()
	{
		clear();
	}

	inline u32			list_t::size() const
	{
		return mCount;
	}

	inline bool			list_t::empty() const
	{
		return mCount == 0;
	}

	inline void			list_t::clear()
	{
		while (mCount > 0)
		{
			list_t::node_t* item;
			pop_back(item);
		}
	}

	inline void			list_t::push_front(list_t::node_t* item)
	{
		list_private::push_front(mFunctors, mHead, mTail, item);
		++mCount;
	}

	inline void			list_t::push_back(list_t::node_t* item)
	{
		list_private::push_back(mFunctors, mHead, mTail, item);
		++mCount;
	}

	inline list_t::node_t*		list_t::front() const
	{
		return mHead;
	}

	inline list_t::node_t*		list_t::back() const
	{
		return mTail;
	}

	inline bool					list_t::pop_front(list_t::node_t*& node)
	{
		if (mCount > 0)
		{
			node = list_private::pop_front(mFunctors, mHead, mTail);
			--mCount;
			return true;
		}
		node = NULL;
		return false;
	}

	inline bool					list_t::pop_back(list_t::node_t*& node)
	{
		if (mCount > 0)
		{
			node = list_private::pop_back(mFunctors, mHead, mTail);
			--mCount;
			return true;
		}
		node = NULL;
		return false;
	}
}
