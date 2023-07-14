#include "implementation.cpp"

// argv[0] = executable file name
int main()
{
    string cmd_str;
    char *argv[6];
    char cmd_cstr[100];
    int argc;

    bool continue_doing_task;
    while (1)
    {
        cout << "> ";
        getline(cin, cmd_str);

        strcpy(cmd_cstr, cmd_str.c_str());
        generate_command_arguments(cmd_cstr, argc, argv);

#if DEBUG
        cout << "argc = " << argc << endl;
        cout << "argvs: ";
        for (int i = 0; i < argc; i++)
        {
            cout << argv[i] << ", ";
        }
        cout << endl;
#endif

        continue_doing_task = do_the_task(argc, argv, false);

        // deallocate argv[i]
        while (argc > 0)
        {
            argc--;
            delete[] argv[argc];
        }

        if (continue_doing_task == false)
            break;
    }

    // deallocate
    for (int i = 0; i < global_variables::regular_room.size(); ++i)
    {
        ns_priority_queue::delete_all_priority_queue(global_variables::regular_room[i]->patients);
        delete global_variables::regular_room[i];
    }
    for (int i = 0; i < global_variables::vip_room.size(); ++i)
    {
        ns_priority_queue::delete_all_priority_queue(global_variables::vip_room[i]->patients);
        delete global_variables::vip_room[i];
    }
}