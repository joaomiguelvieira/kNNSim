#ifndef __ARGUMENT_HEADER__
#define __ARGUMENT_HEADER__

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "Common.h"

typedef enum {integer, string, boolean} argType;

typedef struct Argument_ {
  char fullname[BUFLEN];
  char shortname[BUFLEN];
  char type[BUFLEN];
  char description[BUFLEN];
  char defaultValue[BUFLEN];
  int position;
  int required;
  void *dest;
} Argument;

Argument *newArgument(char *fullname, char *shortname, char *type, int position, char *description, char *defaultValue, void *dest, int required);
void deleteArgument(Argument *arg);
void parseArgument(Argument *arg, char *value);
void dumpArgument(Argument *arg);

#endif
