#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>


/*
	So, let's write a few mental notes here:
	
	count (SeatCount - will not be implemented) -> A variable passed to Station functions to indicate the number of free seats in the train
	
	
	TrainHasArrived -> Condition Variable (Set @ station_init)
	TrainIsFull -> Condition Variable Set WHEN SeatCount reaches ZERO || WaitingPassengers Condition Variable is Changed Set (@ station_load_train to allow it to leave/return)
	WaitingPassengers -> Condition Variable that indicates whether there are passengers waiting in the station or not (Set @ the end of station_wait_for_train (before exiting/return))

	Three Condition Variables: TrainHasArrived, TrainIsFull, WaitingPassengers
	Mutex Lock: MutexLock (Very creative, I know!)
	
*/


struct station {
	// FILL ME IN (Right Away!)
	
	/** Mutex Lock */
	pthread_mutex_t MutexLock;
	//struct lock l;
	/** Condition Variables */
	//struct condition TrainHasArrived, TrainIsFull, WaitingPassengers;
	pthread_cond_t TrainHasArrived, PassengerOnTrain;
	
	/** Variables because I said so */
	unsigned int trainAvailableSeats;
	unsigned int passengersNumberAboard;
	unsigned int passengersNumberWaiting;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);