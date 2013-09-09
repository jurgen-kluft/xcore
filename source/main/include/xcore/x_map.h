// x_map.h - xCore map
#ifndef __XBASE_MAP_H__
#define __XBASE_MAP_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase\x_debug.h"
#include "xbase\x_allocator.h"
#include "xbase\x_tree.h"
#include "xbase\private\x_std.h"

namespace xcore
{
	//------------------------------------------------------------------------------
	// Author:
	//		Jurgen Kluft
	// Description:
	//	    A xmap is a container that stores items in a BST (Binary Search Tree) 
	//      under a key to provide constant time insert/find/remove functions. 
	//
	//		A map does not handle duplicate keys.
	// 
	//<CODE [lang= "c++"]>
	// 		
	// 	Need an example here
	// 		
	// 		
	//</CODE>
	//     
	//------------------------------------------------------------------------------
	struct xmapnode : public xrbnode
	{
		inline		xmapnode() { clear(); }
	};

	template <typename K, typename V>
	class xmap_heap_strategy;

	template <typename K, typename V>
	class xmap_member_strategy;

	template <typename K, typename V>
	class xmap_derive_strategy;

	template <typename K, typename V, typename P>
	class xmap_iterator;

	template <typename K, typename V, typename P>
	class xmap_const_iterator;

	template<typename K, typename V, typename C = xstd::xcompare<K>, typename P = xmap_heap_strategy<K,V> >
	class xmap
	{
	public:
		typedef			xmap_iterator<K,V,P>			iterator;
		typedef			xmap_const_iterator<K,V,P>		const_iterator;

						xmap(P const& policy);
						xmap(const xmap<K,V,C,P>& m);
						~xmap();

		u32				size() const;
		bool			empty() const;

		void			clear();

		xbool			insert(K const& key, V const& value);
		xbool			remove(K const& key);
		iterator		remove(iterator ip);

		iterator		find(K const& key);
		const_iterator	find(K const& key) const;

		V&				operator[](K const& key);

		iterator		imin();
		iterator		imax();
		const_iterator	imin() const;
		const_iterator	imax() const;

		iterator		begin();
		iterator		end();
		const_iterator	begin() const;
		const_iterator	end() const;

		xmap<K,V,C,P>&	operator = (const xmap<K,V,C,P>& m);

	private:
		P				mStrategy;
		xmapnode		mRoot;
		u32				mCount;
	};
};

#include "xbase\private\x_map_inline.h"


#endif	///< __XBASE_MAP_H__

