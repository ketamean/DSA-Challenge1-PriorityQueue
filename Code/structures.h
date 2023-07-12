#ifndef _structures
#define _structures
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>

using namespace std;

enum TASK {Open, New, See, Update, Exit};

// GLOBAL VARIABLES
int number_of_regular_room, number_of_vip_room;

struct Patient {
    string name;
    int year_of_birth;
    int priority;

    Patient () {
        name = "";
        year_of_birth = -1;
        priority = -1;
    }
};

struct Node {
    Patient* patient;
    Node* left;
    Node* right;

    Node () {
        patient = nullptr;
        left = nullptr;
        right = nullptr;
    }
};

struct PriorityQueue {
    Node* top;
    int size;

    PriorityQueue () {
        top = nullptr;
        size = 0;
    }
};

/// @brief Each Room represents a room in the hospital
struct Room {
    char type;                  // type of the room: regular (R) or v.i.p (V)
    int no;                     // ordinal number of the room, starts from 1
    vector<Patient*> patients;  // list of patients in this room
};

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief merge 2 priority queues (PQs) into 1 priority queue
/// @param h1 the first PQ
/// @param h2 the second PQ
/// @return the result PQ after merging
PriorityQueue* merge(PriorityQueue* h1, PriorityQueue* h2);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief 
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
/// @brief deallocate the patient and children of each node and then the node
/// @param pq the PQ to be deallocated
void deletePQ(PriorityQueue* &pq);
#endif