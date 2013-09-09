#include "xbase\x_base.h"
#include "xbase\x_allocator.h"
#include "xbase\x_console.h"

#include "xcore\x_core.h"

#include "xunittest\xunittest.h"

UNITTEST_SUITE_LIST(xCoreUnitTest);

UNITTEST_SUITE_DECLARE(xCoreUnitTest, xlist);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xhashmap);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xqueue);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xmap);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xpqueue);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstack);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xcstring);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xccstring);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xilist);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xset);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstring);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstring_tmp);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xtriemap);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xvector);

namespace xcore
{
	class UnitTestAllocator : public UnitTest::Allocator
	{
		xcore::x_iallocator*	mAllocator;
	public:
						UnitTestAllocator(xcore::x_iallocator* allocator)	{ mAllocator = allocator; }
		virtual void*	Allocate(int size)									{ return mAllocator->allocate(size, 4); }
		virtual void	Deallocate(void* ptr)								{ mAllocator->deallocate(ptr); }
	};

	class TestAllocator : public x_iallocator
	{
		x_iallocator*		mAllocator;
	public:
							TestAllocator(x_iallocator* allocator) : mAllocator(allocator) { }

		virtual const char*	name() const										{ return "xcore unittest test heap allocator"; }

		virtual void*		allocate(u32 size, u32 alignment)
		{
			UnitTest::IncNumAllocations();
			return mAllocator->allocate(size, alignment);
		}

		virtual void*		reallocate(void* mem, u32 size, u32 alignment)
		{
			if (mem==NULL)
				return allocate(size, alignment);
			else 
				return mAllocator->reallocate(mem, size, alignment);
		}

		virtual void		deallocate(void* mem)
		{
			UnitTest::DecNumAllocations();
			mAllocator->deallocate(mem);
		}

		virtual void		release()
		{
			mAllocator->release();
			mAllocator = NULL;
		}
	};
}

xcore::x_iallocator* gTestAllocator = NULL;

bool gRunUnitTest(UnitTest::TestReporter& reporter)
{
	xcore::x_iallocator* systemAllocator = xcore::gCreateSystemAllocator();
	xcore::UnitTestAllocator unittestAllocator( systemAllocator );
	UnitTest::SetAllocator(&unittestAllocator);

	xcore::xconsole::addDefault();

	xcore::TestAllocator testAllocator(systemAllocator);
	gTestAllocator = &testAllocator;

	xcore::x_Init(gTestAllocator);
	int r = UNITTEST_SUITE_RUN(reporter, xCoreUnitTest);
	xcore::x_Exit();

	gTestAllocator->release();

	UnitTest::SetAllocator(NULL);

	return r==0;
}

