namespace xcore
{
	//////////////////////////////////////////////////////////////////////////
	// List Heap Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	class xqueue_heap_strategy
	{
		x_iallocator*	allocator;

		struct xqueuenode_heap
		{
			T			item;
			xqueuenode	node;
			XCORE_CLASS_PLACEMENT_NEW_DELETE
			inline		xqueuenode_heap(T const& _item) : item(_item) {}
		};
	public:
		xqueue_heap_strategy(x_iallocator* a);

		xqueuenode*		allocNode(T const& item);
		void			deallocNode(xqueuenode* node);

		T&				toItem(xqueuenode* node);
		T const&		toItem(xqueuenode const* node) const;
	};


	//////////////////////////////////////////////////////////////////////////
	// List Member Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	class xqueue_member_strategy
	{
		x_iallocator*	allocator;
		u32				item_to_node_offset;

	public:
		xqueue_member_strategy(x_iallocator* a, u32 item_to_node_offset);

		xqueuenode*		allocNode(T const& item);
		void			deallocNode(xqueuenode* node);

		T&				toItem(xqueuenode* node);
		T const&		toItem(xqueuenode const* node) const;
	};


	//////////////////////////////////////////////////////////////////////////
	// List Derive Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	class xqueue_derive_strategy
	{
	public:
		xqueue_derive_strategy();

		xqueuenode*		allocNode(T const& item);
		void			deallocNode(xqueuenode* node);

		T&				toItem(xqueuenode* node);
		T const&		toItem(xqueuenode const* node) const;
	};


	template <typename T>
	inline			xqueue_heap_strategy<T>::xqueue_heap_strategy(x_iallocator* a) 
		: allocator(a)
	{
	}

	template <typename T>
	xqueuenode*		xqueue_heap_strategy<T>::allocNode(T const& item)
	{
		void* node_mem = allocator->allocate(sizeof(xqueuenode_heap), 4);
		xqueuenode_heap* node = new (node_mem) xqueuenode_heap(item);
		return &node->node;
	}

	template <typename T>
	void			xqueue_heap_strategy<T>::deallocNode(xqueuenode* node)
	{
		xqueuenode_heap* _node = (xqueuenode_heap*)((u32)node - X_OFFSET_OF(xqueuenode_heap, node));
		_node->~xqueuenode_heap();
		allocator->deallocate(_node);
	}

	template <typename T>
	T&				xqueue_heap_strategy<T>::toItem(xqueuenode* node)
	{
		xqueuenode_heap* _node = (xqueuenode_heap*)((u32)node - X_OFFSET_OF(xqueuenode_heap, node));
		return _node->item;
	}

	template <typename T>
	T const&		xqueue_heap_strategy<T>::toItem(xqueuenode const* node) const
	{
		xqueuenode_heap const* _node = (xqueuenode_heap const*)((u32)node - X_OFFSET_OF(xqueuenode_heap, node));
		return _node->item;
	}



	template <typename T>
	inline			xqueue_member_strategy<T>::xqueue_member_strategy(x_iallocator* a, u32 item_to_node_offset) 
		: allocator(a)
	{

	}

	template <typename T>
	xqueuenode*		xqueue_member_strategy<T>::allocNode(T const& item)
	{
		xqueuenode* node = (xqueuenode*)((u32)(&item) + item_to_node_offset);
		return node; 
	}

	template <typename T>
	void			xqueue_member_strategy<T>::deallocNode(xqueuenode* node)
	{
		node->mNext = NULL; 
	}

	template <typename T>
	T&				xqueue_member_strategy<T>::toItem(xqueuenode* node)
	{ 
		T* _item = (u32)node - item_to_node_offset; 
		return *_item; 
	}

	template <typename T>
	T const&		xqueue_member_strategy<T>::toItem(xqueuenode const* node) const
	{ 
		T const* _item = (u32)node - item_to_node_offset;
		return *_item;
	}


	template <typename T>
	inline			xqueue_derive_strategy<T>::xqueue_derive_strategy()
	{ 
		ASSERTCT(xstd::xtypetrait<T>::isPointer);
	}

	template <typename T>
	xqueuenode*		xqueue_derive_strategy<T>::allocNode(T const& item)
	{ 
		xqueuenode* node = item; return node;
	}

	template <typename T>
	void			xqueue_derive_strategy<T>::deallocNode(xqueuenode* node)
	{ 
		node->mNext = NULL; 
	}

	template <typename T>
	T&				xqueue_derive_strategy<T>::toItem(xqueuenode* node)
	{
		return *(T*)node;;
	}

	template <typename T>
	T const&		xqueue_derive_strategy<T>::toItem(xqueuenode const* node) const
	{
		return *(T const*)node; 
	}



	namespace xqueue_private
	{
		inline void			enqueue(xqueuenode* mSentry, xqueuenode* node)
		{
			node->mNext = mSentry->mNext;
			node->mPrev = mSentry;
			mSentry->mNext->mPrev = node;
			mSentry->mNext = node;
		}

		inline xqueuenode*	dequeue(xqueuenode* mSentry)
		{
			xqueuenode* node = mSentry->mPrev;
			mSentry->mPrev = node->mPrev;
			mSentry->mPrev->mNext = mSentry;
			return node;
		}
	}

	template <typename T, typename P>
	inline				xqueue<T,P>::xqueue(P const& _policy) 
		: mCount(0)
		, mStrategy(_policy)
	{
		mSentry.mNext = &mSentry;
	}

	template <typename T, typename P>
	inline				xqueue<T,P>::~xqueue() 
	{
		clear();
	}

	template <typename T, typename P>
	inline u32			xqueue<T,P>::size() const
	{
		return mCount; 
	}

	template <typename T, typename P>
	inline bool			xqueue<T,P>::empty() const	
	{
		return mCount == 0; 
	}

	template <typename T, typename P>
	inline void			xqueue<T,P>::clear()
	{
		while (mCount>0)
			dequeue();
	}

	template <typename T, typename P>
	inline void			xqueue<T,P>::enqueue(T const& item)
	{
		xqueuenode* node = mStrategy.allocNode(item);
		xqueue_private::enqueue(&mSentry, node);
		++mCount;
	}

	template <typename T, typename P>
	inline T const&		xqueue<T,P>::front() const
	{
		xqueuenode* node = mSentry.mPrev;
		T const& item = mStrategy.toItem(node);
		return item;
	}

	template <typename T, typename P>
	inline T const&		xqueue<T,P>::back() const
	{
		xqueuenode* node = mSentry.mNext;
		T const& item = mStrategy.toItem(node);
		return item;
	}

	template <typename T, typename P>
	inline void			xqueue<T,P>::dequeue()
	{
		xqueuenode* node = xqueue_private::dequeue(&mSentry);
		mStrategy.deallocNode(node);
		--mCount;
	}
}