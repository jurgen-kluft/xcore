//#define TEST_TRIEMAP
#ifdef TEST_TRIEMAP

// (c) 2011-2012 - Jurgen Kluft
// Next-Generation implementation of the trie (hash) map
// Uses less memory and is much faster.

#ifdef TARGET_PC
#include <windows.h>
#include <map>
#endif
#ifdef TARGET_PS3
#include <map>
#include <sys/sys_time.h>
#include <cell/perf/performance.h>
#endif

#include "xbase\x_target.h"
#include "xbase\x_allocator.h"
#include "xbase\x_debug.h"
#include "xbase\x_integer.h"
#include "xbase\x_string.h"
#include "xbase\x_memory_std.h"

#include "xunittest\xunittest.h"

// <MyCustomAlloc>
const size_t MAX_SIZE = (10000000 + 1) * 12;

static long g_nCnt = 0;  // total # blocks allocated
static long g_nTot = 0;  // total allocated
template <class T>
class MyCustomAlloc
{
public:
	typedef T				    value_type;
	typedef xcore::u32			size_type;
	typedef xcore::s32			difference_type;

	typedef T*         pointer;
	typedef const T*   const_pointer;

	typedef T&         reference;
	typedef const T&   const_reference;

	MyCustomAlloc(xcore::xbyte *pool, int nPoolSize)
	{
		Init();
		m_pool = pool;
		m_nPoolSize = nPoolSize;
	}
	MyCustomAlloc(int n) { Init(); }
	MyCustomAlloc() { Init(); }

	void Init()
	{
		m_pool = 0;
		m_nPoolSize = 0;
		g_nCnt = 0;
		g_nTot = 0;
	}
	MyCustomAlloc(const MyCustomAlloc &obj) // copy constructor
	{
		Init();
		m_pool = obj.m_pool;
		m_nPoolSize = obj.m_nPoolSize;
	}
private:
	void operator =(const MyCustomAlloc &);
public:
	xcore::xbyte *m_pool;
	unsigned  m_nPoolSize;

	template <class _Other>
	MyCustomAlloc(const MyCustomAlloc<_Other> &other)
	{
		Init();
		m_pool= other.m_pool;
		m_nPoolSize = other.m_nPoolSize;
	}

	~MyCustomAlloc() { }

	template <class U>
	struct rebind
	{
		typedef MyCustomAlloc<U> other ;
	};

	pointer address( reference r ) const { return &r; }
	const_pointer address( const_reference r ) const { return &r; }

	pointer allocate( size_type n, const void* /*hint*/=0 )
	{
		pointer p;
		unsigned nSize = n * sizeof(T);
		if (m_pool) // if we have a mem pool from which to allocated
		{
			p = (pointer) m_pool;// just return the next available mem in the pool
			if (g_nTot + nSize > m_nPoolSize)
			{
				ASSERT(0);//,"out of mem pool");
				return 0;
			}
			m_pool += nSize;  // and bump the pointer
		}
		else
		{
			p = NULL;// no pool: error
		}
		g_nCnt+=1;
		g_nTot += nSize;
		ASSERT(p);
		return p;
	}

	void deallocate( pointer p, size_type /*n*/ )
	{
		if (!m_pool)// if there's a pool, nothing to do
		{
			free( p);
		}
	}

	void construct( pointer p, const T& val ) { new (p) T(val); }
	void destroy( pointer p ) { p->~T(); }

	size_type max_size() const
	{ 
		return MAX_SIZE / sizeof(T); 
	}
};

template <class T>
bool operator==( const MyCustomAlloc<T>& left, const MyCustomAlloc<T>& right )
{
	return (left.m_pool == right.m_pool);
}

template <class T>
bool operator!=( const MyCustomAlloc<T>& left, const MyCustomAlloc<T>& right)
{
	return (left.m_pool != right.m_pool);
}

// </MyCustomAlloc>


extern xcore::x_iallocator* gTestAllocator;

namespace xcore
{
#ifdef TARGET_PC
	class CStopwatch2
	{
	public:
		// Does some initialization to get consistent results for all tests.
		CStopwatch2()
			: m_startCount(0), m_elapsedTimeSec(0.0)
		{
		}

		// Starts measuring performance
		// (to be called before the block of code to measure).
		void Start()
		{
			// Clear total elapsed time
			// (it is a spurious value until Stop() is called)
			m_elapsedTimeSec = 0.0;

			// Start ticking
			m_startCount = Counter();
		}

