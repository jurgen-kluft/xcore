#include "xbase\x_base.h"
#include "xbase\x_allocator.h"
#include "xbase\x_string.h"

#include "xunittest\xunittest.h"

#include "lib_text_bank2.h"

#ifdef TARGET_360
#include <xtl.h>
#endif

UNITTEST_SUITE_LIST(xCoreUnitTest);




// SPU-only test list

#ifdef SPU
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xallocator);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xbinary_search);
//UNITTEST_SUITE_DECLARE(xCoreUnitTest, xlist);
//UNITTEST_SUITE_DECLARE(xCoreUnitTest, xhashmap);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xqueue);
//UNITTEST_SUITE_DECLARE(xCoreUnitTest, xmap);
//UNITTEST_SUITE_DECLARE(xCoreUnitTest, xpqueue);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstack);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xbitfield);
//UNITTEST_SUITE_DECLARE(xCoreUnitTest, xcstring);
//UNITTEST_SUITE_DECLARE(xCoreUnitTest, xccstring);
//UNITTEST_SUITE_DECLARE(xCoreUnitTest, xdouble);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xendian);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xfloat);
//UNITTEST_SUITE_DECLARE(xCoreUnitTest, xguid);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xinteger);
//UNITTEST_SUITE_DECLARE(xCoreUnitTest, xilist);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xmemory_std);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xqsort);
//UNITTEST_SUITE_DECLARE(xCoreUnitTest, xset);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xsingleton);
//UNITTEST_SUITE_DECLARE(xCoreUnitTest, xsprintf);
//UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstring);
//UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstring_std);
//UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstring_tmp);
//UNITTEST_SUITE_DECLARE(xCoreUnitTest, xtriemap);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xvector);
#endif





#ifndef SPU
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xallocator);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xbinary_search);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xlist);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xhashmap);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xqueue);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xmap);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xpqueue);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstack);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xbitfield);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xcstring);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xccstring);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xdouble);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xendian);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xfloat);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xguid);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xinteger);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xilist);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xmemory_std);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xqsort);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xrbtree15);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xset);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xsingleton);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xsprintf);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstring);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstring_std);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstring_tmp);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xtree);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xtriemap);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xvector);
#endif

#ifndef X_NO_CUSTOM_INT64
UNITTEST_SUITE_DECLARE(xCoreUnitTest, __xint64);
#endif // X_NO_CUSTOM_INT64

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

		virtual const char*	name() const										{ return "xbase unittest test heap allocator"; }

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

#ifdef SPU
	xcore::s32 progSize;
	xcore::s32 stackSize;

	::getProgramAndStackSizeForSPU(&progSize, &stackSize);

	xcore::gSetSPUConfig(progSize, stackSize);
#endif

	xcore::x_iallocator* systemAllocator = xcore::gCreateSystemAllocator();
	xcore::UnitTestAllocator unittestAllocator( systemAllocator );
	UnitTest::SetAllocator(&unittestAllocator);

#if defined(TARGET_360) || defined(TARGET_PC)

	NGameText::CDataBank db;
	NGameText::CCharString filename("D:\\LHTS\\Japanese.iple");
	db.OpenReadOnly(filename);

	NGameText::CWideString text1 = db.GetTextOfTextEntry(db.FindIndexBySymbol(NGameText::CCharString("TEXT_GUI_NEXT")));

#endif

	xcore::xconsole::addDefault();

	xcore::TestAllocator testAllocator(systemAllocator);
	gTestAllocator = &testAllocator;

	xcore::x_Init(gTestAllocator);
	int r = UNITTEST_SUITE_RUN(reporter, xCoreUnitTest);
	xcore::x_Exit();

	gTestAllocator->release();

#ifndef SPU
	xcore::xstring::sSetAllocator(NULL);
	xcore::xstring_tmp::sSetAllocator(NULL);
#endif

	UnitTest::SetAllocator(NULL);

	return r==0;
}

