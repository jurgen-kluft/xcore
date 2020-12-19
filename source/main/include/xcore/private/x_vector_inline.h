namespace xcore
{
	template <typename T, typename C>
	static s32 xvector_compare(const void* const a,
		const void* const b,
		u32 data)
	{
		C* pCmp = (C*)data;
		ASSERT (pCmp);
		s32 res = (*pCmp)(*((T*)a), *((T*)b) );
		return res <= 0 ? -1 : 1;
	}

	template <class T>
	class xvector_iterator
	{
	public:
		typedef T					value_type;
		typedef s32					difference_type;
		typedef u32					size_type;
		typedef T&					reference;
		typedef T*					pointer;
		typedef xvector_iterator	iterator;

		xvector_iterator(xmemblock* _m, T* _p, u32 item_size_aligned) : m(_m), p((xbyte*)_p), s(item_size_aligned) { }

		inline T*			ptr() const									{ check(); return ((T*)p); }

		reference			operator*() const							{ return *ptr(); }
		pointer				operator->() const							{ return  ptr(); }

		iterator&			operator+=(size_type t)						{ p += t*s; check(); return *this; }
		iterator&			operator-=(size_type t)						{ p -= t*s; check(); return *this; }
		iterator&			operator++()								{ p += s; check(); return *this; }
		iterator&			operator--()								{ p -= s; check(); return *this; }
		iterator			operator++(s32)								{ iterator tmp(*this); p += s; return tmp; }
		iterator			operator--(s32)								{ iterator tmp(*this); p -= s; return tmp; }
		iterator			operator+(difference_type i) const			{ iterator tmp(*this); tmp += i; return tmp; }
		iterator			operator-(difference_type i) const			{ iterator tmp(*this); tmp -= i; return tmp; }
		difference_type		operator-(iterator it) const				{ ASSERT(m== it.m); return ((difference_type)p - (difference_type)it.p) / s; }
		reference			operator[](difference_type n) const			{ return *((T*)(p + n*s)); }

		bool				operator==(const iterator& it) const		{ return m == it.m && p == it.p; }
		bool				operator<(const iterator& it) const			{ ASSERT(m == it.m); return p < it.p; }
		bool				operator!=(const iterator& it) const		{ return !(*this == it); }
		bool				operator<=(const iterator& it) const		{ return !(it < *this); }
		bool				operator>(const iterator& it) const			{ return it < *this; }
		bool				operator>=(const iterator& it) const		{ return !(*this < it); }

		void				check() const								{ ASSERT(p >= m->begin() && p <= (m->begin() + m->capacity())); }
		u32					distance_in_n(const iterator& first, const iterator& last) const { ASSERT(first<=last); return (u32)(last.p - first.p) / s; }
		u32					distance_in_bytes(const iterator& first, const iterator& last) const { ASSERT(first<=last); return (u32)(last.p - first.p); }

	private:
		friend class xvector<T>;
        friend class xvector_const_iterator<T>;
		xmemblock*			m;											///< The managed memory containing the items that we iterate on
		xbyte*				p;											///< item pointer
		u32					s;											///< item_size_aligned
	};

	template<class T>
	class xvector_const_iterator
	{
	public:
		typedef T							value_type;
		typedef s32							difference_type;
		typedef u32							size_type;
		typedef T const&					reference;
		typedef T const*					pointer;
		typedef xvector_const_iterator		iterator;

		xvector_const_iterator(xmemblock const* _m, T const* _p, u32 item_size_aligned) : m(_m), p((xbyte const*)_p), s(item_size_aligned) { }

		inline T const*			ptr() const									{ check(); return ((T*)p); }

		reference				operator*() const							{ return *ptr(); }
		pointer					operator->() const							{ return  ptr(); }

		iterator&				operator+=(size_type t)						{ p += t*s; check(); return *this; }
		iterator&				operator-=(size_type t)						{ p -= t*s; check(); return *this; }
		iterator&				operator++()								{ p += s; check(); return *this; }
		iterator&				operator--()								{ p -= s; check(); return *this; }
		iterator				operator++(s32)								{ iterator tmp(*this); p += s; return tmp; }
		iterator				operator--(s32)								{ iterator tmp(*this); p -= s; return tmp; }
		iterator				operator+(difference_type i) const			{ iterator tmp(*this); tmp += i; return tmp; }
		iterator				operator-(difference_type i) const			{ iterator tmp(*this); tmp -= i; return tmp; }
		difference_type			operator-(iterator it) const				{ ASSERT(m == it.m); return ((difference_type)p - (difference_type)it.p) / s; }
		reference				operator[](difference_type n) const			{ return *((T*)(p + n*s)); }

		bool					operator==(const iterator& it) const		{ return m == it.m && p == it.p; }
		bool					operator<(const iterator& it) const			{ ASSERT(m == it.m); return p < it.p; }
		bool					operator!=(const iterator& it) const		{ return !(*this == it); }
		bool					operator<=(const iterator& it) const		{ return !(it < *this); }
		bool					operator>(const iterator& it) const			{ return it < *this; }
		bool					operator>=(const iterator& it) const		{ return !(*this < it); }

		void					check() const								{ ASSERT(p >= m->begin() && p <= (m->begin() + m->capacity())); }
		u32						distance_in_n(const iterator& first, const iterator& last) const { return (u32)(last.p - first.p) / s; }
		u32						distance_in_bytes(const iterator& first, const iterator& last) const { return (u32)(last.p - first.p); }

	private:
		friend class xvector<T>;

		xmemblock const*		m;											///< The xvector<> that we iterate on
		xbyte const*			p;											///< item pointer
		u32						s;											///< item_size_aligned
	};

	template <typename T>
	inline bool				xvector<T>::operator== (const xvector<T>& v) const
	{
		return (m_Data == v.m_Data); 
	}

	template <typename T>
	inline 					xvector<T>::operator xcmemlink (void) const
	{ 
		return (xcmemlink (m_Data)); 
	}

	template <typename T>
	inline 					xvector<T>::operator xcmemlink (void)
	{ 
		return (xcmemlink (m_Data));
	}

	template <typename T>
	inline 					xvector<T>::operator xmemlink (void)
	{
		return (xmemlink (m_Data));
	}

	template <typename T>
	inline void				relocate(alloc_t* a)
	{

	}

	template <typename T>
	inline u32				xvector<T>::capacity (void) const
	{
		return (m_Data.capacity() / m_ItemSizeAligned);
	}

	template <typename T>
	inline u32				xvector<T>::size (void) const
	{
		return (m_Data.size() / m_ItemSizeAligned); 
	}

	template <typename T>
	inline u32				xvector<T>::max_size (void) const
	{
		return (m_Data.max_size() / m_ItemSizeAligned); 
	}

	template <typename T>
	inline bool				xvector<T>::empty (void) const
	{
		return (m_Data.empty());
	}


	template <typename T>
	inline xvector_iterator<T>			xvector<T>::begin (void)
	{
		return (iterator (&m_Data, (T*)m_Data.begin(), m_ItemSizeAligned)); 
	}

	template <typename T>
	inline xvector_const_iterator<T>	xvector<T>::begin (void) const
	{
		return (const_iterator (&m_Data, (T const*)m_Data.begin(), m_ItemSizeAligned)); 
	}

	template <typename T>
	inline xvector_iterator<T>			xvector<T>::end (void)
	{
		return (iterator (&m_Data, (T*)m_Data.end(), m_ItemSizeAligned)); 
	}

	template <typename T>
	inline xvector_const_iterator<T>	xvector<T>::end (void) const
	{
		return (const_iterator (&m_Data, (T const*)m_Data.end(), m_ItemSizeAligned));	
	}


	template <typename T>
	inline xvector_iterator<T>			xvector<T>::iat (u32 i)
	{ 
		ASSERT (i <= size()); 
		return iterator (&m_Data, (T*)(m_Data.begin() + (i*m_ItemSizeAligned)), m_ItemSizeAligned);
	}

	template <typename T>
	inline xvector_const_iterator<T>	xvector<T>::iat (u32 i) const
	{ 
		ASSERT (i <= size());
		return const_iterator (&m_Data, (T const*)(m_Data.begin() + (i*m_ItemSizeAligned)), m_ItemSizeAligned);
	}


	template <typename T>
	inline T&							xvector<T>::at (u32 i)	
	{ 
		ASSERT (i < size()); 
		return ( begin()[i] );
	}


	template <typename T>
	inline T const&						xvector<T>::at (u32 i) const
	{ 
		ASSERT (i < size());
		return ( begin()[i] ); 
	}

	template <typename T>
	inline xvector_const_iterator<T>	xvector<T>::find(T const& item) const
	{
		const_iterator i = begin();
		const_iterator e = end();
		while (i != e)
		{
			if (*i == item)
				return i;
			++i;
		}
		return e;
	}

	template <typename T>
	inline xvector_iterator<T>			xvector<T>::find(T const& item)
	{
		iterator i = begin();
		iterator e = end();
		while (i != e)
		{
			if (*i == item)
				return i;
			++i;
		}
		return e;
	}

	template <typename T>
	inline T&							xvector<T>::operator[] (u32 i)
	{ 
		return (at (i));
	}

	template <typename T>
	inline T const&					xvector<T>::operator[] (u32 i) const
	{
		return (at (i));
	}


	template <typename T>
	inline T&						xvector<T>::front (void)
	{
		return (at(0));
	}

	template <typename T>
	inline T const&					xvector<T>::front (void) const
	{
		return (at(0)); 
	}

	template <typename T>
	inline T&						xvector<T>::back (void)	
	{
		ASSERT (!empty());
		return (end()[-1]); 
	}

	template <typename T>
	inline T const&					xvector<T>::back (void) const
	{ 
		ASSERT (!empty());
		return (end()[-1]);
	}

	template <typename T>
	inline void				xvector<T>::pop_back (void)
	{
		m_Data.xmemlink::resize (m_Data.size() - m_ItemSizeAligned);
	}


	template <typename T>
	inline void				xvector<T>::clear (void)
	{
		m_Data.clear();
	}

	template <typename T>
	inline void				xvector<T>::swap (xvector<T>& v)
	{
		m_Data.swap (v.m_Data);
	}

	template <typename T>
	inline void				xvector<T>::manage (pointer p, u32 n)
	{
		m_Data.manage (p, n * m_ItemSizeAligned);
	}


	template <typename T>
	inline bool				xvector<T>::is_linked (void) const
	{
		return (m_Data.is_linked()); 
	}

	template <typename T>
	inline void				xvector<T>::unlink (void)
	{
		m_Data.unlink();
	}

	template <typename T>
	inline void				xvector<T>::copy_link (alloc_t* allocator)
	{
		m_Data.copy_link(allocator);
	}


	template <typename T>
	inline void				xvector<T>::link (alloc_t*, const_pointer p, u32 n)
	{
		m_Data.link (p, n * m_ItemSizeAligned);
	}

	template <typename T>
	inline void				xvector<T>::link (alloc_t*, pointer p, u32 n)
	{
		m_Data.link (p, n * m_ItemSizeAligned);
	}

	template <typename T>
	inline void				xvector<T>::link (const xvector<T>& v)	
	{
		m_Data.link (v);
	}

	template <typename T>
	inline void				xvector<T>::link (xvector<T>& v)
	{
		m_Data.link (v);
	}

	template <typename T>
	inline void				xvector<T>::link (alloc_t*, const_pointer first, const_pointer last)
	{ 
		m_Data.link (first, last);
	}

	template <typename T>
	inline void				xvector<T>::link (alloc_t*, pointer first, pointer last)
	{ 
		m_Data.link (first, last);
	}

	/// Allocates space for at least \p n elements.
	template <typename T>
	inline void xvector<T>::reserve (u32 n, bool bExact)
	{
		const u32 oldCapacity = m_Data.capacity();
		m_Data.reserve (n * m_ItemSizeAligned, bExact);
		xstd::xconstruct (iterator (&m_Data, (T*)(m_Data.begin() + oldCapacity), m_ItemSizeAligned), iterator (&m_Data, (T*)(m_Data.begin() + m_Data.capacity()), m_ItemSizeAligned));
	}

	/// Resizes the xvector to contain \p n elements.
	template <typename T>
	inline void xvector<T>::resize (u32 n, bool bExact)
	{
		const u32 nb = n * m_ItemSizeAligned;
		if (m_Data.capacity() < nb)
			reserve (n, bExact);
		m_Data.xcmemlink::resize (nb);
	}

	/// Calls element destructors and frees storage.
	template <typename T>
	inline void xvector<T>::deallocate (void)
	{
		destroy (begin(), iterator (m_Data.begin() + m_Data.capacity()));
		m_Data.deallocate();
	}

	/// Initializes empty xvector.
	template <typename T>
	inline xvector<T>::xvector (void)
		: m_ItemSizeAligned(sizeof(T))
		, m_Data ()
        , m_IsAutoAlloc(false)
	{
	}

	template <typename T>
	inline xvector<T>::xvector (alloc_t* a)
		: m_ItemSizeAligned(sizeof(T))
		, m_Data (a)
        , m_IsAutoAlloc(false)
	{
	}

	/// Initializes a xvector of size \p n.
	template <typename T>
	inline xvector<T>::xvector (alloc_t* a, u32 n)
		: m_ItemSizeAligned(sizeof(T))
		, m_Data (a)
        , m_IsAutoAlloc(false)
	{
		resize (n);
	}

	/// Copies \p n elements from \p v.
	template <typename T>
	xvector<T>::xvector (alloc_t* a, u32 n, const T& v)
		: m_ItemSizeAligned(sizeof(T))
		, m_Data (a)
        , m_IsAutoAlloc(false)
	{
		resize (n);
		xstd::xfill (begin(), end(), v);
	}

	/// Copies \p v.
	template <typename T>
	xvector<T>::xvector (alloc_t* a, const xvector<T>& v)
		: m_ItemSizeAligned(sizeof(T))
		, m_Data (a)
        , m_IsAutoAlloc(v.m_IsAutoAlloc)
	{
		resize (v.size());
		xstd::xcopy (v.begin(), v.end(), begin());
	}

    template <typename T>
    xvector<T>::xvector (const xvector<T>& v)
        : m_ItemSizeAligned(sizeof(T))
        , m_Data(v.m_Data)
        , m_IsAutoAlloc(v.m_IsAutoAlloc)
    {
    }

	/// Copies range [\p i1, \p i2]
	template <typename T>
	xvector<T>::xvector (alloc_t* a, const_iterator& i1, const_iterator& i2)
		: m_ItemSizeAligned(sizeof(T))
		, m_Data (a)
        , m_IsAutoAlloc(false)
	{
		ASSERT (i1 <= i2);
		resize ( (i2.p - i1.p) / m_ItemSizeAligned );
		xstd::xcopy (i1, i2, begin());
	}

	/// Destructor
	template <typename T>
	inline xvector<T>::~xvector (void)
	{
		if(this->size() != 0)
			xstd::xdestroy (begin(), iterator (&m_Data, (T*)(m_Data.begin() + m_Data.capacity()), m_ItemSizeAligned));
	}

	template <typename T>
	inline void xvector<T>::set_allocator (alloc_t* a)
	{
		m_Data.set_allocator(a);
	}


	/// Copies the range [\p i1, \p i2]
	template <typename T>
	inline void xvector<T>::assign (const_iterator i1,const_iterator i2)
	{
		ASSERT (i1 <= i2);
		resize ( i1.distance_in_n(i1, i2) );
		xstd::xcopy (i1, i2, begin());
	}

	/// Copies \p n elements with value \p v.
	template <typename T>
	inline void xvector<T>::assign (u32 n, const T& v)
	{
		resize (n);
		xstd::xfill (begin(), end(), v);
	}

	/// Copies contents of \p v.
	template <typename T>
	inline const xvector<T>& xvector<T>::operator= (const xvector<T>& v)
	{
        assign (v.begin(), v.end());
		return (*this);
	}

	/// Inserts \p n uninitialized elements at \p ip.
	template <typename T>
	inline typename xvector<T>::iterator xvector<T>::insert_space (iterator& ip, u32 n)
	{
		const u32 ipmi = (u32)((ip.p - m_Data.begin()) / m_ItemSizeAligned);
		reserve (size() + n, false);
		return (iterator (&m_Data, (T*)m_Data.insert (m_Data.iat(ipmi), n * m_ItemSizeAligned), m_ItemSizeAligned));
	}

	/// Inserts \p n elements with value \p v at offsets \p ip.
	template <typename T>
	inline typename xvector<T>::iterator xvector<T>::insert (iterator ip, u32 n, const T& v)
	{
		ip = insert_space (ip, n);
		xfill (ip, ip + n, v);
		return (ip);
	}

	/// Inserts value \p v at offset \p ip.
	template <typename T>
	inline typename xvector<T>::iterator xvector<T>::insert (iterator ip, const T& v)
	{
		*(ip = insert_space (ip, 1)) = v;
		return (ip);
	}

	/// Inserts range [\p i1, \p i2] at offset \p ip.
	template <typename T>
	inline typename xvector<T>::iterator xvector<T>::insert (iterator ip, const_iterator i1, const_iterator i2)
	{
		ASSERT (i1 <= i2);
		ip = insert_space (ip, i1.distance_in_n(i1, i2));
		xstd::xcopy (i1, i2, ip);
		return (ip);
	}

	/// Removes \p count elements at offset \p ep.
	template <typename T>
	inline typename xvector<T>::iterator xvector<T>::erase (iterator ep, u32 n)
	{
        xstd::xdestroy(ep.ptr());
		return (iterator (&m_Data, (T*)m_Data.erase (xmemblock::iterator(ep.ptr()), n * m_ItemSizeAligned), m_ItemSizeAligned));
	}

	/// Removes elements from \p ep1 to \p ep2.
	template <typename T>
	inline typename xvector<T>::iterator xvector<T>::erase (iterator ep1, iterator ep2)
	{
		ASSERT (ep1 <= ep2);
		return (erase (ep1, ep1.distance_in_n(ep1, ep2)));
	}

	template <typename T>
	inline void xvector<T>::sort ()
	{
		sort(xstd::xless<T>() );
	}

	template <typename T>
	template <typename C>
	inline void xvector<T>::sort (C cmp)
	{
		xqsort(begin().ptr(), size(), m_ItemSizeAligned, xvector_compare<T, C>, (u32)(&cmp) );
	}

    template <typename T>
    inline void xvector<T>::set_autoalloc(bool bAuto)
    {
        m_IsAutoAlloc = bAuto;
    }

	/// Inserts value \p v at the end of the xvector.
	template <typename T>
	inline s32 xvector<T>::push_back (const T& v)
	{
		const u32 nb = (size()+1) * m_ItemSizeAligned;
        if (m_IsAutoAlloc && nb > m_Data.capacity())
        {
            reserve((size() + 1) * 2, true);
        }

        if (nb <= m_Data.capacity())
        {
            m_Data.xcmemlink::resize(nb);
            //back() = v;

            //xstd::xconstruct() cannot copy construct we call placement new directly
            new(&back(), xcore_t()) T(v);
            return 1;
        }

        return 0;
	}

}