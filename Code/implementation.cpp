// this part contains implementation for all functions in `structures.h`
#include "structures.h"
using namespace ns_priority_queue;

short ns_priority_queue::compare_priority(Patient* p1, Patient* p2) {
    // p1 and p2 are both either vip or non-vip

    if (p1->prior_ord_emergency > p2->prior_ord_emergency)
        return 1;
    else if (p1->prior_ord_emergency < p2->prior_ord_emergency)
        return -1;
    // else p1->prior_ord_emergency == p2->prior_ord_emergency == -1

    if (p1->prior_ord_old > p2->prior_ord_old)
        return 1;
    else if (p1->prior_ord_old < p2->prior_ord_old)
        return -1;
    // else p1->prior_ord_old == p2->prior_ord_old == -1

    if (p1->prior_ord_children > p2->prior_ord_children)
        return 1;
    else if (p1->prior_ord_children < p2->prior_ord_children)
        return -1;
    // else p1->prior_ord_children == p2->prior_ord_children == -1

    // if p1 and p2 are none of above, they must be prior_normal
    if (p1->prior_ord_normal > p2->prior_ord_normal)
        return 1;
    else if (p1->prior_ord_normal < p2->prior_ord_normal)
        return -1;
    
    // this case will never occur
    return 0;
}

Node* ns_priority_queue::merge1(Node *h1, Node *h2) {
    if (h1->left == nullptr) h1->left = h2;
    else {
        h1->right = ns_priority_queue::merge(h1->right, h2);
        if (h1->left->npl < h1->right->npl) {
            swap(h1->left, h1->right);
            h1->npl = h1->right->npl + 1;
        }
    }
    return h1;
}

Node* ns_priority_queue::merge(Node *h1, Node *h2) {
    if (h1 == nullptr)
        return h2;
    else if (h2 == nullptr)
        return h1;
    else if ((ns_priority_queue::compare_priority(h2->patient, h1->patient)) == -1)
        return ns_priority_queue::merge1(h1, h2);
    else
        return ns_priority_queue::merge1(h2, h1);
}

void ns_priority_queue::insertion(PriorityQueue* &pq, Patient* item) {
    Node* node = new Node;
    node->patient = item;
    node->left = node->right = nullptr;
    node->npl = 0;
    pq->top = ns_priority_queue::merge(pq->top, node);
    pq->total_patients++;
}

void ns_priority_queue::deletion(PriorityQueue* &pq) { 
    if (pq->top) {
        Node* tmp = pq->top;
        pq->top = ns_priority_queue::merge(pq->top->left, pq->top->right);
        delete tmp->patient;
        delete tmp;
        pq->total_patients--;
    }
}

Patient* ns_priority_queue::peak(PriorityQueue* pq) {
    if (pq->top) return pq->top->patient;
    return nullptr;
}

bool ns_priority_queue::is_empty(PriorityQueue* pq) {
    if (pq->top == nullptr) return true;
    return false;
}

void ns_priority_queue::delete_all_priority_queue(PriorityQueue* &pq) {
    while (!ns_priority_queue::is_empty(pq)) ns_priority_queue::deletion(pq);
        delete pq;
    pq = nullptr;
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
    global_variables::regular_room.reserve(number_of_regular_room);
    global_variables::vip_room.reserve(number_of_vip_room);
}

void preorder_print(Node* p, char &type_of_room, int &ord_number_of_the_room) {
    if (p == nullptr)
        return;

    // print 1 patient
    cout << p->patient->name << " " << p->patient->year_of_birth << " " << type_of_room << ord_number_of_the_room << endl;
    preorder_print(p->left, type_of_room, ord_number_of_the_room);
    preorder_print(p->right, type_of_room, ord_number_of_the_room);
}

void print_patient_list(char type_of_room, int ord_number_of_the_room) {
    // print all the patients using pre-order traversal
    cout << "Room " << (char)type_of_room << ord_number_of_the_room << ":" << endl;
    if (type_of_room == 'R') {
        // out of range of the list of regular medical room
        if (ord_number_of_the_room < 1 || ord_number_of_the_room > global_variables::regular_room.size())
            return;
        
        ord_number_of_the_room--;
        preorder_print(global_variables::regular_room[ord_number_of_the_room]->patients->top, type_of_room, ord_number_of_the_room);
    } else {
        // out of range of the list of vip medical room
        if (ord_number_of_the_room < 1 || ord_number_of_the_room > global_variables::vip_room.size())
            return;
        
        ord_number_of_the_room--;
        preorder_print(global_variables::vip_room[ord_number_of_the_room]->patients->top, type_of_room, ord_number_of_the_room);
    }
}

void get_room_info(string room, char &type_of_room, int &no_of_room) {
    type_of_room = room[0];
    room.erase(room.begin());
    no_of_room = stoi(room);
}

