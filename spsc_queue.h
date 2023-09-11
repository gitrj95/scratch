#ifndef SPSC_QUEUE_H
#define SPSC_QUEUE_H

#include <stdalign.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

#define CACHE_BLOCK_BYTES 64

#define spscqueue_init_from_static(qp, a)                   \
  do {                                                      \
    size_t len__ = sizeof(a) / sizeof((a)[0]);              \
    spscqueue_init((qp), (a), &(a)[len__], sizeof((a)[0])); \
  } while (0)

typedef struct {
  long item_len;
  void *hd, *tl;
  alignas(CACHE_BLOCK_BYTES) atomic_intptr_t w;
  alignas(CACHE_BLOCK_BYTES) atomic_intptr_t r;
  alignas(CACHE_BLOCK_BYTES) intptr_t readerw;
  alignas(CACHE_BLOCK_BYTES) intptr_t writerr;
} spscqueue;

void spscqueue_init(spscqueue *q, void *hd, void *tl, long item_len);
bool spscqueue_write(spscqueue *q, void *p);
void *spscqueue_read(spscqueue *q);

#endif
