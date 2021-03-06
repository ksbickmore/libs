#include "defines.h"
#include "lstlib.h"
#include <minwindef.h>

#define HEAD    node.next       /* first node in list */  
#define TAIL    node.previous       /* last node in list */  

/*********************************************************************
*
* listLibInit - initializes listLib module
*
* This routine pulls listLib into the vxWorks image.
*
* RETURNS: N/A
*/
void listLibInit(void)
{
	return;
}

/*********************************************************************
*
* listInit - initialize a list descriptor
*
* This routine initializes a specified list to an empty list.
*
* RETURNS: N/A
*/

void listInit
(
FAST LIST *pList     /* ptr to list descriptor to be initialized */
)
{
	pList->HEAD = NULL;
	pList->TAIL = NULL;
	pList->count = 0;
}
/*************************************************************************
*
* listAdd - add a node to the end of a list
*
* This routine adds a specified node to the end of a specified list.
*
* RETURNS: N/A
*/

void listAdd
(
LIST *pList,        /* pointer to list descriptor */
NODE *pNode         /* pointer to node to be added */
)
{
	listInsert(pList, pList->TAIL, pNode);
}
/**************************************************************************
*
* listConcat - concatenate two lists
*
* This routine concatenates the second list to the end of the first list.
* The second list is left empty.  Either list (or both) can be
* empty at the beginning of the operation.
*
* RETURNS: N/A
*/

void listConcat
(
FAST LIST *pDstList,        /* destination list */
FAST LIST *pAddList         /* list to be added to dstList */
)
{
	if (pAddList->count == 0)        /* nothing to do if AddList is empty */
		return;

	if (pDstList->count == 0)
		*pDstList = *pAddList;
	else
	{
		/* both lists non-empty; update DstList pointers */

		pDstList->TAIL->next = pAddList->HEAD;
		pAddList->HEAD->previous = pDstList->TAIL;
		pDstList->TAIL = pAddList->TAIL;

		pDstList->count += pAddList->count;
	}

	/* make AddList empty */

	listInit(pAddList);
}
/**************************************************************************
*
* listCount - report the number of nodes in a list
*
* This routine returns the number of nodes in a specified list.
*
* RETURNS:
* The number of nodes in the list.
*/

int listCount
(
LIST *pList         /* pointer to list descriptor */
)
{
	return (pList->count);
}
/**************************************************************************
*
* listDelete - delete a specified node from a list
*
* This routine deletes a specified node from a specified list.
*
* RETURNS: N/A
*/

void listDelete
(
FAST LIST *pList,   /* pointer to list descriptor */
FAST NODE *pNode    /* pointer to node to be deleted */
)
{
	if (pNode->previous == NULL)
		pList->HEAD = pNode->next;
	else
		pNode->previous->next = pNode->next;

	if (pNode->next == NULL)
		pList->TAIL = pNode->previous;
	else
		pNode->next->previous = pNode->previous;

	/* update node count */

	pList->count--;
}
/************************************************************************
*
* listExtract - extract a sublist from a list
*
* This routine extracts the sublist that starts with <pStartNode> and ends
* with <pEndNode> from a source list.  It places the extracted list in
* <pDstList>.
*
* RETURNS: N/A
*/

void listExtract
(
FAST LIST *pSrcList,      /* pointer to source list */
FAST NODE *pStartNode,    /* first node in sublist to be extracted */
FAST NODE *pEndNode,      /* last node in sublist to be extracted */
FAST LIST *pDstList       /* ptr to list where to put extracted list */
)
{
	FAST int i;
	FAST NODE *pNode;

	/* fix pointers in original list */

	if (pStartNode->previous == NULL)
		pSrcList->HEAD = pEndNode->next;
	else
		pStartNode->previous->next = pEndNode->next;

	if (pEndNode->next == NULL)
		pSrcList->TAIL = pStartNode->previous;
	else
		pEndNode->next->previous = pStartNode->previous;


	/* fix pointers in extracted list */

	pDstList->HEAD = pStartNode;
	pDstList->TAIL = pEndNode;

	pStartNode->previous = NULL;
	pEndNode->next = NULL;


	/* count number of nodes in extracted list and update counts in lists */

	i = 0;

	for (pNode = pStartNode; pNode != NULL; pNode = pNode->next)
		i++;

	pSrcList->count -= i;
	pDstList->count = i;
}
/************************************************************************
*
* listFirst - find first node in list
*
* This routine finds the first node in a linked list.
*
* RETURNS
* A pointer to the first node in a list, or
* NULL if the list is empty.
*/

NODE *listFirst
(
LIST *pList         /* pointer to list descriptor */
)
{
	return (pList->HEAD);
}
/************************************************************************
*
* listGet - delete and return the first node from a list
*
* This routine gets the first node from a specified list, deletes the node
* from the list, and returns a pointer to the node gotten.
*
* RETURNS
* A pointer to the node gotten, or
* NULL if the list is empty.
*/

NODE *listGet
(
FAST LIST *pList    /* ptr to list from which to get node */
)
{
	FAST NODE *pNode = pList->HEAD;

	if (pNode != NULL)                      /* is list empty? */
	{
		pList->HEAD = pNode->next;          /* make next node be 1st */

		if (pNode->next == NULL)            /* is there any next node? */
			pList->TAIL = NULL;             /*   no - list is empty */
		else
			pNode->next->previous = NULL;   /*   yes - make it 1st node */

		pList->count--;                     /* update node count */
	}

	return (pNode);
}
/************************************************************************
*
* listInsert - insert a node in a list after a specified node
*
* This routine inserts a specified node in a specified list.
* The new node is placed following the list node <pPrev>.
* If <pPrev> is NULL, the node is inserted at the head of the list.
*
* RETURNS: N/A
*/

