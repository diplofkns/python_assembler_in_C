#include "pyas/chargroup.h"
#include "pyas/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unitest/unitest.h>

void check_inter_tests( void ) {
  test_suite("Tests of check_inter function, with different char intervals") ;

  char_group *c1 = char_group_new(0) ;
  check_inter("[a-z]", c1->group) ;
  test_oracle(char_group_print_cb(c1), "[97-122 ; ] -> 0", "[a-z]");
  char_group_delete_cb(c1);

  char_group *c2 = char_group_new(1) ;
  check_inter("[A-Za-z]", c2->group) ;
  test_oracle(char_group_print_cb(c2), "[65-90 ; 97-122 ; ] -> 1", "^[a-z]");
  char_group_delete_cb(c2);

  char_group *c3 = char_group_new(1) ;
  check_inter("[a\\-z]", c3->group) ;
  test_oracle(char_group_print_cb(c3), "[45 ; 97 ; 122 ; ] -> 1", "[a\\-z]");
  char_group_delete_cb(c3);

  char_group *c4 = char_group_new(1) ;
  check_inter("[\\\\\\\\]", c4->group) ;
  test_oracle(char_group_print_cb(c4), "[92 ; ] -> 1", "[\\\\\\\\]");
  char_group_delete_cb(c4);

  char_group *c5 = char_group_new(1) ;
  check_inter("[Y-A]", c5->group) ;
  test_oracle(char_group_print_cb(c5), "[] -> 1", "[Y-A]");
  char_group_delete_cb(c5);

  char_group *c6 = char_group_new(1) ;
  check_inter("[abc]", c6->group) ;
  test_oracle(char_group_print_cb(c6), "[97-99 ; ] -> 1", "[abc]");
  char_group_delete_cb(c6);

  char_group *c7 = char_group_new(1) ;
  check_inter("[\\]]", c7->group) ;
  test_oracle(char_group_print_cb(c7), "[93 ; ] -> 1", "[\\]]");
  char_group_delete_cb(c7);

}

void char_group_parse_tests( void ) {
  test_suite("Tests of char_group_parse, with different regular expressions") ;

  list_t l = list_new() ;

  test_oracle(list_print(char_group_parse("a+ze[a-z]*"), char_group_print_cb), "( [97 ; ] -> 2 [122 ; ] -> 0 [101 ; ] -> 0 [97-122 ; ] -> 1 )", "a+ze[a-z]*") ;

  test_oracle(list_print(char_group_parse("^[a-z]?"), char_group_print_cb), "( [0-96 ; 123-255 ; ] -> 3 )", "^[a-z]?") ;

  test_oracle(list_print(char_group_parse("[\\]]"), char_group_print_cb), "( [93 ; ] -> 0 )", "[\\]]") ;

  test_oracle(list_print(char_group_parse("az\\[\\]"), char_group_print_cb), "( [97 ; ] -> 0 [122 ; ] -> 0 [91 ; ] -> 0 [93 ; ] -> 0 )", "az\\[\\]") ;

  test_oracle(list_print(char_group_parse("^[a-zA-Z]+^[0-9]?"), char_group_print_cb), "( [0-64 ; 91-96 ; 123-255 ; ] -> 2 [0-47 ; 58-255 ; ] -> 3 )", "^[a-zA-Z]+^[0-9]?") ;

  test_oracle(list_print(char_group_parse(".*"), char_group_print_cb), "( [0-255 ; ] -> 1 )", ".*") ;

}

int main ( int argc, char *argv[] ) {

  // unit_test( argc, argv );
  //
  // check_inter_tests();
  //
  // char_group_parse_tests() ;

  list_print(char_group_parse("^\\"), char_group_print_cb);

  exit( EXIT_SUCCESS );
}
