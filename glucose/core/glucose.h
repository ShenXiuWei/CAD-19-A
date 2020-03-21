#include <errno.h>

#include <signal.h>
#include <zlib.h>
#include <sys/resource.h>
#include <stdint.h>
#include <limits>

#include "../utils/System.h"
#include "../utils/ParseUtils.h"
#include "../utils/Options.h"
#include "../core/Dimacs.h"
#include "../simp/SimpSolver.h"

using namespace Glucose;

//=================================================================================================

static const char* _certified = "CORE -- CERTIFIED UNSAT";

void printStats(Solver& solver);



static Solver* solver;
// Terminate by notifying the solver and back out gracefully. This is mainly to have a test-case
// for this feature of the Solver as it may take longer than an immediate call to '_exit()'.
static void SIGINT_interrupt(int signum);

// Note that '_exit()' rather than 'exit()' has to be used. The reason is that 'exit()' calls
// destructors and may cause deadlocks if a malloc/free function happens to be running (these
// functions are guarded by locks for multithreaded use).
static void SIGINT_exit(int signum);


//=================================================================================================
// Main:

int glucose(int argc, char** argv);