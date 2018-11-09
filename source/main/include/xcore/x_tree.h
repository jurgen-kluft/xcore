// x_tree.h - xCore red-black tree
#ifndef __XCORE_REDBLACK_TREE_H__
#define __XCORE_REDBLACK_TREE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase/x_debug.h"

namespace xcore
{
	class tree_t
	{
	public:
		struct node_t;

		struct content_t
		{
			node_t*			node;
			node_t*			links[3];
			s32				flags;
		};

		struct functors
		{
			void*			m_user;
			s32(*cmp)(void* user, node_t* a, node_t* n);
			void(*get)(void* user, node_t* n, content_t& nc);
			void(*set)(void* user, node_t* n, content_t& nc);
		};
	}
}


//==============================================================================
// END
//==============================================================================
#endif    // __XCORE_REDBLACK_TREE_H__