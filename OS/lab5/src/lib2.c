#include <stdio.h>
#include <stdlib.h>

float E(int x){
        float f=1;
        float k=1;
        for(int i=1;i<=x;i++){
                k=k*i;
                f=f+1/k;
        }
        return f;
}

float Square(double A, double B){
        return A*B/2;
}
