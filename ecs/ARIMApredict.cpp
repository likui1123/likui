
#include <vector>
#include "cstdio"
#include "iostream"
#include "ARIMAModel.h"
#include <math.h>
#include <numeric>


int predict(vector<double> historySequence){

	int d = 1,m=0;
	int numOfPredict = 5;
	int numOfEffective = 0;
	vector<vector<int> > list;
	vector<int> tmpPredict(numOfPredict);
	bool flag=0;
	while ( m < numOfPredict)			
	{	
		vector<int> pqd = getPqd(historySequence,d, list, flag);	
		tmpPredict[m] = getPredictResult(pqd[0], pqd[1], d,historySequence);
		numOfEffective++;
		list.push_back(pqd);
		 m++;
		flag=1;
	}
	double sumPredict = (double)accumulate(tmpPredict.begin(),tmpPredict.end(),0)/ (double)numOfEffective;
	int predict = (int)(round)(sumPredict );
	if(predict<=0)
		predict=1;
	return predict;
}
