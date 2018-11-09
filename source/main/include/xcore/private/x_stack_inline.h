namespace xcore
{
	namespace stack_private
	{
		inline void	push(stack_t::functors& fns, stack_t::node_t*& front, stack_t::node_t*& back, stack_t::node_t* node)
		{
			if (back == NULL)
			{
				fns.setnext(fns.mUser, node, NULL);
				fns.setprev(fns.mUser, node, NULL);
				back = node;
				front = node;
			}
			else
			{
				fns.setnext(fns.mUser, node, front);
				fns.setprev(fns.mUser, node, NULL);
				fns.setprev(fns.mUser, front, node);
			}
		}

		inline stack_t::node_t*	pop(stack_t::functors& fns, stack_t::node_t*& front, stack_t::node_t*& back)
		{
			stack_t::node_t* node = front;
			front = fns.getnext(fns.mUser, front);
			if (front == NULL)
			{
				back = NULL;
			}
			else
			{
				fns.setprev(fns.mUser, front, NULL);
			}
			fns.setnext(fns.mUser, node, NULL);
			fns.setprev(fns.mUser, node, NULL);
			return node;
		}
	}

	inline				stack_t::stack_t(stack_t::functors fns)
		: mFunctors(fns)
		, mHead(NULL)
		, mTail(NULL)
		, mCount(0)
	{
	}

	inline				stack_t::stack_t(void* user, node_t*(*getnext)(void* user, node_t* n), node_t*(*getprev)(void* user, node_t* n), void(*setnext)(void* user, node_t* node, node_t* next), void(*setprev)(void* user, node_t* node, node_t* prev))
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

	inline				stack_t::~stack_t()
	{
		clear();
	}

	inline u32			stack_t::size() const
	{
		return mCount;
	}

	inline bool			stack_t::empty() const
	{
		return mCount == 0;
	}

	inline void			stack_t::clear()
	{
		while (mCount > 0)
		{
			stack_t::node_t* item;
			pop(item);
		}
	}

	inline void			stack_t::push(stack_t::node_t* item)
	{
		stack_private::push(mFunctors, mHead, mTail, item);
		++mCount;
	}

	inline stack_t::node_t*		stack_t::top() const
	{
		return mHead;
	}

	inline stack_t::node_t*		stack_t::bottom() const
	{
		return mTail;
	}

	inline bool					stack_t::pop(stack_t::node_t*& node)
	{
		if (mCount > 0)
		{
			node = stack_private::pop(mFunctors, mHead, mTail);
			--mCount;
			return true;
		}
		node = NULL;
		return false;
	}
}


namespace xcore
{
	/*
	This use-case shows how to create your own object that can be managed by the queue.
	In this example next/prev are 32-bit integers and all items are actually part of
	a c-array. This just shows how you can tweak/tune the items you want to add into
	a queue.
	*/
	inline void StackUseCase()
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
			f32						node_to_item(stack_t::node_t* n)
			{
				my_item* item = (my_item*)n;
				return item->mItem;
			}

			u32						node_to_idx(stack_t::node_t* n)
			{
				if (n == NULL) return my_item::NULL_IDX;
				my_item* item = (my_item*)n;
				u32 idx = (item - items);
				return idx;
			}
			stack_t::node_t* 		idx_to_node(u32 idx)
			{
				if (idx == my_item::NULL_IDX) return NULL;
				my_item* item = &items[idx];
				return (stack_t::node_t*)item;
			}

			// Interface for a double linked list node used by stack_t
			static stack_t::node_t*		getnext(void* user, stack_t::node_t* n)
			{
				if (n == NULL) return NULL;
				my_items* items = (my_items*)user;
				my_item* item = (my_item*)n;
				u32 idx = item->mNext;
				return items->idx_to_node(idx);
			}
			static stack_t::node_t*		getprev(void* user, stack_t::node_t* n)
			{
				if (n == NULL) return NULL;
				my_items* items = (my_items*)user;
				my_item* item = (my_item*)n;
				u32 idx = item->mPrev;
				return items->idx_to_node(idx);
			}
			static void					setnext(void* user, stack_t::node_t* node, stack_t::node_t* next)
			{
				my_items* items = (my_items*)user;
				my_item* item = (my_item*)node;
				item->mNext = items->node_to_idx(next);
			}
			static void					setprev(void* user, stack_t::node_t* node, stack_t::node_t* prev)
			{
				my_items* items = (my_items*)user;
				my_item* item = (my_item*)node;
				item->mPrev = items->node_to_idx(prev);
			}

			my_item items[100];
		};

		my_items items;
		stack_t stack(&items, &items.getnext, &items.getprev, &items.setnext, &items.setprev);

		// Give all items a value
		for (u32 i = 0; i < 100; i++)
		{
			items.items[i].mItem = i * 3.5f;
		}

		// Add all items to the queue
		for (u32 i = 0; i < 100; i++)
		{
			stack.push(items.idx_to_node(i));
		}

		stack_t::node_t* node;
		while (stack.pop(node))
		{

		}
	}
}
