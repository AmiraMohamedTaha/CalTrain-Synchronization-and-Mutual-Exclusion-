#include "caltrain.h"

void station_init(struct station *station) {
    station->waiting_passengers = 0;
    station->passengers_on_train = 0;
    station->available_seats = 0;
    pthread_mutex_init(&station->lock, NULL);
    pthread_cond_init(&station->train_arrived, NULL);
    pthread_cond_init(&station->passengers_seated, NULL);
}

void station_load_train(struct station *station, int count) {
    pthread_mutex_lock(&station->lock);
    station->available_seats = count;

    if (station->waiting_passengers > 0) {
        pthread_cond_broadcast(&station->train_arrived);
    }

    while ((station->passengers_on_train > 0)  
           (station->available_seats > 0 && station->waiting_passengers > 0)) {
        pthread_cond_wait(&station->passengers_seated, &station->lock);
    }

    station->available_seats = 0;
    pthread_mutex_unlock(&station->lock);
}

void station_wait_for_train(struct station *station) {
    pthread_mutex_lock(&station->lock);
    station->waiting_passengers++;

    while (station->available_seats == 0) {
        pthread_cond_wait(&station->train_arrived, &station->lock);
    }

    station->waiting_passengers--;
    station->passengers_on_train++;
    station->available_seats--;
    pthread_mutex_unlock(&station->lock);
}

void station_on_board(struct station *station) {
    pthread_mutex_lock(&station->lock);
    station->passengers_on_train--;

    if (station->passengers_on_train == 0 && 
        (station->available_seats == 0  station->waiting_passengers == 0)) {
        pthread_cond_signal(&station->passengers_seated);
    }

    pthread_mutex_unlock(&station->lock);
}
