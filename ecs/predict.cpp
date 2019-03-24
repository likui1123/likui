#include "predict.h"

struct info{
    vector<string> flavor;
    vector<int> cpu_mem_vm[2];
} flavorinfo;			//flavor infomation from info file
int cpu_mem_hard_ph[3];		//obtain physical machine models
const int oneDay = 24 * 60 * 60;

vector<string> flavor;
vector<int>  cpu_mem_vm[2];	//vm's cpu memory
vector<int> flavorPridictResult;
vector< vector<int> > sequences;
int predictNumOfDay, historyNumOfDay;//the number of day of need predict and history data 
string dim, predictBeginTime, predictFinishTime, historyBeginTime, historyFinishTime;
time_t predictBeginTime_t, predictFinishTime_t, historyBeginTime_t, historyFinishTime_t;//string to time_t

void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename)
{
    getInfo(info);	//obtain information
    getData(data, data_num);//obtain history data
    myPredict();
    put_flavors(flavor, flavorPridictResult, dim, filename);//put flavor to physical mechine

//	char * result_file = (char *)"17\n\n0 8 0 20";
//	write_result(result_file, filename);
}
int myPredict(void)
{
    // init sequences
    vector< vector<int> > historySequences;
    for(unsigned int i=0; i<sequences.size(); i++)
    {
        vector<int> historySequence;
	vector<int> tmp;
	//double *p;
	double av=avgData(sequences[i]);  //conut average
	double s=stderrData(sequences[i]);// count standard variance
	//cout<<"  av="<<av<<"  s="<<s;
	for(unsigned int j=0; j<sequences[i].size(); j++)
	{
	   // p=boxMap(sequences[i]);
	    //if(sequences[i][j]>p[0]&&sequences[i][j]<p[1])	
	 if((fabs(sequences[i][j]-av)<(6*s))||(s==0))		//remove abnormal data
	 //if(fabs(sequences[i][j]-av)<(3*s+22))
	//if(fabs(sequences[i][j]-av)<(4.1*av))
	        tmp.push_back(sequences[i][j]);
	    else
	    {
		//sequences[i][j]=av;
		//av=avgData(sequences[i]);
		//s=stderrData(sequences[i]);
		//j=0;
		tmp.push_back(ceil(av));
	    }
	}
        for(unsigned int j=0; j<tmp.size()-predictNumOfDay+1; j++)
        {
           historySequence.push_back(accumulate(tmp.begin()+j, tmp.begin()+j+predictNumOfDay, 0));//count the sum of history data by predictNumOfDay
		//historySequence.push_back(sequences[i]);
        }
        historySequences.push_back(historySequence);
    }
    //cout << "historySequences: " << endl;
    for(unsigned int i=0; i<historySequences.size(); i++)
    {
        
	//vector<double> temp(historySequences[i].size()-20,0.4);//80.159
	//vector<double> temp(historySequences[i].size()-33,0);
vector<double> temp;
	//vector<double> temp;
       for(unsigned int j=0; j<historySequences[i].size(); j++)//convert type from int to double
       {   
	    temp.push_back(historySequences[i][j]);
        }

      for(int j=0; j<predictNumOfDay-1; j++)//81.07  81.08   the num of predict
      {

        cout<<"begin predict:"<<predict(temp)<<"  ";
	double ave=avData(temp);
	
	
	double sdata=sterrData(temp);
	int result=predict(temp);

	//if(fabs(result-ave)<0.215*sdata)//81.07   judge the predict is correct
	if(fabs(result-ave)<7.4)//81.084
//if(fabs(result-ave)<10)//80.159
       {
		cout<<"predict is correct"<<endl;
	     temp.push_back(result);	
       }
	//else
	//temp.push_back(0.4*ave+0.6*temp.back());
      }cout<<endl;
	//vector<double> temp = historySequences[i]; 
	flavorPridictResult.push_back(predict(temp));//81.07  81.084

    }

    return 0;
}