		// Stops measuring performance
		// (to be called after the block of code to measure).
		void Stop()
		{
			// Stop ticking
			LONGLONG stopCount = Counter();

			// Calculate total elapsed time since Start() was called;
			// time is measured in seconds
			m_elapsedTimeSec = (stopCount - m_startCount) * 1.0 / Frequency();

			// Clear start count (it is spurious information)
			m_startCount = 0;
		}

		// Returns total elapsed time (in seconds) in Start-Stop interval.
		double ElapsedTimeSec() const
		{
			// Total elapsed time was calculated in Stop() method.
			return m_elapsedTimeSec;
		}

		// Returns total elapsed time (in milliseconds) in Start-Stop interval.
		double ElapsedTimeMilliSec() const
		{
			// Total elapsed time was calculated in Stop() method.
			return m_elapsedTimeSec * 1000.0;
		}

	private:
		// The value of counter on start ticking
		LONGLONG m_startCount;

		// The time (in seconds) elapsed in Start-Stop interval
		double m_elapsedTimeSec;

		// Returns current value of high-resolution performance counter.
		LONGLONG Counter() const
		{
			LARGE_INTEGER counter;
			QueryPerformanceCounter(&counter);
			return counter.QuadPart;
		}

		// Returns the frequency (in counts per second) of the
		// high-resolution performance counter.
		LONGLONG Frequency() const
		{
			LARGE_INTEGER frequency;
			QueryPerformanceFrequency(&frequency);
			return frequency.QuadPart;
		}

	private:
		CStopwatch2(const CStopwatch2 &);
		CStopwatch2 & operator=(const CStopwatch2 &);
	};
#elif defined(TARGET_PS3)

	class CStopwatch2
	{
	public:
		// Does some initialization to get consistent results for all tests.
		CStopwatch2()
			: m_elapsedTimeSec(0.0)
		{
		}

		~CStopwatch2()
		{
		}

		// Starts measuring performance
		// (to be called before the block of code to measure).
		void Start()
		{
			// Clear total elapsed time
			// (it is a spurious value until Stop() is called)
			m_elapsedTimeSec = 0.0;

			// Start ticking
			m_beginTime = sys_time_get_system_time();
		}

		// Stops measuring performance
		// (to be called after the block of code to measure).
		void Stop()
		{
			// Stop ticking
			system_time_t endTime = sys_time_get_system_time();
			double msec = (double)(endTime - m_beginTime)/1000.0;

			// Calculate total elapsed time since Start() was called;
			// time is measured in seconds
			m_elapsedTimeSec = msec / 1000.0;
		}

		// Returns total elapsed time (in seconds) in Start-Stop interval.
		double ElapsedTimeSec() const
		{
			// Total elapsed time was calculated in Stop() method.
			return m_elapsedTimeSec;
		}

		// Returns total elapsed time (in milliseconds) in Start-Stop interval.
		double ElapsedTimeMilliSec() const
		{
			// Total elapsed time was calculated in Stop() method.
			return m_elapsedTimeSec * 1000.0;
		}

	private:
		// The value of counter on start ticking
		system_time_t	m_beginTime;

		// The time (in seconds) elapsed in Start-Stop interval
		double		m_elapsedTimeSec;

	private:
		CStopwatch2(const CStopwatch2 &);
		CStopwatch2 & operator=(const CStopwatch2 &);
	};
#endif

#define MAPSIZE_2LOG		(8)
#define MAPSIZE_MASK		((1<<MAPSIZE_2LOG)-1)
#define MAPSIZE				(1<<MAPSIZE_2LOG)

#define TRIENODE_KEY_BITS	(1)
#define TRIENODE_KEY_FAN	(1<<TRIENODE_KEY_BITS)
#define TRIENODE_KEY_MASK	(TRIENODE_KEY_FAN-1)


	struct trienode
	{
		enum EType { NODE=0, VALUE=1 };
		inline void			clear()										{ mX=0; mLink[0] = mLink[1] = mParent = NULL; }

		f_inline void		set_x(u8 x)									{ mX = x; }
		f_inline u8			get_x() const								{ return mX; }

		f_inline void		set_parent(trienode* n)						{ mParent = n; }
		f_inline trienode*	get_parent() const							{ return mParent; }

