/* os hw2 - launch.c
 * 2020039096 BaekInHyeok
 */

#include<stdio.h>//Header File which 'printf' function is included
#include<stdlib.h>//Header File which 'atoi' function is included
#include<sys/types.h>//Header File which has a lot of variable types
#include<unistd.h>//Header File which 'fork' function is included

int main(int argc, char *argv[]){//get argument vector form console command line

	int pid;//variable which process id will be saved
	pid=fork();//Perform fork() function. process id of child process and parent process are saved

	if(pid==0){
		/*child process*/
		printf("launch : start sumfact %s\n",argv[1]);//print on console
		
		/*
		execvp(filename,command line argument vector)
		give command line argument vector of launch.exe to sumfact.exe and loads 'sumfact.exe' into child process
		*/
		execvp("./sumfact",argv);
		return 0;
	}
	else
	{
		/*parent process*/	
		int N=atoi(argv[1]);//convert argv(char to integer) and save it in integer variable N
		int i=0;//variable for loop
		int sum=0;//variable sum of(1~N) is saved

		/*loop of sum(1~N)*/
		for(i=1;i<=N;i++){
			sum=sum+i;
		}

		printf("launch : sum = %d\n",sum);//print on console
		return 0;
	}
}
