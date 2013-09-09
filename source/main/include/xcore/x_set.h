// x_set.h - xCore set
#ifndef __XBASE_SET_H__
#define __XBASE_SET_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase\x_allocator.h"
#include "xbase\x_debug.h"
#include "xbase\x_tree.h"
#include "xbase\private\x_std.h"

namespace xcore
{
	//------------------------------------------------------------------------------
	// Author:
	//		Virtuos
	// Description:
	//	    A xset is a container that stores unique items in a BST (Binary Search Tree) 
	//      to provide constant time insert/find/remove functions.
	//		
	//<CODE [lang= "c++"]>
	// 		
	// 	Need an example here
	// 		
	// 		
	//</CODE>
	//------------------------------------------------------------------------------
	struct xsetnode : public xrbnode
	{
		inline		xsetnode() { clear(); }
	};

	template <typename T>
	class xset_heap_strategy;

	template <typename T>
	class xset_member_strategy;

	template <typename T>
	class xset_derive_strategy;
	
	template <typename T, typename P>
	class xset_iterator;

	template <typename T, typename P>
	class xset_const_iterator;

	/*
	*/
	template<typename T, typename C = xstd::xcompare<T>, typename P = xset_heap_strategy<T> >
	class xset
	{
	public:
		typedef			xset_iterator<T,P>			iterator;
		typedef			xset_const_iterator<T,P>	const_iterator;

						xset(P const& policy);
						~xset();

		u32				size() const;
		bool			empty() const;

		void			clear();

		iterator		insert(T const& item);
		void			insert(iterator _first, iterator _last);

		xbool			remove(T const& item);
		xbool			remove(iterator _where);
		xbool			remove(const_iterator _where);
		void			remove(iterator _first, iterator _last);

		iterator		find(T const& item);
		const_iterator	find(T const& item) const;

		iterator		imin();
		iterator		imax();
		const_iterator	imin() const;
		const_iterator	imax() const;

		iterator		begin();
		iterator		end();
		const_iterator	begin() const;
		const_iterator	end() const;

	private:
		P				mStrategy;
		u32				mCount;
		xsetnode		mRoot;
	};
};

#include "xcore\private\x_set_inline.h"

#endif	///< __XBASE_SET_H__

