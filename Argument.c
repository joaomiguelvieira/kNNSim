#include "Argument.h"

Argument *newArgument(char *fullname, char *shortname, char *type, int position, char *description, char *defaultValue, void *dest, int required) {
  Argument *arg = (Argument *) malloc(sizeof(Argument));
  assert(arg != NULL);

  strcpy(arg->fullname, fullname);
  strcpy(arg->shortname, shortname);
  strcpy(arg->type, type);
  strcpy(arg->description, description);
  arg->position = position;
  arg->dest = dest;
  arg->required = required;

  if (defaultValue) {
    parseArgument(arg, defaultValue);
    strcpy(arg->defaultValue, defaultValue);
  }

  return arg;
}

void deleteArgument(Argument *arg) {
  free(arg);
}

void parseArgument(Argument *arg, char *value) {
       if (!strcmp(arg->type, "integer")) *((int *) arg->dest) = atoi(value);
  else if (!strcmp(arg->type, "float"))   *((float *) arg->dest) = atof(value);
  else if (!strcmp(arg->type, "string"))  strcpy((char *) arg->dest, value);
}

void dumpArgument(Argument *arg) {
  if (arg->position != -1)
    printf("\033[1mPosition:\033[0m %d; \033[1mType:\033[0m %s\n", arg->position, arg->type);
  else
    printf("\033[1mFullname:\033[0m %s; \033[1mShortname:\033[0m %s; \033[1mType:\033[0m %s\n", arg->fullname, arg->shortname, arg->type);
}
