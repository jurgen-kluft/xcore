namespace xcore
{
	//////////////////////////////////////////////////////////////////////////
	// List Heap Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	class xslist_heap_strategy
	{
		x_iallocator*	allocator;

	public:
		xslist_heap_strategy(x_iallocator* a);

		xlistnode*		allocNode(T const& item);
		void			deallocNode(xlistnode* node);

		T&				toItem(xlistnode* node);
		T const&		toItem(xlistnode const* node) const;
	};


	//////////////////////////////////////////////////////////////////////////
	// List Member Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	class xslist_member_strategy
	{
		x_iallocator*	allocator;
		u32				item_to_node_offset;

	public:
		xslist_member_strategy(x_iallocator* a, u32 item_to_node_offset);

		xlistnode*		allocNode(T const& item);
		void			deallocNode(xlistnode* node);

		T&				toItem(xlistnode* node);
		T const&		toItem(xlistnode const* node) const;
	};


	//////////////////////////////////////////////////////////////////////////
	// List Derive Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	class xslist_derive_strategy
	{
	public:
		xslist_derive_strategy();

		xlistnode*		allocNode(T const& item);
		void			deallocNode(xlistnode* node);

		T				toItem(xlistnode* node);
		T const			toItem(xlistnode const* node) const;
	};


	template<typename T, typename P>
	class xslist_iterator
	{
	public:
		typedef T				value_type;
		typedef s32				difference_type;
		typedef u32				size_type;
		typedef T				reference;
		typedef T				pointer;
		typedef xslist_iterator	iterator;

		inline				xslist_iterator(P& _p, xlistnode* _sentinel, u32 _count, xlistnode* _node) : policy(_p), sentinel(_sentinel), count(_count), node(_node) { }

		reference			operator*() const							{ return policy.toItem(node); }
		pointer				operator->() const							{ return policy.toItem(node); }

		iterator&			operator+=(size_type t)						{ while (node!=sentinel && t-->0) { node = node->mNext; ++count; } return *this; }
		iterator&			operator++()								{ node = node->mNext; ++count; return *this; }
		iterator			operator++(s32)								{ iterator tmp(*this); node = node->mNext; ++count; return tmp; }
		iterator			operator+(difference_type i) const			{ iterator tmp(*this); tmp += i; return tmp; }

		bool				operator==(const iterator& it) const		{ return node == it.node && sentinel == it.sentinel; }
		bool				operator<(const iterator& it) const			{ ASSERT(node == it.node); return count < it.count; }
		bool				operator!=(const iterator& it) const		{ return !(*this == it); }
		bool				operator<=(const iterator& it) const		{ return !(it < *this); }
		bool				operator>(const iterator& it) const			{ return it < *this; }
		bool				operator>=(const iterator& it) const		{ return !(*this < it); }

	private:
		P&					policy;
		xlistnode*			sentinel;
		u32					count;
		xlistnode*			node;
	};

	template<typename T, typename P>
	class xslist_const_iterator
	{
	public:
		typedef T						value_type;
		typedef s32						difference_type;
		typedef u32						size_type;
		typedef T const					reference;
		typedef T const					pointer;
		typedef xslist_const_iterator	iterator;

		inline				xslist_const_iterator(P const& _p, xlistnode const* _sentinel, u32 _count, xlistnode const* _node) : policy(_p), sentinel(_sentinel), count(_count), node(_node) { }

		reference			operator*() const							{ return policy.toItem(node); }
		pointer				operator->() const							{ return policy.toItem(node); }

		iterator&			operator+=(size_type t)						{ while (node!=sentinel && t-->0) { node = node->mNext; ++count; } return *this; }
		iterator&			operator++()								{ node = node->mNext; ++count; return *this; }
		iterator			operator++(s32)								{ iterator tmp(*this); node = node->mNext; ++count; return tmp; }
		iterator			operator+(difference_type i) const			{ iterator tmp(*this); tmp += i; return tmp; }

		bool				operator==(const iterator& it) const		{ return node == it.node && sentinel == it.sentinel; }
		bool				operator<(const iterator& it) const			{ ASSERT(node == it.node); return count < it.count; }
		bool				operator!=(const iterator& it) const		{ return !(*this == it); }
		bool				operator<=(const iterator& it) const		{ return !(it < *this); }
		bool				operator>(const iterator& it) const			{ return it < *this; }
		bool				operator>=(const iterator& it) const		{ return !(*this < it); }

	private:
		P const&			policy;
		xlistnode const*	sentinel;
		u32					count;
		xlistnode const*	node;
	};




	namespace xslist_private
	{
		template<typename T>
		struct xlistnode_heap
		{
			inline		xlistnode_heap(T const& _item) : item(_item) {}
			XCORE_CLASS_PLACEMENT_NEW_DELETE
			T			item;
			xlistnode	node;
		};
	}

	template <typename T>
	inline			xslist_heap_strategy<T>::xslist_heap_strategy(x_iallocator* a) 
		: allocator(a)
	{
	}

	template <typename T>
	xlistnode*		xslist_heap_strategy<T>::allocNode(T const& item)
	{
		void* node_mem = allocator->allocate(sizeof(xslist_private::xlistnode_heap<T>), 4);
		xslist_private::xlistnode_heap<T>* node = new (node_mem) xslist_private::xlistnode_heap<T>(item);
		return &node->node;
	}

	template <typename T>
	void			xslist_heap_strategy<T>::deallocNode(xlistnode* node)
	{
		xslist_private::xlistnode_heap<T>* _node = (xslist_private::xlistnode_heap<T>*)((u32)node - X_OFFSET_OF(xslist_private::xlistnode_heap<T>, node));
		_node->xslist_private::~xlistnode_heap<T>();
		allocator->deallocate(node);
	}

	template <typename T>
	T&				xslist_heap_strategy<T>::toItem(xlistnode* node)
	{
		xslist_private::xlistnode_heap<T>* _node = (xslist_private::xlistnode_heap<T>*)((u32)node - X_OFFSET_OF(xslist_private::xlistnode_heap<T>, node));
		return _node->item;
	}

	template <typename T>
	T const&		xslist_heap_strategy<T>::toItem(xlistnode const* node) const
	{
		xslist_private::xlistnode_heap<T> const* _node = (xslist_private::xlistnode_heap<T> const*)((u32)node - X_OFFSET_OF(xslist_private::xlistnode_heap<T>, node));
		return _node->item;
	}



	template <typename T>
	inline			xslist_member_strategy<T>::xslist_member_strategy(x_iallocator* a, u32 item_to_node_offset) 
		: allocator(a)
	{

	}

	template <typename T>
	xlistnode*		xslist_member_strategy<T>::allocNode(T const& item)
	{
		xlistnode* node = (xlistnode*)((u32)(&item) + item_to_node_offset);
		return node; 
	}

	template <typename T>
	void			xslist_member_strategy<T>::deallocNode(xlistnode* node)
	{
		node->mNext = NULL; 
	}

	template <typename T>
	T&				xslist_member_strategy<T>::toItem(xlistnode* node)
	{ 
		T* _item = (u32)node - item_to_node_offset; 
		return *_item; 
	}

	template <typename T>
	T const&		xslist_member_strategy<T>::toItem(xlistnode const* node) const
	{ 
		T const* _item = (u32)node - item_to_node_offset;
		return *_item;
	}


	template <typename T>
	inline			xslist_derive_strategy<T>::xslist_derive_strategy()
	{ 
		ASSERTCT(xstd::xtypetrait<T>::isPointer);
	}

	template <typename T>
	xlistnode*		xslist_derive_strategy<T>::allocNode(T const& item)
	{ 
		xlistnode* node = item; return node;
	}

	template <typename T>
	void			xslist_derive_strategy<T>::deallocNode(xlistnode* node)
	{ 
		node->mNext = NULL; 
	}

	template <typename T>
	T				xslist_derive_strategy<T>::toItem(xlistnode* node)
	{
		T _item = (T)node;
		return _item;
	}

	template <typename T>
	T const			xslist_derive_strategy<T>::toItem(xlistnode const* node) const
	{
		T const _item = (T const)node; 
		return _item; 
	}



	namespace xslist_private
	{
		inline void			insert(xlistnode* before, xlistnode* node)
		{
			node->mNext = before->mNext;
			before->mNext = node;
		}

		inline void			remove(xlistnode* before)
		{
			xlistnode* node = before->mNext;
			before->mNext = node->mNext;
			node->mNext = NULL;
		}

		inline void			push_front(xlistnode* sentry, xlistnode* node)
		{
			node->mNext = sentry->mNext;
			sentry->mNext = node;
		}

		inline xlistnode*	pop_front(xlistnode* sentry)
		{
			xlistnode* n = sentry->mNext;
			sentry->mNext = n->mNext;
			n->mNext = NULL;
			return n;
		}
	}

	template <typename T, typename P>
	inline				xslist<T,P>::xslist(P const& _policy) 
		: count(0)
		, policy(_policy)
	{
		sentry.mNext = &sentry;
	}

	template <typename T, typename P>
	inline u32			xslist<T,P>::size() const
	{
		return count; 
	}

	template <typename T, typename P>
	inline bool			xslist<T,P>::empty() const	
	{
		return count == 0; 
	}

	template <typename T, typename P>
	inline void			xslist<T,P>::clear()
	{
		while (count>0)
			pop_front();
	}

	template <typename T, typename P>
	inline void			xslist<T,P>::push_front(T const& item)
	{
		xlistnode* node = policy.allocNode(item);
		xslist_private::push_front(&sentry, node);
		++count;
	}

	template <typename T, typename P>
	inline void			xslist<T,P>::insert_at(u32 index, T const& item)
	{
		xlistnode* p = &sentry;
		xlistnode* n = sentry.mNext;
		u32 i = 0;
		while ((n!=&sentry) && (i<index))
		{
			p = n;
			n = n->mNext;
			++i;
		}
		xlistnode* node = policy.allocNode(item);
		xslist_private::insert(p, node);
	}

	template <typename T, typename P>
	inline void			xslist<T,P>::remove_at(u32 index)
	{
		xlistnode* p = &sentry;
		xlistnode* n = sentry.mNext;
		u32 i = 0;
		while ((n!=&sentry) && (i<index))
		{
			p = n;
			n = n->mNext;
			++i;
		}
		xslist_private::remove(p);
	}

	template <typename T, typename P>
	inline T const&		xslist<T,P>::front() const
	{
		xlistnode* node = sentry.mNext;
		T const& item = policy.toItem(node);
		return item;
	}

	template <typename T, typename P>
	inline void			xslist<T,P>::pop_front()
	{
		xlistnode* node = sentry.mNext;
		sentry.mNext = node->mNext;
		policy.dealloc(node);
		--count;
	}

	template <typename T, typename P>
	inline xslist_iterator<T,P>			xslist<T,P>::begin()
	{ 
		return iterator(policy, &sentry, 0, sentry.mNext); 
	}

	template <typename T, typename P>
	inline xslist_iterator<T,P>			xslist<T,P>::end()
	{
		return iterator(policy, &sentry, count, &sentry); 
	}

	template <typename T, typename P>
	inline xslist_const_iterator<T,P>	xslist<T,P>::begin() const
	{
		return const_iterator(policy, &sentry, 0, sentry.mNext); 
	}

	template <typename T, typename P>
	inline xslist_const_iterator<T,P>	xslist<T,P>::end() const	
	{
		return const_iterator(policy, &sentry, count, &sentry);
	}

	template <typename T, typename P>
	inline xslist_const_iterator<T,P>	xslist<T,P>::find(T const& item) const
	{
		const_iterator i = begin();
		const_iterator e = end();
		while (i != e)
		{
			if (*i == item)
				return i;
			++i;
		}
		return e;
	}
}