void listInsert
(
FAST LIST *pList,   /* pointer to list descriptor */
FAST NODE *pPrev,   /* pointer to node after which to insert */
FAST NODE *pNode    /* pointer to node to be inserted */
)
{
	FAST NODE *pNext;

	if (pPrev == NULL)
	{               /* new node is to be first in list */
		pNext = pList->HEAD;
		pList->HEAD = pNode;
	}
	else
	{               /* make prev node point fwd to new */
		pNext = pPrev->next;
		pPrev->next = pNode;
	}

	if (pNext == NULL)
		pList->TAIL = pNode;     /* new node is to be last in list */
	else
		pNext->previous = pNode; /* make next node point back to new */


	/* set pointers in new node, and update node count */

	pNode->next = pNext;
	pNode->previous = pPrev;

	pList->count++;
}
/************************************************************************
*
* listLast - find the last node in a list
*
* This routine finds the last node in a list.
*
* RETURNS
* A pointer to the last node in the list, or
* NULL if the list is empty.
*/

NODE *listLast
(
LIST *pList         /* pointer to list descriptor */
)
{
	return (pList->TAIL);
}
/************************************************************************
*
* listNext - find the next node in a list
*
* This routine locates the node immediately following a specified node.
*
* RETURNS:
* A pointer to the next node in the list, or
* NULL if there is no next node.
*/

NODE *listNext
(
NODE *pNode         /* ptr to node whose successor is to be found */
)
{
	return (pNode->next);
}
/************************************************************************
*
* listNth - find the Nth node in a list
*
* This routine returns a pointer to the node specified by a number <nodenum>
* where the first node in the list is numbered 1.
* Note that the search is optimized by searching forward from the beginning
* if the node is closer to the head, and searching back from the end
* if it is closer to the tail.
*
* RETURNS:
* A pointer to the Nth node, or
* NULL if there is no Nth node.
*/

NODE *listNth
(
FAST LIST *pList,           /* pointer to list descriptor */
FAST int nodenum            /* number of node to be found */
)
{
	FAST NODE *pNode;

	/* verify node number is in list */

	if ((nodenum < 1) || (nodenum > pList->count))
		return (NULL);


	/* if nodenum is less than half way, look forward from beginning;
	otherwise look back from end */

	if (nodenum < (pList->count >> 1))
	{
		pNode = pList->HEAD;

		while (--nodenum > 0)
			pNode = pNode->next;
	}

	else
	{
		nodenum -= pList->count;
		pNode = pList->TAIL;

		while (nodenum++ < 0)
			pNode = pNode->previous;
	}

	return (pNode);
}
/************************************************************************
*
* listPrevious - find the previous node in a list
*
* This routine locates the node immediately preceding the node pointed to
* by <pNode>.
*
* RETURNS:
* A pointer to the previous node in the list, or
* NULL if there is no previous node.
*/

NODE *listPrevious
(
NODE *pNode         /* ptr to node whose predecessor is to be found */
)
{
	return (pNode->previous);
}
/************************************************************************
*
* listNStep - find a list node <nStep> steps away from a specified node
*
* This routine locates the node <nStep> steps away in either direction from
* a specified node.  If <nStep> is positive, it steps toward the tail.  If
* <nStep> is negative, it steps toward the head.  If the number of steps is
* out of range, NULL is returned.
*
* RETURNS:
* A pointer to the node <nStep> steps away, or
* NULL if the node is out of range.
*/

NODE *listNStep
(
FAST NODE *pNode,           /* the known node */
int nStep                   /* number of steps away to find */
)
{
	int i;

	for (i = 0; i < abs(nStep); i++)
	{
		if (nStep < 0)
			pNode = pNode->previous;
		else if (nStep > 0)
			pNode = pNode->next;
		if (pNode == NULL)
			break;
	}
	return (pNode);
}

/************************************************************************
*
* listFind - find a node in a list
*
* This routine returns the node number of a specified node (the
* first node is 1).
*
* RETURNS:
* The node number, or
* ERROR if the node is not found.
*/

int listFind
(
LIST *pList,                /* list in which to search */
FAST NODE *pNode            /* pointer to node to search for */
)
{

	FAST NODE *pNextNode;
	FAST int index = 1;

	pNextNode = listFirst(pList);

	while ((pNextNode != NULL) && (pNextNode != pNode))
	{
		index++;
		pNextNode = listNext(pNextNode);
	}

	if (pNextNode == NULL)
		return (ERROR);
	else
		return (index);
}

/************************************************************************
*
* listFree - free up a list
*
* This routine turns any list into an empty list.
* It also frees up memory used for nodes.
*
* RETURNS: N/A
*
* SEE ALSO: free()
*/

void listFree
(
LIST *pList         /* list for which to free all nodes */
)
{
	NODE *p1, *p2;

	if (pList->count > 0)
	{
		p1 = pList->HEAD;
		while (p1 != NULL)
		{
			p2 = p1->next;
			free((char *)p1);
			p1 = p2;
		}
		pList->count = 0;
		pList->HEAD = pList->TAIL = NULL;
	}
}