		f_inline void		set_node(u32 l, trienode* n)				{ mLink[l] = n; }
		f_inline void		set_node(u32 l, trienode* n, EType type)	{ mLink[l] = (trienode*)((u32)n | type); }
		f_inline trienode*	get_node(u32 l, EType& type) const			{ trienode* n=mLink[l]; type=(EType)(((u8)(u32)n)&1); return (trienode*)((u32)n & 0xfffffffe); }
		f_inline trienode*	get_node(u32 l) const						{ trienode* n=mLink[l]; return (trienode*)((u32)n & 0xfffffffe); }
		f_inline EType		get_node_type(u32 l) const					{ trienode* n=mLink[l]; return (EType)(((u8)(u32)n)&1); }

		f_inline s32		side_of(trienode* n)						{ ASSERT(mLink[0]==n || mLink[1]==n); return mLink[0]==n ? 0:1; }
		f_inline s32		side_of(trienode* n, EType type)			{ n = (trienode*)((u32)n|type); ASSERT(mLink[0]==n || mLink[1]==n); return mLink[0]==n ? 0:1; }
		f_inline bool		is_linked() const							{ return mX!=0; }

		inline void			replace(trienode* other)
		{
			other->mX       = mX;
			for (u32 i=0; i<2; ++i)
			{
				trienode* child = mLink[i];
				other->mLink[i] = child;
				if (get_node_type(i)==trienode::NODE)
					child->set_parent(other);
				mLink[i]        = NULL;
			}
			other->mParent  = mParent;
			if (mParent != NULL)
			{
				EType type;
				if (mParent->get_node(0, type)==this)
					mParent->set_node(0, other, type);
				else if (mParent->get_node(1, type)==this)
					mParent->set_node(1, other, type);
			}

			mX       = 0;
			mParent  = NULL;
		}

		//XCORE_CLASS_PLACEMENT_NEW_DELETE

	private:
		u8					mX;
		trienode*			mLink[2];
		trienode*			mParent;
	};

	struct triemap2
	{
		triemap2(x_iallocator* node_allocator) 
			: mNodeAllocator(node_allocator)
			, mNumNodes(0)
			, mNumValues(0)
		{
			for (s32 i=0; i<MAPSIZE; ++i)
				mTable[i] = NULL;
		}

		s32					insert(u32 key, trienode* value);
		trienode*			remove(u32 key);
		trienode*			find(u32 key) const;
		s32					trace(u32 key, s32& outBranching) const;

	private:
		inline trienode*	allocNode();
		inline void			freeNode(trienode* node);

		// The initial MAPSIZE_2LOG bits are distributing the key
		// here, after this one it is 'span-length' bits per node.
		// So the tree has a maximum depth of 32-MAPSIZE_2LOG.
		x_iallocator*		mNodeAllocator;
		trienode*			mTable[MAPSIZE];

		u32					mNumNodes;
		u32					mNumValues;
	};

	static trienode		sTrieNodes[10000000];
	static u32			sTrieNodeIdx = 0;

	// This is a HACK, we need to expose this to the public interface
	inline u32				get_key_of(trienode* node)
	{
		return (u32)node;
	}

	inline trienode*		triemap2::allocNode()
	{
		++mNumNodes;
		trienode* nn = &sTrieNodes[sTrieNodeIdx++];
		return nn;
	}

	inline void			triemap2::freeNode(trienode* node)
	{
		--mNumNodes;
		//mNodeAllocator->deallocate(node);
		//free(node);
	}

	trienode*		triemap2::find(u32 f_key) const
	{
		u32 const table_idx = f_key & MAPSIZE_MASK;
		trienode* n_node = mTable[table_idx];
		if (n_node == NULL)
			return NULL;

		u32 n_x  = (MAPSIZE_2LOG-1);
		u32 n_bx = 1<<(MAPSIZE_2LOG-1);
		while (true)
		{
			u32 const n_key = (u32)(n_node);
			u32 const diff = f_key ^ n_key;
			if (diff==0)
				return n_node;

			u32 const n_y = n_x;
			n_x = n_node->get_x();

			if (n_x == (n_y+1))
			{
				n_bx = n_bx << 1;
			}
			else
			{
				u32 const n_by = n_bx << 1;
				n_bx = 1 << n_x;
				if ((diff&((n_bx-1) & (~(n_by-1))))!=0)
					break;
			}

			u32 const n_dir = ((f_key&n_bx)!=0);
			trienode::EType n_type;
			n_node = n_node->get_node(n_dir, n_type);
			if (n_type == trienode::VALUE)
			{
				u32 const n_key = (u32)(n_node);
				return (f_key == n_key) ? n_node : NULL;
			}
			ASSERT(n_node!=NULL);
			ASSERT(n_node->get_parent()->side_of(n_node, n_type)!=2);
		}
		return NULL;
	}

