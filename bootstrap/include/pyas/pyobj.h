#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef unsigned char pyobj_type ;

struct pyobj_t ;
typedef struct pyobj *pyobj_t ;

typedef struct {
  int version_pyvm ;
  union {
    struct {
      uint32_t arg_count ;
      uint32_t local_count ;
      uint32_t stack_size ;
      uint32_t flags ;
    } _0 ; /* Up to 3.7 (2.7 , etc .) */
    struct {
      uint32_t arg_count ;
      uint32_t posonly_arg_count ;
      uint32_t kwonly_arg_count ;
      uint32_t local_count ;
      uint32_t stack_size ;
      uint32_t flags ;
    } _1 ; /* From 3.8 onwards */
  } header ;
  pyobj_t parent ;
  struct {
    union {
      struct {
        uint32_t magic ;
        time_t timestamp ;
      } _0 ;
      struct {
        uint32_t magic ;
        time_t timestamp ;
        uint32_t source_size ;
      } _1 ;
      struct {
        uint32_t magic ;
        uint32_t bitfield ;
        time_t timestamp ;
        uint32_t source_size ;
        uint64_t hash ;
      } _2 ;
    } header ;
    struct {
      pyobj_t interned ;
      pyobj_t bytecode ;
      pyobj_t consts ;
      pyobj_t names ;
      pyobj_t varnames ;
      pyobj_t freevars ;
      pyobj_t cellvars ;
    } content ;
    struct {
      pyobj_t filename ;
      pyobj_t name ;
      uint32_t firstlineno ;
      pyobj_t lnotab ;
    } trailer ;
  } binary ;
} py_codeblock ;

struct pyobj {
  char type ;
  unsigned int refcount;
  union {
    struct {
      pyobj_t *value;
      int32_t size;
    } list ;

    struct {
      char *buffer;
      int length;
    } string;
    py_codeblock *codeblock;
    union {
      int32_t integer;
      int64_t integer64;
      double real;
      struct {
        double real;
        double imag;
      } complex ;
    } number ;
  } py;
};


pyobj_t pyobj_new(char type);
void pyobj_delete(pyobj_t p);
void code_block_delete(pyobj_t p);
void pyobj_print(pyobj_t p);
void codeblock_print(pyobj_t p);
