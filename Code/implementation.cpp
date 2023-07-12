// this part contains implementation for all functions in `structures.h`
#include "structures.h"

short comparePriority(Patient* p1, Patient* p2) {
    if (p1->prior_ord_emergency > p2->prior_ord_emergency) return 1;
    else if (p1->prior_ord_emergency > p2->prior_ord_emergency) return -1;

    if (p1->prior_ord_old > p2->prior_ord_old) return 1;
    else if (p1->prior_ord_old > p2->prior_ord_old) return -1;

    if (p1->prior_ord_children > p2->prior_ord_children) return 1;
    else if (p1->prior_ord_children > p2->prior_ord_children) return -1;

    if (p1->prior_ord_normal > p2->prior_ord_normal) return 1;
    return -1;
}

bool isEmpty(PriorityQueue* pq) {
    if (pq->top == nullptr) return true;
    return false;
}
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

void generate_command_arguments(char cmd[], int &argc, char* argv[]) {
    argc = 0;
    char* pch = strtok(cmd, " ");
    while (pch != nullptr) {
        strcpy(argv[argc++], pch);
        pch = strtok(nullptr, " ");
    }
}

void open_medical_room(int number_of_regular_room, int number_of_vip_room) {

}

void preorder_print(Node* patient) {
    if (patient == nullptr) return;

    // print 1 patient
    cout << patient->patient->name << " " << patient->patient->year_of_birth << endl;
    preorder_print(patient->left);
    preorder_print(patient->right);
}

void print_patient_list(char type_of_room, int ord_number_of_the_room) {
    // print all the patients using pre-order traversal
    cout << "Room " << (char)type_of_room << ord_number_of_the_room << ":" << endl;
    if (type_of_room == 'R') {
        // out of range of the list of regular medical room
        if (ord_number_of_the_room < 1 || ord_number_of_the_room > var::regular_room.size()) return;
        
        ord_number_of_the_room--;
        preorder_print(var::regular_room[ord_number_of_the_room]->patients->top);
    } else {
        // out of range of the list of vip medical room
        if (ord_number_of_the_room < 1 || ord_number_of_the_room > var::vip_room.size()) return;

        ord_number_of_the_room--;
        preorder_print(var::vip_room[ord_number_of_the_room]->patients->top);
    }
}

void get_room_info(string room, char &type_of_room, int &no_of_room) {
    type_of_room = room[0];
    room.erase(room.begin());
    no_of_room = stoi(room);
}

bool do_the_task(int argc, const char* argv[]) {
    string tmp_task = string(argv[0]);
    TASK task = getTask(tmp_task);
    switch (task) {
        case Open:
            {
                if (argc == 3) {
                    // > Open x y
                    open_medical_room(stoi(string(argv[1])), stoi(string(argv[2])));
                } else {
                    // > Open x
                    open_medical_room(stoi(string(argv[1])), 0);
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
                        generate_command_arguments(tmp_cstr, file_argc, file_argv);
                        do_the_task(file_argc, file_argv);
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
                
                if (strcmp(argv[1], "All") == 0) {
                    // > See All
                    for (int i = 0; i < var::regular_room.size(); ++i) {
                        print_patient_list(var::regular_room[i]->type, var::regular_room[i]->no);
                    }

                    for (int i = 0; i < var::vip_room.size(); ++i) {
                        print_patient_list(var::vip_room[i]->type, var::vip_room[i]->no);
                    }
                } else {
                    // > See medical_room_ID
                    char type_of_room;
                    int no_of_room;
                    get_room_info(string(argv[1]), type_of_room, no_of_room);
                    print_patient_list(type_of_room, no_of_room);
                }
            }
        
        case Update:
            {
                // > Update medical_room_id _number_of_finished_treatments
                int _number_of_finished_treatments = stoi(string(argv[2]));
                int i = 0;
                string room = string(argv[1]);
                int no_of_room;     // [1..n]
                char type_of_room;  
                get_room_info(room, type_of_room, no_of_room);
                if (type_of_room == 'R') {
                    if (no_of_room <= var::regular_room.size()) {
                        while (i < _number_of_finished_treatments && isEmpty(var::regular_room[no_of_room-1]->patients) == false) {
                            deletion(var::regular_room[no_of_room-1]->patients);
                            i++;
                        }
                    }
                } else {
                    if (no_of_room <= var::vip_room.size()) {
                        while (i < _number_of_finished_treatments && isEmpty(var::vip_room[no_of_room-1]->patients) == false) {
                            deletion(var::vip_room[no_of_room-1]->patients);
                            i++;
                        }
                    }
                }
            }

        default:
            {
                // > Exit
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