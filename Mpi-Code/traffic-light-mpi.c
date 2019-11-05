#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MASTER          0

MPI_Status status;

int numberOfProcesses, rank, dest, source, startPosition, endPosition, laneNumber;

// laneNum[Instance][0] = number of cars
// laneNum[Instance][1] = time
int laneOne[2][2] = {{2,0}, {10,34}};
int laneTwo[2][2] = {{14,1}, {31,46}};
int laneThree[2][2] = {{15,2}, {27,51}};
int laneFour[2][2] = {{18,6}, {17,9}};
int maxCars = 0;


void initialization(int argc, char **argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
}

void masterTask() {
    if(rank == MASTER) {
        
        int numberOfitems = 1;
        
        for (dest=1; dest < numberOfProcesses; dest++) {
            
            /**Sending the arrays**/
            // lane 1
            MPI_Send(&laneOne, numberOfitems, MPI_INT, dest, 10, MPI_COMM_WORLD);
            // lane2
            MPI_Send(&laneTwo, numberOfitems, MPI_INT, dest, 10, MPI_COMM_WORLD);
            // lane 3
            MPI_Send(&laneThree, numberOfitems, MPI_INT, dest, 10, MPI_COMM_WORLD);
            // lane 4
            MPI_Send(&laneFour, numberOfitems, MPI_INT, dest, 10, MPI_COMM_WORLD);
            
            
            // variables that will be returned
            MPI_Send(&maxCars, numberOfitems, MPI_INT, dest, 10, MPI_COMM_WORLD);
            MPI_Send(&laneNumber, numberOfitems, MPI_INT, dest, 10, MPI_COMM_WORLD);
        }
            
        
        
        for (source=1; source<numberOfProcesses; source++) {
            // Recv variables back
            MPI_Recv(&maxCars, numberOfitems, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Recv(&laneNumber, numberOfitems, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            
            // output results
            printf("Max number = %d in lane %d, from process %d\n",maxCars, laneNumber, source);
        }
            
    }
}

void nonMasterTask() {
    if(rank > MASTER) {
        int numberOfitems = 1;

        // lane 1
        MPI_Recv(&laneOne, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        // lane 2
        MPI_Recv(&laneTwo, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        // lane 3
        MPI_Recv(&laneThree, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        // lane 4
        MPI_Recv(&laneFour, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        // max number of cars to send back
        MPI_Recv(&maxCars, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        // lane number with max number of cars
        MPI_Recv(&laneNumber, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        
        // Calculate max cars in the lane (right now only doing instant one based on amount of cars)
        laneOne[0][0] = maxCars;
        laneNumber = 1;
        if(laneTwo[0][0] > maxCars ) {
            maxCars = laneTwo[0][0];
            laneNumber = 2;
        }
        if(laneThree[0][0] > maxCars) {
            maxCars = laneThree[0][0];
            laneNumber = 3;
        }
        if(laneFour[0][0] > maxCars) {
            maxCars = laneFour[0][0];
            laneNumber = 4;
        }
        
        // send back max number of cars in a lane
        MPI_Send(&maxCars, numberOfitems, MPI_INT, MASTER, 10, MPI_COMM_WORLD);
        MPI_Send(&laneNumber, numberOfitems, MPI_INT, MASTER, 10, MPI_COMM_WORLD);
    }
}

int main(int argc, char *argv[]) {

    /** Initialization **/
     initialization(argc, argv);
    
    /**Master Task**/
    masterTask();
    
    /**Non-Master Task**/
    nonMasterTask();
    
    /**Finalize the MPI environment**/
    MPI_Finalize();

    
}

