#include <windows.h>
#include <stdio.h>
#define N 5
HANDLE forks[N];
DWORD WINAPI philosopher(LPVOID arg){
 int i=(int)(INT_PTR)arg,left=i,right=(i+1)%N;
 int first=left<right?left:right,second=left<right?right:left;
 WaitForSingleObject(forks[first],INFINITE);WaitForSingleObject(forks[second],INFINITE);
 printf("Philosopher %d is eating\n",i+1);
 ReleaseMutex(forks[second]);ReleaseMutex(forks[first]);return 0;
}
int main(void){
 HANDLE th[N];int i;
 for(i=0;i<N;i++)forks[i]=CreateMutex(NULL,FALSE,NULL);
 for(i=0;i<N;i++)th[i]=CreateThread(NULL,0,philosopher,(LPVOID)(INT_PTR)i,0,NULL);
 WaitForMultipleObjects(N,th,TRUE,INFINITE);
 for(i=0;i<N;i++){CloseHandle(th[i]);CloseHandle(forks[i]);}
 puts("Simulation completed");return 0;
}