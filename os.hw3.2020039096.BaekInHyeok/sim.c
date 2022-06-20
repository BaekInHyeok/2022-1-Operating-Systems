/*os hw3 - sim.c
 * 2020039096 BaekInHyeok
 */
#include<stdio.h>//Header File which 'printf' function is included
#include<stdlib.h>//Header File which 'atoi' function is included
#include<sys/types.h>//Header File which has a lot of variable types
#include<unistd.h>//Header File which 'execvp' function is included
#include<string.h>//Header File which 'strcmp' function is included

int main(int argc, char *argv[]){//get argument vector form console command line

	const char* type="fifo";//create variable to distinguish command 'fifo' & 'nru'
	
	//command is 'fifo'
	if(strcmp(argv[1],type)==0){
		/*
		execvp(filename,command line argument vector)
		give command line argument vector of sim.exe to fifo.exe and loads 'fifo.exe'
		*/
		execvp("./fifo",argv);
	}
	
	else{
		/*
		execvp(filename,command line argument vector)
		give command line argument vector of sim.exe to nru.exe and loads 'nru.exe'
		*/
		execvp("./nru",argv);
	}
	return 0;
}
	
