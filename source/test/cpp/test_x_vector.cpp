#include "xcore\x_vector.h"
#include "xcore\x_string.h"

#include "xunittest\xunittest.h"

using namespace xcore;

extern xcore::x_iallocator* gTestAllocator;

struct SquareCompare
{
	xbool operator () (const s32 a, const s32 b) const
	{
		return a * a < b * b;
	}
};

UNITTEST_SUITE_BEGIN(xvector)
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
			xvector<s32> s32Vector(gTestAllocator);

			// This one will fail, it will not push!
			CHECK_EQUAL(0, s32Vector.push_back(0));

			CHECK_TRUE(s32Vector.empty());
			CHECK_EQUAL(0, s32Vector.size());
		}

		UNITTEST_TEST(constructor2)
		{
			xvector<s32> s32Vector(gTestAllocator);

			// Reserve(1) will give us space to push one item
			s32Vector.reserve(1);
			CHECK_EQUAL(1, s32Vector.push_back(0));

			CHECK_FALSE(s32Vector.empty());
			CHECK_EQUAL(1, s32Vector.size());
		}

        UNITTEST_TEST(autoalloc)
        {
            xvector<s32> s32Vector(gTestAllocator);

            CHECK_EQUAL(0, s32Vector.push_back(0));
            s32Vector.set_autoalloc(true);
            CHECK_EQUAL(1, s32Vector.push_back(0));

            CHECK_FALSE(s32Vector.empty());
            CHECK_EQUAL(1, s32Vector.size());
            CHECK_EQUAL(2, s32Vector.capacity());

            xvector<s32> s(gTestAllocator);
            s.set_autoalloc();
            for (u32 i = 0; i < 100; ++i)
            {
                s.push_back(i);
            }

            for (u32 i = 0; i < s.size(); ++i)
            {
                CHECK_EQUAL(i, s[i]);
            }
        }

		UNITTEST_TEST(reserve1)
		{
			xvector<s32> s32Vector(gTestAllocator);

			s32Vector.reserve(100);
			CHECK_EQUAL(0, s32Vector.size());
			CHECK_EQUAL(100, s32Vector.capacity());
			s32Vector.resize(100);
			CHECK_EQUAL(100, s32Vector.size());
			CHECK_EQUAL(100, s32Vector.capacity());
		}

		UNITTEST_TEST(resize1)
		{
			xvector<s32> s32Vector(gTestAllocator);

			s32Vector.resize(100);
			CHECK_EQUAL(100, s32Vector.size());
			CHECK_EQUAL(100, s32Vector.capacity());
			s32Vector.resize(0);
			CHECK_EQUAL(0, s32Vector.size());
			CHECK_EQUAL(100, s32Vector.capacity());
		}

		UNITTEST_TEST(max_size1)
		{
			xvector<s32> s32Vector(gTestAllocator);

			s32Vector.resize(100);
			s32Vector.resize(50);
			CHECK_EQUAL(50, s32Vector.size());
			CHECK_EQUAL(100, s32Vector.max_size());
		}

		UNITTEST_TEST(empty1)
		{
			xvector<s32> s32Vector(gTestAllocator);
			CHECK_EQUAL(true, s32Vector.empty());

			s32Vector.resize(100);
			CHECK_EQUAL(false, s32Vector.empty());
			s32Vector.resize(50);
			CHECK_EQUAL(false, s32Vector.empty());
			s32Vector.resize(0);
			CHECK_EQUAL(true, s32Vector.empty());
		}

		UNITTEST_TEST(begin1)
		{
			xvector<s32> s32Vector(gTestAllocator);
			s32Vector.resize(100);
			for (s32 i=0; i<(s32)s32Vector.size(); ++i)
				s32Vector[i] = i;

			xvector<s32>::iterator it = s32Vector.begin();
			CHECK_EQUAL(100, s32Vector.size());
			for (s32 i=0; i<(s32)s32Vector.size(); ++i)
			{
				CHECK_TRUE(s32Vector[i] == *it);
				it++;
			}
		}

		UNITTEST_TEST(begin2)
		{
			xvector<s32> s32Vector(gTestAllocator);
			s32Vector.resize(100);
			for (s32 i=0; i<(s32)s32Vector.size(); ++i)
				s32Vector[i] = i;

			xvector<s32> const& s32Vector_const = s32Vector;

			xvector<s32>::const_iterator it = s32Vector_const.begin();
			CHECK_EQUAL(100, s32Vector_const.size());
			for (s32 i=0; i<(s32)s32Vector_const.size(); ++i)
			{
				CHECK_TRUE(s32Vector_const[i] == *it);
				it++;
			}
		}

		UNITTEST_TEST(end1)
		{
			xvector<s32> s32Vector(gTestAllocator);
			s32Vector.resize(100);
			for (s32 i=0; i<(s32)s32Vector.size(); ++i)
				s32Vector[i] = i;

			xvector<s32>::iterator it = s32Vector.begin();
			CHECK_EQUAL(100, s32Vector.size());
			for (s32 i=0; i<(s32)s32Vector.size(); ++i)
			{
				CHECK_TRUE(s32Vector[i] == *it);
				it++;
			}
			CHECK_TRUE(it == s32Vector.end());
		}

		UNITTEST_TEST(end2)
		{
			xvector<s32> s32Vector(gTestAllocator);
			s32Vector.resize(100);
			for (s32 i=0; i<(s32)s32Vector.size(); ++i)
				s32Vector[i] = i;

			xvector<s32> const& s32Vector_const = s32Vector;

			xvector<s32>::const_iterator it = s32Vector_const.begin();
			CHECK_EQUAL(100, s32Vector_const.size());
			for (s32 i=0; i<(s32)s32Vector_const.size(); ++i)
			{
				CHECK_TRUE(s32Vector_const[i] == *it);
				it++;
			}
			CHECK_TRUE(it == s32Vector_const.end());
		}

		UNITTEST_TEST(iat1)
		{
			xvector<s32> s32Vector(gTestAllocator);
			s32Vector.resize(100);
			for (s32 i=0; i<(s32)s32Vector.size(); ++i)
				s32Vector[i] = i;

			xvector<s32>::iterator it1 = s32Vector.iat(50);
			CHECK_EQUAL(50, *it1);
			xvector<s32>::iterator it2 = s32Vector.iat(51);
			CHECK_EQUAL(51, *it2);
		}

		UNITTEST_TEST(iat2)
		{
			xvector<s32> s32Vector(gTestAllocator);
			s32Vector.resize(100);
			for (s32 i=0; i<(s32)s32Vector.size(); ++i)
				s32Vector[i] = i;

			xvector<s32> const& s32Vector_const = s32Vector;

			xvector<s32>::const_iterator it1 = s32Vector_const.iat(50);
			CHECK_EQUAL(50, *it1);
			xvector<s32>::const_iterator it2 = s32Vector_const.iat(51);
			CHECK_EQUAL(51, *it2);
		}

		UNITTEST_TEST(at1)
		{
			xvector<s32> s32Vector(gTestAllocator);
			s32Vector.resize(100);
			for (s32 i=0; i<(s32)s32Vector.size(); ++i)
				s32Vector[i] = i;

			for (s32 i=0; i<(s32)s32Vector.size(); ++i)
			{
				CHECK_EQUAL(i, s32Vector.at(i));
			}
		}

		UNITTEST_TEST(at2)
		{
			xvector<s32> s32Vector(gTestAllocator);
			s32Vector.resize(100);
			for (s32 i=0; i<(s32)s32Vector.size(); ++i)
				s32Vector[i] = i;

			xvector<s32> const& s32Vector_const = s32Vector;

			for (s32 i=0; i<(s32)s32Vector_const.size(); ++i)
			{
				CHECK_EQUAL(i, s32Vector_const.at(i));
			}		
		}

		UNITTEST_TEST(erase)
		{
			xvector<s32> s32Vector(gTestAllocator);
			s32Vector.resize(5);
			for (s32 i=0; i<(s32)s32Vector.size(); ++i)
				s32Vector[i] = i;

			xvector<s32>::iterator iter = s32Vector.iat(2);
			CHECK_EQUAL(5, s32Vector.size());
			s32Vector.erase(iter);
			CHECK_EQUAL(4, s32Vector.size());

			CHECK_EQUAL(0, s32Vector[0]);
			CHECK_EQUAL(1, s32Vector[1]);
			CHECK_EQUAL(3, s32Vector[2]);
			CHECK_EQUAL(4, s32Vector[3]);
		}
		
		UNITTEST_TEST(find1)
		{
			xvector<s32> s32Vector(gTestAllocator);
			s32Vector.resize(100);
			for (s32 i=0; i<(s32)s32Vector.size(); ++i)
				s32Vector[i] = i;

			xvector<s32>::iterator it1 = s32Vector.find(50);
			CHECK_EQUAL(50, *it1);
			xvector<s32>::iterator it2 = s32Vector.find(51);
			CHECK_EQUAL(51, *it2);
			xvector<s32>::iterator it3 = s32Vector.find(200);
			xvector<s32>::iterator it_end = s32Vector.end();
			bool result = it3 == it_end;
			CHECK_EQUAL(result, true);
		}

		UNITTEST_TEST(find2)
		{
			xvector<s32> s32Vector(gTestAllocator);
			s32Vector.resize(100);
			for (s32 i=0; i<(s32)s32Vector.size(); ++i)
				s32Vector[i] = i;

			xvector<s32> const& s32Vector_const = s32Vector;

			xvector<s32>::const_iterator it1 = s32Vector_const.find(50);
			CHECK_EQUAL(50, *it1);
			xvector<s32>::const_iterator it2 = s32Vector_const.find(51);
			CHECK_EQUAL(51, *it2);
			xvector<s32>::const_iterator it3 = s32Vector_const.find(200);
			xvector<s32>::const_iterator it_end = s32Vector_const.end();
			bool result = it3 == it_end;
			CHECK_EQUAL(result, true);
		}

        UNITTEST_TEST(operatorassign)
        {
            xvector<s32> s32Vector(gTestAllocator);
            s32Vector.reserve(2);
            s32Vector.push_back(1);
            s32Vector.push_back(2);

            xvector<s32> s32VectorCopy(gTestAllocator);
            s32VectorCopy.reserve(2);

            s32VectorCopy = s32Vector;

            CHECK_EQUAL(2, s32VectorCopy.size());
            xvector<s32>::iterator i = s32VectorCopy.begin();
            CHECK_EQUAL(1, *i);
            ++i;
            CHECK_EQUAL(2, *i);
        }

        UNITTEST_TEST(instert)
        {
            xvector<s32> s32Vector(gTestAllocator);
            s32Vector.reserve(2);
            s32Vector.push_back(1);
            s32Vector.push_back(2);

            s32Vector.insert(s32Vector.begin(), 3);
            
            xvector<s32>::iterator i = s32Vector.begin();
            CHECK_EQUAL(3, *i);
            i = s32Vector.begin();
            ++i;
            CHECK_EQUAL(1, *i);
        }

        int a = 0;
        struct Teststruct
        {
            ~Teststruct() { a = 1; }
        };

        UNITTEST_TEST(erase2)
        {
            {
                xvector<Teststruct> sStructVec(gTestAllocator);
                sStructVec.reserve(2);
                Teststruct t1, t2;
                sStructVec.push_back(t1);
                sStructVec.push_back(t2);

                sStructVec.erase(sStructVec.begin());

                CHECK_TRUE(a == 1);
            }
        }

		UNITTEST_TEST(sort)
		{
			xvector<s32> s32Vector(gTestAllocator);
			s32Vector.set_autoalloc();
			for (s32 i = 8; i >= -8; --i)
				s32Vector.push_back(i);

			s32Vector.sort();
			s32Vector.sort(xstd::xgreater<s32>() );
			s32Vector.sort(xstd::xcompare<s32>() );
			s32Vector.sort(SquareCompare() );
			s32Vector.sort(xstd::xless<s32>() );
		}
	}
}
UNITTEST_SUITE_END
