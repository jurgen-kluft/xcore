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

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	//------------------------------------------------------------------------------
	// Author:
	//		Jurgen Kluft
	// Description:
	//	A xislist is an intrusive list that requires the user to derive its object from
	//  xislistnode. However, this kind of list is very fast since linked list node and
	//  the actual object have a direct association. Also you only have to allocate the
	//  object which saves one allocation.
	//	
	//  To iterate over all the nodes:
	//<CODE [lang= "c++"]>
	// 		
	//  struct myobject : public xislistnode { ..my members.. };
	//  
	// 	xislist list;
	//
	//  .. a lot of nodes are added to 'list' ..
	// 
	// 	xislistnode* node = list.head();
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
	//------------------------------------------------------------------------------
	struct xislistnode
	{
	public:
		inline				xislistnode()									{ mNext = this; ; }
		inline bool			is_used() const									{ return mNext!=this; }
		inline xislistnode*	next() const									{ return mNext; }
	protected:
		friend struct xislist;
		xislistnode*		mNext;
	};

	#define DECLARE_XISLISTNODES(N)																			\
		struct xislistnodes##N																				\
		{																									\
			inline				xislistnodes##N()		{ }													\
			inline bool			is_used(u32 i) const	{ ASSERT(i<N); return mNodes[i].is_used(); }		\
			xislistnode *		node(u32 i)				{ ASSERT(i<N); return (xislistnode *)&mNodes[i]; }	\
			xislistnode *		node(u32 i) const		{ ASSERT(i<N); return (xislistnode *)&mNodes[i]; }	\
		protected:																							\
			xislistnode			mNodes[N];																	\
		}

	DECLARE_XISLISTNODES(2);
	DECLARE_XISLISTNODES(3);
	DECLARE_XISLISTNODES(4);

	struct xislist : public xislistnode
	{
	public:
		inline				xislist()										{ mTail = this; mNext = this; mListIndex = 0; mCount = 0; }
		inline				xislist(u32 list_index)							{ mTail = this; mNext = this; mListIndex = list_index; mCount = 0; }

		inline u32			index() const									{ return mListIndex; }

		inline xislistnode*	head() const									{ return mNext; }
		inline xislistnode*	tail() const									{ return mTail; }

		template<class T>
		inline T*			cast(xislistnode* node)							{ node -= mListIndex; return (T*)node; }

		void				push_front(xislistnode* node);
		void				push_back(xislistnode* node);

		void				push_front(xislistnodes2* nodes)				{ push_front(nodes->node(mListIndex)); }
		void				push_back(xislistnodes2* nodes)					{ push_back(nodes->node(mListIndex)); }
		void				push_front(xislistnodes3* nodes)				{ push_front(nodes->node(mListIndex)); }
		void				push_back(xislistnodes3* nodes)					{ push_back(nodes->node(mListIndex)); }
		void				push_front(xislistnodes4* nodes)				{ push_front(nodes->node(mListIndex)); }
		void				push_back(xislistnodes4* nodes)					{ push_back(nodes->node(mListIndex)); }

		void				insert_after(xislistnode* node, xislistnode* new_node);

		xislistnode*		pop_front();

		void				remove(xislistnode* node);

		inline bool			empty() const									{ return mCount == 0; }
		inline u32			size() const									{ return mCount; }

		void				clear();

	private:
		xislistnode*		mTail;
		u32					mListIndex;
		u32					mCount;
	};



	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

//==============================================================================
// END
//==============================================================================
#endif    /// __XBASE_ILIST_H__

