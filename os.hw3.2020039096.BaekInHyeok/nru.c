/*os hw3 - nru.c
 * 2020039096 BaekInHyeok
 */

#include<stdio.h>//Header File which 'printf'. 'fopen', 'fgets', 'fclose' function is included
#include<stdlib.h>//Header File which 'atoi' function is included
#include<sys/types.h>//Header File which has a lot of variable types
#include<string.h>//Header File which 'strcmp' function is included

/*Use Double-Linked List*/
/*Recently used page node is located in front of list. Node which is not used for the longest time is located in the end of the list*/

//Node Type
typedef struct Node{
	int pagenumber;//variable storing pagenumber
	int rbit;//variable storing rbit(0 or 1)
	int wbit;//variable storing wbit(0 or 1)
	struct Node* next;
	struct Node* prev;
}Node;

//Creating Node operation
Node* makenode(int pagenumber, int r, int w){
	Node *p=(Node *)malloc(sizeof(Node));//create new node 'p'
	
	p->pagenumber=pagenumber;//store pagenumber data into p
	p->rbit=r;//store rbit data into p
	p->wbit=w;//store wbit data into p
	p->next=p->prev=NULL;//init pointer
		
}

//Init head, tail
void init(Node *head, Node *tail){
	/*link head - tail Nodes*/
	head -> next = tail;
	head -> prev = head;
	tail -> prev = head;
	tail -> next = tail;
	
	/*init head data*/
	head->pagenumber=-1;
	head->rbit=-1;
	head->wbit=-1;
	
	/*init tail data*/
	tail->pagenumber=-1;
	tail->rbit=0;
	tail->wbit=0;
	
}

//Operation counting frame size
int count(Node *head, Node *tail){
	int listsize=0;//variable storing size of frame
	Node *p=head;
	
	//count size
	while(p->next !=tail){
		p=p->next;
		listsize++;
	}
	
	//return size
	return listsize;
}

//Operation Searching Node to write
int searchNodetoWrite(Node *head, Node *tail, int r, int w){
	int count=0;//variable storing how many counts to reach the page
	Node *p;
	
	//Finding where page is located
	for(p=tail;p!=head;p=p->prev){
		if(p->rbit==r && p->wbit==w) break;
		count++;
	}
	//return counts to reach the page
	return count;
}

//Operation Searching Node which pagenumber matched
int searchNodetoRead(Node *head, Node *tail, int pn){
	int count=0;
	Node *p;
	for(p=tail;p!=head;p=p->prev){
		if(p->pagenumber==pn) break;
		count++;
	}
	return count;
}

//Operation adding new page into frame
void writefunction(Node *head,int pagenumber,int r, int w){
	Node* newnode=makenode(pagenumber,r,w);//create new node with given data
	
	//link new node : head - nodeA -> head - new node - nodeA
	Node* p=head;
	p->next->prev=newnode;
	newnode->next=p->next;
	p->next=newnode;
	newnode->prev=p;
}

//Operation finding optimal page and writing new pagenumber into it
void hitwrite(Node *head, Node *tail, int pagenumber, int r, int w){
	Node* p;
	//start from tail to head. check rbit and wbit
	for(p=tail;p!=head;p=p->prev){
		//if rbit and wbit is matched
		if(p -> rbit==r && p -> wbit==w){
			//change pagenumber, rbit, wbit
			p->pagenumber=pagenumber;
			p->rbit=0;
			p->wbit=1;
			
			//link prev node and next node of node 'p'
			p->prev->next=p->next;
			p->next->prev=p->prev;
			
			//move node 'p' next to head node
			head->next->prev=p;
			p->next=head->next;
			head->next=p;
			p->prev=head;
			
			break;
		}
	}
	
}

//Operation modifying pagenumber and bits in optimal page
void faultread(Node* head, Node* tail, int pagenumber, int r, int w){
	Node* p;
	//start from tail to head. check rbit and wbit
	for(p=tail;p!=head;p=p->prev){
		//if rbit and wbit is matched
		if(p->rbit==r && p->wbit==w){
			//change pagenumber, rbit, wbit
			p->pagenumber=pagenumber;
			p->rbit=1;
			p->wbit=1;
			
			//link prev node and next node of node 'p'
			p->prev->next=p->next;
			p->next->prev=p->prev;
			
			//move node 'p' next to head node
			head->next->prev=p;
			p->next=head->next;
			head->next=p;
			p->prev=head;
			
			break;
		}
	}
}

//Operation finding page pagenumber matched and change rbit,wbit;
void hitread(Node *head, Node *tail, int pagenumber){
	Node *p;
	//start from tail to head. check rbit and wbit
	for(p=tail->prev;p->prev!=head;p=p->prev){
		//if pagenumber is matched, stop for loop
		if(p->pagenumber==pagenumber) break;
	}
	
	//change rbit, wbit
	p->rbit=1;
	p->wbit=0;
	
	//link prev node and next node of node 'p'
	p->prev->next=p->next;
	p->next->prev=p->prev;
	
	//move node 'p' next to head node	
	head->next->prev=p;
	p->next=head->next;
	head->next=p;
	p->prev=head;
}

