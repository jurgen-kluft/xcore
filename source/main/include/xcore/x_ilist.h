// x_ilist.h - xCore intrusive doubly linked list requiring inheritance
#ifndef __XBASE_ILIST_H__
#define __XBASE_ILIST_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"
#include "xcore/x_list.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	//--------------------------------------------------------------------------
	// Author:
	//		Jurgen Kluft
	// Description:
	//	A xilist is an intrusive list that requires the user to derive its o
	//  bject from xdlistnode. However, this kind of list is very fast since 
	//  linked list node and the actual object have a direct association.
	//	
	//		
	//  To iterate over all the nodes:
	//<CODE [lang= "c++"]>
	// 		
	//  struct myobject : public xdlistnode { ..my members.. };
	//  
	// 	xilist list;
	//
	//  .. a lot of nodes (myobject) are added to 'list' ..
	// 
	// 	xdlistnode* node = list.head();
	//  while (node != &list)
	//  {
	// 		myobject* o = (myobject*)node;
	//      ASSERT(o->is_used);
	// 		
	// 		... do something with your object 'o'
	// 		
	//      node = node->next();
	//  }
	//  
	//  list.clear();
	// 		
	//</CODE>
	//     
	//--------------------------------------------------------------------------

	#define DECLARE_XILISTNODES(N)																			\
		struct xdlistnodes##N																				\
		{																									\
			inline				xdlistnodes##N()		{ }													\
			inline bool			is_used(u32 i) const	{ ASSERT(i<N); return mNodes[i].is_used(); }		\
			xdlistnode *		node(u32 i)				{ ASSERT(i<N); return (xdlistnode *)&mNodes[i]; }	\
			xdlistnode *		node(u32 i) const		{ ASSERT(i<N); return (xdlistnode *)&mNodes[i]; }	\
		protected:																							\
			xdlistnode			mNodes[N];																	\
		}

	DECLARE_XILISTNODES(2);
	DECLARE_XILISTNODES(3);
	DECLARE_XILISTNODES(4);

	struct xilist : public xdlistnode
	{
	public:
		typedef				xlist_derive_strategy<xdlistnode*>					strategy_type;
		typedef				xlist_iterator<xdlistnode, strategy_type>			iterator;
		typedef				xlist_const_iterator<xdlistnode, strategy_type>		const_iterator;

		inline				xilist()											{ mPrev = this; mNext = this; mListIndex = 0; mCount = 0; }
		inline				xilist(u32 list_index)								{ mPrev = this; mNext = this; mListIndex = list_index; mCount = 0; }

		inline u32			index() const										{ return mListIndex; }

		inline xdlistnode*	head() const										{ return mNext; }
		inline xdlistnode*	tail() const										{ return mPrev; }

		template<class T>
		inline T*			cast(xdlistnode* node)								{ node -= mListIndex; return (T*)node; }

		void				push_front(xdlistnode* node);
		void				push_back(xdlistnode* node);

		// Helper functions
		void				push_front(xdlistnodes2* nodes)						{ push_front(nodes->node(mListIndex)); }
		void				push_back(xdlistnodes2* nodes)						{ push_back(nodes->node(mListIndex)); }
		void				push_front(xdlistnodes3* nodes)						{ push_front(nodes->node(mListIndex)); }
		void				push_back(xdlistnodes3* nodes)						{ push_back(nodes->node(mListIndex)); }
		void				push_front(xdlistnodes4* nodes)						{ push_front(nodes->node(mListIndex)); }
		void				push_back(xdlistnodes4* nodes)						{ push_back(nodes->node(mListIndex)); }

		void				insert_before(xdlistnode* node, xdlistnode* new_node);
		void				insert_after(xdlistnode* node, xdlistnode* new_node);

		xdlistnode*			pop_front();
		xdlistnode*			pop_back();

		void				remove(xdlistnode* node);

		inline bool			empty() const										{ return mCount == 0; }
		inline u32			size() const										{ return mCount; }

		void				clear();

		iterator			begin()												{ return xlist_iterator<xdlistnode, strategy_type>(&mStrategy, this, mCount, head()); }
		iterator			end()												{ return xlist_iterator<xdlistnode, strategy_type>(&mStrategy, this, mCount, head()); }
		const_iterator		begin() const										{ return xlist_const_iterator<xdlistnode, strategy_type>(&mStrategy, this, mCount, head()); }
		const_iterator		end() const											{ return xlist_const_iterator<xdlistnode, strategy_type>(&mStrategy, this, mCount, head()); }

	private:
		strategy_type		mStrategy;
		u32					mListIndex;
		u32					mCount;
	};



	//==========================================================================
	// END xCore namespace
	//==========================================================================
};


//==========================================================================
// END
//==========================================================================
#endif    /// __XBASE_ILIST_H__

