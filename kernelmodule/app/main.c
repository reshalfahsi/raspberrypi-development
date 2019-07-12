#include <stdio.h>

#define device "/dev/chardev"

int main() {
	FILE *fp;
	char buffer[256];

	printf("Demo calling kernel module using character device driver\n");
	printf("Reading file %s...\n",device);

	fp = fopen(device,"r");
	if(fp == NULL) {
		printf("Can't open file %s\n",device);
		return 0;
	}
	
	fread(buffer,sizeof(buffer),1,fp);
	printf("1. respond from kernel: %s\n",buffer);
	
	fread(buffer,sizeof(buffer),1,fp);
	printf("2. respond from kernel: %s\n",buffer);

	fclose(fp);
	return 0;
}
