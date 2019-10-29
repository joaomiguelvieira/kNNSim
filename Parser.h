#ifndef __PARSER_HEADER__
#define __PARSER_HEADER__

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "SinglyLinkedList.h"
#include "Argument.h"
#include "Common.h"

typedef struct Parser_ {
  int argc;
  char **argv;
  SinglyLinkedList *argList;
} Parser;

Parser *newParser(int argc, char **argv);
void deleteParser(Parser *parser);

void addArgumentToParser(Parser *parser, int position, char *fullname, char *shortname, char *type, char *description, char *defaultValue, void *dest, int required);

int parseAllArguments(Parser *parser);

void dumpParser(Parser *parser);
void usage(Parser *parser);

#endif
