////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
///////////// STRUCTURES ///////////////////
////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <cmath>
#include <iomanip>

using namespace std;
enum TASK
{
    Open,
    New,
    See,
    Update,
    Exit,
    WRONG_CMD
};
#define CURRENT_YEAR 2023
#define MAX_EMERGENCY_PATIENT_IN_REGULAR_ROOM 3
struct Room;

struct Patient
{
    string name;
    int year_of_birth, age;
    bool prior_ord_vip;
    int prior_ord_emergency; // if the patient priority is `Emergency`, prior_ord_emergency = ord number of this patient among emergency patients (in 0-base);
                             // otherwise `prior_ord_emergency` = -1
    int prior_ord_old;       // if the patient priority is `Old`, prior_ord_old = ord number of this patient among the old (in 0-base);
                             // otherwise `prior_ord_old` = -1
    int prior_ord_children;  // if the patient priority is `Children`, prior_ord_children = ord number of this patient among children (in 0-base);
                             // otherwise `prior_ord_children` = -1
    int prior_ord_normal;    // if the patient priority is `Normal`, prior_ord_normal = ord number of this patient among the normal (in 0-base);
                             // otherwise `prior_ord_normal` = -1
    int order;               // other of adding
    Room *room;
    Patient()
    {
        name = "";
        year_of_birth = -1;
        age = -1;
        prior_ord_vip = false;
        order = -1;
        prior_ord_emergency = prior_ord_old = prior_ord_children = prior_ord_normal = -1;
        room = nullptr;
    }
}; // end struct Patient

struct Node
{
    Patient *patient; // the patient in the node
    Node *left;       // left child of the node
    Node *right;      // right child of the node
    int npl;          // null path length of the node (meaning the shortest path to the NULL node)

    Node()
    {
        patient = nullptr;
        left = nullptr;
        right = nullptr;
        npl = 0;
    }
}; // end struct Node

namespace ns_priority_queue
{
    struct PriorityQueue
    {
        Node *top;          // root node of the PQ
        int total_patients; // number of nodes in the PQ

        PriorityQueue()
        {
            top = nullptr;
            total_patients = 0;
        }
    };

    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    /// @brief compare 2 priority of 2 patients
    /// @param p1 Patient 1
    /// @param p2 Patient 2
    /// @return 1 : if p1 > p2
    ///         0 : if p1 == p2 (NEVER OCCURS)
    ///        -1 : if p1 < p2
    short compare_priority(Patient *p1, Patient *p2);
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    /// @brief merge 2 priority queues (PQs) into 1 priority queue
    /// @param h1 the first PQ
    /// @param h2 the second PQ
    /// @return the result PQ after merging
    Node *merge1(Node *h1, Node *h2);
    Node *merge(Node *h1, Node *h2);
    void swapChildren(Node * t);
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    /// @brief insert a new patient to a priority queue and INCREASE TOTAL PATIENTs IN THE QUEUE BY 1
    /// @param pq the priority queue to input the new item in
    /// @param item the patient to be added to
    void insertion(PriorityQueue *&pq, Patient *item);
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    /// @brief delete the root item of the PQ
    /// @param pq the given PQ
    void deletion(PriorityQueue *&pq);
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    /// @brief get the highest priority patient in the room
    /// @param pq the priority queue
    /// @return the pointer to the patient
    Patient *peak(PriorityQueue *pq);
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    /// @brief to know whether priority queue is empty
    /// @param pq the given priority queue
    /// @return `true` if the PQ is empty; otherwise, returns `false`
    bool is_empty(PriorityQueue *pq);
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    /// @brief deallocate the patient and children of each node and then the node
    /// @param pq the PQ to be deallocated
    void delete_all_priority_queue(PriorityQueue *&pq);
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
} // end namespace ns_priority_queue

struct Room
{
    char type;                                  // type of room, R - regular or V - vip
    int no;                                     // ord number of the room, starts from 1
    ns_priority_queue::PriorityQueue *patients; // list of patients in the room
    int ord_emergency;                          // number of emergency in the room
    int ord_old;                                // number of old in the room
    int ord_children;                           // number of chuldren in the room
    int ord_normal;                             // number of normal in the room

    Room()
    {
        type = 0;
        no = 0;
        patients = nullptr;
        ord_emergency = ord_old = ord_children = ord_normal = 0;
    }
}; // end struct Room

