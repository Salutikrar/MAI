#include <stdio.h>
#include <stdlib.h>

float E(int X){
        float x=X;
        float f=1+1/x;
        float h=1;
        while(x>0){
                x--;
                h=h*f;
        }
        return h;
}

float Square(double A, double B){
        return A*B;
}