#ifndef CALTRAIN_H
#define CALTRAIN_H

#include <pthread.h>

struct station {
    int waiting_passengers;
    int passengers_on_train;
    int available_seats;
    pthread_mutex_t lock;
    pthread_cond_t train_arrived;
    pthread_cond_t passengers_seated;
};

void station_init(struct station *station);
void station_load_train(struct station *station, int count);
void station_wait_for_train(struct station *station);
void station_on_board(struct station *station);

#endif