// this part contains implementation for all functions in `structures.h`
#include "structures.h"


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// this part contains implementation for all functions in `auxiliary.h`
#include "auxiliary.h"

void generateCommandArguments(char cmd[], int &argc, char* argv[]) {
    argc = 0;
    char* pch = strtok(cmd, " ");
    while (pch != nullptr) {
        strcpy(argv[argc++], pch);
        pch = strtok(nullptr, " ");
    }
}

bool doTheTask(PriorityQueue* &PQ, int argc, const char* argv[]) {
    string tmp_task = string(argv[0]);
    TASK task = getTask(tmp_task);
    switch (task) {
        case Open:
            {
                if (argc == 3) {
                    // > Open x y
                } else {
                    // > Open x
                }
            }

        case New:
            {   
                if (argc == 2) {
                    // > New filename
                    ifstream ifs;
                    ifs.open(string(argv[2]));
                    string tmp_str;
                    char* file_argv[6], *tmp_cstr;
                    int file_argc;
                    while (!ifs.eof()) {
                        getline(ifs, tmp_str);
                        strcpy(tmp_cstr, tmp_str.c_str());
                        generateCommandArguments(tmp_cstr, file_argc, file_argv);
                        doTheTask(PQ, file_argc, file_argv);
                    }
                    ifs.close();
                } else if (argc == 3) {
                    // > New Name Year_of_birth
                } else if (argc == 4) {
                    if (argv[1][0] == 'e') {
                        // > New e Name Year_of_birth
                    } else {
                        // > New v Name Year_of_birth
                    }
                } else {
                    // > New e v Name Year_of_birth
                }
            }
        
        case See:
            {

            }
        
        case Update:
            {

            }

        default:
            {
                return 0;
            }
    }
    return 1;
}

TASK getTask(string task) {
    if (task == "New") return New;
    if (task == "See") return See;
    if (task == "Update") return Update;
    if (task == "Open") return Open;
    return Exit;
}