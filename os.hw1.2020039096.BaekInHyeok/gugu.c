/* os hw1 - gugu.c
 * 2020039096 Baek InHyeok
 * */

#include<stdio.h>//Header File which 'fopen', 'fprintf', 'fclose' functions are included

int main(void){//return integer value when function end
	
	FILE* fp=fopen("out.txt","w");//Open "out.txt" File in Writing Mode
	
	/*These 3 Variable are used for Multiplication*/
	int x=0;//Operand #1
	int y=0;//Operand #2
	int r=0;//Variable where Result of Operand #1 * Operand #2 wil be Saved.
	
	/*Use Nested Loop to make Mutiplication Table <1X1 ~ 9X9>*/
	for(x=1;x<10;x++){//Operand #1 : 1~9
		for(y=1;y<10;y++){//Operand #2 :  1~9
			r=x*y;//Mutiplication
			fprintf(fp,"%d X %d = %d\n",x,y,r);//Use fprintf function to write integer variables in "out.txt" File
		}
	}
	return 0;//program finished
}
