#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

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
        int fd1[2], fd2[2];
        if (pipe(fd1) == -1) {
               perror("pipe1");
               exit(EXIT_FAILURE);
        }
        if (pipe(fd2) == -1) {
               perror("pipe2");
               exit(EXIT_FAILURE);
        }
        int id1 = fork();
        if (id1 == -1) {
                perror("fork error");
                exit(EXIT_FAILURE);
        } else if (id1 == 0){
                close(fd1[1]);
                close(fd2[0]);
                if (dup2(fd1[0], STDIN_FILENO) < 0) {
                        perror("Can't redirect stdin for child process");
                }
                if (dup2(fd2[1], STDOUT_FILENO) < 0) {
                     perror("Can't redirect stdout for child process");
                }
                close(fd1[0]);
                close(fd2[1]);
                execl("child.out", file_name, NULL);
        } else{
                close(fd1[0]);
                close(fd2[1]);
                int M[64];
                int k=0;
                while(read(0,&p,sizeof(char))!=0){
			while(p!='\n'){
                        	int n=0;
                                int z=1;
                                while(p==' ' || p=='\t'){
                                        read(0,&p,sizeof(char));
                                }
                                if (p=='-'){
                                        z=-1;
                                        read(0,&p,sizeof(char));
                                }
                                if(p<'0' || p>'9'){
                                        while(read(0,&p,sizeof(char))){
                                        }
                                        exit(EXIT_FAILURE);
                                }
                                while(p!=' ' && p!='\n' && p!='\t'){
                                        if(p<'0' || p>'9'){
                                                while(read(0,&p,sizeof(char))){
                                                }
                                                exit(EXIT_FAILURE);
                                        }
                                        n=n*10+p-'0';
                                        read(0,&p,sizeof(char));
                                }
                                M[k]=z*n;
                                k++;
                        }
                        write(fd1[1], &k, sizeof(int));
                        write(fd1[1], &M, sizeof(int)*k);
			int sum=0;
			read(fd2[0], &sum, sizeof(int));
			char t[11];
			int u;
			int j=0;
                        if (sum<0){
                                p='-';
                                write(1, &p, sizeof(char));
                                sum*=-1;
                        }
                        if (sum==0){
                                p='0';
                                write(1, &p, sizeof(char));
                        }
			while(sum!=0){
				u=sum%10;
				sum=sum/10;
				t[j]=u+'0';
				j++;
			}
			for(int i=j-1;i>-1;i--){
				write(1, &t[i], sizeof(char));
			}
			p='\n';
			write(1, &p, sizeof(char));
			k=0;
		}
               	k=-1;
               	write(fd1[1], &k, sizeof(int));
               	close(fd1[1]);
               	close(fd2[0]);
        }
        return 0;
}
