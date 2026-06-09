#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// ============================================================================
// ========================= Utility Functions ================================
// ============================================================================
// ============================================================================

char *tmp_sprintf(const char *fmt, ...) {
  static struct {
    char *elements;
    size_t count;
    size_t capacity;
  } buf;

  va_list args;
  va_start(args, fmt);
  int ret = vsnprintf(NULL, 0, fmt, args);
  if (ret == 0) {
    return NULL;
  }
  va_end(args);

  va_start(args, fmt);
  ret += 1;
  buf.count = 0;
  if (buf.capacity < buf.count + ret) {
    if (buf.capacity == 0) {
      buf.capacity = 64;
    }
    while (buf.capacity < buf.count + ret) {
      buf.capacity = buf.capacity * 2;
    }
    buf.elements = realloc(buf.elements, sizeof(buf.elements) * buf.capacity);
    if (buf.elements == NULL) {
      fprintf(stderr, "Failed temp allocation: %s: %d\n", __FILE__, __LINE__);
      abort();
    }
  }
  memcpy(buf.elements + buf.count, "\0", sizeof(buf.elements) * ret);
  buf.count += ret;
  vsnprintf(buf.elements, ret, fmt, args);
  va_end(args);
  return buf.elements;
}