//Main
int main(int argc, char *argv[]){
	
	int frames=0;//variable of max size of frame
	frames=atoi(argv[2]);//set max size of frame from command line
	char *cwrite="write";//store 'write' to distinguish 'read' & 'write'
	
	int i=0;//index variable
	int secondchance=0;
	
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
	
	
	//These two Variables are used to store data read from access.list
	int pagenumber=0;//Storing pagenumber
	char *command=NULL;//Storing operation(read & write)
	
	//head Node and tail Node;
	Node *head=(Node *)malloc(sizeof(Node));
	Node *tail=(Node *)malloc(sizeof(Node));
	int size=0;
	
	//variables used when finding page with rbit, wbit
	int is00page=0;
	int is01page=0;
	int is10page=0;
	int is11page=0;
	
	init(head,tail);//init head node and tail node
	
	FILE* fp=fopen("access.list","r");//open access.list in read mode

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
		if(compare<0){
			write+=1;//increase write count
			
			size=count(head,tail);
			
			if(size<frames){//frame is not full -> Load new page
				faults++;
				writefunction(head,pagenumber,0,1);
			}
			
			else{//frame is full -> modify loaded page
				is00page=searchNodetoWrite(head,tail,0,0);//Searching optimal page which rbit=0, wbit=0. Priority 1
				is01page=searchNodetoWrite(head,tail,0,1);//Searching optimal page which rbit=0, wbit=1. Priority 2
				is10page=searchNodetoWrite(head,tail,1,0);//Searching optimal page which rbit=1, wbit=0. Priority 3
				is11page=searchNodetoWrite(head,tail,1,1);//Searching optimal page which rbit=1, wbit=1. Priority 4
				
				hits++;//increase hits count
				
				//find optimal page which rbit=0, wbit=0
				if(is00page<=frames && is00page>0){
					hitwrite(head, tail, pagenumber, 0, 0);//change pagenumber and set rbit, wbit
				}
				//find optimal page which rbit=0, wbit=1
				else if((is00page<=0 || is00page>frames) && (is01page<=frames && is01page>0)){
					hitwrite(head, tail, pagenumber, 0, 1);//change pagenumber and set rbit, wbit
				}
				//find optimal page which rbit=1, wbit=0
				else if((is00page<=0 || is00page>frames) && (is01page<=0 || is01page>frames) && (is10page<=frames && is10page>0)){
					hitwrite(head, tail, pagenumber, 1, 0);//change pagenumber and set rbit, wbit
				}
				//find optimal page which rbit=1, wbit=1
				else if((is00page<=0 || is00page>frames) && (is01page<=0 || is01page>frames) && (is10page<=0 || is10page>frames) && (is11page<=frames && is11page>0)){
					hitwrite(head, tail, pagenumber, 1, 1);//change pagenumber and set rbit, wbit
				}
			}	
		}
		//when comand is 'read'
		else if(compare>0){
		
			read+=1;//increase read count
			
			size=count(head,tail);//size of frame
			
			int isSearched=searchNodetoRead(head, tail, pagenumber);//find page which pagenumber is matched
			
			if(size<isSearched){//failed to find same pagenumber
				faults++;//increase faults count
				
				if(size<frames){//frame is not full -> load new page
					writefunction(head,pagenumber,1,1);
				}
				else{//frame is full -> modify already loaded page
					is00page=searchNodetoWrite(head,tail,0,0);//Searching optimal page which rbit=0, wbit=0. Priority 1
					is01page=searchNodetoWrite(head,tail,0,1);//Searching optimal page which rbit=0, wbit=1. Priority 2
					is10page=searchNodetoWrite(head,tail,1,0);//Searching optimal page which rbit=1, wbit=0. Priority 3
					is11page=searchNodetoWrite(head,tail,1,1);//Searching optimal page which rbit=1, wbit=1. Priority 4
				
					//find optimal page which rbit=0, wbit=0
					if(is00page<=frames && is00page>0){
						faultread(head, tail, pagenumber, 0, 0);//change pagenumber and set rbit, wbit
					}
					//find optimal page which rbit=0, wbit=1
					else if((is00page<=0 || is00page>frames) && (is01page<=frames && is01page>0)){
						faultread(head, tail, pagenumber, 0, 1);//change pagenumber and set rbit, wbit
					}
					//find optimal page which rbit=1, wbit=0
					else if((is00page<=0 || is00page>frames) && (is01page<=0 || is01page>frames) && (is10page<=frames && is10page>0)){
						faultread(head, tail, pagenumber, 1, 0);//change pagenumber and set rbit, wbit
					}
					//find optimal page which rbit=1, wbit=1
					else if((is00page<=0 || is00page>frames) && (is01page<=0 || is01page>frames) && (is10page<=0 || is10page>frames) && (is11page<=frames && is11page>0)){
						faultread(head, tail, pagenumber, 1, 1);//change pagenumber and set rbit, wbit
					}
					
				}
			}
			else{//find same pagenumber
				hits++;//increase hits count
				hitread(head, tail, pagenumber);//update list(relocate node)
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
