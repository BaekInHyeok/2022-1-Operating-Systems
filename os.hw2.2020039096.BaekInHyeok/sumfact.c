/* os hw2 - sumfact.c
 * 2020039096 BaekInHyeok
 */

#include<stdio.h>//Header File which 'printf' function is included
#include<stdlib.h>//Header File which 'atoi' function is included
#include<sys/types.h>//Header File which has a lot of variable types
#include<unistd.h>//Header File which 'fork' function is included
#include<pthread.h>//Header File which has functions for using thread

/*Thread function 1 : factorial(N)*/
void *thread_function1(void *data){//in this program, get argument vector from launch.exe
	int *N=(int *)data;//convert data to integer and save it in *N

	int factorial=1;//variable which factorial loop will be saved
	
	/*loop of factorial*/
	
	//result of 1*2*3*~*N and N*(N-1)*~2*1 are same
	for(int i=1;i<=*N;i++){
		factorial=factorial*i;
	}
	printf("sumfact : factorial = %d\n",factorial);//print on console

	return NULL;
}

/*Thread function 2 : sum(1~N)*/
void *thread_function2(void *data){
	int *N=(int *)data;//convert data to integer and save it in *N

	int sum=0;//variable which loop of sum(1~N) will be saved 

	/*loop of sum(1~N)*/
	for(int i=1;i<=*N;i++){
		sum=sum+i;
	}

	printf("sumfact : sum = %d\n",sum);//print on console

	return NULL;
}

int main(int argc, char *argv[]){
	int N=atoi(argv[1]);//convert argv(char to integer) and save it in integer variable N
	pthread_t thread_id[N];//create thread array. size is N

	int i=0;//variable which will be used for loop

	/*loop of pthread_create*/
	for(i=0;i<N;i++){

		if(i==0){//first thread
			/*create thread which perform thread_function1*/
			pthread_create(&thread_id[i],NULL,thread_function1,(void *)&N);
		}

		else{//other threads
			/*crate thread which perform thread_function2*/
			pthread_create(&thread_id[i],NULL,thread_function2,(void *)&N);

		}
	}
	/*loop of pthread_join*/
	for(i=0;i<N;i++){
		/*wait for end of thread and return NULL*/
		pthread_join(thread_id[i],NULL);
	}

	return 0;
}
