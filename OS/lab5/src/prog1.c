#include <stdio.h>
#include "functions.h"

int main(){
        printf("1 + float x - calculation of exponent with accuracy of x\n");
        printf("2 + float A + float B - square of reactangle with sides A and B\n");
        int command;
        while(scanf("%d", &command) != EOF){
                if(command==2){
                        float A,B;
                        if(scanf("%f%f", &A, &B) != 2){
                                printf("Wrong arguements!\n");
                                continue;
                        }
                        float S=Square(A,B);
                        printf("%f\n",S);
                } else if(command==1){
                        float x;
                        if(scanf("%f", &x) != 1){
                                printf("Wrong arguements!\n");
                                continue;
                        }
                        printf("%f\n", E(x));
                } else{
                        printf("Wrong command!\n");
                }
        }
}