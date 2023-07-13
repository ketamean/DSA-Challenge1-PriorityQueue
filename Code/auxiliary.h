#pragma once
#ifndef _auxiliary
#define _auxiliary

#include "structures.h"

#define MAX_EMERGENCY_PATIENT_IN_REGULAR_ROOM 20

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief to add a new patient to the room
/// @param room room to add patient to
/// @param item patient to be added
void add_a_new_patient_to_room(Room* room, Patient* item);
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
/// @param type_of_room type of room (R or V)
/// @param ord_number_of_the_room no. of the room (starts from 1)
void preorder_print(Node* p, char &type_of_room, int &ord_number_of_the_room);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief print all the patients in the given room
/// @param type_of_room type of medical room (R - regular or V - vip)
/// @param ord_number_of_the_room ordi number of the room (start from 1)
void print_patient_list(char type_of_room, int ord_number_of_the_room);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief convert from string room name to 2 parts: type of room and ord number of room
/// @param room given string room name
/// @param type_of_room type of room return value
/// @param no_of_room ord number of room return value
void get_room_info(string room, char &type_of_room, int &no_of_room);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief given command line arguments, do the task following to the cmd line
/// @param argc 
/// @param argv 
/// @return if the given cmd line is "Exit", return false; otherwise, returns true
bool do_the_task(int argc, char* argv[]);
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
/// @param is_emergency determine whether this patient is an emergency case
void coordinate_patient_to_room(Patient* p, bool is_emergency);
// Auxiliary function
/// @brief NOT DONE
/// @param p patient who needs to be coordinated to a room
/// @return the room
Room* choose_room_for_new_patient(Patient* p);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief remove an amount of the highest priority patients of a room 
/// @param room the room whose patients will be removed
/// @param number_of_finished_patients number of patients will be removed
void update_medical_room(Room* room, int number_of_finished_patients);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
#endif