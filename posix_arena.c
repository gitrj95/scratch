/*
  DANGER: As far as the C standard is concerned, this implementation
  is UB. If the system is POSIX and the uintptr_t representation of an
  object pointer is precisely its address, then it is both correct and
  non-portable.
*/

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "arena.h"

#define unlikely(expr) __builtin_expect(!!(expr), 0)

struct arena {
  char *hd, *tl, *p;
  void (*deleter)(void *, ptrdiff_t);
};

struct arena *arena_create(ptrdiff_t len) {
  assert(0 < len);
  void *buf =
      mmap(0, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (MAP_FAILED == buf) return 0;
  return arena_create3(buf, len, 0);
}

struct arena *arena_create3(void *buf, ptrdiff_t buflen,
                            void (*deleter)(void *, ptrdiff_t)) {
  assert(buf);
  assert(0 < buflen);
  struct arena *a = malloc(sizeof(*a));
  if (!a) return 0;
  *a =
      (struct arena){.hd = buf, .tl = (char *)buf + buflen, .deleter = deleter};
  a->p = a->tl;
  return a;
}

int arena_delete(struct arena **a) {
  assert(a);
  assert(*a);
  ptrdiff_t len = a[0]->tl - a[0]->hd;
  if (a[0]->deleter)
    a[0]->deleter(a[0]->hd, len);
  else if (munmap(a[0]->hd, len))
    return -1;
  free(*a);
  *a = 0;
  return 0;
}

void *linalloc_explicit(struct arena *a, ptrdiff_t itemsz, int align) {
  assert(a);
  assert(0 < itemsz);
  assert(-1 < align);
  assert(!(align & (align - 1)));
  static_assert(sizeof(uintptr_t) >= sizeof(int));
  uintptr_t addr = (uintptr_t)a->p;
  addr -= itemsz;
  addr = addr & ~(align - 1); /* need sign extension */
  if (unlikely(addr < (uintptr_t)a->hd))
    return 0; /* prefer speculative execution to cmov */
  a->p = (char *)addr;
  return a->p;
}

void arena_reset(arena *a) {
  assert(a);
  a->p = a->tl;
}
