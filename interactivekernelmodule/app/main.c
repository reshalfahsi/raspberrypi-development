#include <stdio.h>
#include<signal.h> 

#define device "/dev/chardev"

FILE *fp;

void handle_sigint(int sig) 
{ 
    if(fp!=NULL){
	fclose(fp);
    }
} 

int main() {
	FILE *fp;
	char buffer[256];

	printf("Demo calling kernel module using character device driver\n");
	printf("Reading file %s...\n",device);

	signal(SIGINT, handle_sigint);
	fp = fopen(device,"r");
	if(fp == NULL) {
		printf("Can't open file %s\n",device);
		return 0;
	}
	else{
		fread(buffer,sizeof(buffer),1,fp);
		printf("%s\n",buffer);
	
	//fread(buffer,sizeof(buffer),1,fp);
	//printf("2. respond from kernel: %s\n",buffer);
		
	}
	
	//fwrite();

	int button1_state = 0;
	int button2_state = 0;

	while(true){
		strcpy(buffer,"READ BUTTON 1");
		button1_state += fwrite(buffer,sizeof(buffer),1,fp);
		strcpy(buffer,"");

		strcpy(buffer,"READ BUTTON 2");
		button2_state += fwrite(buffer,sizeof(buffer),1,fp);
		strcpy(buffer,"");
		
		if(button1_state==0&&button2_state==1){
			button2_state = 0;		
		}

		if((button1_state%2)==1&&(button2_state%2)==0){
			strcpy(buffer,"LED ON");
			fwrite(buffer,sizeof(buffer),1,fp);
			strcpy(buffer,"");

			strcpy(buffer,"DELAY");
			fwrite(buffer,sizeof(buffer),1,fp);
			strcpy(buffer, "");
		}
		else if(((button1_state%2)==(button2_state%2))){
			strcpy(buffer,"LED OFF");
			fwrite(buffer,sizeof(buffer),1,fp);
			strcpy(buffer, "");

			strcpy(buffer,"DELAY");
			fwrite(buffer,sizeof(buffer),1,fp);
			strcpy(buffer, "");

			button2_state = 0;
			button1_state = 0;
		}

		
	}
	//fread(buffer,sizeof(buffer),1,fp);
	//printf("1. respond from kernel: %s\n",buffer);
	
	//fread(buffer,sizeof(buffer),1,fp);
	//printf("2. respond from kernel: %s\n",buffer);

	fclose(fp);
	return 0;
}
