#include <bits/stdc++.h>
using namespace std;
struct Process_Data
{
	//*****************************************************************************
	//variable declaration
	//*****************************************************************************
	int number;
	int process;
	int arrival_time; 
	int begining_time; 
	int priority;
	int finish_time; 
	int remaining_time; 
	int waiting_time;
	int starting_time;
	int Res_time;
};
//********************************************************************************
//sorting arrival time
//********************************************************************************
struct Process_Data current;
typedef struct Process_Data P_d ;
bool idsort(const P_d& x , const P_d& y)
{
	return x.process < y.process;
}
bool arrivalsort( const P_d& x ,const P_d& y)
{
	if(x.arrival_time < y.arrival_time)
		return true;
	else if(x.arrival_time > y.arrival_time)
		return false;
	if(x.priority < y.priority)
		return true;
	else if(x.priority > y.priority)
		return false;
	if(x.process < y.process)
		return true;
	return false;
}
bool numsort( const P_d& x ,const P_d& y)
{
	return x.number < y.number;
}
//********************************************************************************
//comparing priority
//********************************************************************************
struct compare
{
	bool operator()(const P_d& x ,const P_d& y)
	{
		if( x.priority > y.priority )
			return true;
		else if( x.priority < y.priority )
			return false;
		if( x.process > y.process )
			return true;
		return false;		
	}	
};
void my_check(vector<P_d> mv)
{
	for(unsigned int i= 0; i < mv.size() ;i++)
	{
		cout<<" process :"<<mv[i].process<<" _time : "<<mv[i].arrival_time<<" begining_time : "<<mv[i].begining_time<<" priority : "<<mv[i].priority<<endl;
	}
}
//********************************************************************************
//main function
//********************************************************************************
int main()
{
	int i;
	vector< P_d > input;
	vector<P_d> input_copy;
	P_d temp;
	int pq_process = 0;
	int rq_process = 0;
	int arrival_time;
	int begining_time;
	int process;
	int priority;
	int n;
	int clock;
	int total_exection_time = 0;
	cout<<"Enter the number of processes\n";
	cin>>n;
//********************************************************************************
//taking inputs from user
//********************************************************************************
	for( i= 0; i< n; i++ )
	{
		cout<<"|Process ID|\n";
		cin>>process;
		cout<<"|Arrival Time|\n";
		cin>>arrival_time;
		cout<<"|Burst Time|\n";
		cin>>begining_time;
		cout<<"|Priority|\n";
		cin>>priority;
		temp.number = i+1;
		temp.arrival_time = arrival_time;
		temp.begining_time = begining_time;
		temp.remaining_time = begining_time;
		temp.process = process;
		temp.priority = priority;
		input.push_back(temp);
	}
	input_copy = input;
	sort( input.begin(), input.end(), arrivalsort );
    total_exection_time = total_exection_time + input[0].arrival_time;
//********************************************************************************
//calculating execution time
//********************************************************************************
    for( i= 0 ;i< n; i++ )
    {
    	if( total_exection_time >= input[i].arrival_time )
    	{
    		total_exection_time = total_exection_time +input[i].begining_time;
    	}
    	else
    	{
    		int diff = (input[i].arrival_time - total_exection_time);
    		total_exection_time = total_exection_time + diff + begining_time;
    	}
    }
	int Ghant[total_exection_time]={0};
	for( i= 0; i< total_exection_time; i++ )
	{
		Ghant[i]=-1;
	}
	priority_queue < P_d ,vector<Process_Data> ,compare> pq;
	queue< P_d > rq;
	int cpu_state = 0;
	int quantum = 4 ;
	current.process = -2;
	current.priority = 999999;
	for ( clock = 0; clock< total_exection_time; clock++ )
	{
		for( int j = 0; j< n ; j++ )
		{
			if(clock == input[j].arrival_time)
			{
				pq.push(input[j]);
			}
		}
		if(cpu_state == 0)
		{
			if(!pq.empty())
			{
				current = pq.top();
				cpu_state = 1;
				pq_process = 1;
				pq.pop();
				quantum = 4; 
			}
			else if(!rq.empty())
			{
				current = rq.front();
				cpu_state = 1;
				rq_process = 1;
				rq.pop();
				quantum = 4;
			}
		}
		else if(cpu_state == 1)
		{
			if(pq_process == 1 && (!pq.empty()))
			{
				if(pq.top().priority < current.priority )
				{
					rq.push(current);
					current = pq.top();
					pq.pop();
					quantum = 4; 
				}
			}
			else if(rq_process == 1 && (!pq.empty()))
			{
				rq.push(current);
				current = pq.top();
				pq.pop();
				rq_process = 0;
				pq_process = 1;
				quantum = 4 ;
			}
					}
		if(current.process != -2)
		{
			current.remaining_time--;
			quantum--;
			Ghant[clock] = current.process;
			if(current.remaining_time == 0) 
			{
				cpu_state = 0 ;
				quantum = 4 ;
				current.process = -2;
				current.priority = 999999;
				rq_process = 0;
				pq_process = 0;
			}
			else if(quantum == 0 ) 
			{
				rq.push(current);
				current.process = -2;
				current.priority = 999999;
				rq_process = 0;
				pq_process = 0;
				cpu_state=0;
			}
		}
	}
	sort( input.begin(), input.end(), idsort );
	for(int i=0;i<n;i++)
	{
		for(int k=total_exection_time;k>=0;k--)
		{
			if(Ghant[k]==i+1)
			{
				input[i].finish_time=k+1;
				break;
			}
		}
	}
	for(int i=0;i<n;i++)
	{
		for(int k=0;k<total_exection_time;k++)
		{
			if(Ghant[k]==i+1)
			{
				input[i].starting_time=k;
				break;
			}
		}
	}
	sort( input.begin(), input.end(), numsort );
	for(int i=0;i<n;i++)
	{
		input[i].Res_time=input[i].starting_time-input[i].arrival_time;
		input[i].waiting_time=(input[i].finish_time-input[i].arrival_time)-input[i].begining_time;
	}
	cout<<"pro id"<<" res time"<<"fin time "<<"wait time"<<endl;
	for(int i=0;i<n;i++)
	{
		cout<<input[i].process<<"\t"<<input[i].Res_time<<"\t"<<input[i].finish_time<<"\t"<<input[i].waiting_time<<endl;
	}	
	return 0;
}

