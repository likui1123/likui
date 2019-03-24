#include "ARIMAModel.h"
#include "ARMAMath.h"


#include <iostream>
using namespace std;
vector<vector<double>> parameter;
double predictPara=0;     //reserved nosise value

//n_order different
vector<double> diff(vector<double> data,int period)
{
	vector<double> dat;
	double tmp;
	int i=0;
	while(i<(int)data.size() - period)
	{
		tmp = data[i + period] - data[i];
		dat.push_back(tmp);
		i+=1;
	}
	return dat;//return different data sequence
}

//obtain the model's p q d;
vector<int> getPqd(vector<double> historySequence,int period, vector<vector<int> > notModel, bool needNot)
{
	
	vector<double> data(diff(historySequence,period));//n_different 
	double Aic = 99999999999.0;			//initial AIC value
	vector<int> pqd(3);//0 1 2-->p q d
	int kind = 0;
	vector<vector<double> > para;			//reserve AR MA ARMA parameter
	///initial p q init value
	int model[19][2]={{0,1},{0,2},{0,3},{0,4},{0,5},{1,0},\   
			{1,1},{1,2},{1,3},{1,4},{2,0},{2,1},{2,2},\
			{2,3},{3,0},{3,1},{3,2},{4,0},{4,1}};
	for (int i = 0; i < 19; i++)
	{
		bool token = false;
		if (needNot)
			for (int k = 0; k <(int) notModel.size(); ++k)
				if (model[i][0] == notModel[k][0] && model[i][1] == notModel[k][1])// p q value is the same of front break this loop
				{
					token = true;
					break;
				}
		if (token)
			continue;
		
		if (model[i][0] == 0)
		{
			continue;
		}
		else if (model[i][1] == 0)
		{
			vector<vector<double> > temp;
			vector<double>  arPara(getParaAr(data, model[i][0], model[i][0],0,(int)data.size()));
			temp.push_back(arPara);
			para =temp;
			kind = 2;
		}
		else
		{
			continue;
	
		}

		
		//double aic = getModelAIC(para, data, kind,(int)data.size());
		vector<double> aic ( aicCheck(para, data, kind,(int)data.size()));
		if (aic[0] <= 99999999999.0 && aic[0] < Aic)
		{
			Aic = aic[0];
			predictPara=aic[1];
			pqd[0] = model[i][0];
			pqd[1] = model[i][1];
			pqd[2] = round(Aic);
			
			parameter = para;
			//cout<<"1:"<<"";
		}
	}
	return pqd;
}




int getPredictResult(int p, int q, int period,vector<double> historySequence){
	
	vector<double> data(diff(historySequence,period));
	int n = data.size();
	int predictResult = 0;
	double arTemp = 0.0, maTemp = 0.0;//noise value
	vector<double> errData(q + 1);

	if (p == 0)
	{
		
		predictResult =(int)((0.4*(int)(maTemp) + 0.6*historySequence[historySequence.size() - period])*2) ; //产生预测
	}
	else if (q == 0)
		predictResult =(int)((0.4*(int)(predictPara) + 0.6*historySequence[historySequence.size() - period])*2) ; //产生预测 
	else
	{
		
		predictResult =(int)((0.4*(int)(maTemp+maTemp) + 0.6*historySequence[historySequence.size() - period])*2) ;
	}
	return predictResult;
}


