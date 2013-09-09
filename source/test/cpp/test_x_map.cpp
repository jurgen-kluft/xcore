#include "xcore\x_map.h"
#include "xbase\x_allocator.h"

#include "xunittest\xunittest.h"

using namespace xcore;

extern xcore::x_iallocator* gTestAllocator;


UNITTEST_SUITE_BEGIN(xmap)
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
			xmap_heap_strategy<s32,s32> P(a);
			xmap<s32,s32> test(P);

			test.insert(0,  9);
			test.insert(1, 19);
			test.insert(2, 29);
			xmap<s32,s32>::iterator it1 = test.find(0);

			const xmap<s32,s32>& test2 = test;
			xmap<s32,s32>::const_iterator it2 = test2.find(0);

			xmap<s32,s32> testcp(test);
			testcp = test;

			CHECK_EQUAL(3, test.size());
			CHECK_EQUAL(false, test.empty());

			test.remove(0);
			test.clear();
		}

		struct myobject
		{
						myobject(u32 _key) : key(_key) { value = this; }

			u32			key;
			myobject*	value;
			xmapnode	node;
		};

		UNITTEST_TEST(constructor2)
		{
			x_iallocator* a = gTestAllocator;
			xmap_member_strategy<s32, myobject*> P(a, X_OFFSET_OF(myobject, key), X_OFFSET_OF(myobject, value), X_OFFSET_OF(myobject, node));
			typedef		xmap<s32, myobject*, xstd::xcompare<s32>, xmap_member_strategy<s32, myobject*> >		mymap;
			mymap test(P);

			myobject mya(0);
			myobject myb(1);
			myobject myc(2);

			test.insert(mya.key, &mya);
			test.insert(myb.key, &myb);
			test.insert(myc.key, &myc);

			mymap::iterator it1 = test.find(mya.key);
			CHECK_TRUE(*it1 == &mya);

			const mymap& test2 = test;
			mymap::const_iterator it2 = test2.find(myb.key);
			CHECK_TRUE(it2.value() == &myb);

			CHECK_EQUAL(3, test.size());
			CHECK_EQUAL(false, test.empty());

			test.remove(0);
			test.clear();
		}


		UNITTEST_TEST(insert)
		{
			xcore::f32 float_list[] =
			{
				0.20418437f,0.89940142f,0.35222807f,0.15452193f,0.39873965f,0.57665325f,0.25892441f,0.08795529f,0.02391726f,0.11735384f,
				0.94704581f,0.89539275f,0.97839662f,0.43642375f,0.86605077f,0.56392381f,0.83561822f,0.34804288f,0.51057740f,0.26513622f,
				0.45898144f,0.91629822f,0.75528490f,0.50092159f,0.04428102f,0.90834075f,0.94564757f,0.78467721f,0.29248220f,0.60437235f,
				0.49094059f,0.78501393f,0.40469330f,0.98927504f,0.45939496f,0.24333626f,0.74765115f,0.01162746f,0.84472137f,0.60756670f,
				0.77416885f,0.89363849f,0.69293767f,0.41257972f,0.16360881f,0.25410919f,0.64540821f,0.26084541f,0.64276814f,0.10367363f,
			};

			x_iallocator* a = gTestAllocator;
			xmap_heap_strategy<f32, s32> P(a);
			typedef	xmap<f32, s32>	mymap;
			mymap test(P);

			for (s32 i=0; i<50; ++i)
				CHECK_EQUAL(xTRUE, test.insert(float_list[i], i));
			for (s32 i=0; i<50; ++i)
				CHECK_EQUAL(xFALSE, test.insert(float_list[i], i));

			CHECK_EQUAL(false, test.empty());
			CHECK_EQUAL(50, test.size());
		}

		UNITTEST_TEST(remove)
		{
			x_iallocator* a = gTestAllocator;
			xmap_heap_strategy<s32, s32> P(a);
			typedef	xmap<s32, s32>	mymap;
			mymap test(P);

			for (s32 i=0; i<50; ++i)
				CHECK_EQUAL(xTRUE, test.insert(i, i) );

			mymap::iterator it = test.begin();
			CHECK_EQUAL(0, *it);
			it = test.remove(it);
			CHECK_EQUAL(1, *it);

			++it;
			for (s32 i=0; i<10; ++i)
			{
				it = test.remove(it);
				CHECK_EQUAL(3 + i, *it);
			}

			it = test.end();
			--it;
			it = test.remove(it);
			CHECK_EQUAL(test.end() == it, xTRUE);
		}

		UNITTEST_TEST(insert_remove)
		{
			xcore::f32 float_list[] =
			{
				0.20418437f,0.89940142f,0.35222807f,0.15452193f,0.39873965f,0.57665325f,0.25892441f,0.08795529f,0.02391726f,0.11735384f,
				0.94704581f,0.89539275f,0.97839662f,0.43642375f,0.86605077f,0.56392381f,0.83561822f,0.34804288f,0.51057740f,0.26513622f,
				0.45898144f,0.91629822f,0.75528490f,0.50092159f,0.04428102f,0.90834075f,0.94564757f,0.78467721f,0.29248220f,0.60437235f,
				0.49094059f,0.78501393f,0.40469330f,0.98927504f,0.45939496f,0.24333626f,0.74765115f,0.01162746f,0.84472137f,0.60756670f,
				0.77416885f,0.89363849f,0.69293767f,0.41257972f,0.16360881f,0.25410919f,0.64540821f,0.26084541f,0.64276814f,0.10367363f,
			};

			x_iallocator* a = gTestAllocator;
			xmap_heap_strategy<f32, s32> P(a);
			typedef	xmap<f32, s32>	mymap;
			mymap test(P);

			for (s32 i=0; i<50; ++i)
				test.insert(float_list[i], i);

			CHECK_EQUAL(false, test.empty());
			CHECK_EQUAL(50, test.size());

			for (s32 i=0; i<50; ++i)
				CHECK_EQUAL(xTRUE, test.remove(float_list[i]));
			for (s32 i=0; i<50; ++i)
				CHECK_EQUAL(xFALSE, test.remove(float_list[i]));

			CHECK_EQUAL(true, test.empty());
			CHECK_EQUAL(0, test.size());

		}

		UNITTEST_TEST(operator_square)
		{
			xcore::f32 float_list[] =
			{
				0.20418437f,0.89940142f,0.35222807f,0.15452193f,0.39873965f,0.57665325f,0.25892441f,0.08795529f,0.02391726f,0.11735384f,
				0.94704581f,0.89539275f,0.97839662f,0.43642375f,0.86605077f,0.56392381f,0.83561822f,0.34804288f,0.51057740f,0.26513622f,
				0.45898144f,0.91629822f,0.75528490f,0.50092159f,0.04428102f,0.90834075f,0.94564757f,0.78467721f,0.29248220f,0.60437235f,
				0.49094059f,0.78501393f,0.40469330f,0.98927504f,0.45939496f,0.24333626f,0.74765115f,0.01162746f,0.84472137f,0.60756670f,
				0.77416885f,0.89363849f,0.69293767f,0.41257972f,0.16360881f,0.25410919f,0.64540821f,0.26084541f,0.64276814f,0.10367363f,
			};

			x_iallocator* a = gTestAllocator;
			xmap_heap_strategy<f32, s32> P(a);
			typedef	xmap<f32, s32>	mymap;
			mymap test(P);

			for (s32 i=0; i<50; ++i)
				test[float_list[i]] = i;

			CHECK_EQUAL(false, test.empty());
			CHECK_EQUAL(50, test.size());

			for (s32 i=0; i<50; ++i)
				CHECK_EQUAL(i, test[float_list[i]]);
		}

		UNITTEST_TEST(iteration_inorder)
		{
			xcore::f32 float_list[] =
			{
				0.20418437f,0.89940142f,0.35222807f,0.15452193f,0.39873965f,0.57665325f,0.25892441f,0.08795529f,0.02391726f,0.11735384f,
				0.94704581f,0.89539275f,0.97839662f,0.43642375f,0.86605077f,0.56392381f,0.83561822f,0.34804288f,0.51057740f,0.26513622f,
				0.45898144f,0.91629822f,0.75528490f,0.50092159f,0.04428102f,0.90834075f,0.94564757f,0.78467721f,0.29248220f,0.60437235f,
				0.49094059f,0.78501393f,0.40469330f,0.98927504f,0.45939496f,0.24333626f,0.74765115f,0.01162746f,0.84472137f,0.60756670f,
				0.77416885f,0.89363849f,0.69293767f,0.41257972f,0.16360881f,0.25410919f,0.64540821f,0.26084541f,0.64276814f,0.10367363f,
			};

			x_iallocator* a = gTestAllocator;
			xmap_heap_strategy<f32, s32> P(a);
			typedef	xmap<f32, s32>	mymap;
			mymap test(P);

			for (s32 i=0; i<50; ++i)
				test.insert(float_list[i], i);

			CHECK_EQUAL(false, test.empty());
			CHECK_EQUAL(50, test.size());

			// The default iterator is in-order, so we should be able to iterate over the floats in sorted order
			f32 fmin = 0.0f;
			mymap::iterator it1 = test.imin();
			while (it1 != test.end())
			{
				// Find the next smallest float in the float list
				f32 nfmin = 1.0f;
				for (s32 i=0; i<50; ++i)
				{
					if (float_list[i] > fmin && float_list[i]<nfmin)
						nfmin = float_list[i];
				}
				fmin = nfmin;

				CHECK_EQUAL(fmin, it1.key());
				++it1;
			}
		}

		UNITTEST_TEST(const_iteration_inorder)
		{
			xcore::f32 float_list[] =
			{
				0.20418437f,0.89940142f,0.35222807f,0.15452193f,0.39873965f,0.57665325f,0.25892441f,0.08795529f,0.02391726f,0.11735384f,
				0.94704581f,0.89539275f,0.97839662f,0.43642375f,0.86605077f,0.56392381f,0.83561822f,0.34804288f,0.51057740f,0.26513622f,
				0.45898144f,0.91629822f,0.75528490f,0.50092159f,0.04428102f,0.90834075f,0.94564757f,0.78467721f,0.29248220f,0.60437235f,
				0.49094059f,0.78501393f,0.40469330f,0.98927504f,0.45939496f,0.24333626f,0.74765115f,0.01162746f,0.84472137f,0.60756670f,
				0.77416885f,0.89363849f,0.69293767f,0.41257972f,0.16360881f,0.25410919f,0.64540821f,0.26084541f,0.64276814f,0.10367363f,
			};

			x_iallocator* a = gTestAllocator;
			xmap_heap_strategy<f32, s32> P(a);
			typedef	xmap<f32, s32>	mymap;
			mymap test(P);

			for (s32 i=0; i<50; ++i)
				test.insert(float_list[i], i);

			mymap const& test_const = test;

			CHECK_EQUAL(false, test_const.empty());
			CHECK_EQUAL(50, test_const.size());

			// The default iterator is in-order, so we should be able to iterate over the floats in sorted order
			f32 fmin = 0.0f;
			mymap::const_iterator it1 = test_const.imin();
			while (it1 != test_const.end())
			{
				// Find the next smallest float in the float list
				f32 nfmin = 1.0f;
				for (s32 i=0; i<50; ++i)
				{
					if (float_list[i] > fmin && float_list[i]<nfmin)
						nfmin = float_list[i];
				}
				fmin = nfmin;

				CHECK_EQUAL(fmin, it1.key());
				++it1;
			}
		}
	}
}
UNITTEST_SUITE_END
