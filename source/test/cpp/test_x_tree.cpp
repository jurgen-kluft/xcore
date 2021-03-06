#include "xbase/x_allocator.h"
#include "xbase/x_slice.h"

#include "xcore/x_tree.h"

#include "xunittest/xunittest.h"

using namespace xcore;

extern xcore::x_iallocator* gTestAllocator;


UNITTEST_SUITE_BEGIN(xtree)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP()
		{
			
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{

		}

		struct mynode : public xrbnode
		{
			mynode() : id(0) { clear(); }
			mynode(uptr i) : id(i) { clear(); }
			uptr id;

			XCORE_CLASS_PLACEMENT_NEW_DELETE
		};

		s32		rb_node_compare(xrbnode const* aa, void const* bb)
		{
			mynode const* a = (mynode const*)aa;
			uptr bid = (uptr)bb;

			if (a->id < bid)
				return -1;
			else if (a->id > bid)
				return 1;
			return 0;
		}

		s32		rb_node_node_compare(xrbnode* aa, xrbnode* bb)
		{
			mynode* a = (mynode*)aa;
			mynode* b = (mynode*)bb;

			if (a->id < b->id)
				return -1;
			else if (a->id > b->id)
				return 1;
			return 0;
		}

		void	rb_node_remove(xrbnode* to_replace, xrbnode* to_remove)
		{
			mynode* replacer = (mynode*)to_replace;
			mynode* removed = (mynode*)to_remove;
			uptr id = replacer->id;
			replacer->id = removed->id;
			removed->id = id;
		}

	
		UNITTEST_TEST(constructor1)
		{
			xrbtree tree;
			tree.init();

			mynode a(1);
			mynode b(2);
			mynode c(3);
			mynode d(4);

			const char* result = NULL;

			bool inserted;
			inserted = tree.insert(&a, (void*)a.id, rb_node_compare);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.test(result));
			inserted = tree.insert(&b, (void*)b.id, rb_node_compare);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.test(result));
			inserted = tree.insert(&c, (void*)c.id, rb_node_compare);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.test(result));
			inserted = tree.insert(&d, (void*)d.id, rb_node_compare);
			CHECK_TRUE(inserted);
			CHECK_TRUE(tree.test(result));

			xrbnode* f = NULL;
			tree.find(f, (void*)c.id, rb_node_compare);
			CHECK_EQUAL(&c, f);
			CHECK_TRUE(tree.test(result));
			tree.find(f, (void*)a.id, rb_node_compare);
			CHECK_EQUAL(&a, f);
			CHECK_TRUE(tree.test(result));
			tree.find(f, (void*)b.id, rb_node_compare);
			CHECK_EQUAL(&b, f);
			CHECK_TRUE(tree.test(result));
			tree.find(f, (void*)d.id, rb_node_compare);
			CHECK_EQUAL(&d, f);
			CHECK_TRUE(tree.test(result));

			xrbnode* iterator = NULL;
			s32 i = 0;
			do
			{
				xrbnode* node_to_destroy = tree.clear(iterator);
				if (i == 0)
				{
					CHECK_TRUE(node_to_destroy == &d);
				}
				else if (i == 1)
				{
					CHECK_TRUE(node_to_destroy == &c);
				}
				else if (i == 2)
				{
					CHECK_TRUE(node_to_destroy == &b);
				}
				else if (i == 3)
				{
					CHECK_TRUE(node_to_destroy == &a);
				}
				else if (i == 4)
				{
					CHECK_TRUE(node_to_destroy == NULL);
				}
				i++;
			} while (iterator !=NULL);
		}


		UNITTEST_TEST(case1)
		{
			xheap<mynode> node_heap(gTestAllocator);
			
			xrbtree tree;
			tree.init();

			const int max_tracked_allocs = 1000;
			uptr allocations[max_tracked_allocs];
			for (s32 i = 0; i < max_tracked_allocs; ++i)
			{
				allocations[i] = 0;
			}

			const char* test_result = NULL;

			for (s32 i = 0; i < 3200; ++i)
			{
				mynode* node = xnew<mynode>(node_heap);
				node->clear();
				node->id = (i);

				int alloc_idx = rand() % max_tracked_allocs;
				if (allocations[alloc_idx] != 0)
				{
					uptr pid = allocations[alloc_idx];
					mynode _p_(pid);

					xrbnode* f = NULL;
					if (tree.remove(f, (void*)pid, rb_node_compare))
					{
						CHECK_EQUAL(((mynode*)f)->id, pid);
						CHECK_TRUE(tree.test(test_result));
						xdelete<>(heap, f);
					}
					else
					{
						CHECK_EQUAL(0, 1);
					}
					allocations[alloc_idx] = 0;
				}
				allocations[alloc_idx] = node->id;
				bool inserted = tree.insert(node, (void*)node->id, rb_node_compare);
				CHECK_TRUE(inserted);
				CHECK_TRUE(tree.test(test_result));
			}

			for (s32 i = 0; i < max_tracked_allocs; ++i)
			{
				if (allocations[i] != NULL)
				{
					uptr pid = allocations[i];
					mynode _p_(pid);

					xrbnode* f = NULL;
					if (tree.remove(f, (void*)pid, rb_node_compare))
					{
						CHECK_EQUAL(((mynode*)f)->id, pid);
						xdelete<>(heap, f);
					}
					allocations[i] = 0;
				}
			}

			xrbnode* iterator = NULL;
			xrbnode* node_to_destroy = NULL;
			do 
			{
				node_to_destroy = tree.clear(iterator);
				if (node_to_destroy != NULL)
				{
					xdelete<>(heap, node_to_destroy);
				}
			} while (iterator != NULL);
		}

	}
}
UNITTEST_SUITE_END
