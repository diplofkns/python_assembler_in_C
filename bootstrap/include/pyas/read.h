#include "pyas/list.h"
#include "pyas/queue.h"
#include "pyas/lexem.h"
#include "pyas/lex_def.h"
#include "pyas/regexp_char_group.h" //re_match

#define MAX_LENGTH2 50

list_t read(char *regexp_file);