// GLOBAL VARIABLES
namespace global_variables
{
    vector<Room *> regular_room; // list of regular medical room
    vector<Room *> vip_room;     // list of vip medical room
    long order_hospital = 1; // other of adding to the hospital

} // end namespace global_variables
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
////////// AUXILIARY_FUNCTIONS /////////////
////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
/// @brief given a character array that is the command line entered from keyboard. Convert the input cstring to command-line arguments
/// @param cmd the command line entered from keyboard
/// @param argc number of arguments
/// @param argv arguments
void generate_command_arguments(char cmd[], int &argc, char *argv[]);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief open rooms according to number of each kind of medical rooms
/// @param number_of_regular_room number of regular rooms
/// @param number_of_vip_room number of vip rooms
void open_medical_room(int number_of_regular_room, int number_of_vip_room);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief print the patient using pre-order traversal in binary tree
/// @param p root node of the tree
void preorder_print(Node *p);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief print all the patients in the given room
/// @param room the room whose patients need to be printed
void print_patient_list(Room *room);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief get the room whose name is the given string `room`
/// @param room given string room name
/// @return the pointer to the corresponding room
Room *get_room(string room);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief given command line arguments, do the task following to the cmd line
/// @param argc number of arguments
/// @param argv list of arguments
/// @param print_new_patient in case register online from file, we need to print the patient out, which means this will be true; otherwise it will be false
/// @return if the given cmd line is "Exit", return false; otherwise, returns true
bool do_the_task(int argc, char *argv[], bool print_new_patient);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief given the string determining the key task of the command: "New" or "See" or "Open" or "Update"
/// @param task the given string
/// @return the key task of the command in enumeration type TASK
TASK get_task(string task);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief coordinate the given patient to an approriate active medical room
/// @param p the patient who needs to be coordinated to an active medical room
void coordinate_patient_to_room(Patient *p);
// Auxiliary function
/// @brief NOT DONE
/// @param p patient who needs to be coordinated to a room
/// @return the room
Room *choose_room_for_new_patient(Patient *&p);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief remove an amount of the highest priority patients of a room
/// @param room the room whose patients will be removed
/// @param number_of_finished_patients number of patients will be removed
void update_medical_room(Room *room, int number_of_finished_patients);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
/////// AUXILIARY_IMPLEMENTATION ///////////
////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
int PriorityPatient(Patient *p)
{
    if(p == nullptr)
        return 0;
    int priorty = 0;
    if(p->prior_ord_children >= 0)
        priorty += 1;
    if(p->prior_ord_old >= 0)
        priorty += 2;
    if(p->prior_ord_emergency >= 0)
        priorty += 3;
    return priorty;
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
Node *ns_priority_queue::merge(Node * h1, Node * h2)
{
    if (h1 == nullptr)
        return h2;
    if (h2 == nullptr)
        return h1;
    if (compare_priority(h1->patient, h2->patient) == 1)
        return merge1(h1, h2);
    else { // compare_priority(h1->patient, h2->patient) == -1
        return merge1(h2, h1);
    }
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void ns_priority_queue::swapChildren(Node *t)
{
    Node *tmp = t->left;
    t->left = t->right;
    t->right = tmp;
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
Patient *ns_priority_queue::peak(PriorityQueue *pq)
{
    if (pq && pq->top)
        return pq->top->patient;
    else
        return nullptr;
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void ns_priority_queue::deletion(PriorityQueue *&pq)
{
    if (pq == nullptr || pq->top == nullptr) return;
    Node *oldRoot = pq->top;
    pq->top = merge(pq->top->left, pq->top->right);
    pq->total_patients--;
    delete oldRoot->patient;
    delete oldRoot;
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
bool ns_priority_queue::is_empty(PriorityQueue *pq) 
{
    if (!pq || !pq->top) return true;
    else return false;
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void ns_priority_queue::delete_all_priority_queue(PriorityQueue *&pq) 
{
    while (!is_empty(pq)) deletion(pq);
    delete pq;
}
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

    if (p->prior_ord_emergency >= 0) {
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

                p->order = global_variables::order_hospital;
                global_variables::order_hospital++;

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
    if (room_to_add == nullptr) {
        cout << "Room NULL" << endl;
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
        exit(1);
    }
    #if DEBUG
        cout << "DONE choose room" << endl;
    #endif
    p->room = room_to_add;
    if (p->prior_ord_emergency == 1) {
        p->prior_ord_emergency = room_to_add->ord_emergency++;
    } else if (p->prior_ord_old == 1) {
        p->prior_ord_old = room_to_add->ord_old++;
    } else if (p->prior_ord_children == 1) {
        p->prior_ord_children = room_to_add->ord_children++;
    } else {
        p->prior_ord_normal = room_to_add->ord_normal++;
    }
    #if DEBUG
        cout << "insert patient to room" << endl;
    #endif
    ns_priority_queue::insertion(room_to_add->patients, p);
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
        if (p->prior_ord_emergency == 1 || p->prior_ord_old == 1 || p->prior_ord_children == 1)
        {
            int min_patients = 0, total_patients = 0, min_prior = 0;
            if(global_variables::vip_room.size() > 0)
            {
                if(global_variables::vip_room[0]->patients != nullptr){
                    min_patients = global_variables::vip_room[0]->patients->total_patients;
                    min_prior = global_variables::vip_room[0]->patients->total_patients - global_variables::vip_room[0]->ord_normal;
                }
            }
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
                if (global_variables::vip_room[i]->patients->total_patients - min_patients <= per &&
                    global_variables::vip_room[i]->patients->total_patients - global_variables::vip_room[i]->ord_normal == min_prior )
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
        if (p->prior_ord_emergency == 1 || p->prior_ord_old == 1 || p->prior_ord_children == 1)
        {
            int min_patients = 0, total_patients = 0, min_prior = 0;
            if(global_variables::regular_room.size() > 0)
            {
                if(global_variables::regular_room[0]->patients != nullptr){
                    min_patients = global_variables::regular_room[0]->patients->total_patients;
                    min_prior = global_variables::regular_room[0]->patients->total_patients - global_variables::regular_room[0]->ord_normal;
                }
            }
            for (int i = 0; i < global_variables::regular_room.size(); i++)
            {
                if (global_variables::regular_room[i]->patients == nullptr)
                    return global_variables::regular_room[i];
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
                total_patients += global_variables::regular_room[i]->patients->total_patients;
            }

            int per = ceil(total_patients * global_variables::regular_room.size() / 100.0);
            per = max(per, 1);
            for (int i = 0; i < global_variables::regular_room.size(); i++)
            {
                if (global_variables::regular_room[i]->patients->total_patients - min_patients <= per &&
                    global_variables::regular_room[i]->patients->total_patients - global_variables::regular_room[i]->ord_normal == min_prior &&
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
    if (room == nullptr || room->patients == nullptr) return;
    int i = 0;
    while (i < number_of_finished_patients && room->patients->top)
    {
        ns_priority_queue::deletion(room->patients);
        i++;
    }

    if (room->type == 'R')
    {
        Room* virtual_room = new Room;
        Patient* p;
        Patient* copy_p;
        Patient* paste_p;
        for(int i = 0; i < global_variables::regular_room.size(); i++)
        {
            p = ns_priority_queue::peak(global_variables::regular_room[i]->patients);
            while(p)
            {
                copy_p = new Patient;
                *copy_p = *p;
                ns_priority_queue::deletion(global_variables::regular_room[i]->patients);
                ns_priority_queue::insertion(virtual_room->patients,copy_p);
                p = ns_priority_queue::peak(global_variables::regular_room[i]->patients);
            }
        }
            p = ns_priority_queue::peak(virtual_room->patients);
            while(p)
            {
                paste_p = new Patient;
                *paste_p = *p;
                ns_priority_queue::deletion(virtual_room->patients);
                Room* paste_room = choose_room_for_new_patient(paste_p);
                ns_priority_queue::insertion(paste_room->patients,paste_p);
                p = ns_priority_queue::peak(virtual_room->patients);
                paste_p->room = paste_room;
        }
    }
    else
    {
        Room* virtual_room = new Room;
        Patient* p;
        Patient* copy_p;
        Patient* paste_p;
        for(int i = 0; i < global_variables::vip_room.size(); i++)
        {
            p = ns_priority_queue::peak(global_variables::vip_room[i]->patients);
            while(p)
            {
                copy_p = new Patient;
                *copy_p = *p;
                ns_priority_queue::deletion(global_variables::vip_room[i]->patients);
                ns_priority_queue::insertion(virtual_room->patients,copy_p);
                p = ns_priority_queue::peak(global_variables::vip_room[i]->patients);
            }
        }
        p = ns_priority_queue::peak(virtual_room->patients);
        while(p)
        {
            paste_p = new Patient;
            *paste_p = *p;
            ns_priority_queue::deletion(virtual_room->patients);
            Room* paste_room = choose_room_for_new_patient(paste_p);
            ns_priority_queue::insertion(paste_room->patients,paste_p);
            p = ns_priority_queue::peak(virtual_room->patients);
            paste_p->room = paste_room;
        }
    }
} // end function update_medical_room()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
////////////////// MAIN ////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
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