#ifndef _structures
#define _structures
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>

using namespace std;
enum TASK {Open, New, See, Update, Exit};

struct Patient {
    string name;
    int year_of_birth, age;
    bool prior_ord_vip;
    int prior_ord_emergency;    // if the patient priority is `Emergency`, prior_ord_emergency = ord number of this patient among emergency patients (in 0-base);
                                // otherwise `prior_ord_emergency` = -1
    int prior_ord_old;          // if the patient priority is `Old`, prior_ord_old = ord number of this patient among the old (in 0-base);
                                // otherwise `prior_ord_old` = -1
    int prior_ord_children;     // if the patient priority is `Children`, prior_ord_children = ord number of this patient among children (in 0-base);
                                // otherwise `prior_ord_children` = -1
    int prior_ord_normal;       // if the patient priority is `Normal`, prior_ord_normal = ord number of this patient among the normal (in 0-base);
                                // otherwise `prior_ord_normal` = -1

    Patient () {
        name = "";
        year_of_birth = -1;
        age = -1;
        prior_ord_vip = false;
        prior_ord_emergency = prior_ord_old = prior_ord_children = prior_ord_normal = -1;
    }
};

struct Node {
    Patient* patient;           // the patient in the node
    Node* left;                 // left child of the node
    Node* right;                // right child of the node

    Node () {
        patient = nullptr;
        left = nullptr;
        right = nullptr;
    }
};

namespace ns_priority_queue {
    struct PriorityQueue {
        Node* top;                  // root node of the PQ
        int total_patients;         // number of nodes in the PQ

        PriorityQueue () {
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
    short compare_priority(Patient* p1, Patient* p2);
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    /// @brief merge 2 priority queues (PQs) into 1 priority queue
    /// @param h1 the first PQ
    /// @param h2 the second PQ
    /// @return the result PQ after merging
    PriorityQueue* merge(PriorityQueue* h1, PriorityQueue* h2);
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    /// @brief insert a new patient to a priority queue and INCREASE TOTAL PATIENTs IN THE QUEUE BY 1
    /// @param pq 
    /// @param item 
    void insertion(PriorityQueue* &pq, Patient* item);
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    /// @brief delete the root item of the PQ
    /// @param pq the given PQ
    void deletion(PriorityQueue* &pq);
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    /// @brief 
    /// @param pq 
    /// @return 
    Patient* peak(PriorityQueue* pq);
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    /// @brief to know whether priority queue is empty
    /// @param pq the given priority queue
    /// @return `true` if the PQ is empty; otherwise, returns `false`
    bool is_empty(PriorityQueue* pq);
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    /// @brief deallocate the patient and children of each node and then the node
    /// @param pq the PQ to be deallocated
    void delete_all_priority_queue(PriorityQueue* &pq);
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
} // end namespace PriorityQueue

struct Room {
    char type;                                      // type of room, R - regular or V - vip
    int no;                                         // ord number of the room, starts from 1
    ns_priority_queue::PriorityQueue* patients;     // list of patients in the room
    int ord_emergency;                              // number of emergency in the room
    int ord_old;                                    // number of old in the room
    int ord_children;                               // number of chuldren in the room
    int ord_normal;                                 // number of normal in the room

    Room () {
        type = 0;
        no = 0;
        patients = nullptr;
        ord_emergency = ord_old = ord_children = ord_normal = 0;
    }
};

// GLOBAL VARIABLES
namespace global_variables {
    vector<Room*> regular_room;// list of regular medical room
    vector<Room*> vip_room;    // list of vip medical room
}
#endif