bool do_the_task(int argc, char* argv[]) {
    string tmp_task = string(argv[0]);
    TASK task = get_task(tmp_task);
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
            break;

        case New:
            {   
                if (argc == 2) {
                    // > New filename
                    ifstream ifs;
                    ifs.open(string(argv[2]));
                    string tmp_str;
                    char* file_argv[6], *tmp_cstr;
                    int file_argc;
                    
                    if (!ifs.eof()) {
                        cout << "Cannot open file " << string(argv[2]) << endl;
                        break;
                    }
                    while (!ifs.eof()) {
                        getline(ifs, tmp_str);
                        strcpy(tmp_cstr, tmp_str.c_str());
                        generate_command_arguments(tmp_cstr, file_argc, file_argv);
                        do_the_task(file_argc, file_argv);
                    }
                    ifs.close();
                } else if (argc == 3) {
                    // > New Name Year_of_birth
                    Patient* p = new Patient;
                    p->name = string(argv[1]);
                    p->year_of_birth = stoi(string(argv[2]));
                    p->age = CURRENT_YEAR - p->year_of_birth;
                    p->prior_ord_vip = false;
                    coordinate_patient_to_room(p, false);
                } else if (argc == 4) {
                    if (argv[1][0] == 'e') {
                        // > New e Name Year_of_birth
                        Patient* p = new Patient;
                        p->name = string(argv[2]);
                        p->year_of_birth = stoi(string(argv[3]));
                        p->age = CURRENT_YEAR - p->year_of_birth;
                        p->prior_ord_vip = false;
                        coordinate_patient_to_room(p, true);
                    } else {
                        // > New v Name Year_of_birth
                        Patient* p = new Patient;
                        p->name = string(argv[2]);
                        p->year_of_birth = stoi(string(argv[3]));
                        p->age = CURRENT_YEAR - p->year_of_birth;
                        p->prior_ord_vip = true;
                        coordinate_patient_to_room(p, false);
                    }
                } else {
                    // > New e v Name Year_of_birth
                    Patient* p = new Patient;
                    p->name = string(argv[3]);
                    p->year_of_birth = stoi(string(argv[4]));
                    p->age = CURRENT_YEAR - p->year_of_birth;
                    p->prior_ord_vip = true;
                    coordinate_patient_to_room(p, true);
                }
            }
            break;
        
        case See:
            {
                if (strcmp(argv[1], "All") == 0) {
                    // > See All
                    cout << "Regular medical room." << endl;
                    for (int i = 0; i < global_variables::regular_room.size(); ++i) {
                        print_patient_list(global_variables::regular_room[i]->type, global_variables::regular_room[i]->no);
                    }

                    if (global_variables::vip_room.size() > 0) {
                        cout << endl << "VIP medical room." << endl;
                    }
                    for (int i = 0; i < global_variables::vip_room.size(); ++i) {
                        print_patient_list(global_variables::vip_room[i]->type, global_variables::vip_room[i]->no);
                    }
                } else {
                    // > See medical_room_ID
                    char type_of_room;
                    int no_of_room;
                    get_room_info(string(argv[1]), type_of_room, no_of_room);
                    print_patient_list(type_of_room, no_of_room);
                }
            }
            break;
        
        case Update:
            {
                // > Update medical_room_id _number_of_finished_treatments
                int number_of_finished_treatments = stoi(string(argv[2]));
                int i = 0;
                string room = string(argv[1]);
                int no_of_room; // [1..n]
                char type_of_room;
                get_room_info(room, type_of_room, no_of_room);
                if (type_of_room == 'R') {
                    update_medical_room(global_variables::regular_room[no_of_room-1], number_of_finished_treatments);
                } else {
                    update_medical_room(global_variables::vip_room[no_of_room-1], number_of_finished_treatments);
                }
            }
            break;

        default:
            {
                // > Exit
                return 0;
            }
    }
    return 1;
}

TASK get_task(string task) {
    if (task == "New") return New;
    if (task == "See") return See;
    if (task == "Update") return Update;
    if (task == "Open") return Open;
    return Exit;
}

void coordinate_patient_to_room(Patient* p, bool is_emergency) {
    // allocate a new Node
    Node* new_item = new Node;
    new_item->patient = p;
    new_item->left = new_item->right = nullptr;

    // choose an approriate room
    Room* room_to_add = choose_room_for_new_patient(p);

    if (is_emergency) {
        p->prior_ord_emergency = room_to_add->ord_emergency++;
    } else if (p->age >= 60) {
        p->prior_ord_old = room_to_add->ord_old++;
    } else if (p->age <= 10) {
        p->prior_ord_children = room_to_add->ord_children++;
    } else {
        p->prior_ord_normal = room_to_add->ord_normal++;
    }
    insertion(room_to_add->patients, p);
}

Room* choose_room_for_new_patient(Patient* p) {
    if (p->prior_ord_vip) {
        // traverse in the list of VIP ROOM
    } else {
        // traverse in the list of REGULAR ROOM
    }
}

void update_medical_room(Room* room, int number_of_finished_patients) {
    while (number_of_finished_patients--) {
        ns_priority_queue::deletion(room->patients);
    }
}