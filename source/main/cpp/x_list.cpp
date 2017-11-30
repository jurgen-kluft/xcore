#include "xcore/x_list.h"

namespace xcore
{
#ifdef LIST_SORT
	#define MAX_LIST_LENGTH_BITS 32

	/*
	* Returns a list organized in an intermediate format suited
	* to chaining of merge() calls: null-terminated, no reserved or
	* sentinel head node, "prev" links not maintained.
	*/
	static struct list_head *merge(void *priv, int (*cmp)(void *priv, const void *, const void *),
	struct list_head *a, struct list_head *b)
	{
		struct list_head head, *tail = &head;

		while (a && b)
		{
			/* if equal, take 'a' -- important for sort stability */
			if ((*cmp)(priv, a, b) <= 0)
			{
				tail->next = a;
				a = a->next;
			} 
			else
			{
				tail->next = b;
				b = b->next;
			}
			tail = tail->next;
		}
		tail->next = a?:b;
		return head.next;
	}

	/*
	* Combine final list merge with restoration of standard doubly-linked
	* list structure.  This approach duplicates code from merge(), but
	* runs faster than the tidier alternatives of either a separate final
	* prev-link restoration pass, or maintaining the prev links
	* throughout.
	*/
	static void merge_and_restore_back_links(void *priv, int (*cmp)(void *priv, const void *, const void *), struct list_head *head, struct list_head *a, struct list_head *b)
	{
		struct list_head *tail = head;

		while (a && b) 
		{
			/* if equal, take 'a' -- important for sort stability */
			if ((*cmp)(priv, a, b) <= 0)
			{
				tail->next = a;
				a->prev = tail;
				a = a->next;
			}
			else
			{
				tail->next = b;
				b->prev = tail;
				b = b->next;
			}
			tail = tail->next;
		}
		tail->next = a ? : b;

		do
		{
			/*
			* In worst cases this loop may run many iterations.
			* Continue callbacks to the client even though no
			* element comparison is needed, so the client's cmp()
			* routine can invoke cond_resched() periodically.
			*/
			(*cmp)(priv, tail->next, tail->next);

			tail->next->prev = tail;
			tail = tail->next;
		} while (tail->next);

		tail->next = head;
		head->prev = tail;
	}

	void list_sort(void *priv, struct list_head *head, int (*cmp)(void *priv, const void *, const void *))
	{
		struct list_head *part[MAX_LIST_LENGTH_BITS + 1]; /* sorted partial lists
														  -- last slot is a sentinel */
		int lev;  /* index into part[] */
		int max_lev = 0;
		struct list_head *list;

		if (list_empty(head))
			return;

		memset(part, 0, sizeof(part));

		head->prev->next = NULL;
		list = head->next;

		while (list) 
		{
			struct list_head *cur = list;
			list = list->next;
			cur->next = NULL;

			for (lev = 0; part[lev]; lev++)
			{
				cur = merge(priv, cmp, part[lev], cur);
				part[lev] = NULL;
			}
			if (lev > max_lev)
				max_lev = lev;
			part[lev] = cur;
		}

		for (lev = 0; lev < max_lev; lev++)
			if (part[lev])
				list = merge(priv, cmp, part[lev], list);

		merge_and_restore_back_links(priv, cmp, head, part[max_lev], list);
	}







	// This is a O(n log(n)) merge sort
	void			xlist_sort(xlist* list, s32 (*cmp)(const void* d1, const void* d2))
	{
		u32 i;
		u32 nmerges,insize,psize,qsize;

		xdlistnode* head = list->mHead;
		xdlistnode* tail = list->mTail;

		xdlistnode* pn, *qn, *en, *s_list, *s_tail;
		if (head)
		{
			insize = 1;
			s_list = head;
			while (true)
			{
				pn = s_list;
				s_list = NULL;
				s_tail = NULL;
				nmerges = 0;
				while (pn)
				{
					nmerges++;
					qn = pn;
					psize = 0;
					for (i = 0; i<insize; i++)
					{
						psize++;
						qn = (xdlistnode*)qn->mNext;
						if (! (qn) )
							break;
					}
					qsize = insize;
					while ((psize > 0) || ((qsize > 0) && qn ))
					{
						if (psize == 0)
						{
							en = qn;
							qn = (xdlistnode*)qn->mNext;
							qsize--;
						} 
						else if ( (qsize == 0) || !(qn) )
						{
							en = pn;
							pn = (xdlistnode*)pn->mNext;
							psize--;
						}
						else if (cmp(list->mFunctors->mFNodeToItem(*list, pn), list->mFunctors->mFNodeToItem(*list, qn)) <= 0)
						{
							en = pn;
							pn = (xdlistnode*)pn->mNext;
							psize--;
						}
						else
						{
							en = qn;
							qn = (xdlistnode*)qn->mNext;
							qsize--;
						}
						if ( s_tail )
						{
							s_tail->mNext = en;
						}
						else
						{
							s_list = en;
						}
						en->mPrev = s_tail;
						s_tail = en;
					}
					pn = qn;
				}
				s_tail->mNext = NULL;
				if ( nmerges <= 1 )
				{
					tail = s_tail;
					head = s_list;
					break;
				}
				insize *= 2;
			}

			list->mHead = head;
			list->mTail = tail;
		}
	}
#endif

}
