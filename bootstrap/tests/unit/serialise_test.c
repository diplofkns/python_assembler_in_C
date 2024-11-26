#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pyas/serialise.h"

int main(){
  FILE* f = fopen("results/f.bin","ab");

  pyobj_t p1 = pyobj_new('s');
  pyobj_t p2 = pyobj_new('[');
  pyobj_t p3 = pyobj_new('i');
  pyobj_t p4 = pyobj_new('g');
  pyobj_t p5 = pyobj_new('y');
  pyobj_t p6 = pyobj_new('0');
  pyobj_t p7 = pyobj_new('N');
  pyobj_t p8 = pyobj_new('F');
  pyobj_t p9 = pyobj_new('T');

  p1->py.string.buffer = calloc(4,sizeof(char));
  p1->py.string.buffer[0] = 'A';
  p1->py.string.buffer[1] = 'M';
  p1->py.string.buffer[2] = 'B';
  p1->py.string.buffer[3] = '\0';
  p1->py.string.length = 3;

  p3->py.number.integer = 99;

  p4->py.number.real = 3.1415;

  p5->py.number.complex.real = 34.797;
  p5->py.number.complex.imag = 299792458;

  p2->py.list.value = calloc(6,sizeof(pyobj_t));
  p2->py.list.size = 6;
  p2->py.list.value[0] = p1;
  p2->py.list.value[1] = p7;
  p2->py.list.value[2] = p3;
  p2->py.list.value[3] = p4;
  p2->py.list.value[4] = p5;
  p2->py.list.value[5] = p6;

  pyobj_write(f,p1);
  // pyobj_write(f,p2);
  // pyobj_write(f,p3);
  // pyobj_write(f,p4);
  // pyobj_write(f,p5);
  // pyobj_write(f,p6);
  // pyobj_write(f,p7);
  // pyobj_write(f,p8);
  // pyobj_write(f,p9);

  fclose(f);
}
