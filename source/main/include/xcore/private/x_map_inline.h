namespace xcore
{

	template<typename K, typename V, typename P>
	class xmap_iterator
	{
	public:
		typedef V			value_type;
		typedef s32			difference_type;
		typedef u32			size_type;
		typedef V&			value_ref;
		typedef K&			key_ref;
		typedef xmap_iterator<K,V,P> iterator;

		inline				xmap_iterator(iterator const& other) : strategy(other.strategy), node(other.node) { }
		inline				xmap_iterator(P& _p, xmapnode* _node) : strategy(_p), node(_node) { }

		value_ref			operator*() const							{ return strategy.toItem(node); }

		key_ref				key() const									{ return strategy.toKey(node); }
		value_ref			value() const								{ return strategy.toItem(node); }

		iterator&			operator=(const iterator& it)				{ node = it.node; return *this; }
		iterator&			operator+=(size_type t)						{ while (t>0) { node = (xmapnode*)rb_inorder(0, (xrbnode*)node); --t; } return *this; }
		iterator&			operator-=(size_type t)						{ while (t>0) { node = (xmapnode*)rb_inorder(1, (xrbnode*)node); --t; } return *this; }
		iterator&			operator++()								{ node = (xmapnode*)rb_inorder(0, (xrbnode*)node); return *this; }
		iterator&			operator--()								{ node = (xmapnode*)rb_inorder(1, (xrbnode*)node); return *this; }
		iterator			operator++(s32)								{ iterator tmp(*this); node = (xmapnode*)rb_inorder(0, (xrbnode*)node); return tmp; }
		iterator			operator--(s32)								{ iterator tmp(*this); node = (xmapnode*)rb_inorder(1, (xrbnode*)node); return tmp; }
		iterator			operator+(difference_type i) const			{ iterator tmp(*this); tmp += i; return tmp; }
		iterator			operator-(difference_type i) const			{ iterator tmp(*this); tmp -= i; return tmp; }

		bool				operator==(const iterator& it) const		{ return node == it.node; }
		bool				operator!=(const iterator& it) const		{ return !(*this == it); }

	private:
		P&					strategy;
		xmapnode*			node;
	};

	template<typename K, typename V, typename P>
	class xmap_const_iterator
	{
	public:
		typedef V			value_type;
		typedef s32			difference_type;
		typedef u32			size_type;
		typedef V const&	value_ref;
		typedef K const&	key_ref;
		typedef xmap_const_iterator iterator;

		inline				xmap_const_iterator(P const& _p, xmapnode const* _node) : strategy(_p), node(_node) { }

		value_ref			operator*() const							{ return strategy.toItem(node); }

		key_ref				key() const									{ return strategy.toKey(node); }
		value_ref			value() const								{ return strategy.toItem(node); }

		iterator&			operator+=(size_type t)						{ while (t>0) { node = (xmapnode*)rb_inorder(0, (xrbnode*)node); --t; } return *this; }
		iterator&			operator-=(size_type t)						{ while (t>0) { node = (xmapnode*)rb_inorder(1, (xrbnode*)node); --t; } return *this; }
		iterator&			operator++()								{ node = (xmapnode*)rb_inorder(0, (xrbnode*)node); return *this; }
		iterator&			operator--()								{ node = (xmapnode*)rb_inorder(1, (xrbnode*)node); return *this; }
		iterator			operator++(s32)								{ iterator tmp(*this); node = (xmapnode*)rb_inorder(0, (xrbnode*)node); return tmp; }
		iterator			operator--(s32)								{ iterator tmp(*this); node = (xmapnode*)rb_inorder(1, (xrbnode*)node); return tmp; }
		iterator			operator+(difference_type i) const			{ iterator tmp(*this); tmp += i; return tmp; }
		iterator			operator-(difference_type i) const			{ iterator tmp(*this); tmp -= i; return tmp; }

		bool				operator==(const iterator& it) const		{ return node == it.node; }
		bool				operator!=(const iterator& it) const		{ return !(*this == it); }

	private:
		P const&			strategy;
		xmapnode const*		node;
	};

	//////////////////////////////////////////////////////////////////////////
	// Heap Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename K, typename V>
	class xmap_heap_strategy
	{
		x_iallocator*	allocator;

		struct xmapnode_heap
		{
			K			key;
			V			item;
			xmapnode	node;

			inline		xmapnode_heap(K const& _key, V const& _value) : key(_key), item(_value) {}
			XCORE_CLASS_PLACEMENT_NEW_DELETE
		};

	public:
						xmap_heap_strategy(x_iallocator* a);

		xmapnode*		allocNode(K const& key, V const& value);
		void			deallocNode(xmapnode* node);

		V&				toItem(xmapnode* node);
		V const&		toItem(xmapnode const* node) const;

		K&				toKey(xmapnode* node);
		K const&		toKey(xmapnode const* node) const;	
	};


	//////////////////////////////////////////////////////////////////////////
	// Member Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename K, typename V>
	class xmap_member_strategy
	{
		x_iallocator*	allocator;
		u32				item_to_key_offset;
		u32				item_to_value_offset;
		u32				item_to_node_offset;

	public:
						xmap_member_strategy(x_iallocator* a, u32 item_to_key_offset, u32 item_to_value_offset, u32 item_to_node_offset);

		xmapnode*		allocNode(K const& key, V const& value);
		void			deallocNode(xmapnode* node);

		V&				toItem(xmapnode* node);
		V const&		toItem(xmapnode const* node) const;

		K&				toKey(xmapnode* node);
		K const&		toKey(xmapnode const* node) const;	
	};


	//////////////////////////////////////////////////////////////////////////
	// Derive Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename K, typename V>
	class xmap_derive_strategy
	{
		u32				mOffsetToKey;
	public:
						xmap_derive_strategy(u32 offset_to_key);

		xmapnode*		allocNode(K const& key, V const& value);
		void			deallocNode(xmapnode* node);

		V&				toItem(xmapnode* node);
		V const&		toItem(xmapnode const* node) const;

		K&				toKey(xmapnode* node);
		K const&		toKey(xmapnode const* node) const;	
	};


	template <typename K, typename V>
	inline			xmap_heap_strategy<K,V>::xmap_heap_strategy(x_iallocator* a) 
		: allocator(a)
	{
	}

	template <typename K, typename V>
	inline xmapnode*		xmap_heap_strategy<K,V>::allocNode(K const& key, V const& value)
	{
		void* node_mem = allocator->allocate(sizeof(xmapnode_heap), 4);
		xmapnode_heap* node = new (node_mem) xmapnode_heap(key, value);
		node->node.clear();
		return &node->node;
	}

	template <typename K, typename V>
	inline void			xmap_heap_strategy<K,V>::deallocNode(xmapnode* node)
	{
		xmapnode_heap* _node = (xmapnode_heap*)((u32)node - X_OFFSET_OF(xmapnode_heap, node));
		_node->~xmapnode_heap();
		allocator->deallocate(_node);
	}

	template <typename K, typename V>
	inline V&				xmap_heap_strategy<K,V>::toItem(xmapnode* node)
	{
		xmapnode_heap* _node = (xmapnode_heap*)((u32)node - X_OFFSET_OF(xmapnode_heap, node));
		return _node->item;
	}

	template <typename K, typename V>
	inline V const&		xmap_heap_strategy<K,V>::toItem(xmapnode const* node) const
	{
		xmapnode_heap const* _node = (xmapnode_heap const*)((u32)node - X_OFFSET_OF(xmapnode_heap, node));
		return _node->item;
	}

	template <typename K, typename V>
	inline K&				xmap_heap_strategy<K,V>::toKey(xmapnode* node)
	{
		xmapnode_heap* _node = (xmapnode_heap*)((u32)node - X_OFFSET_OF(xmapnode_heap, node));
		return _node->key;
	}

	template <typename K, typename V>
	inline K const&		xmap_heap_strategy<K,V>::toKey(xmapnode const* node) const
	{
		xmapnode_heap const* _node = (xmapnode_heap const*)((u32)node - X_OFFSET_OF(xmapnode_heap, node));
		return _node->key;
	}

	template <typename K, typename V>
	inline			xmap_member_strategy<K,V>::xmap_member_strategy(x_iallocator* a, u32 _item_to_key_offset, u32 _item_to_value_offset, u32 _item_to_node_offset) 
		: allocator(a)
		, item_to_key_offset(_item_to_key_offset)
		, item_to_value_offset(_item_to_value_offset)
		, item_to_node_offset(_item_to_node_offset)
	{
	}

	template <typename K, typename V>
	inline xmapnode*		xmap_member_strategy<K,V>::allocNode(K const& key, V const& value)
	{
		// value should be a pointer
		xmapnode* node = (xmapnode*)((xbyte*)(value) + item_to_node_offset);
		return node; 
	}

	template <typename K, typename V>
	inline void			xmap_member_strategy<K,V>::deallocNode(xmapnode* node)
	{
		node->clear();
	}

	template <typename K, typename V>
	inline V&				xmap_member_strategy<K,V>::toItem(xmapnode* node)
	{ 
		return *(V*)((xbyte*)node - item_to_node_offset + item_to_value_offset);
	}

	template <typename K, typename V>
	inline V const&		xmap_member_strategy<K,V>::toItem(xmapnode const* node) const
	{ 
		return *(V const*)((xbyte const*)node - item_to_node_offset + item_to_value_offset);
	}

	template <typename K, typename V>
	inline K&				xmap_member_strategy<K,V>::toKey(xmapnode* node)
	{ 
		return *(K*)((xbyte*)node - item_to_node_offset + item_to_key_offset);
	}

	template <typename K, typename V>
	inline K const&		xmap_member_strategy<K,V>::toKey(xmapnode const* node) const
	{ 
		return *(K const*)((xbyte const*)node - item_to_node_offset + item_to_key_offset);
	}


	template <typename K, typename V>
	inline			xmap_derive_strategy<K,V>::xmap_derive_strategy(u32 offset_to_key)
		: mOffsetToKey(offset_to_key)
	{ 
		ASSERTCT(xstd::xtypetrait<V>::isPointer);
	}

	template <typename K, typename V>
	inline xmapnode*		xmap_derive_strategy<K,V>::allocNode(K const& key, V const& value)
	{ 
		xmapnode* node = &value; 
		return node;
	}

	template <typename K, typename V>
	inline void			xmap_derive_strategy<K,V>::deallocNode(xmapnode* node)
	{ 
		node->clear();
	}

	template <typename K, typename V>
	inline V&				xmap_derive_strategy<K,V>::toItem(xmapnode* node)
	{
		V _item = (V)node;
		return _item;
	}

	template <typename K, typename V>
	inline V const&		xmap_derive_strategy<K,V>::toItem(xmapnode const* node) const
	{
		V const _item = (V const)node; 
		return _item; 
	}

	template <typename K, typename V>
	inline K&				xmap_derive_strategy<K,V>::toKey(xmapnode* node)
	{
		K _item = (K)node;
		return _item;
	}

	template <typename K, typename V>
	inline K const&		xmap_derive_strategy<K,V>::toKey(xmapnode const* node) const
	{
		K const _item = (K const)((u32)node + mOffsetToKey); 
		return _item; 
	}


	template <typename K, typename V, typename C, typename P>
	inline				xmap<K,V,C,P>::xmap(P const& _policy) 
		: mCount(0)
		, mStrategy(_policy)
	{
		mRoot.clear();
	}

	template <typename K, typename V, typename C, typename P>
	inline				xmap<K,V,C,P>::xmap(const xmap<K,V,C,P>& m)
		: mCount(0)
		, mStrategy(m.mStrategy)
	{
		mRoot.clear();
		for (const_iterator i = m.begin(); i != m.end(); ++i)
			insert(i.key(), i.value() );
	}

	template <typename K, typename V, typename C, typename P>
	inline				xmap<K,V,C,P>::~xmap()
	{
		clear();
	}

	template <typename K, typename V, typename C, typename P>
	inline u32			xmap<K,V,C,P>::size() const
	{
		return mCount; 
	}

	template <typename K, typename V, typename C, typename P>
	inline bool			xmap<K,V,C,P>::empty() const	
	{
		return mCount == 0; 
	}

	template <typename K, typename V, typename C, typename P>
	inline void			xmap<K,V,C,P>::clear()
	{
		//	Rotate away the left links so that
		//	we can treat this like the destruction
		//	of a linked list
		xmapnode* nill = &mRoot;
		xmapnode* root = nill;
		xmapnode* it = (xmapnode*)root->get_child(xmapnode::LEFT);
		while ( it != nill ) 
		{
			xmapnode *save;
			if ( it->get_child(xmapnode::LEFT) == nill ) 
			{
				/* No left links, just kill the node and move on */
				save = (xmapnode*)it->get_child(xmapnode::RIGHT);
				if (it != nill)
				{
					mStrategy.deallocNode(it);
					--mCount;
				}
			}
			else
			{
				/* Rotate away the left link and check again */
				save = (xmapnode*)it->get_child(xmapnode::LEFT);
				it->set_child(save->get_child(xmapnode::RIGHT), xmapnode::LEFT);
				save->set_child(it, xmapnode::RIGHT);
			}

			it = save;
		}

		root->set_parent(nill);
		root->set_child(nill, xmapnode::LEFT);
	}


	template <typename K, typename V, typename C, typename P>
	inline xbool xmap<K,V,C,P>::insert(K const& key, V const& value)
	{
		xmapnode* root     = &mRoot;
		xmapnode* endNode  = root;
		xmapnode* lastNode = root;
		xmapnode* curNode  = (xmapnode*)root->get_child(xmapnode::LEFT);
		s32 s = xmapnode::LEFT;
		C comparer;
		while (curNode != endNode)
		{
			lastNode = curNode;

			K const& curNodeKey = mStrategy.toKey(curNode);
			s32 c = comparer(key, curNodeKey);
			if (c == 0)
				return xFALSE;	// Duplicate
			s = (c + 1) >> 1;
			curNode = (xmapnode*)curNode->get_child(s);
		}

		xmapnode* node = mStrategy.allocNode(key, value);

		rb_attach_to(node, lastNode, s);
		rb_insert_fixup(*root, node);
		++mCount;

#ifdef DEBUG_RBTREE
		rb_check(root);
#endif
		return xTRUE;
	}

	template <typename K, typename V, typename C, typename P>
	inline xbool xmap<K,V,C,P>::remove(K const& key)
	{
		xmapnode* root = &mRoot;
		xmapnode* nill = root;
		xmapnode* it   = (xmapnode*)root->get_child(xmapnode::LEFT);
		C comparer;
		while ( it != nill )
		{
			K const& it_key = mStrategy.toKey(it);
			s32 cmp = comparer(key, it_key);
			if ( cmp == 0 )
				break;
			s32 const s = (cmp + 1) >> 1;
			it = (xmapnode*)it->get_child(s);
		}

		// Did we find it? If not we have don't have to continue.
		if (it == nill)
			return xFALSE;

		// The node to remove
		xmapnode* node = it;
		ASSERT(node != root);

		xmapnode* endNode = root;
		xmapnode* repl = node;
		s32 s = xmapnode::LEFT;
		if (node->get_child(xmapnode::RIGHT) != endNode)
		{
			if (node->get_child(xmapnode::LEFT) != endNode)
			{
				repl = (xmapnode*)node->get_child(xmapnode::RIGHT);
				while (repl->get_child(xmapnode::LEFT) != endNode)
					repl = (xmapnode*)repl->get_child(xmapnode::LEFT);
			}
			s = xmapnode::RIGHT;
		}
		ASSERT(repl->get_child(1-s) == endNode);
		bool red = repl->is_red();
		xmapnode* replChild = (xmapnode*)repl->get_child(s);

		rb_substitute_with(repl, replChild);
		ASSERT(endNode->is_black());

		if (repl != node)
			rb_switch_with(repl, node);

		ASSERT(endNode->is_black());

		if (!red) 
			rb_erase_fixup(root, replChild);

		--mCount;
		mStrategy.deallocNode(node);

#ifdef DEBUG_RBTREE
		rb_check(root);
#endif
		return xTRUE;
	}

	template <typename K, typename V, typename C, typename P>
	inline xmap_iterator<K,V,P> xmap<K,V,C,P>::remove(iterator ip)
	{
		iterator i = ip;
		++i;
		remove(ip.key() );
		return i;
	}

	template <typename K, typename V, typename C, typename P>
	inline V& xmap<K,V,C,P>::operator[](K const& key)
	{
		// TODO: Improve performance after adding lower_bound & upper_bound
		insert(key, V() );
		iterator it = find(key);
		return *it;
	}

	template <typename K, typename V, typename C, typename P>
	inline xmap_iterator<K,V,P>	xmap<K,V,C,P>::find(K const& key)
	{
		xmapnode* root = &mRoot;
		xmapnode* nill = root;
		xmapnode* it = (xmapnode*)root->get_child(xmapnode::LEFT);
		C comparer;
		while ( it != nill )
		{
			K const& it_key = mStrategy.toKey(it);
			s32 const cmp = comparer(key, it_key);
			if ( cmp == 0 )
				break;
			s32 const s = (cmp + 1) >> 1;
			it = (xmapnode*)it->get_child(s);
		}
		return xmap_iterator<K,V,P>(mStrategy, it);
	}

	template <typename K, typename V, typename C, typename P>
	inline xmap_const_iterator<K,V,P>	xmap<K,V,C,P>::find(K const& key) const
	{
		xmapnode const* root = &mRoot;
		xmapnode const* nill = root;
		xmapnode const* it = (xmapnode const*)root->get_child(xmapnode::LEFT);
		C comparer;
		while ( it != nill )
		{
			K const& it_key = mStrategy.toKey(it);
			s32 cmp = comparer(key, it_key);
			if ( cmp == 0 )
				break;
			s32 const s = (cmp + 1) >> 1;
			it = (xmapnode const*)it->get_child(s);
		}
		return xmap_const_iterator<K,V,P>(mStrategy, it);
	}

	template <typename K, typename V, typename C, typename P>
	inline xmap_iterator<K,V,P>	xmap<K,V,C,P>::imin()
	{
		// Traverse to the far left
		xmapnode* root = &mRoot;
		xmapnode* nill = root;
		xmapnode* it = (xmapnode*)root->get_child(xmapnode::LEFT);
		xmapnode* n;
		do 
		{
			n  = it;
			it = (xmapnode*)it->get_child(xmapnode::LEFT);
		} while ( it != nill );
		return xmap_iterator<K,V,P>(mStrategy, n);
	}

	template <typename K, typename V, typename C, typename P>
	inline xmap_iterator<K,V,P>	xmap<K,V,C,P>::imax()
	{
		// Traverse to the far right
		xmapnode* root = &mRoot;
		xmapnode* nill = root;
		xmapnode* it = (xmapnode*)root->get_child(xmapnode::LEFT);
		xmapnode* n;
		do 
		{
			n  = it;
			it = (xmapnode*)it->get_child(xmapnode::RIGHT);
		} while ( it != nill );
		return xmap_iterator<K,V,P>(mStrategy, n);
	}

	template <typename K, typename V, typename C, typename P>
	inline xmap_const_iterator<K,V,P>	xmap<K,V,C,P>::imin() const
	{
		xmapnode const* root = &mRoot;
		xmapnode const* nill = root;
		xmapnode const* it = (xmapnode const*)root->get_child(xmapnode::LEFT);
		xmapnode const* n;
		do 
		{
			n  = it;
			it = (xmapnode const*)it->get_child(xmapnode::LEFT);
		} while ( it != nill );
		return xmap_const_iterator<K,V,P>(mStrategy, n);
	}

	template <typename K, typename V, typename C, typename P>
	inline xmap_const_iterator<K,V,P>	xmap<K,V,C,P>::imax() const
	{
		xmapnode const* root = &mRoot;
		xmapnode const* nill = root;
		xmapnode const* it = (xmapnode const*)root->get_child(xmapnode::LEFT);
		xmapnode* n;
		do 
		{
			n  = it;
			it = (xmapnode const*)it->get_child(xmapnode::RIGHT);
		} while ( it != nill );
		return xmap_const_iterator<K,V,P>(mStrategy, n);
	}

	template <typename K, typename V, typename C, typename P>
	inline xmap_iterator<K,V,P>	xmap<K,V,C,P>::begin()
	{
		return imin();
	}

	template <typename K, typename V, typename C, typename P>
	inline xmap_iterator<K,V,P>	xmap<K,V,C,P>::end()
	{
		xmapnode* nil = &mRoot;
		return xmap_iterator<K,V,P>(mStrategy, nil);
	}

	template <typename K, typename V, typename C, typename P>
	inline xmap_const_iterator<K,V,P>	xmap<K,V,C,P>::begin() const
	{
		return imin();
	}

	template <typename K, typename V, typename C, typename P>
	inline xmap_const_iterator<K,V,P>	xmap<K,V,C,P>::end() const
	{
		xmapnode const* nil = &mRoot;
		return xmap_const_iterator<K,V,P>(mStrategy, nil);
	}

	template <typename K, typename V, typename C, typename P>
	inline xmap<K,V,C,P>& xmap<K,V,C,P>::operator=(const xmap<K,V,C,P>& m)
	{
		clear();
		for (const_iterator i = m.begin(); i != m.end(); ++i)
			insert(i.key(), i.value() );
		return *this;
	}
}