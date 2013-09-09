#include "xcore\x_stack.h"
#include "xcore\x_slist.h"
#include "xbase\x_idx_allocator.h"

#include "xunittest\xunittest.h"

using namespace xcore;

extern xcore::x_iallocator* gTestAllocator;

UNITTEST_SUITE_BEGIN(xstack)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{
			
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}

		UNITTEST_TEST(constructor1)
		{
			x_iallocator* a = gTestAllocator;
			xstack_heap_strategy<s32> P(a);
			xstack<s32> mystack(P);

			mystack.push(0);
			mystack.push(1);
			mystack.push(2);

			CHECK_FALSE(mystack.empty());
			CHECK_EQUAL(3, mystack.size());

			CHECK_EQUAL(2, mystack.peek());
			mystack.pop();
			CHECK_EQUAL(1, mystack.peek());
			mystack.pop();
			CHECK_EQUAL(0, mystack.peek());
			mystack.pop();
			CHECK_TRUE(mystack.empty());
			CHECK_EQUAL(0, mystack.size());

			mystack.push(0);
			mystack.push(1);
			mystack.push(2);
			
			mystack.clear();
			CHECK_TRUE(mystack.empty());
			CHECK_EQUAL(0, mystack.size());
		}


		UNITTEST_TEST(constructor2)
		{
			
		}
	}
}
UNITTEST_SUITE_END
