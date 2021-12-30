#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include "unistd.h"

int main(){
        char string[100];
        char p;
        read(0,&p,sizeof(char));
        int z=0;
        while(p!='\n'){
                string[z]=p;
                z++;
                read(0,&p,sizeof(char));
        }
        char file_name[z];
        for(int i=0;i<z;i++){
                file_name[i]=string[i];
        }
        char template[] = "/tmp/tmpXXXXXX";
        int desc = mkstemp(template);
        if(desc < 0){
                perror("Tmp file not created\n");
                return -6;
        }
        printf("%s\n", template);
        int id1 = fork();
        if (id1 == -1) {
                perror("fork error");
                exit(EXIT_FAILURE);
        } else if (id1 == 0){
                sleep(5);
                execl("child.out", "child.out", template, file_name, NULL);
        } else{
                char M[3000];
                int k=0;
                while(read(0,&p,sizeof(char))!=0){
                        if((p<'0' || p>'9') && p!=' ' && p!='\n' && p!='\t' && p!='-'){
                                while(read(0,&p,sizeof(char))){
                                }
                                exit(EXIT_FAILURE);
                        }
                        M[k]=p;
                        k++;
                }
                ftruncate(desc, sizeof(char)*k);
                char* fd = mmap(NULL, sizeof(char)*k, PROT_READ | PROT_WRITE, MAP_SHARED, desc, 0);
                if (fd == MAP_FAILED){
                        perror("mmap error\n");
                        exit(EXIT_FAILURE);
                }
                for(int i=0;i<k;i++){
                        fd[i]=M[i];
                }
                msync(fd, sizeof(char)*k, MS_SYNC);
                munmap(fd, sizeof(char)*k);
                int status;
                waitpid(id1, &status,WUNTRACED);
                if(WEXITSTATUS(status)){
                        exit(EXIT_FAILURE);
                }
                struct stat buff;
                fstat (desc, &buff);
                k=buff.st_size;
                fd = mmap(NULL, sizeof(char)*k, PROT_READ | PROT_WRITE, MAP_SHARED, desc, 0);
                for(int i=0;i<k;i++){
                        printf("%c",fd[i]);
                }
        }
        return 0;
}