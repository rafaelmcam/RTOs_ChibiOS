#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

typedef enum{
    
    estado0 = 0, estado1, estado2, estado3

}estados;

typedef enum{

    input0 = 0, input1

}inputs;

int main(){

    estados state = estado0;
    //inputs input = input0;
    
    char input = 0;

    FILE *fp;
    fp = fopen("template2.c", "rt");
    
    while((input=getc(fp))!=EOF){

    switch(state){
    
        case estado0:
            switch (input){
                case '/':{
                    state = estado1;
                    break;
            }
                default:
                    {
                    	state = estado0;
                    	printf("%c", input);
                    	break;
                    }
            }       
            break;

        case estado1:
            switch (input){
                case '*':{
                    state = estado2;
               
                    break;
            }
           
                default:{
                			state = estado0;
                			printf("/%c", input);
                			break;
                }
            }                
            break;

        case estado2:
            switch (input){
                case '*':{
                    state = estado3;               
                    break;
            }
                default:{
                	state = estado2;
                    break;
                }
            
            }
            break;

         case estado3:
         	switch(input){
         			case '/':{
         				state = estado0;
         				break;
         			}
         			default:{
         				state = estado2;
         				break;
         			}
         	}
         	break;
        }
    }

    fclose(fp);
    return 0;
}