int getInfo(char * info[MAX_INFO_NUM])
{
    int numVM,space[2];
    string str = info[0];

    space[0] = str.find_first_of(' ');
    space[1] = str.find_last_of(' ');
    cpu_mem_hard_ph[0] = atoi(str.substr(0,space[0]).c_str());  //string to int
    cpu_mem_hard_ph[1] = atoi(str.substr(space[0]+1, space[1]-space[0]-1).c_str()); 
    cpu_mem_hard_ph[2] = atoi(str.substr(space[1]+1).c_str());

    str = info[2];
    numVM = atoi(str.c_str());

    for(int i=3; i<3+numVM; i++)
    {
        str = info[i];
        space[0] = str.find_first_of(' ');
        space[1] = str.find_last_of(' ');
        flavor.push_back(str.substr(0,space[0]));
        cpu_mem_vm[0].push_back(atoi(str.substr(space[0]+1, space[1]-space[0]-1).c_str()));
        cpu_mem_vm[1].push_back(atoi(str.substr(space[1]+1).c_str())/1024);
    }

    flavorinfo.flavor = flavor;
    flavorinfo.cpu_mem_vm[0] = cpu_mem_vm[0];
    flavorinfo.cpu_mem_vm[1] = cpu_mem_vm[1];
////read file ,get predict time
    dim = info[3+numVM+1];
    predictBeginTime = info[3+numVM+3];
    predictFinishTime = info[3+numVM+4];
    predictBeginTime_t = strToTime(predictBeginTime.c_str());
    predictFinishTime_t = strToTime(predictFinishTime.c_str());
    predictNumOfDay = ceil((float)(predictFinishTime_t - predictBeginTime_t)/oneDay);

    return 0;
}
int getData(char * data[MAX_DATA_NUM], int data_num)
{
	//by the history data format obtain the history data
    int tab[2],lastSpace;
    string str = data[0];
   // int firstTab = str.find_first_of('\t');
    tab[1] = str.find_last_of('\t');
    lastSpace = str.find_last_of(' ');
    historyBeginTime = str.substr(tab[1]+sizeof("\t")-1, lastSpace-tab[1]-1);
    historyBeginTime.append(" 00:00:00");
    cout << historyBeginTime <<endl;
    str = data[data_num-1];
    tab[0] = str.find_first_of('\t');
    tab[1] = str.find_last_of('\t');
    lastSpace = str.find_last_of(' ');
    historyFinishTime = str.substr(tab[1]+sizeof("\t")-1, lastSpace-tab[1]-1);//
    historyFinishTime.append(" 23:59:59");
    cout << historyFinishTime <<endl;

    historyBeginTime_t = strToTime(historyBeginTime.c_str());
    cout << historyBeginTime_t << endl;
    historyFinishTime_t = strToTime(historyFinishTime.c_str());
    cout << historyFinishTime_t << endl;
    historyNumOfDay = ceil((float)(historyFinishTime_t - historyBeginTime_t)/oneDay);
    cout << "historyNumOfDay: " << historyNumOfDay << endl;

    // get sequences
    for(unsigned int i=0; i<flavor.size(); i++)
    {
        vector<int> sequence(historyNumOfDay, 0); //historyNumOfDay ints with value 0;
        //search data
        for(int j=0; j<data_num; j++)
        {
            string str = data[j];
            tab[0] = str.find_first_of('\t');
            tab[1] = str.find_last_of('\t');
            lastSpace = str.find_last_of(' ');
            string vflavor = str.substr(tab[0]+sizeof("\t")-1, tab[1]-tab[0]-sizeof("\t")+1);
            time_t time =  strToTime(str.substr(tab[1]+sizeof("\t")-1, lastSpace-tab[1]-1).append(" 00:00:00").c_str());
            int dayDiff = (time - historyBeginTime_t)/oneDay; // if the data is same day indexs do not increase
            if(vflavor == flavor[i]) //this kind of flavor need to be predicted
            {
                sequence[dayDiff]++;   //add a record at daydiff index
            }
        }
        sequences.push_back(sequence);
    }
    return 0;
}
//time string format convert time_t format
time_t strToTime(string str)
{
    tm tms;
    time_t t;
    strptime(str.c_str(),"%Y-%m-%d %H:%M:%S",&tms);
    t=mktime(&tms);
    return t;
}





//count average and stander error
double avgData(vector<int> dataArray) {
	double sumData = accumulate(dataArray.begin(),dataArray.end(),0);
	//for (unsigned int i = 0; i<dataArray.size(); i++) sumData += dataArray[i];

	return sumData/dataArray.size();
}

double avData(vector<double> dataArray) {
	double sumData = accumulate(dataArray.begin(),dataArray.end(),0);
	//for (unsigned int i = 0; i<dataArray.size(); i++) sumData += dataArray[i];

	return sumData/dataArray.size();
}

double stderrData(vector<int> dataArray) {
	if (dataArray.size() <= 1) return 0.0;
	double variance = 0;
	double s =0;
double avgsumData =0;
for (unsigned int i = 0; i<dataArray.size(); i++) s += dataArray[i];
	avgsumData=s/dataArray.size();
	for (unsigned int i = 0; i<dataArray.size(); i++){
		dataArray[i] -= avgsumData;
		variance += dataArray[i] * dataArray[i];
	}
	return sqrt(variance / dataArray.size());

}
double sterrData(vector<double> dataArray) {
	if (dataArray.size() <= 1) return 0.0;
	double variance = 0;
	double s =0;
double avgsumData =0;
for (unsigned int i = 0; i<dataArray.size(); i++) s += dataArray[i];
	avgsumData=s/dataArray.size();
	for (unsigned int i = 0; i<dataArray.size(); i++){
		dataArray[i] -= avgsumData;
		variance += dataArray[i] * dataArray[i];
	}
	return sqrt(variance / dataArray.size() );

}

/*
double *boxMap(vector<int> array)

{
	double box[2];
	double q1,q3,temp1,temp2;
	sort(array.begin(),array.end());
	temp1=(double)(array.size()+1)/4;
	//q1=array[floor(temp1)-1]*(1-(temp1-floor(temp1)))+array[floor(temp1)]*(temp1-floor(temp1));
	q1=array[floor(temp1)-1]*(1-(ceil(temp1)-temp1))+array[floor(temp1)]*(ceil(temp1)-temp1);
	temp2=temp1*3;
	//q3=array[floor(temp2)-1]*(1-(temp2-floor(temp2)))+array[floor(temp2)]*(temp2-floor(temp2));
	q3=array[floor(temp2)-1]*(1-(ceil(temp2)-temp2))+array[floor(temp2)]*(ceil(temp2)-temp2);
	box[0]=q1-1.5*(q3-q1);
	box[1]=q3+1.5*(q3-q1);
	return box;
}*/


