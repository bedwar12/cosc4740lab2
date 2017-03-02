#include <cstdio> //needed for perror
#include <iostream>
#include <pthread.h>
#include <cstdlib>

//GLOBAL VARIABLES for the statistics
//Threads can access global data
//It is important to note that no two threads are accessing
//these variables at the same time!
double myMean;
double myMin;
double myMax;
using namespace std;
//A structure to hold parameters to pass to a thread
struct StatData
{
    //The number of numbers
    int N;
    //The array of numbers
    double* numbers;
};

void* minimum(void* data)
{
    //You should calculate the minimum here and store in min
    StatData* myData = (StatData*)data;
    myMin = myData->numbers[0];
    for(int i = 1; i < myData->N; i++)
    {
        if(myData->numbers[i] < myMin)
        {
            myMin = myData->numbers[i];
        }
    }
}

void* maximum(void* data)
{
    StatData* myData = (StatData*)data;
    myMax = myData->numbers[0];
    for(int i = 1; i < myData->N; i++)
    {
        if(myData->numbers[i] > myMax)
        {
            myMax = myData->numbers[i];
        }
    }
}

void* mean(void* data)
{
    StatData* myData = (StatData*)data;
    double sum=0;
    for(int i = 0; i < myData->N; i++)
    {
        sum= (sum+myData->numbers[i]);
    }
    myMean=sum/myData->N;
	cout<<sum<<"\n";
}


/*
    You need to modify the main function so that it creates
    the maximum and average worker threads which execute the
    correct worker functions you created above.  Remember to
    make sure the main processes waits until they are done before
    printing the results.

    As an interesting excercise, what happens if you don't wait?
*/
int main(int argc, char** argv)
{
    pthread_t min_t, max_t, mean_t;
   
	
    //Initialize the stat_thread_data structure to hold 
    //the input parameters    
    StatData data;
    data.N = argc - 1;
    double* numbers = new double[data.N];
    for(int i = 1; i < argc; i++)
    {
        numbers[i-1] = atof(argv[i]);
    }
	data.numbers = numbers;
    if (pthread_create(&min_t, NULL, &minimum, &data)) 
    {
        perror("error creating the min worker thread");
        exit(1);
    }
	if (pthread_create(&max_t, NULL, &maximum, &data)) 
    {
        perror("error creating the max worker thread");
        exit(1);
    }
    	if (pthread_create(&mean_t, NULL, &mean, &data)) 
    {
        perror("error creating the average worker thread");
        exit(1);
    }
    pthread_join(min_t, 0);
	pthread_join(max_t, 0);
	pthread_join(mean_t, 0);

    cout<<"\nThe average value is "<<myMean<<"\nThe minimum value is "<<myMin<<"\nThe Maximum value is "<<myMax<<"\n\n";
    delete numbers;
}