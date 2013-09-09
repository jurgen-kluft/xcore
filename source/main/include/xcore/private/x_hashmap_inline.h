namespace xcore
{
	//////////////////////////////////////////////////////////////////////////
	// Heap Policy
	//////////////////////////////////////////////////////////////////////////
	template <typename K, typename V>
	class xmap_heap_strategy
	{
		x_iallocator*	allocator;

		struct xtrievalue
		{
			xdictvalue	dvalue;
			V			value;

			inline		xtrievalue(V const& _value) : value(_value) {}
		};

		struct xtrienode
		{
			K			key;
			xtrievalue	tvalue;
			xdictnode	dnode;

			inline		xtrienode(K const& _key, V const& _value) : key(_key), tvalue(_value) {}
			XCORE_CLASS_PLACEMENT_NEW_DELETE
		};

	public:
		xmap_heap_strategy(x_iallocator* a);

		xdictnode*		allocNode(K const& key, V const& value);
		void			deallocNode(xdictnode* node);

		V&				toItem(xdictnode* node);
		V const&		toItem(xdictnode const* node) const;

		K&				toKey(xdictnode* node);
		K const&		toKey(xdictnode const* node) const;	
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

		xdictnode*		allocNode(K const& key, V const& value);
		void			deallocNode(xdictnode* node);

		V&				toItem(xdictnode* node);
		V const&		toItem(xdictnode const* node) const;

		K&				toKey(xdictnode* node);
		K const&		toKey(xdictnode const* node) const;	
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

		xdictnode*		allocNode(K const& key, V const& value);
		void			deallocNode(xdictnode* node);

		V&				toItem(xdictnode* node);
		V const&		toItem(xdictnode const* node) const;

		K&				toKey(xdictnode* node);
		K const&		toKey(xdictnode const* node) const;	
	};
}