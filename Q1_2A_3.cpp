#include<sstream>
#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
using namespace std;

int no_of_rows = 1024;
float A[1024][1024] = {0};
float B[1024][1024] = {0};
float Normal_Result[1024][1024] = {0};
float Thread_Result[1024][1024] = {0};

//=================Serial Multiplication

void Normal_Multiply(float A[][1024], float B[][1024])
{
    for(int i=0; i<1024; i++)
    {
        for(int j=0; j<1024; j++)
        {
            Normal_Result[i][j] = 0;
        }
    }

	for(int i=0; i<1024; i++)
    {
        for(int j=0; j<1024; j++)
        {
          	for(int k=0; k<1024; k++)
            {
                Normal_Result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

//================ Thread Multiplication =================

void *Thread_Multiply(void *data)
{
    int *inf = (int *) data;
    srand((unsigned)time(NULL));
    // cout<<"Thread"<<" "<<inf[0]<<" "<<inf[1]<<endl;
    for(int i=inf[0]; i<inf[1]; i++)
    {
        for(int j=0; j<1024; j++)
        {
          	for(int k=0; k<1024; k++)
            {
                Thread_Result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

bool isEqual(float Norm_R[][1024], float Thread_R[][1024])
{
    for(int i=0; i<1024; i++)
    {
        for(int j=0; j<1024; j++)
        {
            if( Norm_R[i][j] != Thread_R[i][j] )
            {
                return false;
            }
        }
    }
    return true;
}

void intilize_Matrix()
{
    for(int i=0; i<1024; i++)
    {
        for(int j=0; j<1024; j++)
        {
            A[i][j] = 2*i+j+0.01;
        }
    }
    
    for(int i=0; i<1024; i++)
    {
        for(int j=0; j<1024; j++)
        {
            B[i][j] = i+j+0.05;
        }
    }

    for(int i=0; i<1024; i++)
    {
        for(int j=0; j<1024; j++)
        {
            Thread_Result[i][j] = 0;
        }
    }
}

void Print(float Matrxi[][1024], int N)
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            cout<<Matrxi[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

int main(int argc, char** argv)
{
    int no_of_threads = atoi(argv[1]);
    pthread_t tid[no_of_threads];
    
    int matrix_size = 1024;
    int size_to_send = matrix_size/no_of_threads;
    
    cout<<"No of Threads: "<<no_of_threads<<endl;
    cout<<"No of Blocks: "<<matrix_size/size_to_send<<endl;

    intilize_Matrix();
    Normal_Multiply(A, B);
    
    int start_Add = 0;
    int end_Add = size_to_send;
    cout<<endl;
    for(int i=0; i<no_of_threads; i++)
    {
        int *arr = new int[2];
        arr[0] = start_Add;
        arr[1] = end_Add;
        cout<<start_Add<<" "<<end_Add<<endl;
        pthread_create(&tid[i], NULL, Thread_Multiply, (void *)arr);
        
        start_Add += size_to_send;
        end_Add += size_to_send;
    }

    for(int i=0; i<no_of_threads; i++)
    {
        pthread_join(tid[i], NULL);
    }  
    
    // cout<<"Result"<<endl;
    // Print(Normal_Result, 1024);

    // cout<<"Result"<<endl;
    // Print(Thread_Result, 1024);
        
    if(isEqual(Normal_Result, Thread_Result)){
        cout<<"Matched :)"<<endl;
    }else{
        cout<<"Not Matched"<<endl;
    }

    return 0;
}