#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
        if (argc != 1){
                perror("incorrect number of arguments\n");
                exit(EXIT_FAILURE);
        }
        int out=open(argv[0],O_CREAT | O_WRONLY | O_TRUNC);
        if (out==-1){
                close(out);
                perror("I can not open file\n");
                exit(EXIT_FAILURE);
        }
        int k;
        read(0,&k,sizeof(int));
        while(k!=-1){
                int  M[k];
                read(0, &M, sizeof(int)*k);
                int sum=0;
                for(int i=0;i<k;i++){
                        sum = sum + M[i];
                }
                //printf("%d\n",sum);
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
		char o='\n';
		write(out,&g,sizeof(char)*j);
                write(out,&o,sizeof(char));
		write(1, &sum, sizeof(int));

        	read(0,&k,sizeof(int));
      	}
        close(out);
        return 0;
}




