#ifndef _auxiliary
#define _auxiliary

#include "structures.h"

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief given a character array that is the command line entered from keyboard. Convert the input cstring to command-line arguments
/// @param cmd the command line entered from keyboard
/// @param argc number of arguments
/// @param argv arguments
void generateCommandArguments(char cmd[], int &argc, char* argv[]);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief 
/// @param tmp_task 
/// @param task 
/// @return if the given 
bool doTheTask(PriorityQueue* &PQ, int argc, const char* argv[]);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
/// @brief given the string determining the key task of the command: "New" or "See" or "Open" or "Update"
/// @param task the given string
/// @return the key task of the command in enumeration type TASK
TASK getTask(string task);
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
#endif