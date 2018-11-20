#include "xbase/x_base.h"
#include "xbase/x_allocator.h"
#include "xbase/x_memory.h"
#include "xcore/x_sorted.h"
#include "xbase/x_binary_search.h"

namespace xcore
{
	sorted_t::sorted_t()
		: m_allocator(nullptr)
		, m_capacity(0)
		, m_count(0)
		, m_sizeof_key(0)
		, m_sizeof_val(0)
		, m_keys(nullptr)
		, m_vals(nullptr)
		, m_data(nullptr)
	{

	}

	static s32 sorted_compare_predicate(const void* inItem, const void* inData, s32 inIndex)
	{
		sorted_t const* array = (sorted_t const*)inData;
		return x_memcmp(&array->m_keys[inIndex], inItem);
	}

	void		sorted_t::make(xalloc* _allocator, u32 _sizeof_key, u32 _sizeof_value, u32 _max_size)
	{
		m_allocator = _allocator;
		m_capacity = _max_size;
		m_count = 0;
		m_sizeof_key = _sizeof_key;
		m_sizeof_val = _sizeof_val;
		m_keys = (xbyte*)m_allocator->allocate(_sizeof_key * _max_size, X_ALIGNMENT_DEFAULT);
		m_vals = (xbyte*)m_allocator->allocate(_sizeof_val * _max_size, X_ALIGNMENT_DEFAULT);
		m_data = (u32*)m_allocator->allocate(_max_size, X_ALIGNMENT_DEFAULT);
		m_comparefn = sorted_compare_predicate;
	}

	void		sorted_t::destroy()
	{
		if (m_data != nullptr)
		{
			m_allocator->deallocate(m_keys);
			m_allocator->deallocate(m_values);
			m_allocator->deallocate(m_data);
			m_allocator = nullptr;
			m_capacity = 0;
			m_count = 0;
			m_sizeof_key = 0;
			m_sizeof_val = 0;
			m_keys = nullptr;
			m_vals = nullptr;
			m_data = nullptr;
			m_comparefn = nullptr;
		}
	}

	void		sorted_t::clear()
	{
		m_count = 0;
	}

	bool		sorted_t::insert(void const* key, const void* value)
	{
		// guard our capacity
		if (m_count >= m_capacity)
			return false;

		// insertion in sorted data
		u32 index = x_LowerBound(key, this, m_count, m_comparefn);
		if (m_comparefn(key, this, index) != 0)
		{
			u32 kvidx = m_data[m_count];
			x_memcpy(m_keys[kvidx], key, m_sizeof_key);
			x_memcpy(m_vals[kvidx], value, m_sizeof_val);

			// insert space at m_data[index]
			for (s32 i=m_count; i>index; --i)
			{
				m_data[i] = m_data[i-1];
			}
			m_data[index] = kvidx;

			m_count++;
			return true;
		}
		return false;
	}

	bool		sorted_t::retrieve(void const* key, void* value)
	{
		u32 index = x_BinarySearch(key, this, m_count, m_comparefn);
		if (m_comparefn(key, this, index) == 0)
		{
			x_memcpy(value, &m_vals[index], m_sizeof_val);
			return true;
		}
		return false;
	}

	bool		sorted_t::contains(void const* key)
	{
		u32 index = x_BinarySearch(key, this, m_count, m_comparefn);
		return (m_comparefn(key, this, index) == 0);
	}

	bool		sorted_t::remove(void const* key)
	{
		// find the index of this key
		// remove this index from the 'm_data' array
		u32 index = x_BinarySearch(key, this, m_count, m_comparefn);
		if (m_comparefn(key, this, index) == 0)
		{
			u32 const kvidx = m_data[index];

			// 0 1 2 3, index = 2, count = 4
			// 0 1 3 2, count = 3
			
			// remove m_data[index]
			m_count--;
			for (s32 i=index; i<m_count; ++i)
			{
				m_data[i] = m_data[i+1];
			}

			m_data[m_count] = kvidx;	// Remember the removed kv index for use in insert()
			return true;
		}
		return false;
	}


};


