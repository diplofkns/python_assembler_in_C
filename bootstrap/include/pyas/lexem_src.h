#include "pyas/lexem.h"
#include "pyas/lex_def.h"
#include "pyas/list.h"
#include "pyas/queue.h"
#include "pyas/regexp_char_group.h"
#include "pyas/read.h"

#define MAX_LENGTH 500

list_t lex (char *source_file, char *regexp_file);
