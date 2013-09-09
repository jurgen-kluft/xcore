#include "xbase\x_queue.h"

#include "xunittest\xunittest.h"

using namespace xcore;

extern xcore::x_iallocator* gTestAllocator;

UNITTEST_SUITE_BEGIN(xqueue)
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
			xqueue_heap_strategy<s32> P(a);
			xqueue<s32> myqueue(P);

			myqueue.enqueue(0);
			myqueue.enqueue(1);
			myqueue.enqueue(2);

			CHECK_FALSE(myqueue.empty());
			CHECK_EQUAL(3, myqueue.size());

			CHECK_EQUAL(0, myqueue.front());
			CHECK_EQUAL(2, myqueue.back());
			myqueue.dequeue();
			CHECK_EQUAL(1, myqueue.front());
			CHECK_EQUAL(2, myqueue.back());
			myqueue.dequeue();
			CHECK_EQUAL(2, myqueue.front());
			CHECK_EQUAL(2, myqueue.back());
			myqueue.dequeue();

			CHECK_TRUE(myqueue.empty());
			CHECK_EQUAL(0, myqueue.size());

			myqueue.enqueue(0);
			myqueue.enqueue(1);
			myqueue.enqueue(2);

			myqueue.clear();
			CHECK_TRUE(myqueue.empty());
			CHECK_EQUAL(0, myqueue.size());

			myqueue.enqueue(2);
		}

		
	}
}
UNITTEST_SUITE_END
