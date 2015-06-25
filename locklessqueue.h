#ifndef LOCKLESSQUEUE_H
#define LOCKLESSQUEUE_H

#include <stdbool.h>

typedef struct llqueue llqueue_t;
typedef void (*dispose_fn)(void *);

llqueue_t * create_llqueue();
void llqueue_dispose(llqueue_t * q, dispose_fn dispose);
void llqueue_enqueue(llqueue_t * q, void * element);
bool llqueue_dequeue(llqueue_t * q, void ** element);

#endif // LOCKLESSQUEUE_H
