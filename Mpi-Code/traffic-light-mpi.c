#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MASTER          0



int numberOfProcesses, rank, dest, source, laneNumber, chunkSize;

// laneNum[Instance][0] = number of cars
// laneNum[Instance][1] = time
int laneOne[14][2] = {{2,0}, {10,34}, {7,44}, {19,49}, {9,50}, {16,38}, {15,20}, {3,42}, {6,40}, {9,56}, {9,15}, {0,57}, {2,31}, {3,33}};
int laneTwo[14][2] = {{14,1}, {31,46}, {17,38}, {12,56}, {10,11}, {5,46}, {6,58}, {6,56}, {11,0}, {15,41}, {11,20}, {2,55}, {0,14}, {4,17}};
int laneThree[14][2] = {{15,27}, {27,51}, {4,52}, {18,43}, {19,17}, {15,15}, {0,25}, {15,21}, {5,9}, {19,50}, {7,20}, {15,10}, {9,46}, {18,51}};
int laneFour[14][2] = {{18,6}, {17,9}, {2,7}, {14,52}, {2,27}, {17,39}, {7,16}, {11,15}, {12,4}, {2,51}, {7,25}, {19,21}, {8,30}, {10,47}};
int sizeofArray = 14;
int maxCars = 0;

MPI_Status status;

int calcLane(int laneOne[14][2], int laneTwo[14][2],int laneThree[14][2],int laneFour[14][2], int instance, int process) {
    int maxNumCars = 0;
    int laneGo;
    
    if (laneOne[instance][1] >= 60 || laneTwo[instance][1] >= 60 || laneThree[instance][1] >= 60 || laneFour[instance][1] >= 60) {
        if(laneOne[instance][1] >= 60) {
            laneGo = 1;
        } else if(laneTwo[instance][1] >= 60) {
            laneGo = 2;
        }
        else if(laneThree[instance][1] >= 60) {
            laneGo = 3;
        }
        else if(laneFour[instance][1] >= 60) {
            laneGo = 4;
        }
    }
    else {
        maxNumCars = laneOne[instance][0];
        laneGo = 1;
        if(laneTwo[instance][0] > maxNumCars ) {
            maxNumCars = laneTwo[instance][0];
            laneGo = 2;
        }
        if(laneThree[instance][0] > maxNumCars) {
            maxNumCars = laneThree[instance][0];
            laneGo = 3;
        }
        if(laneFour[instance][0] > maxNumCars) {
            maxCars = laneFour[instance][0];
            laneGo = 4;
        }
    }
    printf("In Instance %d\tlane %d gets to go\t(calc by process %d)\n", (instance + 1), laneGo, process);
    return laneGo;
}

void initialization(int argc, char **argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    
    // Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    chunkSize = (sizeofArray / (numberOfProcesses - 1));
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
            
            MPI_Send(&chunkSize, numberOfitems, MPI_INT, dest, 10, MPI_COMM_WORLD);

        }
        
//        for (source=1; source<numberOfProcesses; source++) {
//
////            MPI_Recv(&maxCars, numberOfitems, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
//            MPI_Recv(&laneNumber, numberOfitems, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
//
//            // output results
//            printf("Lane %d gets to go, sent from process %d\n", laneNumber, source);
//        }
    }
}

