/***
 * KP Utils
 */
#include <numeric>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "utilities.h"
#include "stdlib.h"

using namespace std;

//********************** private to this compilation unit **********************

vector<process_stats> stats;

//if myString does not contain a string rep of number returns o
//if int not large enough has undefined behaviour, very fragile
int stringToInt(const char *myString) {
	return atoi(myString);
}

//clears vector (or whatever datastructure you choose) holding process_stats structs
//attempt to open file 'filename' to read, parse its rows
//into process_stats structs and add these structs to the vector holding these structs
//NOTE: be sure to ignore malformed rows (see 'Project 1 Description')
//
//if ignoreFirstRow ==true then discard the first row
//
//returns SUCCESS if all goes well or COULD_NOT_OPEN_FILE
int loadData(const char* filename, bool ignoreFirstRow) {
	//Clears the vector to prepare for new data
	stats.clear();

	//Makes sure contents is empty
	ifstream inputFile;
	inputFile.open(filename, ios::in);
	//Makes sure the file opened properly
	bool firstRow = true;
	if(inputFile.is_open()){
		//Input data one line at a time
		string line;
		while(!inputFile.eof()){
			getline(inputFile, line);
			if(firstRow and ignoreFirstRow){
				firstRow = false;
			}
			else{
				//Checks if there are 3 commas
				size_t num_commas = std::count(line.begin(), line.end(), ',');
				if(num_commas == 3){

					//Adds the values to a process_stat
					//If any of the values are missing, the line is not added to the vector

					stringstream ss(line);
					vector<int> data;
					while(ss.good()){
						string num;
						getline(ss, num, ',');
						//Only adds valid data
						if(num != " " && num != ""){
							data.push_back(stringToInt(num.c_str()));
						}
					}

					//Adds the process_stat to the vector
					if(data.size() == 4){
						process_stats stat;
						stat.process_number = data[0];
						stat.start_time = data[1];
						stat.cpu_time = data[2];
						stat.io_time = data[3];
						stats.push_back(stat);
					}
				}
			}
		}
		inputFile.close();
		return SUCCESS;
	}
	return COULD_NOT_OPEN_FILE;
}

bool sortByCPUTime(process_stats i, process_stats j){
	return i.cpu_time < j.cpu_time;
}

bool sortByPNum(process_stats i, process_stats j){
	return i.process_number < j.process_number;
}

bool sortByStartTime(process_stats i, process_stats j){
	return i.start_time < j.start_time;
}

bool sortByIOTime(process_stats i, process_stats j){
	return i.io_time < j.io_time;
}

//will sort according to user preference
//sorts the vector, returns nothing (thats what void means)
//sorts low to high
void sortData(SORT_ORDER mySortOrder) {
	switch(mySortOrder){
	case CPU_TIME:
		std::sort(stats.begin(), stats.end(), sortByCPUTime);
		break;
	case PROCESS_NUMBER:
		std::sort(stats.begin(), stats.end(), sortByPNum);
		break;
	case START_TIME:
		std::sort(stats.begin(), stats.end(), sortByStartTime);
		break;
	case IO_TIME:
		std::sort(stats.begin(), stats.end(), sortByIOTime);
		break;
	}
}

//return the first struct in the vector
//then deletes it from the vector
process_stats getNext() {
	process_stats stat;
	stat = stats[0];
	stats.erase(stats.begin());
	return stat;
}

//returns number of process_stats structs in the vector holding them
int getNumbRows(){
	return stats.size();
}


