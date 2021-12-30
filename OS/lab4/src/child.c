#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>

int main(int argc, char* argv[]){
        if (argc != 3){
                perror("incorrect number of arguments\n");
                exit(EXIT_FAILURE);
        }
        int out=open(argv[2],O_CREAT | O_WRONLY | O_TRUNC);
        if (out==-1){
                close(out);
                perror("I can not open file\n");
                exit(EXIT_FAILURE);
        }
        int k;
        int desc = open(argv[1], O_RDWR);
        if(desc < 0){
        perror("Tmp file not created\n");
                return -6;
        }
        struct stat buff;
        stat(argv[1], &buff);
        k=buff.st_size;
        char* fd = mmap(NULL, sizeof(char)*k,PROT_WRITE|PROT_READ,MAP_SHARED,desc, 0);
        if (fd == MAP_FAILED){
                perror("mmap error\n");
                return -5;
        }
        int cnt=0;
        char M[k];
        for(int i=0;i<k;i++){
                int sum=0;
                while(fd[i]!='\n'){
                        int n=0;
                        int z=1;
                        while(fd[i]==' ' || fd[i]=='\t'){
                                i++;
                        }
                        if (fd[i]=='-'){
                                z=-1;
                                i++;
                        }
                        while(fd[i]!=' ' && fd[i]!='\n' && fd[i]!='\t' && i<k){
                                n=n*10+fd[i]-'0';
                                i++;
                        }
                        sum=sum+z*n;
                }
                char t[11];
                int u;
                int j=0;
                int sum1=sum;
                if (sum1<0){
                        t[j]='-';
                        j++;
                        sum1*=-1;
                }
                if (sum==0){
                        t[j]='0';
                        j++;
                }
                while(sum1!=0){
                        u=sum1%10;
                        sum1=sum1/10;
                        t[j]=u+'0';
                        j++;
                }
                char g[j];
                if (t[0]=='-'){
                        g[0]='-';
                        for(int i=j-1;i>0;i--){
                                g[j-i]=t[i];
                        }
                }
                else{
                        for(int i=j-1;i>-1;i--){
                               g[j-i-1]=t[i];
                        }
                }
                for(int i=0;i<j;i++){
                        M[cnt]=g[i];
                        cnt++;
                }
                M[cnt]='\n';
                cnt++;
        }
        cnt++;
        fd[0]='\n';
        write(out,&M,sizeof(char)*(cnt-1));
        for(int i=1;i<cnt;i++){
                fd[i]=M[i-1];
        }
        msync(fd, sizeof(char)*cnt, MS_SYNC);
        munmap(fd, sizeof(char)*cnt);
        ftruncate(desc, sizeof(char)*cnt);
        close(desc);
        close(out);
        return 0;
}