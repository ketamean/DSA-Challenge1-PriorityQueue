// this part contains implementation for all functions in `structures.h`
#include "structures.h"
using namespace ns_priority_queue;
using namespace global_variables;

short ns_priority_queue::compare_priority(Patient *p1, Patient *p2)
{
    // p1 and p2 are both either vip or non-vip

    if ((p1->prior_ord_emergency >= 0) ^ (p2->prior_ord_emergency >= 0))
    {
        // if one of them is emergency and the other is not
        if (p1->prior_ord_emergency > p2->prior_ord_emergency)
        {
            // p1 is emergency and p2 is not
            return 1;
        }
        else
        {
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

Node *ns_priority_queue::merge1(Node *h1, Node *h2)
{
    if (h1->left == nullptr)
        h1->left = h2;
    else
    {
        h1->right = ns_priority_queue::merge(h1->right, h2);
        if (h1->left->npl < h1->right->npl)
        {
            swap(h1->left, h1->right);
            h1->npl = h1->right->npl + 1;
        }
    }
    return h1;
}

Node *ns_priority_queue::merge(Node *h1, Node *h2)
{
    if (h1 == nullptr)
        return h2;
    else if (h2 == nullptr)
        return h1;
    else if ((ns_priority_queue::compare_priority(h2->patient, h1->patient)) == -1)
        return ns_priority_queue::merge1(h1, h2);
    else
        return ns_priority_queue::merge1(h2, h1);
}

void ns_priority_queue::insertion(PriorityQueue *&pq, Patient *item)
{
    Node *node = new Node;
    node->patient = item;
    node->left = node->right = nullptr;
    node->npl = 0;
    pq->top = ns_priority_queue::merge(pq->top, node);
    pq->total_patients++;
}

void ns_priority_queue::deletion(PriorityQueue *&pq)
{
    if (pq->top)
    {
        Node *tmp = pq->top;
        pq->top = ns_priority_queue::merge(pq->top->left, pq->top->right);
        delete tmp->patient;
        delete tmp;
        pq->total_patients--;
    }
}

Patient *ns_priority_queue::peak(PriorityQueue *pq)
{
    if (pq->top)
        return pq->top->patient;
    return nullptr;
}

bool ns_priority_queue::is_empty(PriorityQueue *pq)
{
    if (pq->top == nullptr)
        return true;
    return false;
}

void ns_priority_queue::delete_all_priority_queue(PriorityQueue *&pq)
{
    while (!ns_priority_queue::is_empty(pq))
        ns_priority_queue::deletion(pq);
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

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void generate_command_arguments(char cmd[], int &argc, char *argv[])
{
    argc = 0;
    char *pch = strtok(cmd, " ");
    while (pch != nullptr)
    {
        argv[argc++] = pch;
        pch = strtok(nullptr, " ");
    }
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void open_medical_room(int number_of_regular_room, int number_of_vip_room)
{
    regular_room.reserve(number_of_regular_room);
    for (int i = 0; i < number_of_regular_room; ++i)
    {
        regular_room[i] = new Room;
        regular_room[i]->type = 'R';
        regular_room[i]->no = i + 1;
    }

    vip_room.reserve(number_of_vip_room);
    for (int i = 0; i < number_of_vip_room; ++i)
    {
        vip_room[i] = new Room;
        vip_room[i]->type = 'V';
        vip_room[i]->no = i + 1;
    }
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void print_one_patient(Patient *p)
{
    cout << p->name << " " << p->year_of_birth << " " << p->room->type << p->room->no << endl;
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void preorder_print(Node *p)
{
    if (p == nullptr)
    {
        cout << "End." << endl;
        return;
    }
    // print all the patients using pre-order traversal in binary tree
    print_one_patient(p->patient);
    preorder_print(p->left);
    preorder_print(p->right);
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void print_patient_list(Room *room)
{
    cout << "Room " << (char)room->type << room->no << ":" << endl;
    preorder_print(room->patients->top);
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
Room *get_room(string room)
{
    char type_of_room = room[0];
    room.erase(room.begin());
    if (type_of_room == 'R')
    {
        return regular_room[stoi(room) - 1];
    }
    else
    {
        return vip_room[stoi(room) - 1];
    }
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
bool do_the_task(int argc, char *argv[], bool print_new_patient)
{
    string tmp_task = string(argv[0]);
    TASK task = get_task(tmp_task);
    switch (task)
    {
    case Open:
    {
        if (argc == 3)
        {
            // > Open x y
            open_medical_room(stoi(string(argv[1])), stoi(string(argv[2])));
        }
        else if (argc == 2)
        {
            // > Open x
            open_medical_room(stoi(string(argv[1])), 0);
        }
    }
    break;

    case New:
    {
        Patient *p = nullptr;
        if (argc == 2)
        {
            // > New filename
            ifstream ifs;
            ifs.open(string(argv[1]));
            string tmp_str;
            char *file_argv[6], *tmp_cstr;
            int file_argc;

            if (!ifs.eof())
            {
                cout << "Cannot open file " << string(argv[1]) << endl;
                break;
            }
            while (!ifs.eof())
            {
                getline(ifs, tmp_str);
                strcpy(tmp_cstr, tmp_str.c_str());
                generate_command_arguments(tmp_cstr, file_argc, file_argv);
                do_the_task(file_argc, file_argv, true);

                // deallocate argv[i]
                while (file_argc > 0)
                {
                    file_argc--;
                    delete[] file_argv[file_argc];
                }
            }
            ifs.close();
        }
        else if (argc == 3)
        {
            // > New Name Year_of_birth
            p = new Patient;
            p->name = string(argv[1]);
            p->year_of_birth = stoi(string(argv[2]));
            p->prior_ord_vip = false;
        }
        else if (argc == 4)
        {
            if (argv[1][0] == 'e')
            {
                // > New e Name Year_of_birth
                p = new Patient;
                p->name = string(argv[2]);
                p->year_of_birth = stoi(string(argv[3]));
                p->prior_ord_vip = false;
                p->prior_ord_emergency = 1;
            }
            else if (argv[1][0] == 'v')
            {
                // > New v Name Year_of_birth
                p = new Patient;
                p->name = string(argv[2]);
                p->year_of_birth = stoi(string(argv[3]));
                p->prior_ord_vip = true;
            }
        }
        else if (argc == 5)
        {
            // > New e v Name Year_of_birth
            p = new Patient;
            p->name = string(argv[3]);
            p->year_of_birth = stoi(string(argv[4]));
            p->prior_ord_vip = true;
            p->prior_ord_emergency = 1;
        }

        if (p)
        {
            p->age = CURRENT_YEAR - p->year_of_birth;
            cout << p->age;
            if (p->age >= 60)
            {
                p->prior_ord_old = 1;
            }
            else if (p->age <= 10)
            {
                p->prior_ord_children = 1;
            }
            coordinate_patient_to_room(p);
        }
    }
    break;

    case See:
    {
        if (argv[1][0] == 'A')
        {
            // > See All
            cout << "Regular medical room." << endl;
            for (int i = 0; i < regular_room.size(); ++i)
            {
                cout << "1";
                // print_patient_list(regular_room[i]);
                // cout << "Number of waiting patients: " << regular_room[i]->patients->total_patients << endl;
            }

            if (vip_room.size() > 0)
            {
                cout << endl
                     << "VIP medical room." << endl;
            }
            for (int i = 0; i < vip_room.size(); ++i)
            {
                print_patient_list(vip_room[i]);
                cout << "Number of waiting patients: " << vip_room[i]->patients->total_patients << endl;
            }
        }
        else
        {
            // > See medical_room_ID
            Room *room = get_room(string(argv[1]));
            print_patient_list(room);
            cout << "Number of waiting patients: " << room->patients->total_patients << endl;
        }
    }
    break;

    case Update:
    {
        if (argc == 3)
        {
            // > Update medical_room_id number_of_finished_treatments
            int i = 0;
            Room *room = get_room(string(argv[1]));
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
TASK get_task(string task)
{
    if (task == "New")
        return New;
    if (task == "See")
        return See;
    if (task == "Update")
        return Update;
    if (task == "Open")
        return Open;
    if (task == "Exit")
        return Exit;
    return WRONG_CMD;
} // end function get_task()
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void coordinate_patient_to_room(Patient *p)
{
    // allocate a new Node
    Node *new_item = new Node;
    new_item->patient = p;
    new_item->left = new_item->right = nullptr;

    // choose an approriate room
    cout << "choose room" << endl;
    Room *room_to_add = choose_room_for_new_patient(p);

    cout << "done choose room" << endl;
    if (p->prior_ord_emergency == 1)
    {
        p->prior_ord_emergency = room_to_add->ord_emergency++;
    }
    else if (p->prior_ord_old == 1)
    {
        p->prior_ord_old = room_to_add->ord_old++;
    }
    else if (p->prior_ord_children == 1)
    {
        p->prior_ord_children = room_to_add->ord_children++;
    }
    else
    {
        p->prior_ord_normal = room_to_add->ord_normal++;
    }
    cout << "insert" << endl;
    insertion(room_to_add->patients, p);
    cout << "done insert" << endl;
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// if patient is normal, add room that has number min_patient
// else if 0 priority case, find room has patient min, insert to it
// else if is priority case, find room that is min prior and min patient, total patient, calc x% of patient,
// which is room prior == min, and patient of room - min patien <= %, and emer case <= max
Room *choose_room_for_new_patient(Patient *&p)
{
    if (p->prior_ord_vip)
    {
        // add vip
        if (p->prior_ord_emergency || p->prior_ord_old == 1 || p->prior_ord_children == 1)
        {
            int min_patients = 0, total_patients = 0, min_prior = 0;
            for (int i = 0; i < vip_room.size(); i++)
            {
                total_patients += vip_room[i]->patients->total_patients;

                // find minimum number of patients
                if (vip_room[i]->patients->total_patients < min_patients)
                {
                    min_patients = vip_room[i]->patients->total_patients;
                }

                // find minimum prior
                if (vip_room[i]->patients->total_patients - vip_room[i]->ord_normal < min_prior)
                {
                    min_prior = vip_room[i]->patients->total_patients - vip_room[i]->ord_normal;
                }
            }

            int per = ceil(total_patients * vip_room.size() / 100);

            for (int i = 0; i < vip_room.size(); i++)
            {
                if (vip_room[i]->patients->total_patients == min_patients &&
                    vip_room[i]->patients->total_patients - vip_room[i]->ord_normal - min_prior < per)
                {
                    return vip_room[i];
                }
            }
        }
        else // not priority case
        {
            int ind_min = 0;
            for (int i = 0; i < vip_room.size(); i++)
            {
                if (vip_room[i]->patients->total_patients < vip_room[ind_min]->patients->total_patients)
                {
                    ind_min = i;
                }
            }
            return vip_room[ind_min];
        }
    }
    else
    {
        // patient is not vip
        if (p->prior_ord_emergency == 1 || p->prior_ord_old == 1 || p->prior_ord_children == 1)
        {
            int min_patients = 0, total_patients = 0, min_prior = 0;
            for (int i = 0; i < regular_room.size(); i++)
            {
                total_patients += regular_room[i]->patients->total_patients;
                if (regular_room[i]->patients->total_patients < min_patients)
                {
                    min_patients = regular_room[i]->patients->total_patients;
                }
                if (regular_room[i]->patients->total_patients - regular_room[i]->ord_normal < min_prior)
                {
                    min_prior = regular_room[i]->patients->total_patients - regular_room[i]->ord_normal;
                }
            }

            int per = ceil(total_patients * regular_room.size() / 100);

            for (int i = 0; i < regular_room.size(); i++)
            {
                if (regular_room[i]->patients->total_patients == min_patients &&
                    regular_room[i]->patients->total_patients - regular_room[i]->ord_normal - min_prior < per &&
                    p->prior_ord_vip + regular_room[i]->ord_emergency <= MAX_EMERGENCY_PATIENT_IN_REGULAR_ROOM)
                {
                    return regular_room[i];
                }
            }
            p->prior_ord_vip = true;
            return choose_room_for_new_patient(p);
        }
        else // not priority case, normal patient
        {
            int ind_min = 0;
            for (int i = 0; i < regular_room.size(); i++)
            {
                if (regular_room[i]->patients->total_patients < regular_room[ind_min]->patients->total_patients)
                {
                    ind_min = i;
                }
            }
            return regular_room[ind_min];
        }
    }
} // end function choose_room_for_new_patient()
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void update_medical_room(Room *room, int number_of_finished_patients)
{
    for (int i = 0; i < number_of_finished_patients; i++)
    {
        ns_priority_queue::deletion(room->patients);
    }
    for (int i = 0; i < number_of_finished_patients; i++)
    {
        if (room->type == 'R')
        {
            // find room has prior cases patient max
            int ind_max = 0, max = 0;
            for (int i = 0; i < regular_room.size(); i++)
            {
                if (regular_room[i]->patients->total_patients - regular_room[i]->ord_normal > max)
                {
                    ind_max = i;
                    max = regular_room[i]->patients->total_patients - regular_room[i]->ord_normal;
                }
            }
            // copy and paste to room
            Patient *p = ns_priority_queue::peak(regular_room[ind_max]->patients);
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
            ns_priority_queue::deletion(regular_room[ind_max]->patients);
        }
        else
        {
            // find room has prior cases patient max
            int ind_max = 0, max = 0;
            for (int i = 0; i < vip_room.size(); i++)
            {
                if (vip_room[i]->patients->total_patients - vip_room[i]->ord_normal > max)
                {
                    ind_max = i;
                    max = vip_room[i]->patients->total_patients - vip_room[i]->ord_normal;
                }
            }
            // copy and paste to room
            Patient *p = ns_priority_queue::peak(vip_room[ind_max]->patients);
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
            ns_priority_queue::deletion(regular_room[ind_max]->patients);
        }
    } // end for loop
} // end function update_medical_room()
  /////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////