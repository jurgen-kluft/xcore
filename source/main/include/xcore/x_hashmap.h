// x_hashmap.h - xbase hashmap, a map / trie hybrid
#ifndef __XBASE_HASHMAP_H__
#define __XBASE_HASHMAP_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"

#if 0

namespace xcore
{
	struct xdictvalue
	{
		u32				mSpan;
	};

	struct xdictnode
	{
		xdictnode*		mChild[2];
		u32				mSpan;
		xdictvalue		mValue;
	};

	template <typename K, typename V>
	class xdict_heap_strategy;

	template <typename K, typename V>
	class xdict_member_strategy;

	template <typename K, typename V>
	class xdict_derive_strategy;


	template<typename K, typename V, typename C, typename P = xdict_heap_strategy<K,V> >
	class xdict
	{
	public:
						xdict(P& policy);
						~xdict();

		u32				size() const;
		bool			empty() const;

		void			clear();

		void			insert(K const& key, V const& value);
		void			remove(K const& key);

		//iterator		find(K const& key);
		//const_iterator	find(K const& key) const;

		//iterator		begin();
		//iterator		end();

		//const_iterator	begin() const;
		//const_iterator	end() const;
	};
};

#include "xbase/private/x_hashmap_inline.h"

#endif

#endif	///< __XBASE_HASHMAP_H__

