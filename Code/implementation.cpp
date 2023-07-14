// this part contains implementation for all functions in `structures.h`
#include "structures.h"
using namespace ns_priority_queue;

#define DEBUG 0

short ns_priority_queue::compare_priority(Patient* p1, Patient* p2) {
    // p1 and p2 are both either vip or non-vip

    if ((p1->prior_ord_emergency) ^ (p2->prior_ord_emergency)) {
        // if one of them is emergency and the other is not
        if (p1->prior_ord_emergency && !p2->prior_ord_emergency) {
            // p1 is emergency and p2 is not
            return 1;
        } else {
            // p2 is emergency and p1 is not
            return -1;
        }
    }
    // else both of them or none of them is emergency

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

Node *ns_priority_queue::merge(Node * h1, Node * h2)
{
    if (h1 == nullptr)
        return h2;
    if (h2 == nullptr)
        return h1;
    if (h1->patient < h2->patient)
        return merge1(h1, h2);
    else
        return merge1(h2, h1);
}
 
Node *ns_priority_queue::merge1(Node * h1, Node * h2)
{
    if (h1->left == nullptr)
        h1->left = h2;
    else
    {
        h1->right = merge(h1->right, h2);
        if (h1->left->npl < h1->right->npl)
            swapChildren(h1);
        h1->npl = h1->right->npl + 1;
    }
    return h1;
}
 
void ns_priority_queue::swapChildren(Node *t)
{
    Node *tmp = t->left;
    t->left = t->right;
    t->right = tmp;
}
 
void ns_priority_queue::insertion(PriorityQueue *&pq, Patient *item)
{
    if (pq == nullptr) pq = new PriorityQueue;
    Node *new_patient = new Node;
    new_patient->patient = item;
    new_patient->left = new_patient->right = NULL;
    new_patient->npl = 0;
    pq->top = ns_priority_queue::merge(pq->top, new_patient);
    pq->total_patients++;
}
 
Patient *ns_priority_queue::peak(PriorityQueue *pq)
{
    if (pq)
        return pq->top->patient;
    else
        return nullptr;
}
 
void ns_priority_queue::deletion(PriorityQueue *&pq)
{
    if (pq == nullptr || pq->top == nullptr) return;
    Node *oldRoot = pq->top;
    pq->top = merge(pq->top->left, pq->top->right);
    pq->total_patients--;
    delete oldRoot->patient;
    delete oldRoot;
}

bool ns_priority_queue::is_empty(PriorityQueue *pq) 
{
    if (!pq || !pq->top) return true;
    else return false;
}

void ns_priority_queue::delete_all_priority_queue(PriorityQueue *&pq) 
{
    while (!is_empty(pq)) deletion(pq);
    delete pq;
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

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void generate_command_arguments(char cmd[], int &argc, char* argv[]) {
    argc = 0;
    char* pch = strtok(cmd, " ");
    while (pch != nullptr) {
        argv[argc++] = pch;
        pch = strtok(nullptr, " ");
    }
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void open_medical_room(int number_of_regular_room, int number_of_vip_room) {
    global_variables::regular_room.reserve(number_of_regular_room);
    Room* r = nullptr;
    for (int i = 0; i < number_of_regular_room; ++i) {
        r = new Room;
        r->type = 'R';
        r->no = i+1;
        global_variables::regular_room.push_back(r);
    }

    global_variables::vip_room.reserve(number_of_vip_room);
    for (int i = 0; i < number_of_vip_room; ++i) {
        r = new Room;
        r->type = 'V';
        r->no = i+1;
        global_variables::vip_room.push_back(r);
    }
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void print_one_patient(Patient* p) {
    cout << "   " << setw(30) << left << p->name;
    if (p->prior_ord_vip) {
        cout << setw(11) << left << "VIP";
    } else {
        cout << setw(11) << left << "non-VIP";
    }

    if (p->prior_ord_emergency) {
        cout << setw(21) << left << "Emergency";
    } else if (p->prior_ord_old >= 0) {
        cout << setw(21) << left << "Old";
    } else if (p->prior_ord_children >= 0) {
        cout << setw(21) << left << "Children";
    } else {
        cout << setw(21) << left << "Normal";
    }
    cout << p->room->type << p->room->no << endl;
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void preorder_print(Node* p) {
    if (p == nullptr) {
        return;
    }
    // print all the patients using pre-order traversal in binary tree
    print_one_patient(p->patient);
    preorder_print(p->left);
    preorder_print(p->right);
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void print_patient_list(Room* room) {
    cout << "Room " << (char)room->type << room->no << ":" << endl;
    cout << "   " << setfill('.') << setw(45) << left << "Name"
         << setfill('.') << setw(11) << left << "Is VIP"
         << setfill('.') << setw(21) << left << "Priority"
                                     << left << "Room"
                                     << setfill(' ') << endl;
    if (room->patients)
        preorder_print(room->patients->top);
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
Room* get_room(string room) {
    char type_of_room = room[0];
    room.erase(room.begin());
    if (type_of_room == 'R') {
        return global_variables::regular_room[stoi(room)-1];
    } else {
        return global_variables::vip_room[stoi(room)-1];
    }
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
bool do_the_task(int argc, char* argv[], bool print_new_patient) {
    string tmp_task = string(argv[0]);
    TASK task = get_task(tmp_task);
    switch (task) {
        case Open:
            {
                if (argc == 3) {
                    // > Open x y
                    open_medical_room(stoi(string(argv[1])), stoi(string(argv[2])));
                } else if (argc == 2) {
                    // > Open x
                    open_medical_room(stoi(string(argv[1])), 0);
                }
            }
            break;

        case New:
            {   
                enum CASE {NEW_E, NEW_V, NEW_E_V, NEW_};
                CASE status;
                Patient* p = nullptr;
                if (print_new_patient) { // command `New filename` recalls function `do_the_task()`
                    if (argc == 3) {
                        // > New name year_of_birth
                        status = NEW_;
                    } else if (argc == 5) {
                        // > New e v name year_of_birth
                        status = NEW_E_V;
                    } else { // argc == 4
                        if (argv[1][0] == 'e') {
                            // > New e name year_of_birth
                            status = NEW_E;
                        } else {
                            // > New v name year_of_birth
                            status = NEW_V;
                        }
                    }
                } else {
                    if (argc == 1) {
                        // > New
                        status = NEW_;
                    } else if (argc == 3) {
                        // > New e v
                        status = NEW_E_V;
                    } else { // argc == 2
                        if (strlen(argv[1]) == 1) {
                            if (argv[1][0] == 'e') {
                                // > New e
                                status = NEW_E;
                            } else {
                                // > New v
                                status = NEW_V;
                            }
                        } else {
                            // > New filename
                            ifstream ifs;
                            ifs.open(string(argv[1]));
                            string tmp_str;
                            char* file_argv[6], tmp_cstr[200];
                            int file_argc;
                            
                            if (!ifs.is_open()) {
                                cout << "Cannot open file " << string(argv[1]) << endl;
                                break;
                            }
                            cout << "Adding patients..." << endl; 
                            while (!ifs.eof()) {
                                getline(ifs, tmp_str);
                                strcpy(tmp_cstr, tmp_str.c_str());
                                generate_command_arguments(tmp_cstr, file_argc, file_argv);
                                do_the_task(file_argc, file_argv, true);

                                // deallocate argv[i]
                                while (file_argc > 0) {
                                    file_argc--;
                                    delete[] file_argv[file_argc];
                                }
                            }
                            ifs.close();
                            break;
                        }
                    }
                } // endif print_new_patient

                p = new Patient;

                // get p->name and p->year_of_birth
                if (print_new_patient == false) {
                    cout << ">> ";
                    string tmp;

                    getline(cin, tmp);
                    // name year-of-birth

                    char tmp_cstr[100];
                    strcpy(tmp_cstr, tmp.c_str());
                    char* pch = strtok(tmp_cstr, " ");
                    p->name = string(pch);

                    pch = strtok(nullptr, " ");
                    p->year_of_birth = stoi(string(pch));
                } else {
                    p->name = string(argv[argc - 2]);
                    p->year_of_birth = stoi(string(argv[argc-1]));
                }
                p->age = CURRENT_YEAR - p->year_of_birth;

                if (status == NEW_E || status == NEW_E_V) {
                    p->prior_ord_emergency = 1;
                }
                if (status == NEW_V || status == NEW_E_V) {
                    p->prior_ord_vip = 1;
                }
                
                if (p->age >= 60) {
                    p->prior_ord_old = 1;
                } else if (p->age <= 10) {
                    p->prior_ord_children = 1;
                }

                coordinate_patient_to_room(p);

                if (print_new_patient) {
                    print_one_patient(p);
                }
            } // end case New
            break;

        case See:
            {
                if (argv[1][0] == 'A') {
                    // > See All
                    cout << "Regular medical room." << endl;
                    for (int i = 0; i < global_variables::regular_room.size(); ++i) {
                        print_patient_list(global_variables::regular_room[i]);
                        cout << "Number of waiting patients: ";
                        if (global_variables::regular_room[i]->patients)
                            cout << global_variables::regular_room[i]->patients->total_patients << endl;
                        else
                            cout << 0 << endl;
                    }
                    cout << "==========" << endl;
                    if (global_variables::vip_room.size() > 0) {
                        cout << endl << "VIP medical room." << endl;
                    }
                    for (int i = 0; i < global_variables::vip_room.size(); ++i) {
                        print_patient_list(global_variables::vip_room[i]);
                        cout << "Number of waiting patients: ";
                        if (global_variables::vip_room[i]->patients)
                            cout << global_variables::vip_room[i]->patients->total_patients << endl;
                        else
                            cout << 0 << endl;                    
                    }
                } else {
                    // > See medical_room_ID
                    Room* room = get_room(string(argv[1]));
                    print_patient_list(room);
                    cout << "Number of waiting patients: " << room->patients->total_patients << endl;
                }
            }
            break;
        
        case Update:
            {
                if (argc == 3) {
                    // > Update medical_room_id number_of_finished_treatments
                    int i = 0;
                    Room* room = get_room(string(argv[1]));
                    update_medical_room(room, stoi(string(argv[2])));
                }
            }
            break;

        case Exit:
            {
                return 0;
            }
            break;
        default:
            {
                // the input command line is not valid
                cout << "You entered a wrong command line." << endl;
            }
    } // end switch
    return 1;
} // end function do_the_task()
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
TASK get_task(string task) {
    if (task == "New") return New;
    if (task == "See") return See;
    if (task == "Update") return Update;
    if (task == "Open") return Open;
    if (task == "Exit") return Exit;
    return WRONG_CMD;
} // end function get_task()
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void coordinate_patient_to_room(Patient* p) {
    // allocate a new Node
    Node* new_item = new Node;
    new_item->patient = p;
    new_item->left = new_item->right = nullptr;

    // choose an approriate room
    #if DEBUG
        cout << "choose room" << endl;
    #endif
    Room* room_to_add = choose_room_for_new_patient(p);

    #if DEBUG
        cout << "DONE choose room" << endl;
    #endif
    p->room = room_to_add;
    if (p->prior_ord_old == 1) {
        p->prior_ord_old = room_to_add->ord_old++;
    } else if (p->prior_ord_children == 1) {
        p->prior_ord_children = room_to_add->ord_children++;
    } else {
        p->prior_ord_normal = room_to_add->ord_normal++;
    }
    #if DEBUG
        cout << "insert patient to room" << endl;
    #endif
    insertion(room_to_add->patients, p);
    #if DEBUG
        cout << "DONE insert patient to room" << endl;
    #endif
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// if patient is normal, add room that has number min_patient
// else if 0 priority case, find room has patient min, insert to it
// else if is priority case, find room that is min prior and min patient, total patient, calc x% of patient,
// which is room prior == min, and patient of room - min patien <= %, and emer case <= max
Room *choose_room_for_new_patient(Patient *&p)
{
    if (p == nullptr)
        return nullptr;

    if (p->prior_ord_vip)
    {
        // add vip
        if (p->prior_ord_emergency || p->prior_ord_old || p->prior_ord_children)
        {
            int min_patients = 0, total_patients = 0, min_prior = 0;
            for (int i = 0; i < global_variables::vip_room.size(); i++)
            {
                if (global_variables::vip_room[i]->patients == nullptr)
                    return global_variables::vip_room[i];
                total_patients += global_variables::vip_room[i]->patients->total_patients;

                // find minimum number of patients
                if (global_variables::vip_room[i]->patients->total_patients < min_patients)
                {
                    min_patients = global_variables::vip_room[i]->patients->total_patients;
                }

                // find minimum prior
                if (global_variables::vip_room[i]->patients->total_patients - global_variables::vip_room[i]->ord_normal < min_prior)
                {
                    min_prior = global_variables::vip_room[i]->patients->total_patients - global_variables::vip_room[i]->ord_normal;
                }
            }

            int per = ceil(total_patients * global_variables::vip_room.size() / 100.0);
            per = max(per, 1);
            for (int i = 0; i < global_variables::vip_room.size(); i++)
            {
                if (global_variables::vip_room[i]->patients->total_patients == min_patients &&
                    global_variables::vip_room[i]->patients->total_patients - global_variables::vip_room[i]->ord_normal - min_prior < per)
                {
                    return global_variables::vip_room[i];
                }
            }
        }
        else // not priority case
        {
            int ind_min = 0;
            for (int i = 0; i < global_variables::vip_room.size(); i++)
            {
                if (global_variables::vip_room[i]->patients == nullptr)
                {
                    return global_variables::vip_room[i];
                }
                if (global_variables::vip_room[i]->patients->total_patients < global_variables::vip_room[ind_min]->patients->total_patients)
                {
                    ind_min = i;
                }
            }
            return global_variables::vip_room[ind_min];
        }
    }
    else
    {
        // patient is not vip
        if (p->prior_ord_emergency || p->prior_ord_old || p->prior_ord_children == 1)
        {
            int min_patients = 0, total_patients = 0, min_prior = 0;
            for (int i = 0; i < global_variables::regular_room.size(); i++)
            {
                if (global_variables::regular_room[i]->patients == nullptr)
                    return global_variables::regular_room[i];
                total_patients += global_variables::regular_room[i]->patients->total_patients;
                if (global_variables::regular_room[i]->patients == nullptr)
                {
                    min_prior = min_patients = 0;
                }
                else
                {
                    if (global_variables::regular_room[i]->patients->total_patients < min_patients)
                    {
                        min_patients = global_variables::regular_room[i]->patients->total_patients;
                    }
                    if (global_variables::regular_room[i]->patients->total_patients - global_variables::regular_room[i]->ord_normal < min_prior)
                    {
                        min_prior = global_variables::regular_room[i]->patients->total_patients - global_variables::regular_room[i]->ord_normal;
                    }
                }
            }

            int per = ceil(total_patients * global_variables::regular_room.size() / 100.0);
            per = max(per, 1);
            for (int i = 0; i < global_variables::regular_room.size(); i++)
            {
                if (global_variables::regular_room[i]->patients == nullptr)
                {
                    return global_variables::regular_room[i];
                }
                if (global_variables::regular_room[i]->patients->total_patients == min_patients &&
                    global_variables::regular_room[i]->patients->total_patients - global_variables::regular_room[i]->ord_normal - min_prior <= per &&
                    p->prior_ord_emergency + global_variables::regular_room[i]->ord_emergency <= MAX_EMERGENCY_PATIENT_IN_REGULAR_ROOM)
                {
                    return global_variables::regular_room[i];
                }
            }
            p->prior_ord_vip = true;
            return choose_room_for_new_patient(p);
        }
        else // not priority case, normal patient
        {
            int ind_min = 0;
            for (int i = 0; i < global_variables::regular_room.size(); i++)
            {
                if (global_variables::regular_room[i]->patients == nullptr)
                    return global_variables::regular_room[i];
                if (global_variables::regular_room[i]->patients->total_patients < global_variables::regular_room[ind_min]->patients->total_patients)
                {
                    ind_min = i;
                }
            }
            return global_variables::regular_room[ind_min];
        }
    }
} // end function choose_room_for_new_patient()
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void update_medical_room(Room *room, int number_of_finished_patients)
{
    if (room->patients == nullptr) return;
    int i = 0;
    while (i < number_of_finished_patients && room->patients->top)
    {
        ns_priority_queue::deletion(room->patients);
    }
    for (int i = 0; i < number_of_finished_patients; i++)
    {
        if (room->type == 'R')
        {
            // find room has prior cases patient max
            int ind_max = 0, max = 0;
            for (int i = 0; i < global_variables::regular_room.size(); i++)
            {
                if (global_variables::regular_room[i]->patients && global_variables::regular_room[i]->patients->total_patients - global_variables::regular_room[i]->ord_normal > max)
                {
                    ind_max = i;
                    max = global_variables::regular_room[i]->patients->total_patients - global_variables::regular_room[i]->ord_normal;
                }
            }
            if (global_variables::regular_room[ind_max] == room || global_variables::regular_room[ind_max]->patients == nullptr)
                return;
            // copy and paste to room
            Patient *p = ns_priority_queue::peak(global_variables::regular_room[ind_max]->patients);
            Patient *copy_p = new Patient;
            *copy_p = *p;
            // copy_p->name = p->name;
            // copy_p->age = p->age;
            // copy_p->prior_ord_children = p->prior_ord_children;
            // copy_p->prior_ord_emergency = p->prior_ord_emergency;
            // copy_p->prior_ord_normal = p->prior_ord_normal;
            // copy_p->prior_ord_old = p->prior_ord_old;
            // copy_p->prior_ord_vip = p->prior_ord_vip;
            // copy_p->year_of_birth = p->year_of_birth;

            ns_priority_queue::insertion(room->patients, copy_p);
            ns_priority_queue::deletion(global_variables::regular_room[ind_max]->patients);
        }
        else
        {
            // find room has prior cases patient max
            int ind_max = 0, max = 0;
            for (int i = 0; i < global_variables::vip_room.size(); i++)
            {
                if (global_variables::vip_room[i]->patients && global_variables::vip_room[i]->patients->total_patients - global_variables::vip_room[i]->ord_normal > max)
                {
                    ind_max = i;
                    max = global_variables::vip_room[i]->patients->total_patients - global_variables::vip_room[i]->ord_normal;
                }
            }
            if (global_variables::vip_room[ind_max] == room || global_variables::vip_room[ind_max]->patients == nullptr)
                return;
            // copy and paste to room
            Patient *p = ns_priority_queue::peak(global_variables::vip_room[ind_max]->patients);
            Patient *copy_p = new Patient;
            *copy_p = *p;
            // copy_p->name = p->name;
            // copy_p->age = p->age;
            // copy_p->prior_ord_children = p->prior_ord_children;
            // copy_p->prior_ord_emergency = p->prior_ord_emergency;
            // copy_p->prior_ord_normal = p->prior_ord_normal;
            // copy_p->prior_ord_old = p->prior_ord_old;
            // copy_p->prior_ord_vip = p->prior_ord_vip;
            // copy_p->year_of_birth = p->year_of_birth;

            ns_priority_queue::insertion(room->patients, copy_p);
            ns_priority_queue::deletion(global_variables::regular_room[ind_max]->patients);
        }
    } // end for loop
} // end function update_medical_room()
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////