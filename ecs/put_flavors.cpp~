#include "put_flavors.h"

struct put{
    int id;
    vector<string> flavor;
    vector<int> num;  //能放置该类型虚拟机个数
    int cpu_mem_surplus[2];   
};

extern int cpu_mem_hard_ph[3];
extern struct info{
    vector<string> flavor;
    vector<int> cpu_mem_vm[2];
} flavorinfo;


int put_flavors(vector<string> flavor, vector<int> flavorPridictResult, string dim, char* filename)
{

    vector<struct put> puts;
    bool flag=0,reFlag=0;
    int reNum=1;
    const char *ss="MEM";
    const char *gs=dim.substr(0,3).c_str();
    if(strcmp(ss,gs)==0)
     {
        flag=1;
	cout<<"desicion MEM"<<endl;
    }
    for( int i=0;i<(int)flavor.size();i++)
    {
	cout<<"qian:"<<flavorinfo.flavor[i]<<" "<<flavorinfo.cpu_mem_vm[flag][i]<<" "<<flavorPridictResult[i]<<endl;
    }
   for( int i=0;i<(int)flavor.size();i++)
	for( int j=0;j<(int)flavor.size()-i-1;j++)
		if(flavorinfo.cpu_mem_vm[flag][j]>flavorinfo.cpu_mem_vm[flag][j+1])
		{

		    int t;
		    string s;
		    t=flavorinfo.cpu_mem_vm[flag][j];
		    flavorinfo.cpu_mem_vm[flag][j]=flavorinfo.cpu_mem_vm[flag][j+1];
		    flavorinfo.cpu_mem_vm[flag][j+1]=t;

		    s=flavorinfo.flavor[j];
		    flavorinfo.flavor[j]=flavorinfo.flavor[j+1];
		    flavorinfo.flavor[j+1]=s;

		    t=flavorinfo.cpu_mem_vm[!flag][j];
		    flavorinfo.cpu_mem_vm[!flag][j]=flavorinfo.cpu_mem_vm[!flag][j+1];
		    flavorinfo.cpu_mem_vm[!flag][j+1]=t;
			
		    t=flavorPridictResult[j];
		    flavorPridictResult[j]=flavorPridictResult[j+1];
		    flavorPridictResult[j+1]=t;
		}

    for( int i=0;i<(int)flavor.size();i++)
	cout<<flavorinfo.flavor[i]<<" "<<flavorinfo.cpu_mem_vm[flag][i]<<" "<<flavorPridictResult[i]<<endl;
    vector<int> predict_backup = flavorPridictResult;
    do{
	    reFlag=0;
	    flavorPridictResult.assign(predict_backup.begin(),predict_backup.end());
	    struct put new_put_init;//init put struct
	    new_put_init.id = puts.size() + 1;
	    new_put_init.flavor = flavorinfo.flavor;
	    vector<int> num(flavorinfo.flavor.size(), 0);
	    new_put_init.num = num;
	    new_put_init.cpu_mem_surplus[0] = cpu_mem_hard_ph[0];
	    new_put_init.cpu_mem_surplus[1] = cpu_mem_hard_ph[1];
	    
	    struct put new_put=new_put_init;//open a new physical machine
	    puts.push_back(new_put);
	    for( int i=(int)flavor.size()-1; i>=0; i--)
	    {
		struct put new_put = puts.back();
		while(flavorPridictResult[i]>0) 
		{
		    if(flavorinfo.cpu_mem_vm[0][i] <= new_put.cpu_mem_surplus[0] && flavorinfo.cpu_mem_vm[1][i] <= new_put.cpu_mem_surplus[1]) //this physics still has space
		    {
			//cout<<" surplus:"<< new_put.cpu_mem_surplus[1]/1024<<" ";
		        new_put.num[i]++;   //put
		        new_put.cpu_mem_surplus[0] -= flavorinfo.cpu_mem_vm[0][i]; 
		        new_put.cpu_mem_surplus[1] -= flavorinfo.cpu_mem_vm[1][i];
		        flavorPridictResult[i]--;  //vm substract;
			//cout<<flavorPridictResult[i]<<"  ";
		    }
		    else  //this physics can't put this kind of flavor
		    {
		        int j=i-1;
			while(j>=0)//search a smaller flavor
		        {
		            if(flavorinfo.cpu_mem_vm[0][j] <= new_put.cpu_mem_surplus[0] && flavorinfo.cpu_mem_vm[1][j] <= new_put.cpu_mem_surplus[1]) 
		            {
		                new_put.num[j]++;  //put
		                new_put.cpu_mem_surplus[0] -= flavorinfo.cpu_mem_vm[0][j]; //resources substract
		               new_put.cpu_mem_surplus[1] -= flavorinfo.cpu_mem_vm[1][j];
		                flavorPridictResult[j]--;  //vm substract;
			      //cout<<"seek for small vm"<<flavorPridictResult[i]<<"  ";
		                break;  //when find a kind of flavor can put need break this loop
		            }
			    j-=1;
		        }

		        if(j>=0) //have smaller flavor
		            continue;
		        else   
		        {
		            puts.erase(puts.end());
		            puts.push_back(new_put);  //current physics machine put finish			                   
		            struct put new_put=new_put_init;  //and open a new physical machine  
			    new_put.id = puts.size() + 1;           
		            puts.push_back(new_put);
		            break;
		        }

		    }
		}
		if(flavorPridictResult[i]==0 || puts.back().cpu_mem_surplus[0] != cpu_mem_hard_ph[0] || puts.back().cpu_mem_surplus[1] != cpu_mem_hard_ph[1])
		{
		    puts.erase(puts.end());
		    puts.push_back(new_put);  //current physics machine put finish	
		    // struct put new_put=new_put_init;
		}
		if(flavorPridictResult[i]>0)
		    i+=1;  
	    }
	for(int i=0;i<(int)puts.size();i++)
	{
               cout<<"backup:"<<predict_backup[i]<<endl;
		double temp=(double)(cpu_mem_hard_ph[flag]-puts[i].cpu_mem_surplus[flag])/cpu_mem_hard_ph[flag];
		if(temp<0.150)
		{
		 for( int j=0; j<(int)puts[i].flavor.size(); j++)
		{
			predict_backup[j]-=puts[i].num[j];
			reFlag=1;
		}
			reNum++;
		}
		
	
	    cout<<"surplus:"<<(double)(cpu_mem_hard_ph[flag]-puts[i].cpu_mem_surplus[flag])/cpu_mem_hard_ph[flag]<<endl;
	
	}if((reNum==2)&&reFlag)
		{
		cout<<"clear"<<endl;puts.clear();
		}
	
}while(reFlag&&(reNum<3));


    //output to output.txt
    string result_file = "";
    char str[15];
    int i=0,totalVM = accumulate(predict_backup.begin(), predict_backup.end(), 0);
    sprintf(str, "%d", totalVM);
    result_file.append(str).append("\n");
    for( int k=0; k<(int)flavor.size(); k++)
    {
        sprintf(str, "%d", predict_backup[k]);
        result_file.append(flavor[k]).append(" ").append(str).append("\n");
    }
    sprintf(str, "%d", (int)puts.size());
    result_file.append("\n").append(str).append("\n");
    while(i<(int)puts.size())
    {
        sprintf(str, "%d", puts[i].id);
        result_file.append(str).append(" ");
        for( int j=0; j<(int)puts[i].flavor.size(); j++)
        {
            if(puts[i].num[j]!=0) 
            {
		
		//int temp =;
		sprintf(str, "%d", puts[i].num[j]);
                result_file.append(puts[i].flavor[j]).append(" ").append(str).append(" ");
            }
		//sprintf(str, "%d", puts[i].cpu_mem_surplus[flag]); result_file.append("sur:").append(str);
        }
        result_file.append("\n");
	i+=1;
    }

    cout << result_file;
    write_result(result_file.c_str(), filename);
return 0;
}
