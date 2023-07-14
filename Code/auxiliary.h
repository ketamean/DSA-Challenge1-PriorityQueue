#ifndef _auxiliary
#define _auxiliary

#include "structures.h"

#define MAX_EMERGENCY_PATIENT_IN_REGULAR_ROOM 20

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief given a character array that is the command line entered from keyboard. Convert the input cstring to command-line arguments
/// @param cmd the command line entered from keyboard
/// @param argc number of arguments
/// @param argv arguments
void generate_command_arguments(char cmd[], int &argc, char* argv[]);
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
void preorder_print(Node* p);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief print all the patients in the given room
/// @param room the room whose patients need to be printed
void print_patient_list(Room* room);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief get the room whose name is the given string `room`
/// @param room given string room name
/// @return the pointer to the corresponding room 
Room* get_room(string room);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief given command line arguments, do the task following to the cmd line
/// @param argc number of arguments
/// @param argv list of arguments
/// @param print_new_patient in case register online from file, we need to print the patient out, which means this will be true; otherwise it will be false
/// @return if the given cmd line is "Exit", return false; otherwise, returns true
bool do_the_task(int argc, char* argv[], bool print_new_patient);
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
void coordinate_patient_to_room(Patient* p);
// Auxiliary function
/// @brief NOT DONE
/// @param p patient who needs to be coordinated to a room
/// @return the room
Room* choose_room_for_new_patient(Patient* &p);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief remove an amount of the highest priority patients of a room 
/// @param room the room whose patients will be removed
/// @param number_of_finished_patients number of patients will be removed
void update_medical_room(Room* room, int number_of_finished_patients);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
#endif