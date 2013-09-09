#include "xcore\x_set.h"
#include "xbase\x_allocator.h"

#include "xunittest\xunittest.h"

using namespace xcore;

extern xcore::x_iallocator* gTestAllocator;


UNITTEST_SUITE_BEGIN(xset)
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
			xset_heap_strategy<s32> P(a);
			xset<s32> test(P);
			xset<s32> test3(P);

			xset<s32>::iterator isDone0 = test.insert(0);
			s32 i0 = *isDone0;
			xset<s32>::iterator isDone1 = test.insert(1);
			s32 i1 = *isDone1;
			xset<s32>::iterator isDone2 = test.insert(2);
			s32 i2 = *isDone2;
			test.insert(3);
			test.insert(4);
			test.insert(5);
			xset<s32>::iterator isDone00 = test3.insert(10);
			s32 i00 = *isDone00;
			xset<s32>::iterator isDone01 = test3.insert(11);
			s32 i01 = *isDone01;
			xset<s32>::iterator isDone02 = test3.insert(12);
			s32 i02 = *isDone02;

			xset<s32> const & test_const = test;
			xset<s32>::const_iterator ci = test_const.begin();
			++ci;

			test3.insert(test.begin(),test.end());
			//CHECK_EQUAL(5,test3.size());

			//test3.remove((test3.begin()++),(test3.end()--));

			test.remove(isDone0,isDone2);
			xset<s32>::iterator it = test.begin();
			while(it!=test.end())
			{
				s32 temp = *it;
				if (temp>0)
				{
					int i = 0;
				}
				it++;
			}
			xset<s32>::iterator isDone3 = test.insert(1);
			s32 i4 = *isDone3;
			CHECK_FALSE(isDone3==test.end());

			xset<s32>::iterator it1 = test.find(20);
			CHECK_TRUE(it1==test.end());
			xset<s32>::iterator it3 = test.find(0);

			const xset<s32>& test2 = test;
			xset<s32>::const_iterator it2 = test2.find(0);

			CHECK_EQUAL(5, test.size());
			CHECK_EQUAL(false, test.empty());

			test.remove(0);
			test.clear();
		}

		struct myobject
		{
						myobject(u32 _key) : key(_key) { value = this; }

			u32			key;
			myobject*	value;
			xsetnode	node;
		};

		struct xcompare_myobject
		{
			inline s32 operator()(myobject* const& a, myobject* const& b) const
			{
				if (a->key < b->key) return -1;
				else if (a->key > b->key) return 1;
				else return 0;
			}
		};

		UNITTEST_TEST(constructor2)
		{
			xset_member_strategy<myobject*> P(X_OFFSET_OF(myobject, value), X_OFFSET_OF(myobject, node));
			typedef		xset<myobject*, xcompare_myobject, xset_member_strategy<myobject*> >		myset;
			//x_iallocator* a = gTestAllocator;
			//xset_heap_strategy<myobject*> P(a);
			//typedef xset<myobject*> myset;
			myset test(P);
			myset test2(P);

			myobject mya(0);
			myobject myb(1);
			myobject myc(2);

			myobject myd(3);
			myobject mye(4);
			myobject myf(5);
			myobject myg(6);
			myobject myh(7);
			myobject myi(8);

			myobject mya2(20);
			myobject myb2(21);
			myobject myc2(22);
			myobject myd2(23);

			myset::iterator isDoneA = test.insert(&mya);
			myobject* _mya = *isDoneA;
			myset::iterator isDoneB = test.insert(&myb);
			myobject* _myb = *isDoneB;
			myset::iterator isDoneC = test.insert(&myc);
			myobject* _myc = *isDoneC;
			myset::iterator isDoneD = test.insert(&myd);
			myset::iterator isDoneE = test.insert(&mye);
			myset::iterator isDoneF = test.insert(&myf);
			myset::iterator isDoneG = test.insert(&myg);
			myset::iterator isDoneH = test.insert(&myh);
			myset::iterator isDoneI = test.insert(&myi);

			myset::iterator isDoneA2 = test2.insert(&mya2);
			myobject* _mya2 = *isDoneA2;
			
			myset::iterator isDoneB2 = test2.insert(&myb2);
			myobject* _myb2 = *isDoneB2;
		
			//myset::iterator isDoneC2 = test2.insert(&myc2);
			//myobject* _myc2 = *isDoneC2;
			
			isDoneA2++;
			_myb2 = *isDoneA2;
			//isDoneB2++;
			//_myc2 = *isDoneB2;
			myset::iterator iiii = test2.begin();
			//myset::iterator isDoneC2 = test.insert(*iiii);

			myset::iterator it = test.begin();
			while (it != test.end())
			{
				myobject* tmo = *it;
				if (tmo->key>=0)
				{
					int i = 0 ;
				}
				it++;
			}

			myset::iterator it1 = test.find(&mya);
			myobject* p = *it1;
			CHECK_TRUE(p == &mya);

			const myset& test3 = test;
			myset::const_iterator it2 = test3.find(&myb);
			CHECK_TRUE(it2.value() == &myb);

			CHECK_EQUAL(9, test.size());
			CHECK_EQUAL(false, test.empty());

			test.remove(&mya);
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
			xset_heap_strategy<f32> P(a);
			typedef	xset<f32>	myset;
			myset test(P);

			for (s32 i=0; i<50; ++i)
			{
				test.insert(float_list[i]);
				myset::iterator it = test.insert(float_list[i]);
				CHECK_TRUE(it==test.end());
			}
			CHECK_EQUAL(false, test.empty());
			CHECK_EQUAL(50, test.size());
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
			xset_heap_strategy<f32> P(a);
			typedef	xset<f32>	myset;
			myset test(P);

			for (s32 i=0; i<50; ++i)
				test.insert(float_list[i]);

			CHECK_EQUAL(false, test.empty());
			CHECK_EQUAL(50, test.size());

			for (s32 i=0; i<50; ++i)
			{
				xbool removeState = test.remove(float_list[i]);
				CHECK_TRUE(removeState);
				xbool removeState2 = test.remove(-float_list[i]);
				CHECK_FALSE(removeState2);
			}

			CHECK_EQUAL(true, test.empty());
			CHECK_EQUAL(0, test.size());

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
			xset_heap_strategy<f32> P(a);
			typedef	xset<f32>	myset;
			myset test(P);

			for (s32 i=0; i<50; ++i)
				test.insert(float_list[i]);

			CHECK_EQUAL(false, test.empty());
			CHECK_EQUAL(50, test.size());

			// The default iterator is in-order, so we should be able to iterate over the floats in sorted order
			f32 fmin = 0.0f;
			myset::iterator it1 = test.imin();
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

				CHECK_EQUAL(fmin, it1.value());
				++it1;
			}
		}
	}
}
UNITTEST_SUITE_END