void nonMasterTasks() {
    
    if(numberOfProcesses == 2) {
        if(rank == 1) {
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
            MPI_Recv(&chunkSize, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            
            int startPosition = 0;
            int endPosition = 14;
            
            int i;
            for (i=startPosition; i<endPosition; i++) {
                laneNumber = calcLane(laneOne, laneTwo, laneThree, laneFour, i, rank);
        //      MPI_Send(&laneNumber, numberOfitems, MPI_INT, MASTER, 10, MPI_COMM_WORLD);

            }
                // send back max number of cars in a lane
        //         MPI_Send(&maxCars, numberOfitems, MPI_INT, MASTER, 10, MPI_COMM_WORLD);
        }
    }
    if(numberOfProcesses == 8) {
        if(rank == 1) {
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
            MPI_Recv(&chunkSize, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            
            int startPosition = 0;
            int endPosition = 1;
            
            int i;
            for (i=startPosition; i<=endPosition; i++) {
                laneNumber = calcLane(laneOne, laneTwo, laneThree, laneFour, i, rank);
                //      MPI_Send(&laneNumber, numberOfitems, MPI_INT, MASTER, 10, MPI_COMM_WORLD);

            }
            // send back max number of cars in a lane
            //         MPI_Send(&maxCars, numberOfitems, MPI_INT, MASTER, 10, MPI_COMM_WORLD);
        }
        if(rank == 2) {
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
            MPI_Recv(&chunkSize, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            
            int startPosition = 2;
            int endPosition = 3;
            
            int i;
            for (i=startPosition; i<=endPosition; i++) {
                laneNumber = calcLane(laneOne, laneTwo, laneThree, laneFour, i, rank);
                //      MPI_Send(&laneNumber, numberOfitems, MPI_INT, MASTER, 10, MPI_COMM_WORLD);

            }
            // send back max number of cars in a lane
            //         MPI_Send(&maxCars, numberOfitems, MPI_INT, MASTER, 10, MPI_COMM_WORLD);
        }
        if(rank == 3) {
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
            MPI_Recv(&chunkSize, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            
            int startPosition = 4;
            int endPosition = 5;
            
            int i;
            for (i=startPosition; i<=endPosition; i++) {
                laneNumber = calcLane(laneOne, laneTwo, laneThree, laneFour, i, rank);
                //      MPI_Send(&laneNumber, numberOfitems, MPI_INT, MASTER, 10, MPI_COMM_WORLD);

            }
            // send back max number of cars in a lane
            //         MPI_Send(&maxCars, numberOfitems, MPI_INT, MASTER, 10, MPI_COMM_WORLD);
        }
        if(rank == 4) {
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
            MPI_Recv(&chunkSize, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            
            int startPosition = 6;
            int endPosition = 7;
            
            int i;
            for (i=startPosition; i<=endPosition; i++) {
                laneNumber = calcLane(laneOne, laneTwo, laneThree, laneFour, i, rank);
                //      MPI_Send(&laneNumber, numberOfitems, MPI_INT, MASTER, 10, MPI_COMM_WORLD);

            }
            // send back max number of cars in a lane
            //         MPI_Send(&maxCars, numberOfitems, MPI_INT, MASTER, 10, MPI_COMM_WORLD);
        }
        if(rank == 5) {
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
            MPI_Recv(&chunkSize, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            
            int startPosition = 8;
            int endPosition = 9;
            
            int i;
            for (i=startPosition; i<=endPosition; i++) {
                laneNumber = calcLane(laneOne, laneTwo, laneThree, laneFour, i, rank);
                //      MPI_Send(&laneNumber, numberOfitems, MPI_INT, MASTER, 10, MPI_COMM_WORLD);

            }
            // send back max number of cars in a lane
            //         MPI_Send(&maxCars, numberOfitems, MPI_INT, MASTER, 10, MPI_COMM_WORLD);
        }
        if(rank == 6) {
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
            MPI_Recv(&chunkSize, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            
            int startPosition = 10;
            int endPosition = 11;
            
            int i;
            for (i=startPosition; i<=endPosition; i++) {
                laneNumber = calcLane(laneOne, laneTwo, laneThree, laneFour, i, rank);
                //      MPI_Send(&laneNumber, numberOfitems, MPI_INT, MASTER, 10, MPI_COMM_WORLD);

            }
            // send back max number of cars in a lane
            //         MPI_Send(&maxCars, numberOfitems, MPI_INT, MASTER, 10, MPI_COMM_WORLD);
        }
        if(rank == 7) {
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
            MPI_Recv(&chunkSize, numberOfitems, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            
            int startPosition = 12;
            int endPosition = 13;
            
            int i;
            for (i=startPosition; i<=endPosition; i++) {
                laneNumber = calcLane(laneOne, laneTwo, laneThree, laneFour, i, rank);
                //      MPI_Send(&laneNumber, numberOfitems, MPI_INT, MASTER, 10, MPI_COMM_WORLD);

            }
            // send back max number of cars in a lane
            //         MPI_Send(&maxCars, numberOfitems, MPI_INT, MASTER, 10, MPI_COMM_WORLD);
        }
    }
}

int main(int argc, char *argv[]) {

    /** Initialization **/
     initialization(argc, argv);
    
    /**Master Task**/
    masterTask();
    
    /**Non-Master Task**/
    nonMasterTasks();
    
    /**Finalize the MPI environment**/
    MPI_Finalize();

    
}

