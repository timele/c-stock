/* main.c */
#include <errno.h>
#include <getopt.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include "stock/stock.h"

#define OPTSTR "vi:o:h"
#define USAGE_FMT "%s [-v] [-i inputfile] [-o outputfile] [-h]\n"
#define ERR_FOPEN_INPUT "fopen(input, r)"
#define ERR_FOPEN_OUTPUT "fopen(output, w)"
#define ERR_STOCK_OPERATION "stock_operation failed"
#define DEFAULT_PROGNAME "stock"

typedef struct {
  int verbose;
  bool console;
  FILE *input;
  FILE *output;
} options_t;

static options_t global_options;

void usage(char *progname, int opt);
void interrupt_handler(int s);
int stock_operation(options_t *options);

int main(int argc, char *argv[]) {
  int opt;
  options_t options = {0, 0x0, stdin, stdout};
  opterr = 0;

  while ((opt = getopt(argc, argv, OPTSTR)) != EOF)
    switch (opt) {
    // case 'c':
    //   options.console = true;
    //   break;
    case 'i':
      if (!(options.input = fopen(optarg, "r"))) {
        perror(ERR_FOPEN_INPUT);
        exit(EXIT_FAILURE);
        /* NOTREACHED */
      }
      break;
    case 'o':
      if (!(options.output = fopen(optarg, "w"))) {
        perror(ERR_FOPEN_OUTPUT);
        exit(EXIT_FAILURE);
        /* NOTREACHED */
      }
      break;
    case 'v':
      options.verbose += 1;
      break;
    case 'h':
    default:
      usage(basename(argv[0]), opt);
      /* NOTREACHED */
      break;
    }

  global_options = options;
  set_log_verbosity(global_options.verbose);

  DEBUG_V("Log level: [%d]\n", VERBOSITY_LEVEL);

  // attach interrupt handler
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = interrupt_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);


  if (stock_operation(&options) != EXIT_SUCCESS) {
    perror(ERR_STOCK_OPERATION);
    exit(EXIT_FAILURE);
    /* NOTREACHED */
  }

  return EXIT_SUCCESS;
}

void usage(char *progname, int opt) {
  fprintf(stderr, USAGE_FMT, progname ? progname : DEFAULT_PROGNAME);
  exit(EXIT_FAILURE);
  /* NOTREACHED */
}

void interrupt_handler(int s) {
  fprintf(stderr, "Interrupted. Caught signal %d\n", s);
  // close open files
  if(global_options.input) {
    fclose(global_options.input);
  }
  if(global_options.output) {
    fclose(global_options.output);
  }
  // cleanup stock
  if(static_global_stock != NULL) {
    destroy_stock(&static_global_stock);
  }
  exit(EXIT_SUCCESS);
}

int stock_operation(options_t *options) {
  if (!options) {
    errno = EINVAL;
    perror("No options provided.");
    return EXIT_FAILURE;
  }

  run_stock(options->input, options->output);

  fclose(options->input);
  fclose(options->output);

  return EXIT_SUCCESS;
}