	// 0 = Failure
	// 1 = Success
	s32				triemap2::insert(u32 i_key, trienode* i_node)
	{
		u32 const map_index = i_key & MAPSIZE_MASK;
		trienode* n_node = mTable[map_index];
		if (n_node == NULL)
		{
			u32 const i_x = 31;
			i_node->set_x(i_x);
			mTable[map_index] = i_node;
			u32 const i_dir = ((i_key & (u32)(1<<i_x))==0) ? 0 : 1;
			i_node->set_node(i_dir, i_node, trienode::VALUE);
			i_node->set_node(1-i_dir, NULL, trienode::VALUE);
			ASSERT(i_node->get_parent()==NULL);
			mNumValues++;
			return 1;
		}
		
		trienode::EType n_type = trienode::NODE;
		u32 n_x  = (MAPSIZE_2LOG-1);
		u32 n_bx = 1<<(MAPSIZE_2LOG-1);
		while (true)
		{
			u32 const n_key = (u32)(n_node);
			u32 const diff = i_key ^ n_key;				// XOR to identify the mismatching bit(s)
			if (diff == 0)
				break;

			u32 const n_y  = n_x;
			n_x = n_node->get_x();
			if (n_x == (n_y+1))
			{
				n_bx = n_bx<<1;
			}
			else
			{
				u32 const n_by = n_bx<<1;
				n_bx = 1<<n_x;
				if ((diff&((n_bx-1) & (~(n_by-1))))!=0)
				{
					ASSERT( (diff & ((n_by - 1))) == 0);	// Any bits before and including n_y should match

					// Count the leading zeros
					u32 i_x;
#ifdef TARGET_PC
					BitScanForward((DWORD*)&i_x, diff);
#else
					if (diff != 0)
					{
						s32 d = diff;
						i_x = 0;
						if ((d & 0x0000FFFF) == 0) {i_x += 16; d = d >>16;}
						if ((d & 0x000000FF) == 0) {i_x +=  8; d = d >> 8;}
						if ((d & 0x0000000F) == 0) {i_x +=  4; d = d >> 4;}
						if ((d & 0x00000003) == 0) {i_x +=  2; d = d >> 2;}
						if ((d & 0x00000001) == 0) {i_x +=  1;}
					}
					else
					{
						i_x = 32;
					}
#endif
					ASSERT((u32)(i_x) > (n_y-1));

					i_node->set_x(i_x);
					u32 const n_dir = ((n_key & (1<<i_x))!=0);
					i_node->set_node(  n_dir, n_node, n_type);
					i_node->set_node(1-n_dir, i_node, trienode::VALUE);

					trienode* p_node = n_node->get_parent();
					i_node->set_parent(p_node);
					n_node->set_parent(i_node);

					if (p_node == NULL)
					{
						mTable[map_index] = i_node;
					}
					else
					{
						u32 const p_dir = p_node->side_of(n_node);
						p_node->set_node(p_dir, i_node/*, trienode::NODE*/);
					}

					mNumValues++;
					return 1;
				}
			}

			u32 const n_cdir = ((i_key & n_bx)!=0);
			trienode* n_child = n_node->get_node(n_cdir, n_type);
			if (n_type == trienode::NODE)
			{
				ASSERT(n_child!=NULL && n_type==trienode::NODE);
				n_node = n_child;
			}
			else 
			{
				if (n_child == NULL)
				{
					n_node->set_node(n_cdir, i_node, trienode::VALUE);
				}
				else
				{
					u32 const c_key = (u32)(n_child);
					u32 const c_diff = i_key ^ c_key;

					// Count the leading zeros
					u32 i_x;
#ifdef TARGET_PC
					BitScanForward((DWORD*)&i_x, c_diff);
#else
					if (c_diff != 0)
					{
						s32 d = c_diff;
						i_x = 0;
						if ((d & 0x0000FFFF) == 0) {i_x += 16; d = d >>16;}
						if ((d & 0x000000FF) == 0) {i_x +=  8; d = d >> 8;}
						if ((d & 0x0000000F) == 0) {i_x +=  4; d = d >> 4;}
						if ((d & 0x00000003) == 0) {i_x +=  2; d = d >> 2;}
						if ((d & 0x00000001) == 0) {i_x +=  1;}
					}
					else
					{
						i_x = 32;
					}
#endif
					ASSERT((u32)(i_x) > (n_y-1));

					i_node->set_x(i_x);
					u32 const n_dir = ((c_key & (1<<i_x))!=0);
					i_node->set_node(  n_dir, n_child, trienode::VALUE);
					i_node->set_node(1-n_dir, i_node, trienode::VALUE);

					n_node->set_node(n_cdir, i_node/*, trienode::NODE*/);
					i_node->set_parent(n_node);
				}
				mNumValues++;
				return 1;
			}
			ASSERT(n_node == n_child);
		}
		return 0;
	}


