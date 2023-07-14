// this part contains implementation for all functions in `structures.h`
#include "structures.h"
using namespace ns_priority_queue;

short ns_priority_queue::compare_priority(Patient *p1, Patient *p2)
{
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

Node *ns_priority_queue::merge1(Node *h1, Node *h2)
{
    if (!h1->left)
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
};
Node *ns_priority_queue::merge(Node *h1, Node *h2)
{
    if (!h1)
        return h2;
    else if (!h2)
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

void generate_command_arguments(char cmd[], int &argc, char *argv[])
{
    argc = 0;
    char *pch = strtok(cmd, " ");
    while (pch != nullptr)
    {
        argv[argc++], pch;
        pch = strtok(nullptr, " ");
    }
}

void open_medical_room(int number_of_regular_room, int number_of_vip_room)
{
    global_variables::regular_room.reserve(number_of_regular_room);
    global_variables::vip_room.reserve(number_of_vip_room);
}

void preorder_print(Node *p)
{
    if (p == nullptr)
        return;

    // print 1 patient
    cout << p->patient->name << " " << p->patient->year_of_birth << " " << endl;
    preorder_print(p->left);
    preorder_print(p->right);
}

void print_patient_list(char type_of_room, int ord_number_of_the_room)
{
    // print all the patients using pre-order traversal
    cout << "Room " << (char)type_of_room << ord_number_of_the_room << ":" << endl;
    if (type_of_room == 'R')
    {
        // out of range of the list of regular medical room
        if (ord_number_of_the_room < 1 || ord_number_of_the_room > global_variables::regular_room.size())
            return;

        ord_number_of_the_room--;
        preorder_print(global_variables::regular_room[ord_number_of_the_room]->patients->top);
    }
    else
    {
        // out of range of the list of vip medical room
        if (ord_number_of_the_room < 1 || ord_number_of_the_room > global_variables::vip_room.size())
            return;

        ord_number_of_the_room--;
        preorder_print(global_variables::vip_room[ord_number_of_the_room]->patients->top);
    }
}

void get_room_info(string room, char &type_of_room, int &no_of_room)
{
    type_of_room = room[0];
    room.erase(room.begin());
    no_of_room = stoi(room);
}

bool do_the_task(int argc, char *argv[])
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
        else
        {
            // > Open x
            open_medical_room(stoi(string(argv[1])), 0);
        }
    }

    case New:
    {
        Patient *p = nullptr;
        if (argc == 2)
        {
            // > New filename
            ifstream ifs;
            ifs.open(string(argv[2]));
            string tmp_str;
            char *file_argv[6], *tmp_cstr;
            int file_argc;
            while (!ifs.eof())
            {
                getline(ifs, tmp_str);
                strcpy(tmp_cstr, tmp_str.c_str());
                generate_command_arguments(tmp_cstr, file_argc, file_argv);
                do_the_task(file_argc, file_argv);
            }
            ifs.close();
        }
        else if (argc > 2)
        {
            p = new Patient;
            p->name = string(argv[1]);
            if (argc == 3)
            {
                // > New Name Year_of_birth
                p->year_of_birth = stoi(string(argv[2]));
            }
            else if (argc == 4)
            {
                if (argv[1][0] == 'e')
                {
                    // > New e Name Year_of_birth
                    p->prior_ord_emergency = 1;
                    p->year_of_birth = stoi(string(argv[3]));
                }
                else
                {
                    // > New v Name Year_of_birth
                    p->prior_ord_vip = true;
                    p->year_of_birth = stoi(string(argv[3]));
                }
            }
            else
            {
                // > New e v Name Year_of_birth
                p->prior_ord_vip = true;
                p->prior_ord_emergency = 1;
                p->year_of_birth = stoi(string(argv[4]));
            }
            p->age = CURRENT_YEAR - p->year_of_birth;
            if (p->age >= 60)
            {
                p->prior_ord_old = 1;
            }
            else if (p->age <= 10)
            {
                p->prior_ord_children = 1;
            }
        }
    }

    case See:
    {

        if (strcmp(argv[1], "All") == 0)
        {
            // > See All
            for (int i = 0; i < global_variables::regular_room.size(); ++i)
            {
                print_patient_list(global_variables::regular_room[i]->type, global_variables::regular_room[i]->no);
            }

            for (int i = 0; i < global_variables::vip_room.size(); ++i)
            {
                print_patient_list(global_variables::vip_room[i]->type, global_variables::vip_room[i]->no);
            }
        }
        else
        {
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
        int no_of_room; // [1..n]
        char type_of_room;
        get_room_info(room, type_of_room, no_of_room);
        if (type_of_room == 'R')
        {
            if (no_of_room <= global_variables::regular_room.size())
            {
                while (i < _number_of_finished_treatments && ns_priority_queue::is_empty(global_variables::regular_room[no_of_room - 1]->patients) == false)
                {
                    ns_priority_queue::deletion(global_variables::regular_room[no_of_room - 1]->patients);
                    i++;
                }
            }
        }
        else
        {
            if (no_of_room <= global_variables::vip_room.size())
            {
                while (i < _number_of_finished_treatments && ns_priority_queue::is_empty(global_variables::vip_room[no_of_room - 1]->patients) == false)
                {
                    ns_priority_queue::deletion(global_variables::vip_room[no_of_room - 1]->patients);
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
    return Exit;
}

void coordinate_patient_to_room(Patient *p, bool is_emergency)
{
}

// if patient is normal, add room that has number minpatient
// else if 0 prioroty case, find room has patient min, insert to it
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
            for (int i = 0; i < global_variables::vip_room.size(); i++)
            {
                total_patients++;
                if (global_variables::vip_room[i]->patients->total_patients < min_patients)
                {
                    min_patients = global_variables::vip_room[i]->patients->total_patients;
                }
                if (global_variables::vip_room[i]->patients->total_patients - global_variables::vip_room[i]->ord_normal < min_prior)
                {
                    min_prior = global_variables::vip_room[i]->patients->total_patients - global_variables::vip_room[i]->ord_normal;
                }
            }

            int per = ceil(total_patients * global_variables::vip_room.size() / 100);

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
                if (global_variables::vip_room[i]->patients->total_patients < global_variables::vip_room[ind_min]->patients->total_patients)
                {
                    ind_min = i;
                }
            }
            return global_variables::vip_room[ind_min];
        }
    }

    // patient is not vip
    if (p->prior_ord_emergency || p->prior_ord_old == 1 || p->prior_ord_children == 1)
    {
        int min_patients = 0, total_patients = 0, min_prior = 0;
        for (int i = 0; i < global_variables::regular_room.size(); i++)
        {
            total_patients++;
            if (global_variables::regular_room[i]->patients->total_patients < min_patients)
            {
                min_patients = global_variables::regular_room[i]->patients->total_patients;
            }
            if (global_variables::regular_room[i]->patients->total_patients - global_variables::regular_room[i]->ord_normal < min_prior)
            {
                min_prior = global_variables::regular_room[i]->patients->total_patients - global_variables::regular_room[i]->ord_normal;
            }
        }

        int per = ceil(total_patients * global_variables::regular_room.size() / 100);

        for (int i = 0; i < global_variables::regular_room.size(); i++)
        {
            if (global_variables::regular_room[i]->patients->total_patients == min_patients &&
                global_variables::regular_room[i]->patients->total_patients - global_variables::regular_room[i]->ord_normal - min_prior < per &&
                p->prior_ord_vip + global_variables::regular_room[i]->ord_emergency <= MAX_EMERGENCY_PATIENT_IN_REGULAR_ROOM)
            {
                return global_variables::regular_room[i];
            }
        }
        p->prior_ord_vip = true;
        return choose_room_for_new_patient(p);
    }
    else // not priority case
    {
        int ind_min = 0;
        for (int i = 0; i < global_variables::regular_room.size(); i++)
        {
            if (global_variables::regular_room[i]->patients->total_patients < global_variables::regular_room[ind_min]->patients->total_patients)
            {
                ind_min = i;
            }
        }
        return global_variables::regular_room[ind_min];
    }
}

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
            for (int i = 0; i < global_variables::regular_room.size(); i++)
            {
                if (global_variables::regular_room[i]->patients->total_patients - global_variables::regular_room[i]->ord_normal > max)
                {
                    ind_max = i;
                    max = global_variables::regular_room[i]->patients->total_patients - global_variables::regular_room[i]->ord_normal;
                }
            }
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
                if (global_variables::vip_room[i]->patients->total_patients - global_variables::vip_room[i]->ord_normal > max)
                {
                    ind_max = i;
                    max = global_variables::vip_room[i]->patients->total_patients - global_variables::vip_room[i]->ord_normal;
                }
            }
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
    }
}