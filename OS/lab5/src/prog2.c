#include <stdio.h>
#include <dlfcn.h>

int main(){
        int version = 1;
        int command;
        float (*E)(int) = NULL;
        double (*Square)(double, double) = NULL;
        void *lib1_handler = dlopen("./lib1.so",RTLD_LAZY);
        if (!lib1_handler){
            fprintf(stderr,"dlopen() error: %s\n", dlerror());
            return -1;
        }
        void *lib2_handler = dlopen("./lib2.so",RTLD_LAZY);
        if (!lib2_handler){
            fprintf(stderr,"dlopen() error: %s\n", dlerror());
            return -1;
        }
        E = dlsym(lib1_handler,"E");
        Square = dlsym(lib1_handler,"Square");
        printf("0 - to switch realizations\n");
        printf("1 + float x - calculation of exponent with accuracy of x\n");
        printf("2 + float A + float B - square of reactangle (in realisation 1) or right triangle (in realization 2) with sides A and B\n");
        while(scanf("%d", &command) != EOF){
                if(command==0){
                        if(version==2){
                                E = dlsym(lib1_handler,"E");
                                Square = dlsym(lib1_handler,"Square");
                                version=1;
                        } else{
                                E = dlsym(lib2_handler,"E");
                                Square = dlsym(lib2_handler,"Square");
                                version=2;
                        }
                        printf("Switched to realization %d\n", version);
                } else if(command==1){
                        float x;
                        if(scanf("%f", &x) != 1){
                                printf("Wrong arguements!\n");
                                continue;
                        }
                        printf("%f\n", E(x));
                } else if(command==2){
                        float A,B;
                        if(scanf("%f%f", &A, &B) != 2){
                                printf("Wrong arguements!\n");
                                continue;
                        }
                        float S=Square(A,B);
                        printf("%f\n",S);
                } else{
                        printf("Wrong command!\n");
                }
        }
        dlclose(lib1_handler);
        dlclose(lib2_handler);
}