	trienode*		triemap2::remove(u32 f_key)
	{
		u32 const table_idx = f_key & MAPSIZE_MASK;
		trienode* n_node = mTable[table_idx];
		if (n_node == NULL)
			return NULL;

		register u32 n_x  = (MAPSIZE_2LOG-1);
		register u32 n_bx = 1<<(MAPSIZE_2LOG-1);
		register u32 n_dir;

		trienode::EType c_type;
		trienode* c_node = NULL;
		while (true)
		{
			ASSERT(n_node->get_parent()==NULL || n_node->get_parent()->side_of(n_node)!=2);

			u32 const n_key = (u32)(n_node);
			u32 const n_y  = n_x;
			n_x = n_node->get_x();
			if (n_x == (n_y+1))
			{
				n_bx = n_bx << 1;
			}
			else
			{
				u32 const n_by = n_bx << 1;
				n_bx = 1 << n_x;
				u32 const diff = f_key ^ n_key;
				if ((diff&((n_bx-1) & (~(n_by-1))))!=0)
					return NULL;
			}

			n_dir = ((f_key&n_bx)!=0);
			c_node = n_node->get_node(n_dir, c_type);
			if (c_type == trienode::VALUE)
				break;

			ASSERT(c_node!=NULL);
			ASSERT(c_node->get_parent()->side_of(c_node, c_type)!=2);
			n_node = c_node;
		}

		if (c_node==NULL)
			return NULL;

		u32 const n_key = (u32)(c_node);
		if (f_key == n_key)
		{
			trienode::EType o_type;
			trienode* o_node = n_node->get_node(1-n_dir, o_type);
			trienode* n_parent = n_node->get_parent();
			if (n_parent != NULL)
			{
				s32 const s_node = n_parent->side_of(n_node);
				n_parent->set_node(s_node, o_node, o_type);
				if (o_node != NULL && o_type == trienode::NODE)
					o_node->set_parent(n_parent);
			}
			else 
			{
				if (o_type == trienode::NODE)
				{
					mTable[table_idx] = o_node;
					o_node->set_parent(NULL);
				} 
				else if (o_node == NULL)
				{
					mTable[table_idx] = NULL;
				}
				else
				{
					const u32 o_key = (u32)(o_node);
					o_node->set_x(31);
					u32 const o_dir = ((o_key & (u32)(1<<31))==0) ? 0 : 1;
					o_node->set_node(  o_dir, o_node, trienode::VALUE);
					o_node->set_node(1-o_dir,   NULL, trienode::VALUE);
					mTable[table_idx] = o_node;
				}
			}

			// Now we need to see if we have to replace the node
			// associated with our value.
			if (n_node != c_node)
			{
				if (c_node->is_linked())
				{
					c_node->replace(n_node);

					if (n_node->get_parent()==NULL)
						mTable[table_idx] = n_node;
				}
			}
		}
		return c_node;
	}
}


using namespace xcore;

static trienode gValues2[1000000];

