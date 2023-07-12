#include "structures.h"
#include "auxiliary.h"

// argv[0] = executable file name
int main () {
    PriorityQueue* PQ;
    string cmd_str;
    char* argv[6];
    char* cmd_cstr;
    int argc;

    bool continue_doing_task;
    while (1) {
        getline(cin, cmd_str);

        // halt condition

        strcpy(cmd_cstr, cmd_str.c_str());
        generateCommandArguments(cmd_cstr, argc, argv);
        continue_doing_task = doTheTask(PQ, argc, argv);

        if (continue_doing_task == false) break;
    }

    // deallocate
    deletePQ(PQ);
}