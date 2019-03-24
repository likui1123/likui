
#include <cstring>
#include <cmath>
//#include <iostream>
#include <cstdlib>
#include "ARMAMath.h"
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include "ARIMAModel.h"

//count averge value
double average(vector<double> sequence,int len) {
	double sum=accumulate(sequence.begin(),sequence.end(),0);
	return sum / len;
}
//count variance value
double variance(vector<double> sequence,int len) {
	if ((int)sequence.size() == 0)
	     return 0;
	double temp = 0;
	int i=0;
	double avSum = accumulate(sequence.begin(),sequence.end(),0);
	while ( i<len){
		sequence[i] =sequence[i]- avSum;
		temp += sequence[i] * sequence[i];
		i+=1;
	}
	return temp / len;

}

//count autocovariance  as autocorrelation coefficient
vector<double>  autocor(vector<double> sequence, int order,int len) {
	vector<double>  autocor(order + 1,0);
	int i=0;
	double av = average(sequence,len);
	   do
		{
		for (int j = len-i-1; j>=0; j--){
			autocor[i] += (sequence[j + i] - av)*(sequence[j] - av);
		}
		autocor[i] /= (len - i);//addvice select
		autocor[i] /=variance(sequence,len);//stander formula
		autocor[i] *=variance(sequence,len);//stander formula
		i+=1;
	}while(i<=order);
	return autocor;
}


//normal distuibution  ,the E(x)=0.0,V(x)=1.0

//AIC checkout p q   2k-2*ln(L)   the smaller the AIC value ,the better
vector<double> aicCheck(vector<vector<double>> tem, vector<double> data, int kind,int len) {
	
	int p ,q;
	vector<double> predictPara;
	double ar = 0, ma = 0,sumErr =0;
	if (kind == 1){
		return predictPara;
	}
	else if (kind == 2){
		vector<double> arPara = tem[0];
		p = (int)arPara.size();

		for (int i = p - 1; i < len; ++i)
		{
			ar = 0.0;
			for (int j = 0; j < p - 1; ++j)
				ar += arPara[j] * data[i - j - 1];
			sumErr += (data[i] - ar) * (data[i] - ar);//count the sum of residual
		}
		//return (len-(p-1))*log(sumErr/(len-(p-1)))+(p)*log(len-(p-1));
		//double temp= (len - (p - 1)) * log(sumErr / (len - (p - 1))) + (p +1) * 2;//stander
		double temp= 2*log(sumErr / (len - (p - 1)))+2*(p+1);//advice select
		predictPara.clear();
		predictPara.push_back(temp);
		predictPara.push_back(ar);
		return predictPara;
	}
	else{
				
		return predictPara;
	}
}
// Levenson method  select model parameters minimize forecast error power

vector<vector<double>> optiData(vector<double> gar,int o,int t) {
	vector<double> gg;
	gg.assign(gar.begin()+o,gar.begin()+t);
	int order = gg.size() - 1;// the number of iterative
	vector<vector<double>> result;
	result.resize(order + 1);
	for (int i = 0; i<order + 1; i++)
	 result[i].resize(order + 1);
	vector<double> ss(order + 1);
	ss[0] = gg[0];
	result[1][1] = gg[1] / ss[0];
	ss[1] = ss[0] * (1.0 - result[1][1] * result[1][1]);
	for (int k = 1; k < order; k++)
	{
		double sumTop = 0.0;
		double sumSub = 0.0;
		for (int j = 1; j <= k; j++)
		{
			sumTop += gg[k + 1 - j] * result[k][j];
			sumSub += gg[j] * result[k][j];
		}
		result[k + 1][k + 1] = (gg[k + 1] - sumTop) / (gg[0] - sumSub);
		for (int j = 1; j <= k; j++)
		{
			result[k + 1][j] = result[k][j] - result[k + 1][k + 1] * result[k][k + 1 - j];
		}
		ss[k + 1] = ss[k] * (1.0 - result[k + 1][k + 1] * result[k + 1][k + 1]);
	}
	result[0] = ss;

	return result;
}
//by levenson  obtain AR parameter {a11,ss1^2}{a21,a22£¬ss2^2}{a31,a32,s33,ss3^2}...
vector<double> getParaAr(vector<double> sequence,int len, int p,int o,int t) {
	vector<double> gg = autocor(sequence,len,(int)sequence.size());
	vector<vector<double>> result(optiData(gg,o,t));
	vector<double> arPara;
	arPara.assign(result[p].begin()+1,result[p].begin()+p+1);	
	arPara.insert(arPara.end(),result[0][p]);

	return arPara;

}


