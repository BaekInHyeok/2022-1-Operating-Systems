/*os hw3 - fifo.c
 * 2020039096 BaekInHyeok
 */

#include<stdio.h>//Header File which 'printf'. 'fopen', 'fgets', 'fclose' function is included
#include<stdlib.h>//Header File which 'atoi' function is included
#include<sys/types.h>//Header File which has a lot of variable types
#include<string.h>//Header File which 'strcmp' function is included

/*Use array*/

//Main
int main(int argc, char *argv[]){//get argument vector form console command line
	
	int frames=0;//variable of size of frame array
	frames=atoi(argv[2]);//set size of frame array from command line
	char *cwrite="write";//store 'write' to distinguish 'read' & 'write'
	
	int i=0;//index variable
	int firstin=0;//variable storing First-In pagenumber index
	
	int access=0;//count of access
	int read=0;//count of read
	int write=0;//count of write
	int hits=0;//count of hit
	int faults=0;//count of fault
	
	/*variables for calculating faults/access ratio*/
	int ratioint=0;
	float ratiofloat;
	
	char buffer[20];//array variable storing data from 'access.list'
	int compare=0;//variable storing result of strcmp()
	
	/*Create frame array and init array*/
	int frame[frames];
	for(i=0;i<frames;i++){
		frame[i]=-1;
	}

	//These two Variable are used to store sliced data read from access.list
	int pagenumber=0;//Storing pagenumber
	char *command=NULL;//Storing operation(read & write)
	
	FILE* fp=fopen("access.list","r");//open file 'access.list' in read mode
	
	while(!feof(fp)){//while file pointer is not end of file
		i=0;//init i to 0
		fgets(buffer,sizeof(buffer),fp);//read string from 'access.list'
		if(feof(fp)!=0) break;//if it is end of file. while loop end
		
		char *ptr=strtok(buffer," ");//slice string based on a " "(space). store it in char pointer variable 'ptr'
		pagenumber=atoi(ptr);//change ptr to integer and store it in pagenumber
		ptr=strtok(NULL," ");//slice next string and return ptr
		command=ptr;//store ptr in command
		compare=strcmp(cwrite,command);//strcmp(cwrite,command) and store the result to compare
		
		access+=1;//increase access count
		
		//when command is 'write'
		if(compare<0)
		{
			write+=1;//increase write count
			
			//Search frame array is full or not using for loop
			for(i=0;i<frames;i++){
				if(frame[i]==-1) break;
			}
			
			//If frame array is full -> need to modify loaded page
			if(i>=frames){
			
				hits+=1;//increase hits count
				
				//if firsin >= frames, this means all factors of array are changed at least one time so need to go back to start point.
				if(firstin>=frames){
					firstin=0;//store 0 in firstin
				}
				frame[firstin]=pagenumber;//store pagenumber in array which firstin is pointing
				firstin+=1;//increase firstin index
			}
			//If frame array is not full -> need to load new page in frame
			else{
				faults+=1;//increase faults count
				
				frame[i]=pagenumber;//store pagenumber in array which i is pointing
			}
		}
			
		//when command is 'read'
		else if(compare>0){
			
			read+=1;//increase read count
			
			//move index until we find array which pagenumber matched
			for(i=0;i<frames;i++){
				if(frame[i]==pagenumber) break;
			}
			
			//Failed to find same pagenumber in frame array
			if(i>=frames){
			
				faults+=1;//increase faults count
				
				//Search for frame array is full or not using for loop
				for(i=0;i<frames;i++){
					if(frame[i]==-1) break;
				}
				
				//If frame array is full -> need to modify loaded page
				if(i>=frames){
					//if firsin >= frames, this means all factors of array are changed at least one time so need to go back to start point.
					if(firstin>=frames){
						firstin=0;//store 0 in firstin
					}
					frame[firstin]=pagenumber;//store pagenumber in array which firstin is pointing
					firstin+=1;//increase firstin index
				}
				//If frame array is not full -> need to load new page in frame
				else{
					//move index until we find frame which page is not loaded yet
					for(i=0;i<frames;i++){
						if(frame[i]==-1) break;
					}
					
					frame[i]=pagenumber;//store pagenumber in array which i is pointing
				}
			}
			//Find same pagenumber in frame array
			else{
				hits+=1;//increase hits count
			}
		}
	}
	
	/*calculate percent of fault in total access*/
	ratiofloat=(float)faults/(float)access * 100.0;
	ratioint=(int)ratiofloat;
	
	/*print access, read, write, hits, fault count and ratio of faults*/
	printf("Total number of access : %d\n",access);
	printf("Total number of read : %d\n",read);
	printf("Total number of write : %d\n",write);
	printf("Number of page hits : %d\n",hits);
	printf("Number of page faults : %d\n",faults);
	printf("Page fault rate : %d/%d = %d%\n",faults,access,ratioint); 
	
	fclose(fp);//close 'access.list'
	
	return 0;//return 0 -> End
}
