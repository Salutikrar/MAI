#include <pthread.h>
#include <stdlib.h>
#include <mcheck.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>


typedef struct{
        int cnt;
        int *arr;
} pthrData;

int* Sum;

#define memory 724
#define thread_size 4                      

void* threadFunc(void* thread_data){
        pthrData *data = (pthrData*) thread_data;
        for(int i=0;i<data->cnt;i++){
                Sum[i]=Sum[i]+data->arr[i];
        }
        return NULL;
}

int main(int argc, char * argv[]){
        if(argc!=2){
                fprintf(stderr, "Неверное количество аргументов\n");
                return 1;
        }
        int n,m;
        scanf("%d%d",&n,&m);
        long long p=atoi(argv[1]);
        if(p<1){
                fprintf(stderr, "Количество памяти не может быть меньше единицы\n");
                return 2;
        }
        p=p-memory-thread_size*(n-1);
        if(p<0){
                fprintf(stderr, "Количество памяти слишком мало\n");
                return 3;
        }
        int Arr[n][m];
        for(int i=0;i<n;i++){
                for(int j=0;j<m;j++){
                        scanf("%d",&Arr[i][j]);
                }
        }
        pthread_t threads[n];
        pthrData threadData[n];
        Sum=malloc(sizeof(int)*m);
        for(int i=0;i<m;i++){
                Sum[i]=0;
        }
        for(int i=0;i<n;i++){
                threadData[i].cnt=m;
                threadData[i].arr=malloc(sizeof(int)*m);
                for(int j=0;j<m;j++){
                        threadData[i].arr[j]=Arr[i][j];
                }
                pthread_create(&(threads[i]), NULL, threadFunc, &threadData[i]);
        }
        for(int i=0;i<n;i++){
                pthread_join(threads[i], NULL);
        }
        for(int i=0;i<m;i++){
                printf("%d ",Sum[i]);
        }
        printf("\n");
}