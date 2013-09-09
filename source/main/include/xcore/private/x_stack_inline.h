namespace xcore
{
	//////////////////////////////////////////////////////////////////////////
	// List Heap Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	class xstack_heap_strategy
	{
		x_iallocator*	allocator;
		struct xstacknode_heap
		{
			inline		xstacknode_heap(T const& _item) : item(_item) {}
			XCORE_CLASS_PLACEMENT_NEW_DELETE
			T			item;
			xlistnode	node;
		};

	public:
		xstack_heap_strategy(x_iallocator* a);

		xlistnode*		allocNode(T const& item);
		void			deallocNode(xlistnode* node);

		T&				toItem(xlistnode* node);
		T const&		toItem(xlistnode const* node) const;
	};


	//////////////////////////////////////////////////////////////////////////
	// List Member Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	class xstack_member_strategy
	{
		x_iallocator*	allocator;
		u32				item_to_node_offset;

	public:
		xstack_member_strategy(x_iallocator* a, u32 item_to_node_offset);

		xlistnode*		allocNode(T const& item);
		void			deallocNode(xlistnode* node);

		T&				toItem(xlistnode* node);
		T const&		toItem(xlistnode const* node) const;
	};


	//////////////////////////////////////////////////////////////////////////
	// List Derive Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	class xstack_derive_strategy
	{
	public:
		xstack_derive_strategy();

		xlistnode*		allocNode(T const& item);
		void			deallocNode(xlistnode* node);

		T&				toItem(xlistnode* node);
		T const&		toItem(xlistnode const* node) const;
	};

	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline			xstack_heap_strategy<T>::xstack_heap_strategy(x_iallocator* a) 
		: allocator(a)
	{
	}

	template <typename T>
	xlistnode*		xstack_heap_strategy<T>::allocNode(T const& item)
	{
		void* node_mem = allocator->allocate(sizeof(xstacknode_heap), 4);
		xstacknode_heap* node = new (node_mem) xstacknode_heap(item);
		return &node->node;
	}

	template <typename T>
	void			xstack_heap_strategy<T>::deallocNode(xlistnode* node)
	{
		xstacknode_heap* _node = (xstacknode_heap*)((u32)node - X_OFFSET_OF(xstacknode_heap, node));
		_node->~xstacknode_heap();
		allocator->deallocate(_node);
	}

	template <typename T>
	T&				xstack_heap_strategy<T>::toItem(xlistnode* node)
	{
		xstacknode_heap* _node = (xstacknode_heap*)((u32)node - X_OFFSET_OF(xstacknode_heap, node));
		return _node->item;
	}

	template <typename T>
	T const&		xstack_heap_strategy<T>::toItem(xlistnode const* node) const
	{
		xstacknode_heap const* _node = (xstacknode_heap const*)((u32)node - X_OFFSET_OF(xstacknode_heap, node));
		return _node->item;
	}



	template <typename T>
	inline			xstack_member_strategy<T>::xstack_member_strategy(x_iallocator* a, u32 item_to_node_offset) 
		: allocator(a)
	{

	}

	template <typename T>
	xlistnode*		xstack_member_strategy<T>::allocNode(T const& item)
	{
		xlistnode* node = (xlistnode*)((u32)(&item) + item_to_node_offset);
		return node; 
	}

	template <typename T>
	void			xstack_member_strategy<T>::deallocNode(xlistnode* node)
	{
		node->mNext = node; 
	}

	template <typename T>
	T&				xstack_member_strategy<T>::toItem(xlistnode* node)
	{ 
		T* _item = (u32)node - item_to_node_offset; 
		return *_item; 
	}

	template <typename T>
	T const&		xstack_member_strategy<T>::toItem(xlistnode const* node) const
	{ 
		T const* _item = (u32)node - item_to_node_offset;
		return *_item;
	}


	template <typename T>
	inline			xstack_derive_strategy<T>::xstack_derive_strategy()
	{ 
		ASSERTCT(xstd::xtypetrait<T>::isPointer);
	}

	template <typename T>
	xlistnode*		xstack_derive_strategy<T>::allocNode(T const& item)
	{ 
		xlistnode* node = item; return node;
	}

	template <typename T>
	void			xstack_derive_strategy<T>::deallocNode(xlistnode* node)
	{ 
		node->mNext = node; 
	}

	template <typename T>
	T&				xstack_derive_strategy<T>::toItem(xlistnode* node)
	{
		return *(T*)node;
	}

	template <typename T>
	T const&		xstack_derive_strategy<T>::toItem(xlistnode const* node) const
	{
		return *(T const*)node; 
	}



	namespace xstack_private
	{
		inline void			push_front(xlistnode* sentry, xlistnode* node)
		{
			node->mNext = sentry->mNext;
			sentry->mNext = node;
		}

		inline xlistnode*	pop_front(xlistnode* sentry)
		{
			xlistnode* n = sentry->mNext;
			sentry->mNext = n->mNext;
			n->mNext = n;
			return n;
		}
	}

	template <typename T, typename P>
	inline				xstack<T,P>::xstack(P const& _policy) 
		: count(0)
		, policy(_policy)
	{
		sentry.mNext = &sentry;
	}

	template <typename T, typename P>
	inline				xstack<T,P>::~xstack() 
	{
		clear();
	}

	template <typename T, typename P>
	inline u32			xstack<T,P>::size() const
	{
		return count; 
	}

	template <typename T, typename P>
	inline bool			xstack<T,P>::empty() const	
	{
		return count == 0; 
	}

	template <typename T, typename P>
	inline void			xstack<T,P>::clear()
	{
		while (count>0)
			pop();
	}

	template <typename T, typename P>
	inline void			xstack<T,P>::push(T const& item)
	{
		xlistnode* node = policy.allocNode(item);
		xstack_private::push_front(&sentry, node);
		++count;
	}

	template <typename T, typename P>
	inline T const&		xstack<T,P>::peek() const
	{
		ASSERT(count != 0);
		xlistnode* node = sentry.mNext;
		T const& item = policy.toItem(node);
		return item;
	}

	template <typename T, typename P>
	inline void			xstack<T,P>::pop()
	{
		if (count == 0) return;
		xlistnode* node = xstack_private::pop_front(&sentry);
		policy.deallocNode(node);
		--count;
	}
}