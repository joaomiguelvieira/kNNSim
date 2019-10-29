#include "Parser.h"

Parser *newParser(int argc, char **argv) {
  Parser *parser = (Parser *) malloc(sizeof(Parser));
  assert(parser != NULL);

  parser->argList = newSinglyLinkedList((void (*)(void *)) &deleteArgument);
  parser->argc = argc;
  parser->argv = argv;

  return parser;
}

void deleteParser(Parser *parser) {
  deleteSinglyLinkedList(parser->argList);
  free(parser);
}

void addArgumentToParser(Parser *parser, int position, char *fullname, char *shortname, char *type, char *description, char *defaultValue, void *dest, int required) {
  Argument *arg = newArgument(fullname, shortname, type, position, description, defaultValue, dest, required);
  insertObjectIntoSinglyLinkedListAtTail(parser->argList, arg);
}

void dumpParser(Parser *parser) {
  printf("\033[1m[PARSER DUMP]:\033[0m\n");
  for (SinglyLinkedListNode *node = getHeadSinglyLinkedList(parser->argList); node != NULL; node = getNextNodeSinglyLinkedList(node)) {
    printf("   |_ ");
    dumpArgument((Argument *) getObjectFromSinglyLinkedListNode(node));
  }
}

Argument *getPositionalArgument(Parser *parser, int position) {
  for (SinglyLinkedListNode *node = getHeadSinglyLinkedList(parser->argList); node != NULL; node = getNextNodeSinglyLinkedList(node)) {
    Argument *arg = (Argument *) getObjectFromSinglyLinkedListNode(node);

    if (arg->position == position)
      return arg;
  }

  return NULL;
}

int parseAllArguments(Parser *parser) {
  for (SinglyLinkedListNode *node = getHeadSinglyLinkedList(parser->argList); node != NULL; node = getNextNodeSinglyLinkedList(node)) {
    Argument *arg = getObjectFromSinglyLinkedListNode(node);

    // the argument is optional
    if (arg->position < 0) {
      int argFound = 0;

      for (int i = 1; i < parser->argc; i++) {
        if (!strcmp(arg->fullname, parser->argv[i]) || !strcmp(arg->shortname, parser->argv[i])) {
          parseArgument(arg, parser->argv[i + 1]);
          break;
        }
      }

      if (!argFound && arg->required)
        return 1;
    }
    // the argument is positional
    else {
      if (arg->position >= parser->argc)
        return 1;

      parseArgument(arg, parser->argv[arg->position]);
    }
  }

  return 0;
}

void usage(Parser *parser) {
  printf("\033[1m[USAGE]:\033[0m %s", parser->argv[0]);

  for (int i = 1;; i++) {
    Argument *arg = getPositionalArgument(parser, i);

    if (!arg)
      break;

    printf(" <%s>", arg->fullname);
  }

  printf(" [options]\n");

  for (int i = 1;; i++) {
    Argument *arg = getPositionalArgument(parser, i);

    if (!arg)
      break;

    printf("|_ \033[1m%s:\033[0m %s\n", arg->fullname, arg->description);
  }

  printf("|_ \033[1moptions:\033[0m\n");

  for (SinglyLinkedListNode *node = getHeadSinglyLinkedList(parser->argList); node != NULL; node = getNextNodeSinglyLinkedList(node)) {
    Argument *arg = getObjectFromSinglyLinkedListNode(node);

    if (arg->position < 0) {
      printf("   |_ \033[1m%s, %s:\033[0m %s (default=%s)\n", arg->fullname, arg->shortname, arg->description, arg->defaultValue);
    }
  }
}
