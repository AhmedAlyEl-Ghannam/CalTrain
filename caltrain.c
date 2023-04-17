#include "caltrain.h"


/*

	So, let's write a few mental notes here:
	
	count (SeatCount - will not be implemented) -> A variable passed to Station functions to indicate the number of free seats in the train
	
	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	TrainHasArrived -> Condition Variable (Set @ station_init)
	TrainIsFull -> Condition Variable Set WHEN SeatCount reaches ZERO || WaitingPassengers Condition Variable is Changed Set (@ station_load_train to allow it to leave/return)
	WaitingPassengers -> Condition Variable that indicates whether there are passengers waiting in the station or not (Set @ the end of station_wait_for_train (before exiting/return))
	
	==========================================================================================================
	
	Three Condition Variables: TrainHasArrived, TrainIsFull, WaitingPassengers
	Mutex Lock: MutexLock (Very creative, I know!)
	
	-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	Just gonna leave this struct here for reference:
	
	struct station {
	// FILL ME IN (Right Away!)
	
	//  Mutex Lock 
	pthread_mutex_t MutexLock;
	// Condition Variables
	pthread_cond_t TrainHasArrived, TrainIsFull, WaitingPassengers;
	};
	
*/
	
/**
	This function sets the initial values for all condition variables + Mutex Lock + Some variables
*/

void station_init(struct station *station)
{
	// FILL ME IN ('Ight!)
	
	// Initialise mutex
	pthread_mutex_init(&(station->MutexLock), NULL);

	// Initialise condition variables
	pthread_cond_init(&(station->TrainHasArrived), NULL);
	pthread_cond_init(&(station->PassengerOnTrain), NULL);
	
	
	//station->MutexLock = PTHREAD_MUTEX_INITIALIZER;
	//station->TrainHasArrived = PTHREAD_COND_INITIALIZER;
	//station->PassengerOnTrain = PTHREAD_COND_INITIALIZER;
	//station->WaitingPassengers = PTHREAD_COND_INITIALIZER;
	
	// Initize variables 
	station->trainAvailableSeats = 0;
	station->passengersNumberAboard = 0;
	station->passengersNumberWaiting = 0;
	
}

/**
	TRAIN TO PASSENGERS
	
*/

void station_load_train(struct station *station, int count)
{
	
	// FILL ME IN (Roger!)
	
	// MUTEX LOCK: Access the number of empty seats and storing them in trainAvailableSeats
	pthread_mutex_lock(&(station->MutexLock));
	station->trainAvailableSeats = count;
	pthread_mutex_unlock(&(station->MutexLock));	
	
	// Broadcast a signal that the train has arrived in the station
	pthread_cond_broadcast(&station->TrainHasArrived);
	
	// MUTEX LOCK: Wait until no passengers are waiting (Train is Full OR No more passengers are in the station)
	pthread_mutex_lock(&(station->MutexLock));
	
	while (((station->passengersNumberWaiting) > 0) && ((station->passengersNumberAboard) < count)) // count is the OG number of seats
	{
		pthread_cond_wait(&(station->PassengerOnTrain), &(station->MutexLock));
	}
	
	pthread_mutex_unlock(&(station->MutexLock));
	
	station->trainAvailableSeats = 0;
	station->passengersNumberAboard = 0;
	
}

/**
	PASSENGER TO TRAIN
*/

void station_wait_for_train(struct station *station)
{
	// FILL ME IN (On It!)
	
	// MUTEX LOCK: Wait until the train has arrived + keep counting the number of waiting passengers in the station
	pthread_mutex_lock(&(station->MutexLock));
	
	(station->passengersNumberWaiting)++;

	while((station->trainAvailableSeats) == 0)
	{
		pthread_cond_wait(&(station->TrainHasArrived), &(station->MutexLock));		
	}
	
	(station->trainAvailableSeats)--;
	
	pthread_mutex_unlock(&(station->MutexLock));
	
}

/**
	Gives signal to passengers to get in
	
*/

void station_on_board(struct station *station)
{
	// FILL ME IN (Hold my Konafa!)
	
	// MUTEX LOCK: Allow passenger to get on the train + manage the number of empty seats
	pthread_mutex_lock(&(station->MutexLock));
	
	(station->passengersNumberWaiting)--;
	(station->passengersNumberAboard)++;
	
	pthread_mutex_unlock(&(station->MutexLock));
	
	// Signal to indicate a passenger has got on board successfully
	pthread_cond_signal(&station->PassengerOnTrain);
	
}
