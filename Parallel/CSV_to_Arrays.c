#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 100

int main()
{
	char filename[] = "TrafficLog.csv";
	char buffer[SIZE];
	FILE *f;
	char *field;
	int instance,L1Car,L1Time,L2Car,L2Time,L3Car,L3Time,L4Car,L4Time;
	
	/* Initializing arrays and allocating memory */ 
	int n = 1000; //Number of columns to go through. Might make this a user question later.
	int* L1Cars; 
	L1Cars = (int*)malloc(n * sizeof(int));
	int* L1Times; 
	L1Times = (int*)malloc(n * sizeof(int)); 
	int* L2Cars; 
	L2Cars = (int*)malloc(n * sizeof(int));
	int* L2Times; 
	L2Times = (int*)malloc(n * sizeof(int)); 
	int* L3Cars; 
	L3Cars = (int*)malloc(n * sizeof(int));
	int* L3Times; 
	L3Times = (int*)malloc(n * sizeof(int)); 
	int* L4Cars; 
	L4Cars = (int*)malloc(n * sizeof(int));
	int* L4Times; 
	L4Times = (int*)malloc(n * sizeof(int)); 
		
	/* If file not found */
	f = fopen(filename,"r");
	if( f == NULL)
	{
		printf("Unable to open file '%s'\n",filename);
		exit(1);
	}
	
	/* Seperate data */
	while(fgets(buffer,SIZE,f))
	{
		/* get instance */
		field=strtok(buffer,",");
		instance=atoi(field);		
		/* get L1Car */
		field=strtok(NULL,",");
		L1Car=atoi(field);
		L1Cars[instance] = L1Car;
		/* get L1Time */
		field=strtok(NULL,",");
		L1Time=atoi(field); 
		L1Times[instance] = L1Time;
		/* get L2Car */
		field=strtok(NULL,",");
		L2Car=atoi(field);
		L2Cars[instance] = L2Car;
		/* get L2Time */
		field=strtok(NULL,",");
		L2Time=atoi(field);
		L2Times[instance] = L2Time;
		/* get L3Car */
		field=strtok(NULL,",");
		L3Car=atoi(field);
		L3Cars[instance] = L3Car;
		/* get L3Time */
		field=strtok(NULL,",");
		L3Time=atoi(field);
		L3Times[instance] = L3Time;
		/* get L4Car */
		field=strtok(NULL,",");
		L4Car=atoi(field);
		L4Cars[instance] = L4Car;
		/* get L4Time */
		field=strtok(NULL,",");
		L4Time=atoi(field);
		L4Times[instance] = L4Time;
		
		
		/* Reprints Entire Table */
		/*
		printf("instance: %2d, L1Car: %2d, L1Time: %2d, L2Car: %2d, L2Time: %2d, L3Car: %2d, L3Time: %2d, L4Car: %2d, L4Time: %2d \n",
				instance,
				L1Car,
				L1Time,
				L2Car,
				L2Time,
				L3Car,
				L3Time,
				L4Car,
				L4Time);
				*/
	}

	/* close file */
	fclose(f);
		printf("Test at 1000: %d\n",L2Cars[1000]);
     
 }