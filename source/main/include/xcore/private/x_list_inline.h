namespace xcore
{
	//////////////////////////////////////////////////////////////////////////
	// List Heap Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	class xlist_heap_strategy
	{
		x_iallocator*	allocator;

		struct xlistnode_heap
		{
			inline		xlistnode_heap(T const& _item) : item(_item) {}
			T			item;
			xdlistnode	node;
			XCORE_CLASS_PLACEMENT_NEW_DELETE
		};

	public:
		xlist_heap_strategy(x_iallocator* a);

		xdlistnode*		allocNode(T const& item);
		void			deallocNode(xdlistnode* node);

		T&				toItem(xdlistnode* node);
		T const&		toItem(xdlistnode const* node) const;
	};


	//////////////////////////////////////////////////////////////////////////
	// List Member Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	class xlist_member_strategy
	{
		x_iallocator*	allocator;
		u32				item_to_node_offset;

	public:
		xlist_member_strategy(x_iallocator* a, u32 item_to_node_offset);

		xdlistnode*		allocNode(T const& item);
		void			deallocNode(xdlistnode* node);

		T&				toItem(xdlistnode* node);
		T const&		toItem(xdlistnode const* node) const;
	};

	//////////////////////////////////////////////////////////////////////////
	// List Derive Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	class xlist_derive_strategy
	{
	public:
		xlist_derive_strategy();

		xdlistnode*		allocNode(T const& item);
		void			deallocNode(xdlistnode* node);

		T				toItem(xdlistnode* node);
		T const			toItem(xdlistnode const* node) const;
	};

	//////////////////////////////////////////////////////////////////////////
	// Template List 
	//////////////////////////////////////////////////////////////////////////
	template<typename T, typename P>
	class xlist_iterator
	{
	public:
		typedef T				value_type;
		typedef s32				difference_type;
		typedef u32				size_type;
		typedef T				reference;
		typedef T				pointer;
		typedef xlist_iterator	iterator;

		inline				xlist_iterator(P* _p, xdlistnode* _sentinel, u32 _count, xdlistnode* _node) : policy(_p), sentinel(_sentinel), count(_count), node(_node) { }
		inline				xlist_iterator(xlist_iterator const& other) : policy(other.policy), sentinel(other.sentinel), count(other.count), node(other.node) { }

		reference			operator*() const							{ return policy->toItem(node); }
		pointer				operator->() const							{ return policy->toItem(node); }

		iterator&			operator+=(s32 t)						
		{
			if (t==0)
				return *this;

			// We are end(), stop here
			if (node == sentinel && count > 0)
				return *this;

			while (node!=sentinel && t-->0)
			{ 
				node = node->next(); 
				++count; 
			}
			return *this; 
		}
		iterator&			operator-=(s32 t)
		{
			if (t==0)
				return *this;

			// We are begin(), stop here
			if (node == sentinel && count == 0)
				return *this;

			while (node!=sentinel && t-->0)
			{ 
				node = node->prev(); 
				--count; 
			}
			return *this; 
		}
		iterator&			operator++()
		{ 
			if (node == sentinel && count > 0)
				return *this;
			ASSERT(count >= 0);
			++count;
			node = node->next();
			return *this;
		}
		iterator&			operator--()								
		{ 
			if (node == sentinel && count == 0)
				return *this;
			ASSERT(count >= 0);
			--count;
			node = node->prev();
			return *this; 
		}
		iterator			operator++(s32)	
		{ 
			iterator tmp(*this);
			if (node == sentinel && count > 0)
				return tmp;
			ASSERT(count >= 0);
			++count;
			node = node->next();
			return tmp;
		}
		iterator			operator--(s32)								
		{
			iterator tmp(*this);
			if (node == sentinel && count == 0)
				return tmp;
			ASSERT(count >= 0);
			--count;
			node = node->prev();
			return tmp;
		}

		iterator			operator+(difference_type i) const			{ iterator tmp(*this); tmp += i; return tmp; }
		iterator			operator-(difference_type i) const			{ iterator tmp(*this); tmp -= i; return tmp; }

		bool				operator==(const iterator& it) const		{ return node == it.node && sentinel == it.sentinel; }
		bool				operator<(const iterator& it) const			{ ASSERT(sentinel == it.sentinel); return count < it.count; }
		bool				operator!=(const iterator& it) const		{ return !(*this == it); }
		bool				operator<=(const iterator& it) const		{ return !(it < *this); }
		bool				operator>(const iterator& it) const			{ return it < *this; }
		bool				operator>=(const iterator& it) const		{ return !(*this < it); }

		iterator&			operator=(const iterator& it)
		{
			if (this != &it)
			{
				policy   = it.policy;
				sentinel = it.sentinel;
				count    = it.count;
				node     = it.node;
			}
			return *this;
		}

	private:
		friend class xlist<T,P>;
		P*					policy;
		xdlistnode*			sentinel;
		u32					count;
		xdlistnode*			node;
	};

	template<typename T, typename P>
	class xlist_const_iterator
	{
	public:
		typedef T						value_type;
		typedef s32						difference_type;
		typedef u32						size_type;
		typedef T const					reference;
		typedef T const					pointer;
		typedef xlist_const_iterator	iterator;

		inline				xlist_const_iterator(P const* _p, xdlistnode const* _sentinel, u32 _count, xdlistnode const* _node) : policy(_p), sentinel(_sentinel), count(_count), node(_node) { }
		inline				xlist_const_iterator(xlist_const_iterator const& other) : policy(other.policy), sentinel(other.sentinel), count(other.count), node(other.node) { }

		reference			operator*() const							{ return policy->toItem(node); }
		pointer				operator->() const							{ return policy->toItem(node); }

		iterator&			operator+=(size_type t)						
		{
			if (t==0)
				return *this;

			// We are end(), stop here
			if (node == sentinel && count > 0)
				return *this;

			while (node!=sentinel && t-->0)
			{ 
				node = node->next(); 
				++count; 
			}
			return *this; 
		}
		iterator&			operator-=(size_type t)
		{
			if (t==0)
				return *this;

			// We are begin(), stop here
			if (node == sentinel && count == 0)
				return *this;

			while (node!=sentinel && t-->0)
			{ 
				node = node->prev(); 
				--count; 
			}
			return *this; 
		}
		iterator&			operator++()
		{ 
			if (node == sentinel && count > 0)
				return *this;
			ASSERT(count >= 0);
			++count;
			node = node->next();
			return *this;
		}
		iterator&			operator--()								
		{ 
			if (node == sentinel && count == 0)
				return *this;
			ASSERT(count >= 0);
			--count;
			node = node->prev();
			return *this;
		}
		iterator			operator++(s32)	
		{ 
			iterator tmp(*this);
			if (node == sentinel && count > 0)
				return tmp;
			ASSERT(count >= 0);
			++count;
			node = node->next();
			return tmp;
		}
		iterator			operator--(s32)								
		{
			iterator tmp(*this);
			if (node == sentinel && count == 0)
				return tmp;
			ASSERT(count >= 0);
			--count;
			node = node->prev();
			return tmp;
		}

		iterator			operator+(difference_type i) const			{ iterator tmp(*this); tmp += i; return tmp; }
		iterator			operator-(difference_type i) const			{ iterator tmp(*this); tmp -= i; return tmp; }

		bool				operator==(const iterator& it) const		{ return node == it.node && sentinel == it.sentinel; }
		bool				operator<(const iterator& it) const			{ ASSERT(sentinel == it.sentinel); return count < it.count; }
		bool				operator!=(const iterator& it) const		{ return !(*this == it); }
		bool				operator<=(const iterator& it) const		{ return !(it < *this); }
		bool				operator>(const iterator& it) const			{ return it < *this; }
		bool				operator>=(const iterator& it) const		{ return !(*this < it); }

		iterator&			operator=(const iterator& it)
		{
			if (this != &it)
			{
				policy   = it.policy;
				sentinel = it.sentinel;
				count    = it.count;
				node     = it.node;
			}
			return *this;
		}

	private:
		friend class xlist<T,P>;
		P const*			policy;
		xdlistnode const*	sentinel;
		u32					count;
		xdlistnode const*	node;
	};

	template <typename T>
	inline			xlist_heap_strategy<T>::xlist_heap_strategy(x_iallocator* a) 
		: allocator(a)
	{
	}

	template <typename T>
	xdlistnode*		xlist_heap_strategy<T>::allocNode(T const& item)
	{
		void* node_mem = allocator->allocate(sizeof(xlistnode_heap), 4);
		xlistnode_heap* node = new (node_mem) xlistnode_heap(item);
		return &node->node;
	}

	template <typename T>
	void			xlist_heap_strategy<T>::deallocNode(xdlistnode* node)
	{
		xlistnode_heap* _node = (xlistnode_heap*)((u32)node - X_OFFSET_OF(xlistnode_heap, node));
		_node->~xlistnode_heap();
		allocator->deallocate(_node);
	}

	template <typename T>
	T&				xlist_heap_strategy<T>::toItem(xdlistnode* node)
	{
		xlistnode_heap* _node = (xlistnode_heap*)((u32)node - X_OFFSET_OF(xlistnode_heap, node));
		return _node->item;
	}

	template <typename T>
	T const&		xlist_heap_strategy<T>::toItem(xdlistnode const* node) const
	{
		xlistnode_heap const* _node = (xlistnode_heap const*)((u32)node - X_OFFSET_OF(xlistnode_heap, node));
		return _node->item;
	}


	template <typename T>
	inline			xlist_member_strategy<T>::xlist_member_strategy(x_iallocator* a, u32 item_to_node_offset) 
		: allocator(a)
	{

	}

	template <typename T>
	xdlistnode*		xlist_member_strategy<T>::allocNode(T const& item)
	{
		xdlistnode* node = (xdlistnode*)((u32)(&item) + item_to_node_offset);
		return node; 
	}

	template <typename T>
	void			xlist_member_strategy<T>::deallocNode(xdlistnode* node)
	{
		node->set_next(NULL); 
		node->set_prev(NULL); 
	}

	template <typename T>
	T&				xlist_member_strategy<T>::toItem(xdlistnode* node)
	{ 
		T* _item = (u32)node - item_to_node_offset; 
		return *_item; 
	}

	template <typename T>
	T const&		xlist_member_strategy<T>::toItem(xdlistnode const* node) const
	{ 
		T const* _item = (u32)node - item_to_node_offset;
		return *_item;
	}


	template <typename T>
	inline			xlist_derive_strategy<T>::xlist_derive_strategy()
	{ 
		//ASSERTCT(xstd::xtypetrait<T>::isPointer);
	}

	template <typename T>
	xdlistnode*		xlist_derive_strategy<T>::allocNode(T const& item)
	{ 
		xdlistnode* node = item; return node;
	}

	template <typename T>
	void			xlist_derive_strategy<T>::deallocNode(xdlistnode* node)
	{ 
		node->set_next(NULL); 
		node->set_prev(NULL);
	}

	template <typename T>
	T				xlist_derive_strategy<T>::toItem(xdlistnode* node)
	{
		T _item = (T)node;
		return _item;
	}

	template <typename T>
	T const			xlist_derive_strategy<T>::toItem(xdlistnode const* node) const
	{
		T const _item = (T const)node; 
		return _item; 
	}



	namespace xlist_private
	{
		inline void			insert_before(xdlistnode* pivot, xdlistnode* node)
		{
			node->set_next(pivot);
			node->set_prev(pivot->prev());
			pivot->prev()->set_next(node);
			pivot->set_prev(node);
		}

		inline void			insert_after(xdlistnode* pivot, xdlistnode* node)
		{
			node->set_next(pivot->next());
			node->set_prev(pivot);
			pivot->next()->set_prev(node);
			pivot->set_next(node);
		}

		inline void			remove(xdlistnode* node)
		{
			node->prev()->set_next(node->next());
			node->next()->set_prev(node->prev());
			node->set_next(NULL);
			node->set_prev(NULL);
		}
	}

	template <typename T, typename P>
	inline				xlist<T,P>::xlist(P const& _policy) 
		: count(0)
		, policy(_policy)
	{
		sentry.set_next(&sentry);
		sentry.set_prev(&sentry);
	}

	template <typename T, typename P>
	inline				xlist<T,P>::~xlist() 
	{
		clear();
	}

	template <typename T, typename P>
	inline u32			xlist<T,P>::size() const
	{
		return count; 
	}

	template <typename T, typename P>
	inline bool			xlist<T,P>::empty() const	
	{
		return count == 0; 
	}

	template <typename T, typename P>
	inline void			xlist<T,P>::clear()
	{
		while (count>0)
			pop_back();
	}

	template <typename T, typename P>
	inline void			xlist<T,P>::push_back(T const& item)
	{
		xdlistnode* node = policy.allocNode(item);
		xlist_private::insert_before(&sentry, node);
		++count;
	}

	template <typename T, typename P>
	inline void			xlist<T,P>::push_front(T const& item)
	{
		xdlistnode* node = policy.allocNode(item);
		xlist_private::insert_after(&sentry, node);
		++count;
	}

	template <typename T, typename P>
	inline xlist_iterator<T,P>			xlist<T,P>::iat(u32 index)
	{
		xdlistnode* n = sentry.next();
		u32 i = 0;
		while ((n!=&sentry) && (i<index))
		{
			n = n->next();
			++i;
		}
		return iterator(policy, &sentry, i, n); 
	}

	template <typename T, typename P>
	inline xlist_const_iterator<T,P>	xlist<T,P>::iat(u32 index) const
	{
		xdlistnode const* n = sentry.next();
		u32 i = 0;
		while ((n!=&sentry) && (i<index))
		{
			n = n->next();
			++i;
		}
		return const_iterator(&policy, &sentry, i, n); 
	}

	template <typename T, typename P>
	inline void			xlist<T,P>::insert_at(iterator iter, T const& item)
	{
		xdlistnode* node = policy.allocNode(item);
		xlist_private::insert_before(iter.node, node);
	}

	template <typename T, typename P>
	inline void			xlist<T,P>::remove_at(iterator iter)	
	{
		ASSERT(iter.node != iter.sentinel);
		xlist_private::remove(iter.node);
	}

	template <typename T, typename P>
	inline T const&		xlist<T,P>::back() const
	{
		xdlistnode* node = sentry.prev();
		T const& item = policy.toItem(node);
		return item;
	}

	template <typename T, typename P>
	inline T const&		xlist<T,P>::front() const
	{
		xdlistnode* node = sentry.next();
		T const& item = policy.toItem(node);
		return item;
	}

	template <typename T, typename P>
	inline void			xlist<T,P>::pop_back()
	{
		xdlistnode* node = sentry.prev();
		sentry.set_prev(node->prev());
		sentry.prev()->set_next(&sentry);
		policy.deallocNode(node);
		--count;
	}

	template <typename T, typename P>
	inline void			xlist<T,P>::pop_front()
	{
		xdlistnode* node = sentry.next();
		sentry.set_next(node->next());
		sentry.next()->set_prev(&sentry);
		policy.deallocNode(node);
		--count;
	}

	template <typename T, typename P>
	inline xlist_iterator<T,P>			xlist<T,P>::begin()
	{ 
		return iterator(&policy, &sentry, 0, sentry.next()); 
	}

	template <typename T, typename P>
	inline xlist_iterator<T,P>			xlist<T,P>::end()
	{
		return iterator(&policy, &sentry, count, &sentry); 
	}

	template <typename T, typename P>
	inline xlist_const_iterator<T,P>	xlist<T,P>::begin() const
	{
		return const_iterator(&policy, &sentry, 0, sentry.next()); 
	}

	template <typename T, typename P>
	inline xlist_const_iterator<T,P>	xlist<T,P>::end() const	
	{
		return const_iterator(&policy, &sentry, count, &sentry);
	}

	template <typename T, typename P>
	inline xlist_const_iterator<T,P>	xlist<T,P>::find(T const& item) const
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

	template <typename T, typename P>
	inline xlist_iterator<T,P>	xlist<T,P>::find(T const& item)
	{
		iterator i = begin();
		iterator e = end();
		while (i != e)
		{
			if (*i == item)
				return i;
			++i;
		}
		return e;
	}
}