UNITTEST_SUITE_BEGIN(xtriemap)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{

		}

		UNITTEST_FIXTURE_TEARDOWN()
		{

		}

		UNITTEST_TEST(trienode1)
		{
			trienode n;

			for (u32 i=0; i<32; ++i)
			{
				n.set_x(i);
				CHECK_EQUAL(i, n.get_x());

				trienode left, right;
				n.set_node(0, &left);
				n.set_node(1, &right);

				CHECK_EQUAL(i, n.get_x());

				n.set_x(i);
				CHECK_EQUAL(i, n.get_x());

				CHECK_TRUE(n.get_node(0) == &left);
				CHECK_TRUE(n.get_node(1) == &right);
			}
		}

		UNITTEST_TEST(constructor1)
		{
			CStopwatch2 sw;

			s32 N = 990000;

			triemap2 m1(gTestAllocator);
			//triemap2 m2(gTestAllocator);

			double insertTimeMs;
			double findTimeMs;
			double removeTimeMs;
			{

				sw.Start();
				for (s32 i=0; i<N; ++i)
				{	
					//m.insert(i*4, &gValues[i]);
					//CHECK_EQUAL(1, m.insert((u32)&gValues2[i], &gValues2[i]));		/// Lowest bit of value should not be used
					m1.insert((u32)&gValues2[i], &gValues2[i]);
				}
				sw.Stop();
				insertTimeMs = sw.ElapsedTimeMilliSec();

				sw.Start();
				trienode* value;
				for (s32 i=0; i<N; ++i)
				{	
					value = m1.find((u32)&gValues2[i]);
					//CHECK_TRUE(NULL != value);
					CHECK_TRUE(&gValues2[i] == value);
				}
				sw.Stop();
				findTimeMs = sw.ElapsedTimeMilliSec();

				sw.Start();
				for (s32 i=0; i<N; ++i)
				{	
					trienode* value = m1.remove((u32)&gValues2[i]);
					CHECK_TRUE(&gValues2[i] == value);
				}
				sw.Stop();
				removeTimeMs = sw.ElapsedTimeMilliSec();
			}

			x_printf("triemap 2, insert time: %f\n", insertTimeMs);
			x_printf("triemap 2, find   time: %f\n", findTimeMs);
			x_printf("triemap 2, remove time: %f\n", removeTimeMs);

#if 1
			typedef MyCustomAlloc< std::pair<const int, int> > MyAllocator;
			typedef std::map<int, int, std::less<int>, MyAllocator> MyMap;

			xcore::xbyte* MyMemoryPool = (xcore::xbyte*)gTestAllocator->allocate((N+100) * 64, 32);
			x_memset(MyMemoryPool, 0, (N+100) * 64);

			double stdMapInsertTimeMs;
			double stdMapFindTimeMs;
			double stdMapRemoveTimeMs;
			{
				MyAllocator z = MyAllocator(MyMemoryPool, (N+100) * 64);
				MyMap stdmap(std::less<int>(), z);
				sw.Start();
				for (s32 i=0; i<N; ++i)
				{	
					stdmap[(int)&gValues2[i]] = i;
				}
				sw.Stop();
				stdMapInsertTimeMs = sw.ElapsedTimeMilliSec();

				sw.Start();
				for (s32 i=0; i<N; ++i)
				{	
					MyMap::iterator iter = stdmap.find((int)&gValues2[i]);
					int value = (*iter).second;
					CHECK_EQUAL(i, value);
				}
				sw.Stop();
				stdMapFindTimeMs = sw.ElapsedTimeMilliSec();

				sw.Start();
				for (s32 i=0; i<N; ++i)
				{	
					MyMap::iterator iter = stdmap.find((int)&gValues2[i]);
					int value = (*iter).second;
					CHECK_EQUAL(i, value);
					stdmap.erase(iter);
				}
				sw.Stop();
				stdMapRemoveTimeMs = sw.ElapsedTimeMilliSec();
			}
			gTestAllocator->deallocate(MyMemoryPool);

			x_printf("stdmap, insert time: %f\n", stdMapInsertTimeMs);
			x_printf("stdmap, find   time: %f\n", stdMapFindTimeMs);
			x_printf("stdmap, remove time: %f\n", stdMapRemoveTimeMs);
#endif
		}


	}
}
UNITTEST_SUITE_END

#else

#include "xbase\x_target.h"
#include "xbase\x_allocator.h"
#include "xbase\x_debug.h"
#include "xbase\x_integer.h"
#include "xcore\x_string.h"

#include "xunittest\xunittest.h"

UNITTEST_SUITE_BEGIN(xtriemap)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{

		}

		UNITTEST_FIXTURE_TEARDOWN()
		{

		}
	}
}
UNITTEST_SUITE_END

#endif