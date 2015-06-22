#include "list.h"
#include <stdlib.h>
#include <stdio.h>

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
    struct ListNode tmp;
    ListTraversal(&head, next) {
        struct T* p = ListData(struct T, node, next);
        tmp = *next;
        DeleteNode(next);
        next = &tmp;
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