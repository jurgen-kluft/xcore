namespace xcore
{
	//////////////////////////////////////////////////////////////////////////
	// List Heap Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename T, typename P>
	class xpqueue_heap_strategy
	{
		x_iallocator*	allocator;

		struct xpqueuenode_heap
		{
			T				item;
			P				priority;
			xpqueuenode		node;
			XCORE_CLASS_PLACEMENT_NEW_DELETE
			inline			xpqueuenode_heap(T const& _item, P const& _prio) : item(_item), priority(_prio) {}
		};
	public:
		xpqueue_heap_strategy(x_iallocator* a);

		xpqueuenode*	allocNode(T const& item, P const& priority);
		void			deallocNode(xpqueuenode* node);

		T&				toItem(xpqueuenode* node);
		T const&		toItem(xpqueuenode const* node) const;

		P&				toPriority(xpqueuenode* node);
		P const&		toPriority(xpqueuenode const* node) const;
	};


	//////////////////////////////////////////////////////////////////////////
	// List Member Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename T, typename P>
	class xpqueue_member_strategy
	{
		x_iallocator*	allocator;
		u32				item_to_node_offset;

	public:
		xpqueue_member_strategy(x_iallocator* a, u32 item_to_node_offset);

		xpqueuenode*	allocNode(T const& item, P const& priority);
		void			deallocNode(xpqueuenode* node);

		T&				toItem(xpqueuenode* node);
		T const&		toItem(xpqueuenode const* node) const;

		P&				toPriority(xpqueuenode* node);
		P const&		toPriority(xpqueuenode const* node) const;
	};


	//////////////////////////////////////////////////////////////////////////
	// List Derive Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename T, typename P>
	class xpqueue_derive_strategy
	{
	public:
		xpqueue_derive_strategy();

		xpqueuenode*	allocNode(T const& item, P const& priority);
		void			deallocNode(xpqueuenode* node);

		T&				toItem(xpqueuenode* node);
		T const&		toItem(xpqueuenode const* node) const;

		P&				toPriority(xpqueuenode* node);
		P const&		toPriority(xpqueuenode const* node) const;
	};


	template <typename T, typename P>
	inline			xpqueue_heap_strategy<T,P>::xpqueue_heap_strategy(x_iallocator* a) 
		: allocator(a)
	{
	}

	template <typename T, typename P>
	xpqueuenode*		xpqueue_heap_strategy<T,P>::allocNode(T const& item, P const& priority)
	{
		void* node_mem = allocator->allocate(sizeof(xpqueuenode_heap), 4);
		xpqueuenode_heap* node = new (node_mem) xpqueuenode_heap(item, priority);
		return &node->node;
	}

	template <typename T, typename P>
	void			xpqueue_heap_strategy<T,P>::deallocNode(xpqueuenode* node)
	{
		xpqueuenode_heap* _node = (xpqueuenode_heap*)((u32)node - X_OFFSET_OF(xpqueuenode_heap, node));
		_node->~xpqueuenode_heap();
		allocator->deallocate(_node);
	}

	template <typename T, typename P>
	T&				xpqueue_heap_strategy<T,P>::toItem(xpqueuenode* node)
	{
		xpqueuenode_heap* _node = (xpqueuenode_heap*)((u32)node - X_OFFSET_OF(xpqueuenode_heap, node));
		return _node->item;
	}

	template <typename T, typename P>
	T const&		xpqueue_heap_strategy<T,P>::toItem(xpqueuenode const* node) const
	{
		xpqueuenode_heap const* _node = (xpqueuenode_heap const*)((u32)node - X_OFFSET_OF(xpqueuenode_heap, node));
		return _node->item;
	}

	template <typename T, typename P>
	P&				xpqueue_heap_strategy<T,P>::toPriority(xpqueuenode* node)
	{
		xpqueuenode_heap* _node = (xpqueuenode_heap*)((u32)node - X_OFFSET_OF(xpqueuenode_heap, node));
		return _node->priority;
	}

	template <typename T, typename P>
	P const&		xpqueue_heap_strategy<T,P>::toPriority(xpqueuenode const* node) const
	{
		xpqueuenode_heap* _node = (xpqueuenode_heap*)((u32)node - X_OFFSET_OF(xpqueuenode_heap, node));
		return _node->priority;
	}


	template <typename T, typename P>
	inline			xpqueue_member_strategy<T,P>::xpqueue_member_strategy(x_iallocator* a, u32 item_to_node_offset) 
		: allocator(a)
	{

	}

	template <typename T, typename P>
	xpqueuenode*		xpqueue_member_strategy<T,P>::allocNode(T const& item, P const& priority)
	{
		xpqueuenode* node = (xpqueuenode*)((u32)(&item) + item_to_node_offset);
		return node; 
	}

	template <typename T, typename P>
	void			xpqueue_member_strategy<T,P>::deallocNode(xpqueuenode* node)
	{
		node->clear();
	}

	template <typename T, typename P>
	T&				xpqueue_member_strategy<T,P>::toItem(xpqueuenode* node)
	{ 
		T* _item = (u32)node - item_to_node_offset; 
		return *_item; 
	}

	template <typename T, typename P>
	T const&		xpqueue_member_strategy<T,P>::toItem(xpqueuenode const* node) const
	{ 
		T const* _item = (u32)node - item_to_node_offset;
		return *_item;
	}


	template <typename T, typename P>
	inline			xpqueue_derive_strategy<T,P>::xpqueue_derive_strategy()
	{ 
		ASSERTCT(xstd::xtypetrait<T>::isPointer);
	}

	template <typename T, typename P>
	xpqueuenode*		xpqueue_derive_strategy<T,P>::allocNode(T const& item, P const& priority)
	{ 
		xpqueuenode* node = (xpqueuenode*)item; 
		return node;
	}

	template <typename T, typename P>
	void			xpqueue_derive_strategy<T,P>::deallocNode(xpqueuenode* node)
	{ 
		node->clear();
	}

	template <typename T, typename P>
	T&				xpqueue_derive_strategy<T,P>::toItem(xpqueuenode* node)
	{
		return *(T*)node;;
	}

	template <typename T, typename P>
	T const&		xpqueue_derive_strategy<T,P>::toItem(xpqueuenode const* node) const
	{
		return *(T const*)node; 
	}

	template <typename T, typename P>
	P&				xpqueue_derive_strategy<T,P>::toPriority(xpqueuenode* node)
	{
		return *(P*)node;
	}

	template <typename T, typename P>
	P const&		xpqueue_derive_strategy<T,P>::toPriority(xpqueuenode const* node) const
	{
		return *(P const*)((xbyte const*)node);
	}



	template <typename T, typename P, typename C, typename S>
	inline				xpqueue<T,P,C,S>::xpqueue(S const& _strategy) 
		: mCount(0)
		, mStrategy(_strategy)
	{
		
	}

	template <typename T, typename P, typename C, typename S>
	inline				xpqueue<T,P,C,S>::~xpqueue() 
	{
		clear();
	}

	template <typename T, typename P, typename C, typename S>
	inline u32			xpqueue<T,P,C,S>::size() const
	{
		return mCount; 
	}

	template <typename T, typename P, typename C, typename S>
	inline bool			xpqueue<T,P,C,S>::empty() const	
	{
		return mCount == 0; 
	}

	template <typename T, typename P, typename C, typename S>
	inline void			xpqueue<T,P,C,S>::clear()
	{
		//	Rotate away the left links so that
		//	we can treat this like the destruction
		//	of a linked list
		xpqueuenode* nill = &mRoot;
		xpqueuenode* root = nill;
		xpqueuenode* it = (xpqueuenode*)root->get_child(xpqueuenode::LEFT);
		while ( it != nill ) 
		{
			xpqueuenode *save;
			if ( it->get_child(xpqueuenode::LEFT) == nill ) 
			{
				/* No left links, just kill the node and move on */
				save = (xpqueuenode*)it->get_child(xpqueuenode::RIGHT);
				if (it != nill)
				{
					// Check for siblings
					xpqueuenode* head = (xpqueuenode*)it->get_siblings();
					if (head != NULL)
					{
						xpqueuenode* sib = head;
						do
						{	xpqueuenode* nsib = (xpqueuenode*)sib->get_child(xrbnode::RIGHT);
							mStrategy.deallocNode(sib);
							sib = nsib;
						} while (sib!=head);
					}
					mStrategy.deallocNode(it);
					--mCount;
				}
			}
			else
			{
				/* Rotate away the left link and check again */
				save = (xpqueuenode*)it->get_child(xpqueuenode::LEFT);
				it->set_child(save->get_child(xpqueuenode::RIGHT), xpqueuenode::LEFT);
				save->set_child(it, xpqueuenode::RIGHT);
			}

			it = save;
		}

		root->set_parent(nill);
		root->set_child(nill, xpqueuenode::LEFT);
	}

	template <typename T, typename P, typename C, typename S>
	inline void			xpqueue<T,P,C,S>::enqueue(T const& item, P const& priority)
	{
		xpqueuenode* root     = &mRoot;
		xpqueuenode* endNode  = root;
		xpqueuenode* lastNode = root;
		xpqueuenode* curNode  = (xpqueuenode*)root->get_child(xpqueuenode::LEFT);;
		s32 s = xpqueuenode::LEFT;
		C comparer;
		while (curNode != endNode)
		{
			lastNode = curNode;

			P const& curNodePriority = mStrategy.toPriority(curNode);
			s32 c = comparer(priority, curNodePriority);
			if (c == 0)
			{
				xpqueuenode* sibling = mStrategy.allocNode(item, priority);
				curNode->insert_sibling(sibling);
			}
			s = (c < 0) ? xpqueuenode::LEFT : xpqueuenode::RIGHT;
			curNode = (xpqueuenode*)curNode->get_child(s);
		}

		xpqueuenode* node = mStrategy.allocNode(item, priority);

		rb_attach_to(node, lastNode, s);
		rb_insert_fixup(*root, node);
		++mCount;

#ifdef DEBUG_RBTREE
		rb_check(root);
#endif
	}

}