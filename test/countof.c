#include "../countof.h"
#include <assert.h>
#include "../test.h"

struct a {
  int b;
  float c;
  char d;
};
static_assert(12 == sizeof(struct a));

struct a buf[11];

void buffers(void) {
  test();
  expect_true(11 == countof(buf));
  char buf1[991];
  expect_true(991 == countof(buf1));
}

void sliterals(void) {
  test();
  expect_true(6 == countof("apple"));
}

int main(void) {
  suite();
  buffers();
  sliterals();
}
