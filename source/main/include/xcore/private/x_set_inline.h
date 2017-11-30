namespace xcore
{

	template<typename T, typename P>
	class xset_iterator
	{
	public:
		typedef T			value_type;
		typedef s32			difference_type;
		typedef u32			size_type;
		typedef T&			value_ref;
		typedef xset_iterator<T,P> iterator;

		inline				xset_iterator(iterator const& other) : strategy(other.strategy), node(other.node) { }
		inline				xset_iterator(P& _p, xsetnode* _node) : strategy(_p), node(_node) { }

		value_ref			operator*() const							{ return strategy.toItem(node); }
		value_ref			value() const								{ return strategy.toItem(node); }

		iterator&			operator+=(size_type t)						{ while (t>0) { node = (xsetnode*)rb_inorder(0, (xrbnode*)node); --t; } return *this; }
		iterator&			operator-=(size_type t)						{ while (t>0) { node = (xsetnode*)rb_inorder(1, (xrbnode*)node); --t; } return *this; }
		iterator&			operator++()								{ node = (xsetnode*)rb_inorder(0, (xrbnode*)node); return *this; }
		iterator&			operator--()								{ node = (xsetnode*)rb_inorder(1, (xrbnode*)node); return *this; }
		iterator			operator++(s32)								{ iterator tmp(*this); node = (xsetnode*)rb_inorder(0, (xrbnode*)node); return tmp; }
		iterator			operator--(s32)								{ iterator tmp(*this); node = (xsetnode*)rb_inorder(1, (xrbnode*)node); return tmp; }
		iterator			operator+(difference_type i) const			{ iterator tmp(*this); tmp += i; return tmp; }
		iterator			operator-(difference_type i) const			{ iterator tmp(*this); tmp -= i; return tmp; }

		bool				operator==(const iterator& it) const		{ return node == it.node; }
		bool				operator!=(const iterator& it) const		{ return !(*this == it); }

	private:
		P&					strategy;
		xsetnode*			node;

	};

	template<typename T, typename P>
	class xset_const_iterator
	{
	public:
		typedef T			value_type;
		typedef s32			difference_type;
		typedef u32			size_type;
		typedef T const&	value_ref;
		typedef xset_const_iterator iterator;

		inline				xset_const_iterator(P const& _p, xsetnode const* _node) : strategy(_p), node(_node) { }

		value_ref			operator*() const							{ return strategy.toItem(node); }
		value_ref			value() const								{ return strategy.toItem(node); }

		iterator&			operator+=(size_type t)						{ while (t>0) { node = (xsetnode*)rb_inorder(0, (xsetnode*)node); --t; } return *this; }
		iterator&			operator-=(size_type t)						{ while (t>0) { node = (xsetnode*)rb_inorder(1, (xsetnode*)node); --t; } return *this; }
		iterator&			operator++()								{ node = (xsetnode*)rb_inorder(0, (xsetnode*)node); return *this; }
		iterator&			operator--()								{ node = (xsetnode*)rb_inorder(1, (xsetnode*)node); return *this; }
		iterator			operator++(s32)								{ iterator tmp(*this); node = (xsetnode*)rb_inorder(0, (xsetnode*)node); return tmp; }
		iterator			operator--(s32)								{ iterator tmp(*this); node = (xsetnode*)rb_inorder(1, (xsetnode*)node); return tmp; }
		iterator			operator+(difference_type i) const			{ iterator tmp(*this); tmp += i; return tmp; }
		iterator			operator-(difference_type i) const			{ iterator tmp(*this); tmp -= i; return tmp; }

		bool				operator==(const iterator& it) const		{ return node == it.node; }
		bool				operator!=(const iterator& it) const		{ return !(*this == it); }

	private:
		P const&			strategy;
		xsetnode const*		node;
	};


	//////////////////////////////////////////////////////////////////////////
	// Heap Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	class xset_heap_strategy
	{
		x_iallocator*	allocator;

		struct xsetnode_heap
		{
			T			item;
			xsetnode	node;
			XCORE_CLASS_PLACEMENT_NEW_DELETE
			inline		xsetnode_heap(T const& _item) : item(_item) {}
		};

	public:
						xset_heap_strategy(x_iallocator* a);

		xsetnode*		allocNode(T const& item);
		void			deallocNode(xsetnode* node);

		T&				toItem(xsetnode* node);
		T const&		toItem(xsetnode const* node) const;
	};


	//////////////////////////////////////////////////////////////////////////
	// Member Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	class xset_member_strategy
	{
		u32				item_to_value_offset;
		u32				item_to_node_offset;

	public:
						xset_member_strategy(u32 item_to_value_offset, u32 item_to_node_offset);

		xsetnode*		allocNode(T const& item);
		void			deallocNode(xsetnode* node);

		T&				toItem(xsetnode* node);
		T const&		toItem(xsetnode const* node) const;
	};


	//////////////////////////////////////////////////////////////////////////
	// Derive Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename T>
	class xset_derive_strategy
	{
	public:
		xset_derive_strategy();

		xsetnode*		allocNode(T const& item);
		void			deallocNode(xsetnode* node);

		T				toItem(xsetnode* node);
		T const			toItem(xsetnode const* node) const;
	};


	template <typename T>
	inline			xset_heap_strategy<T>::xset_heap_strategy(x_iallocator* a) 
		: allocator(a)
	{
	}

	template <typename T>
	xsetnode*		xset_heap_strategy<T>::allocNode(T const& item)
	{
		void* node_mem = allocator->allocate(sizeof(xsetnode_heap), 4);
		xsetnode_heap* node = new (node_mem) xsetnode_heap(item);
		node->node.clear();
		return &node->node;
	}

	template <typename T>
	void			xset_heap_strategy<T>::deallocNode(xsetnode* node)
	{
		xsetnode_heap* _node = (xsetnode_heap*)((uptr)node - X_OFFSET_OF(xsetnode_heap, node));
		_node->~xsetnode_heap();
		allocator->deallocate(_node);
	}

	template <typename T>
	T&				xset_heap_strategy<T>::toItem(xsetnode* node)
	{
		xsetnode_heap* _node = (xsetnode_heap*)((uptr)node - X_OFFSET_OF(xsetnode_heap, node));
		return _node->item;
	}

	template <typename T>
	T const&		xset_heap_strategy<T>::toItem(xsetnode const* node) const
	{
		xsetnode_heap const* _node = (xsetnode_heap const*)((uptr)node - X_OFFSET_OF(xsetnode_heap, node));
		return _node->item;
	}


	template <typename T>
	inline			xset_member_strategy<T>::xset_member_strategy(u32 _item_to_value_offset, u32 _item_to_node_offset) 
		: item_to_value_offset(_item_to_value_offset) 
		, item_to_node_offset(_item_to_node_offset)
	{

	}

	template <typename T>
	xsetnode*		xset_member_strategy<T>::allocNode(T const& item)
	{
		xsetnode* node = (xsetnode*)((uptr)(item) + item_to_node_offset);
		return node; 
	}

	template <typename T>
	void			xset_member_strategy<T>::deallocNode(xsetnode* node)
	{
		node->clear();
	}

	template <typename T>
	T&				xset_member_strategy<T>::toItem(xsetnode* node)
	{ 
		return *(T*)((xbyte*)node - item_to_node_offset + item_to_value_offset);
	}

	template <typename T>
	T const&		xset_member_strategy<T>::toItem(xsetnode const* node) const
	{ 
		return *(T const*)((xbyte const*)node - item_to_node_offset + item_to_value_offset);
	}


	template <typename T>
	inline			xset_derive_strategy<T>::xset_derive_strategy()
	{ 
		ASSERTCT(xstd::xtypetrait<T>::isPointer);
	}

	template <typename T>
	xsetnode*		xset_derive_strategy<T>::allocNode(T const& item)
	{ 
		xsetnode* node = item; return node;
	}

	template <typename T>
	void			xset_derive_strategy<T>::deallocNode(xsetnode* node)
	{ 
		node->clear();
	}

	template <typename T>
	T				xset_derive_strategy<T>::toItem(xsetnode* node)
	{
		T _item = (T)node;
		return _item;
	}

	template <typename T>
	T const			xset_derive_strategy<T>::toItem(xsetnode const* node) const
	{
		T const _item = (T const)node; 
		return _item; 
	}



	template <typename T, typename C, typename P>
	inline				xset<T,C,P>::xset(P const& _strategy) 
		: mCount(0)
		, mStrategy(_strategy)
	{
		mRoot.clear();
	}

	template <typename T, typename C, typename P>
	inline				xset<T,C,P>::~xset()
	{
		clear();
	}

	template <typename T, typename C, typename P>
	inline u32			xset<T,C,P>::size() const
	{
		return mCount; 
	}

	template <typename T, typename C, typename P>
	inline bool			xset<T,C,P>::empty() const	
	{
		return mCount == 0; 
	}


	template <typename T, typename C, typename P>
	inline void			xset<T,C,P>::clear()
	{
		//	Rotate away the left links so that
		//	we can treat this like the destruction
		//	of a linked list
		xsetnode* nill = &mRoot;
		xsetnode* root = &mRoot;
		xsetnode* it = (xsetnode*)root->get_child(xsetnode::LEFT);
		while ( it != nill ) 
		{
			xsetnode *save;
			if ( it->get_child(xsetnode::LEFT) == nill ) 
			{
				/* No left links, just kill the node and move on */
				save = (xsetnode*)it->get_child(xsetnode::RIGHT);
				if (it != nill)
				{
					mStrategy.deallocNode(it);
					--mCount;
				}
			}
			else
			{
				/* Rotate away the left link and check again */
				save = (xsetnode*)it->get_child(xsetnode::LEFT);
				it->set_child(save->get_child(xsetnode::RIGHT), xsetnode::LEFT);
				save->set_child(it, xsetnode::RIGHT);
			}

			it = save;
		}

		mRoot.clear();
	}

	template <typename T, typename C, typename P>
	inline xset_iterator<T,P>			xset<T,C,P>::insert(T const& item)
	{
		xsetnode* root     = &mRoot;
		xsetnode* nill     = &mRoot;

		xsetnode* lastNode = root;
		xsetnode* curNode  = (xsetnode*)root->get_child(xsetnode::LEFT);;
		s32 s = xsetnode::LEFT;
		C comparer;
		while (curNode != nill)
		{
			lastNode = curNode;

			T const& curNodeItem = mStrategy.toItem(curNode);
			s32 c = comparer(item, curNodeItem);
			if (c == 0)
				return xset_iterator<T,P>(mStrategy,root);   //Duplicate
			s = (c < 0) ? xsetnode::LEFT : xsetnode::RIGHT;
			curNode = (xsetnode*)curNode->get_child(s);
		}

		xsetnode* node = mStrategy.allocNode(item);

		rb_attach_to(node, lastNode, s);
		rb_insert_fixup(nill, *root, node);
		++mCount;

#ifdef DEBUG_RBTREE
		rb_test(root, nill);
#endif
		return xset_iterator<T,P>(mStrategy, node);
	}


	template <typename T, typename C, typename P>
	inline void			xset<T,C,P>::insert(iterator _first, iterator _last)
	{
		iterator tempIt = _first;
		while (tempIt != _last)
		{
			tempIt++;
			insert(*tempIt);		
		}
	}

	template <typename T, typename C, typename P>
	inline xbool			xset<T,C,P>::remove(T const& item)
	{
		xsetnode* nill = &mRoot;
		xsetnode* root = &mRoot;

		xsetnode* it   = (xsetnode*)root->get_child(xsetnode::LEFT);
		C comparer;
		while ( it != nill )
		{
			T const& it_item = mStrategy.toItem(it);
			s32 cmp = comparer(item, it_item);
			if ( cmp == 0 )
				break;
			it = (xsetnode*)it->get_child(cmp < 0 ? xsetnode::LEFT : xsetnode::RIGHT);
		}

		// Did we find it? If not we have don't have to continue.
		if (it == nill)
			return xFalse;

		// The node to remove
		xsetnode* node = it;
		ASSERT(node != root);

		xsetnode* repl = node;
		s32 s = xsetnode::LEFT;
		if (node->get_child(xsetnode::RIGHT) != nill)
		{
			if (node->get_child(xsetnode::LEFT) != nill)
			{
				repl = (xsetnode*)node->get_child(xsetnode::RIGHT);
				while (repl->get_child(xsetnode::LEFT) != nill)
					repl = (xsetnode*)repl->get_child(xsetnode::LEFT);
			}
			s = xsetnode::RIGHT;
		}
		ASSERT(repl->get_child(1-s) == nill);
		bool red = repl->is_red();
		xsetnode* replChild = (xsetnode*)repl->get_child(s);

		rb_substitute_with(repl, replChild);
		ASSERT(nill->is_black());

		if (repl != node)
			rb_switch_with(repl, node);

		ASSERT(nill->is_black());

		if (!red) 
			rb_erase_fixup(nill, root, replChild);

		--mCount;
		mStrategy.deallocNode(node);

#ifdef DEBUG_RBTREE
		rb_test(root, nill);
#endif
		return xTrue;
	}


	template <typename T, typename C, typename P>
	inline xbool			xset<T,C,P>::remove(iterator _where)
	{
		return remove(*_where);
	}


	template <typename T, typename C, typename P>
	inline xbool			xset<T,C,P>::remove(const_iterator _where)
	{
		return remove(*_where);
	}


	template <typename T, typename C, typename P>
	inline void			xset<T,C,P>::remove(iterator _first, iterator _last)
	{
		while(_first != _last)
		{
			iterator tempIt = _first;
			_first++;
			remove(tempIt);
		}
	}

	template <typename T, typename C, typename P>
	inline xset_iterator<T,P>	xset<T,C,P>::find(T const& key)
	{
		xsetnode* nill = &mRoot;
		xsetnode* root = &mRoot;
		xsetnode* it = (xsetnode*)root->get_child(xsetnode::LEFT);
		C comparer;
		while ( it != nill )
		{
			T const& it_key = mStrategy.toItem(it);
			s32 cmp = comparer(key, it_key);
			if ( cmp == 0 )
				break;
			it = (xsetnode*)it->get_child(cmp < 0 ? xsetnode::LEFT : xsetnode::RIGHT);
		}
		return xset_iterator<T,P>(mStrategy, it);
	}

	template <typename T, typename C, typename P>
	inline xset_const_iterator<T,P>	xset<T,C,P>::find(T const& key) const
	{
		xsetnode const* nill = &mRoot;
		xsetnode const* root = &mRoot;
		xsetnode const* it = (xsetnode const*)root->get_child(xsetnode::LEFT);
		C comparer;
		while ( it != nill )
		{
			T const& it_key = mStrategy.toItem(it);
			s32 cmp = comparer(key, it_key);
			if ( cmp == 0 )
				break;
			it = (xsetnode const*)it->get_child(cmp < 0 ? xsetnode::LEFT : xsetnode::RIGHT);
		}
		return xset_const_iterator<T,P>(mStrategy, it);
	}

	template <typename T, typename C, typename P>
	inline xset_iterator<T,P>	xset<T,C,P>::imin()
	{
		// Traverse to the far left
		xsetnode* nill = &mRoot;
		xsetnode* root = &mRoot;
		xsetnode* it = (xsetnode*)root->get_child(xsetnode::LEFT);
		xsetnode* n;
		do 
		{
			n  = it;
			it = (xsetnode*)it->get_child(xsetnode::LEFT);
		} while ( it != nill );
		return xset_iterator<T,P>(mStrategy, n);
	}

	template <typename T, typename C, typename P>
	inline xset_iterator<T,P>	xset<T,C,P>::imax()
	{
		// Traverse to the far right
		xsetnode* nill = &mRoot;
		xsetnode* root = &mRoot;
		xsetnode* it = (xsetnode*)root->get_child(xsetnode::LEFT);
		xsetnode* n;
		do 
		{
			n  = it;
			it = (xsetnode*)it->get_child(xsetnode::RIGHT);
		} while ( it != nill );
		return xset_iterator<T,P>(mStrategy, n);
	}

	template <typename T, typename C, typename P>
	inline xset_const_iterator<T,P>	xset<T,C,P>::imin() const
	{
		xsetnode const* nill = &mRoot;
		xsetnode const* root = &mRoot;
		xsetnode const* it = (xsetnode const*)root->get_child(xsetnode::LEFT);
		xsetnode* n;
		do 
		{
			n  = (xsetnode*)it;
			it = (xsetnode const*)it->get_child(xsetnode::LEFT);
		} while ( it != nill );
		return xset_const_iterator<T,P>(mStrategy, n);
	}

	template <typename T, typename C, typename P>
	inline xset_const_iterator<T,P>	xset<T,C,P>::imax() const
	{
		xsetnode const* nill = &mRoot;
		xsetnode const* root = &mRoot;
		xsetnode const* it = (xsetnode const*)root->get_child(xsetnode::LEFT);
		xsetnode* n;
		do 
		{
			n  = it;
			it = (xsetnode const*)it->get_child(xsetnode::RIGHT);
		} while ( it != nill );
		return xset_const_iterator<T,P>(mStrategy, n);
	}

	template <typename T, typename C, typename P>
	inline xset_iterator<T,P>	xset<T,C,P>::begin()
	{
		return imin();
	}

	template <typename T, typename C, typename P>
	inline xset_iterator<T,P>	xset<T,C,P>::end()
	{
		xsetnode * nill = &mRoot;
		return xset_iterator<T,P>(mStrategy, nill);
	}

	template <typename T, typename C, typename P>
	inline xset_const_iterator<T,P>	xset<T,C,P>::begin() const
	{
		return imin();
	}

	template <typename T, typename C, typename P>
	inline xset_const_iterator<T,P>	xset<T,C,P>::end() const
	{
		xsetnode const* nill = &mRoot;
		return xset_const_iterator<T,P>(mStrategy, nill);
	}

}