#include <bits/stdc++.h>
#include<iostream>
#include <pthread.h>
#include <chrono>

#define ROW1 3
#define ROW2_COL1 5
#define COL 4

using namespace std;
using namespace std::chrono;

int first_Matrix [ROW1][ROW2_COL1];
int second_Matrix[ROW2_COL1][COL];
int answer_Matrix[ROW1][COL];

void* matrix_calculation_row(void*dectector_id){
    for (int j = 0; j < COL; j++)
    {
        for (int k = 0; k < ROW2_COL1; k++)
        {
            answer_Matrix[(long)dectector_id][j] += first_Matrix[(long)dectector_id][k] * second_Matrix[k][j];
        }
    }
}
void* matrix_calculation_element(void* dectector_id){
    int*x = (int*)dectector_id;
    for (int k = 0; k < ROW2_COL1; k++)
    {
        answer_Matrix[x[0]][x[1]] += first_Matrix[x[0]][k] * second_Matrix[k][x[1]];
    }
}
void read_input_file(int flag,int row,int coloum){
    if(flag)
    {
        for(int i=0; i<row; i++)
        {
            for(int j=0; j<coloum; j++)
            {
                cin>>first_Matrix[i][j];
            }
        }
    }
    else
    {
        for(int i=0; i<row; i++)
        {
            for(int j=0; j<coloum; j++)
            {
                cin>>second_Matrix[i][j];
            }
        }
    }
}
void Thread_by_row(int rows_first){
    pthread_t threads[rows_first];
    for(int i=0; i<rows_first; i++)
    {
        pthread_create(&threads[i], NULL,matrix_calculation_row,(void*)i);
    }
    for(int i=0; i<rows_first; i++)
    {
        pthread_join(threads[i],NULL);
    }
}
void Thread_by_element(int row,int coloum){
    pthread_t threads[row][coloum];
    int* x;
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<coloum; j++)
        {
            x = new int [2];
            x[0]=i;
            x[1]=j;
            pthread_create(&threads[i][j], NULL,&matrix_calculation_element,(void*)x);
        }
    }
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<coloum; j++)
        {
            pthread_join(threads[i][j], NULL);
        }
    }
}
void print_answer(int rows,int coloum,int time){
    for(int i=0; i<rows; i++)
    {
        cout<<endl;
        for(int j=0; j<coloum; j++)
        {
            cout<<answer_Matrix[i][j]<<" ";
        }
    }
    cout<<endl;
    cout <<"it's duration is equal "<< time << endl;
    cout<<endl;
}
void update_file(int time1,int time2,int row,int coloum){
    FILE * TFile;
    TFile = fopen ("output.txt","w");
    fputs ("The Result from threading of each Row\n",TFile);
    for(int i=0;i<row;i++){

        for(int j=0;j<coloum;j++){
            fprintf (TFile,"%d ",answer_Matrix[i][j]);
        }
        fprintf(TFile,"\n");
    }
    fprintf (TFile,"the time is equals  %d  microseconds\n\n",time1);
    fputs ("The Result from threading of each element\n",TFile);
    for(int i=0;i<row;i++){
        for(int j=0;j<coloum;j++){
            fprintf (TFile,"%d ",answer_Matrix[i][j]);
        }
        fprintf(TFile,"\n");
    }
    fprintf (TFile,"the time is equals  %d  microseconds\n",time2);
    fclose (TFile);
    return ;
}
void init(){
    for(int i=0; i<ROW1; i++)
    {
        for(int j=0; j<COL; j++)
        {
            answer_Matrix[i][j]=0;
        }
    }
}
int main(){
    freopen("input.txt","r",stdin);
    int rows_first,coloum_first,rows_second,coloum_second;
    cin>>rows_first>>coloum_first;
    read_input_file(1,rows_first,coloum_first);
    cin>>rows_second>>coloum_second;
    read_input_file(0,rows_second,coloum_second);
    auto t_start = high_resolution_clock::now();
    Thread_by_row(rows_first);
    auto t_stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(t_stop - t_start);
    cout<<"The Result from threading of each row :";
    print_answer(rows_first,coloum_second,duration.count());
    init();
    auto t_start1 = high_resolution_clock::now();
    Thread_by_element(rows_first,coloum_second);
    auto t_stop1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(t_stop1 - t_start1);
    cout<<"The Result from threading of each element :";
    print_answer(rows_first,coloum_second,duration1.count());
    update_file(duration.count(),duration1.count(),rows_first,coloum_second);
    return 0;
}
