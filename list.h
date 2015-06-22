#ifndef _LIST_H__
#define _LIST_H__

#include <stddef.h>
#include <stdlib.h>
#include "macro.h"

struct ListNode {
    struct ListNode*    next;
    // pointer of previous node's next member
    struct ListNode**   pprev;
};

#define ListHeadInit(n) \
    (n)->next = n;\
    (n)->pprev = &((n)->next)

#define ListNodeInit(n) \
    (n)->next = NULL;\
    (n)->pprev = NULL

#define ListData(type, member, pnode)\
    ((type*)((size_t)(pnode)-offsetof(type, member)))

#define ListNext(node) ((node)->next)
#define ListPrev(node) ListData(struct ListNode, next, (node)->pprev)

#define InsertNode(t, n) \
    (n)->next = (t)->next;\
    (n)->pprev = &((t)->next);\
    (t)->next->pprev = &(n)->next;\
    (t)->next = (n)

#define InsertFront(head, n)    InsertNode(head, n)

// need a tmp node _prev to save last node
// ListPrev depend head->pprev which modified in InsertNode third statement
#define InsertBack(head, n) \
    struct ListNode* _prev = ListPrev(head);\
    InsertNode(_prev, n)

#define DeleteNode(n) \
    (n)->next->pprev = (n)->pprev;\
    ListPrev(n)->next = (n)->next

#define ListIsEmpty(head) \
    ((size_t)(head)->next == (size_t)(head))

#define ListTraversal(head, next) \
    for ((next) = ListNext(head); (next) != (head); (next) = ListNext(next))

#define ListTraversalReverse(head, prev) \
    for ((prev) = ListPrev(head); (prev) != (head); (prev) = ListPrev(prev))

void Test(List) {
    struct T {
        int i;
        struct ListNode node;
    };
    
    struct ListNode head;
    struct ListNode* next;

    printf("InsertFront\n");
    ListHeadInit(&head);
    for (int i = 0; i < 10; i++) {
        struct T *t = malloc(sizeof(struct T));
        t->i = i;
        ListNodeInit(&t->node);
        InsertFront(&head, &t->node);
    }
    
    printf("ListTraversal\n");
    ListTraversal(&head, next) {
        struct T* p = ListData(struct T, node, next);
        printf("%d\n", p->i);
    }
    printf("ListTraversalReverse\n");
    ListTraversalReverse(&head, next) {
        struct T* p = ListData(struct T, node, next);
        printf("%d\n", p->i);
    }

    printf("delete\n");
    ListTraversal(&head, next) {
        struct T* p = ListData(struct T, node, next);
        DeleteNode(next);
        free(p);
    }

    printf("InsertBack\n");
    for (int i = 0; i < 10; i++) {
        struct T *t = malloc(sizeof(struct T));
        t->i = i;
        ListNodeInit(&t->node);
        InsertBack(&head, &t->node);
    }

    printf("ListTraversal\n");
    ListTraversal(&head, next) {
        struct T* p = ListData(struct T, node, next);
        printf("%d\n", p->i);
    }
    printf("ListTraversalReverse\n");
    ListTraversalReverse(&head, next) {
        struct T* p = ListData(struct T, node, next);
        printf("%d\n", p->i);
    }    
}
#endif
