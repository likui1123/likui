

#ifndef ARIMAMODEL_H
#define ARIMAMODEL_H
#include <vector>
#include<math.h>
#include<stdlib.h>
#include "ARMAMath.h"
#include <stdlib.h>

using namespace std;
int predict(vector<double> );//predict the next index data by multi_num average
vector<double> diff(vector<double> ,int );//n_order different
vector<int> getPqd(vector<double> ,int , vector<vector<int> > , bool );//obtain the model's p q d;

int getPredictResult(int , int , int ,vector<double> );//get once predict result


#endif 
