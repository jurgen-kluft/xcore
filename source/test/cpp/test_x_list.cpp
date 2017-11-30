#include "xbase/x_debug.h"
#include "xcore/x_vector.h"
#include "xcore/x_list.h"
#include "xbase/x_idx_allocator.h"

#include "xunittest\xunittest.h"

using namespace xcore;

extern xcore::x_iallocator* gTestAllocator;

UNITTEST_SUITE_BEGIN(xlist)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{
			
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}

		struct mytobject
		{
			xdlistnode	mNode;
			u32			mIndex;
		};

		UNITTEST_TEST(tconstructor1)
		{
			typedef xlist<s32>	mylist;
			xlist_heap_strategy<s32> p(gTestAllocator);
			mylist	l(p);

			l.push_back(0);
			l.push_back(1);

			mylist::iterator iter = l.begin();
			mylist::iterator end = l.end();
			while (iter != end)
			{
				s32 i = *iter;
				++iter;
			}
		}

		struct mytobject2 : public xdlistnode
		{
						mytobject2(u32 i) : mIndex(i) {}
			u32			mIndex;
		};

		UNITTEST_TEST(tconstructor2)
		{
			xlist_derive_strategy<mytobject2*> p;
			typedef xlist<mytobject2*, xlist_derive_strategy<mytobject2*> >	mylist;
			mylist	l(p);

			mytobject2 a(1);
			mytobject2 b(2);
			mytobject2 c(3);
			mytobject2 d(4);

			l.push_back(&a);
			l.push_back(&b);
			l.push_back(&c);
			l.push_back(&d);

			mylist::iterator found = l.find(&c);

			mylist::iterator iter = l.begin();
			mylist::iterator end = l.end();
			while (iter != end)
			{
				mytobject2* i = *iter;
				++iter;
			}
		}

		UNITTEST_TEST(xlist_iterator)
		{
			typedef xlist<s32>	mylist;
			xlist_heap_strategy<s32> p(gTestAllocator);
			mylist	l(p);
			mylist::iterator it =l.begin();
			it++;
			++it;
			it+1;
			l.push_back(0);
			l.push_back(1);
			l.push_back(2);
			mylist::iterator it_end = l.end();
			--it_end;
			CHECK_EQUAL( 2, *it_end);
			it = l.begin() + 2;
			CHECK_TRUE(*it == 2);
			CHECK_EQUAL(it_end == it, true);
			CHECK_EQUAL(it_end < it, false);
			++it_end;
			CHECK_TRUE(it_end == l.end());
			CHECK_EQUAL(it_end < l.end(), false);
			CHECK_EQUAL(l.end() < it_end,false);
			CHECK_EQUAL(it_end <= l.end(), true);
			CHECK_EQUAL(l.end() <= it_end,true);
			++it_end;
			it = l.begin();
			CHECK_TRUE(it_end != it);
			CHECK_EQUAL(it > it_end, false);
			CHECK_EQUAL(it < it_end, true);
			CHECK_EQUAL(it >= it_end, false);
			CHECK_EQUAL(it <= it_end, true);
			CHECK_EQUAL(l.end() == it_end, true);
			
			it_end += 77;
			CHECK_TRUE(it_end == l.end());

			mylist m(p);
			m.push_back(0);
			m.push_back(1);
			m.push_back(2);
			mylist::iterator mit = m.begin();
			CHECK_EQUAL(mit == it, false);
		}

	}
}
UNITTEST_SUITE_END
