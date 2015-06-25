#include <stdbool.h>
#include <stdatomic.h>
#include <stdlib.h>

#include "locklessqueue.h"

typedef struct llnode llnode_t;
typedef _Atomic(llnode_t *) atomic_llnodeptr_t;

struct llnode {
    void * element;
    atomic_llnodeptr_t next;
};

struct llqueue {
    atomic_llnodeptr_t head;
    atomic_llnodeptr_t last;
};

llnode_t * create_llnode(void * element) {
    llnode_t * n = malloc(sizeof(llnode_t));
    n->next = ATOMIC_VAR_INIT(NULL);
    n->element = element;
    return n;
}

llqueue_t * create_llqueue() {
    llqueue_t * q = malloc(sizeof(llqueue_t));
    llnode_t * node = create_llnode(NULL);
    q->head = ATOMIC_VAR_INIT(node);
    q->last = ATOMIC_VAR_INIT(node);

    return q;
}


void llqueue_dispose(llqueue_t * q, dispose_fn dispose) {
    void * element;
    while(llqueue_dequeue(q, &element) != false) {
        dispose(element);
    }
    llnode_t * node = q->head;
    free(node);
    free(q);
}


void llqueue_enqueue(llqueue_t * q, void * element) {
    llnode_t * last;
    llnode_t * next;

    llnode_t * node = create_llnode(element);

    while(1) {
        last = q->last;
        next = last->next;
        if(last == q->last) {
            if(next == 0) { // Todo se mantiene intacto.
                if(atomic_compare_exchange_strong(&last->next, &next, node)) {
                    break;
                }
            } else { // Alguien me agrego un nodo siguiente.
                atomic_compare_exchange_strong(&q->last, &last, next);
            }
        }
    }
    atomic_compare_exchange_strong(&q->last, &last, node);
}

bool llqueue_dequeue(llqueue_t * q, void ** element) {
    llnode_t *head;
    llnode_t *last;
    llnode_t *next;

    while(1) {
        head = q->head;
        last = q->last;
        next = head->next;
        if(head == q->head) {
            if (head == last) {
                if (next == 0) {
                    return false;
                }
                atomic_compare_exchange_strong(&q->last, &last, next);
            } else {
                *element = next->element;
                if(atomic_compare_exchange_strong(&q->head, &head, next)) {
                    break;
                }
            }
        }
    }
    free((void*)head);
    return true;
}
