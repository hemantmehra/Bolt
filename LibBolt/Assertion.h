#ifndef ASSERTION_H
#define ASSERTION_H

#include <assert.h>

#define ASSERT_NOT_REACHED() assert(0)
#define NOT_IMPLEMENTED() assert(0)
#define CHECK(exp) assert(exp)
#define TEST(exp) assert(exp)

#endif
