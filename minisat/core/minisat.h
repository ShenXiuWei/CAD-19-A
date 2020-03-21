#include <errno.h>

#include <signal.h>
#include <zlib.h>

#include "../utils/System.h"
#include "../utils/ParseUtils.h"
#include "../utils/Options.h"
#include "../core/Dimacs.h"
#include "../core/Solver.h"

using namespace Minisat;

void printStats(Solver& solver);

static Solver* solver;

static void SIGINT_exit(int signum) {
    printf("\n"); printf("*** INTERRUPTED ***\n");
    if (solver->verbosity > 0){
        printStats(*solver);
        printf("\n"); printf("*** INTERRUPTED ***\n"); }
    _exit(1); }
	
int minisat(int argc, char** argv);