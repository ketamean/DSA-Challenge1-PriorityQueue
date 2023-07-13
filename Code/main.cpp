#include "structures.h"
#include "auxiliary.h"

// argv[0] = executable file name
int main () {
    string cmd_str;
    char* argv[6];
    char* cmd_cstr;
    int argc;

    bool continue_doing_task;
    while (1) {
        getline(cin, cmd_str);
        strcpy(cmd_cstr, cmd_str.c_str());
        generate_command_arguments(cmd_cstr, argc, argv);
        continue_doing_task = do_the_task(argc, argv);
        if (continue_doing_task == false) break;
    }

    // deallocate
    for (int i = 0; i < global_variables::regular_room.size(); ++i) {
        ns_priority_queue::delete_all_priority_queue(global_variables::regular_room[i]->patients);
    }
    for (int i = 0; i < global_variables::vip_room.size(); ++i) {
        ns_priority_queue::delete_all_priority_queue(global_variables::vip_room[i]->patients);
    }
}