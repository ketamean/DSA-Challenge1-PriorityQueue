#ifndef _auxiliary
#define _auxiliary

#include "structures.h"

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
void preorder_print(Node* p);
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
bool do_the_task(int argc, const char* argv[]);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief given the string determining the key task of the command: "New" or "See" or "Open" or "Update"
/// @param task the given string
/// @return the key task of the command in enumeration type TASK
TASK get_task(string task);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
#endif