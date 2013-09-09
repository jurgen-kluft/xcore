#include "xcore\x_ilist.h"
#include "xbase\x_idx_allocator.h"

#include "xunittest\xunittest.h"

using namespace xcore;

extern xcore::x_iallocator* gTestAllocator;

UNITTEST_SUITE_BEGIN(xilist)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{
			
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
		}

		struct myobject : public xdlistnode
		{
			u32			mIndex;
		};


		UNITTEST_TEST(constructor1)
		{
			xilist list;
			CHECK_TRUE(list.empty());
			CHECK_EQUAL(0, list.size());

			myobject objects[10];
			for (s32 i=0; i<10; ++i)
			{
				CHECK_FALSE(objects[i].is_used());
			}

			for (s32 i=0; i<10; ++i)
			{
				objects[i].mIndex = i;
				list.push_back(&objects[i]);
			}
			
			CHECK_FALSE(list.empty());
			CHECK_EQUAL(10, list.size());

			for (s32 i=0; i<10; ++i)
			{
				CHECK_TRUE(objects[i].is_used());
			}

			s32 k=0;
			xdlistnode* node = list.head();
			while (node != &list)
			{
				myobject* o = list.cast<myobject>(node);
				CHECK_EQUAL(k, o->mIndex);
				node = node->next();
				++k;
			}
			CHECK_EQUAL(10, k);

			k=10;
			node = list.tail();
			while (node != &list)
			{
				myobject* o = list.cast<myobject>(node);
				--k;
				CHECK_EQUAL(k, o->mIndex);
				node = node->prev();
			}
			CHECK_EQUAL(0, k);

			for (s32 i=0; i<10; ++i)
			{
				list.pop_back();
			}

			CHECK_TRUE(list.empty());
			CHECK_EQUAL(0, list.size());

		}

		// This object can be in 2 lists at the same time
		struct myobject2 : public xdlistnodes2
		{
			u32			mIndex;
		};


		UNITTEST_TEST(constructor2)
		{
			xilist list(0);
			CHECK_TRUE(list.empty());
			CHECK_EQUAL(0, list.size());

			myobject2 objects[10];
			for (s32 i=0; i<10; ++i)
			{
				CHECK_FALSE(objects[i].is_used(list.index()));
			}

			for (s32 i=0; i<10; ++i)
			{
				objects[i].mIndex = i;
				list.push_back(&objects[i]);
			}

			xilist list1(1);
			for (s32 i=0; i<10; ++i)
			{
				objects[i].mIndex = i;
				list1.push_back(&objects[i]);
			}

			CHECK_FALSE(list.empty());
			CHECK_EQUAL(10, list.size());

			for (s32 i=0; i<10; ++i)
			{
				CHECK_TRUE(objects[i].is_used(list.index()));
			}

			s32 k=0;
			xdlistnode* node = list.head();
			while (node != &list)
			{
				myobject2* o = list.cast<myobject2>(node);
				CHECK_EQUAL(k, o->mIndex);
				node = node->next();
				++k;
			}
			CHECK_EQUAL(10, k);

			for (s32 i=0; i<10; ++i)
			{
				list.pop_back();
			}

			CHECK_TRUE(list.empty());
			CHECK_EQUAL(0, list.size());

		}
	}
}
UNITTEST_SUITE_END
