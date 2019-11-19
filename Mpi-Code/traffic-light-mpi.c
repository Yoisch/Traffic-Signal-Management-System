#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MASTER 0
#define SIZE 100
#define ARRAY_SIZE 1000

FILE *file;

MPI_Status status;

int numberOfProcesses, rank, dest, source, laneNumber, chunkSize, offset;

int results[1000];

int instance,L1Car,L1Time,L2Car,L2Time,L3Car,L3Time,L4Car,L4Time;
int* L1Cars; 
int* L1Times; 
int* L2Cars; 
int* L2Times;
int* L3Cars;
int* L3Times;
int* L4Cars;  
int* L4Times; 

void fill_array() {
    FILE *f;
    char *field;
    char filename[] = "TrafficLog.csv";
    char buffer[SIZE];
    /* Initializing arrays and allocating memory */ 
    int n = 1000; //Number of columns to go through. Might make this a user question later.
    L1Cars = (int*)malloc(n * sizeof(int));
    L1Times = (int*)malloc(n * sizeof(int)); 
    L2Cars = (int*)malloc(n * sizeof(int));
    L2Times = (int*)malloc(n * sizeof(int)); 
    L3Cars = (int*)malloc(n * sizeof(int));
    L3Times = (int*)malloc(n * sizeof(int)); 
    L4Cars = (int*)malloc(n * sizeof(int));
    L4Times = (int*)malloc(n * sizeof(int)); 

    /* If file not found */
    f = fopen(filename,"r");
    if( f == NULL) {
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
        
        
        // /* Reprints Entire Table */
        
        // printf("instance: %2d, L1Car: %2d, L1Time: %2d, L2Car: %2d, L2Time: %2d, L3Car: %2d, L3Time: %2d, L4Car: %2d, L4Time: %2d \n",
        //         instance,
        //         L1Car,
        //         L1Time,
        //         L2Car,
        //         L2Time,
        //         L3Car,
        //         L3Time,
        //         L4Car,
        //         L4Time);
    }

    /* close file */
    fclose(f);
        // printf("Test at 1000: %d\n",L2Cars[1000]);
}

int calcLane(int* L1Cars, int* L1Times, int* L2Cars,int* L2Times, int* L3Cars, int* L3Times, int* L4Cars, int* L4Times, int startNumber, int endNumber, int process) {
    int maxNumCars = 0;
    int laneGo;
    int i;
    for(i = startNumber; i < endNumber; i++) {
        if (L1Times[i] >= 60 || L2Times[i] >= 60 || L3Times[i] >= 60 || L3Times[i] >= 60) {
                if(L1Times[i] >= 60) {
                    laneGo = 1;
                } else if(L2Times[i] >= 60) {
                    laneGo = 2;
                }
                else if(L3Times[i] >= 60) {
                    laneGo = 3;
                }
                else if(L4Times[i] >= 60) {
                    laneGo = 4;
                }
            }
            else {
                maxNumCars = L1Cars[i];
                laneGo = 1;
                if(L2Cars[i] > maxNumCars ) {
                    maxNumCars = L2Cars[i];
                    laneGo = 2;
                }
                if(L3Cars[i] > maxNumCars) {
                    maxNumCars = L3Cars[i];
                    laneGo = 3;
                }
                if(L4Cars[i] > maxNumCars) {
                    maxNumCars = L4Cars[i];
                    laneGo = 4;
                }
            }

            file = fopen("Results.csv", "a");
            fprintf(file,"%d \t %d \n",i,laneGo);
            fclose(file);
            printf("In Instance %d\tlane %d gets to go\t(calc by process %d)\n", i, laneGo, process);
    }

}

void initialization(int argc, char **argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    
    // Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    chunkSize = (ARRAY_SIZE / (numberOfProcesses));
}

void masterTask() {
    if(rank == MASTER) {
        int numberOfitems = 1;
        
        for (dest=1; dest < numberOfProcesses; dest++) {
            
            int startNumber;
            startNumber = rank * offset;
            int endNumber;
            endNumber = startNumber + offset;
            calcLane(L1Cars, L1Times, L2Cars, L2Times, L3Cars, L3Times,L4Cars, L4Times, startNumber, endNumber, rank);  

            /**Sending the arrays to the child processes**/
            // lane 1
            MPI_Send(L1Cars, numberOfitems, MPI_INT, dest, MPI_ANY_TAG, MPI_COMM_WORLD);
            MPI_Send(L1Times, numberOfitems, MPI_INT, dest, MPI_ANY_TAG, MPI_COMM_WORLD);
            // lane2
            MPI_Send(L2Cars, numberOfitems, MPI_INT, dest, MPI_ANY_TAG, MPI_COMM_WORLD);
            MPI_Send(L2Times, numberOfitems, MPI_INT, dest, MPI_ANY_TAG, MPI_COMM_WORLD);
            // lane3   
            MPI_Send(L3Cars, numberOfitems, MPI_INT, dest, MPI_ANY_TAG, MPI_COMM_WORLD);
            MPI_Send(L3Times, numberOfitems, MPI_INT, dest, MPI_ANY_TAG, MPI_COMM_WORLD);            
            // lane4
            MPI_Send(L4Cars, numberOfitems, MPI_INT, dest, MPI_ANY_TAG, MPI_COMM_WORLD);
            MPI_Send(L4Times, numberOfitems, MPI_INT, dest, MPI_ANY_TAG, MPI_COMM_WORLD);
            // variables that will be returned
            offset = chunkSize;
            // MPI_Send(results, 1000, MPI_INT, dest, MPI_ANY_TAG, MPI_COMM_WORLD);
            MPI_Send(&offset, numberOfitems, MPI_INT, dest, MPI_ANY_TAG, MPI_COMM_WORLD);

            
        }
    }
}

void nonMasterTasks() {
    if(rank > MASTER) {
        int numberOfitems = 1;

        MPI_Recv(L1Cars, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(L1Times, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(L2Cars, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(L2Times, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(L3Cars, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(L3Times, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(L4Cars, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(L4Times, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        // MPI_Recv(results, 1000, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&offset, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        int startNumber;
        startNumber = rank * offset;
        int endNumber;
        endNumber = startNumber + offset;
        calcLane(L1Cars, L1Times, L2Cars, L2Times, L3Cars, L3Times,L4Cars, L4Times, startNumber, endNumber, rank);
    }
}

int main(int argc, char *argv[]) {

    char filename[] = "Results.csv";
    file = fopen(filename,"w");
    fprintf(file,"Instance \t Lane Result \n");
    fclose(file);

    fill_array();

    /** Initialization **/
    initialization(argc, argv);

     /**Master Task**/
    masterTask();
    
    /**Non-Master Task**/
    nonMasterTasks();
    
    /**Finalize the MPI environment**/
    MPI_Finalize();

    return 0;
}