#include <stdio.h>
int main(void){
 int nb,np,b[20],p[20],i,j,choice,idx,best;
 printf("Number of blocks (1-20): ");
 if(scanf("%d",&nb)!=1||nb<1||nb>20)return 1;
 for(i=0;i<nb;i++){printf("Block %d size: ",i+1);
 if(scanf("%d",&b[i])!=1||b[i]<0)return 1;}
 printf("Number of processes (1-20): ");
 if(scanf("%d",&np)!=1||np<1||np>20)return 1;
 for(i=0;i<np;i++){printf("Process %d size: ",i+1);
 if(scanf("%d",&p[i])!=1||p[i]<0)return 1;}
 printf("Choose 1.First Fit\n2.Best Fit\n3.Worst Fit: ");
 if(scanf("%d",&choice)!=1||choice<1||choice>3)return 1;
 for(i=0;i<np;i++){idx=-1;best=(choice==3?-1:2147483647);
  for(j=0;j<nb;j++)if(b[j]>=p[i]&&((choice==1)||(choice==2&&b[j]<best)||(choice==3&&b[j]>best))){idx=j;best=b[j];if(choice==1)break;}
  if(idx<0)printf("P%d not allocated\n",i+1);else{printf("P%d -> Block %d\n",i+1,idx+1);b[idx]-=p[i];}}
 return 0;
}