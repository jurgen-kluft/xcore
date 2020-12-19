#ifndef __XCORE_SORTED_KV_H__
#define __XCORE_SORTED_KV_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

namespace xcore
{
	// Forward declarations
	class alloc_t;

	struct sorted_t
	{ 
		alloc_t *	m_allocator;
		u32			m_capacity;
		u32			m_count;
		u32			m_sizeof_key;
		u32			m_sizeof_val;
		xbyte *		m_keys;
		xbyte *		m_vals;
		u32 *		m_data;

		typedef	s32	(*compare_predicate)(const void* inKey1, const void* inKey2, u32 inKeyLength);
		compare_predicate m_comparefn;

		void		make(alloc_t* _allocator, u32 sizeof_key, u32 sizeof_value, u32 max_size);
		void		destroy();

		void		clear();
		bool		insert(void const* key, const void* value);
		bool		retrieve(void const* key, void* value);
		bool		contains(void const* key);
		bool		remove(void const* key);
	};
};

#endif // __XCORE_SORTED_KV_H__
