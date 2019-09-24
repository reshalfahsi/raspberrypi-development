#include <stdio.h>
#include <errno.h>

#define device "/dev/chardev"

int i = 1;

int main() {
	FILE *fp;
	char buffer[256];

	printf("Demo calling kernel module using character device driver\n");
	printf("Reading file %s...\n\n",device);

        for(i; i<=2; i++)
        {
          fp = fopen(device,"r");
	  if(fp == NULL) {
  		printf("Can't open file %s\n",device);
		fprintf(stderr, "fopen() failed: %s\n", strerror(errno));
  		return 0;
	  }

          fread(buffer,sizeof(buffer),1,fp);
          if(i==1)printf("Nama: %s\n",buffer);
          if(i==2)printf("NIM : %s\n",buffer);        
 
          fclose(fp);
	}
	
	return 0;
}
