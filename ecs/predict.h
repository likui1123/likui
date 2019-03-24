#ifndef __ROUTE_H__
#define __ROUTE_H__

#include <stdio.h>
#include <iostream>
#include "lib_io.h"
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <numeric>
#include "put_flavors.h"
#include "ARIMAModel.h"
#include <algorithm>

using namespace std;

void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename);
int getInfo(char * info[MAX_INFO_NUM]);
int getData(char * data[MAX_DATA_NUM], int data_num);
time_t strToTime(string str);

int myPredict(void);

double avgData(vector<int> dataArray);

double stderrData(vector<int> dataArray);
double avData(vector<double> dataArray);

double sterrData(vector<double> dataArray);
double *boxMap(vector<int> array);

#endif
