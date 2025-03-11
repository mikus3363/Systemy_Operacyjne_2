#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

class Synchro{
public:
    pthread_mutex_t mutex;
    bool locked;

    Synchro(){
        pthread_mutex_init(&mutex, nullptr);
        locked = false;
    }

    void lock(){
        while(locked){
            usleep(1000);
        }
        locked = true;
        pthread_mutex_lock(&mutex);
    }

    void unlock(){
        locked = false;
        pthread_mutex_unlock(&mutex);
    }
};

Synchro synchro;

void* philosopher(void* arg) {
    int id = *(int*)arg;

    while (true) {
        // Jedzenie
        synchro.lock();
        cout << "Filozof " << id << " je." << endl;
        synchro.unlock();
        usleep(1000000); // pauza na 1 sekundę

        // Sen
        synchro.lock();
        cout << "Filozof " << id << " spi." << endl;
        synchro.unlock();
        usleep(1000000); // pauza na 1 sekundę
    }

    return nullptr;
}

int main() {
    int numPhilosophers = 5; // liczba filozofów

    // utworzenie wątków filozofów
    pthread_t* threads_philosopher = new pthread_t[numPhilosophers]; // tablica wątków
    int* id_philosopher = new int[numPhilosophers]; // tablica id filozofów

    for (int i = 0; i < numPhilosophers; ++i) {
        id_philosopher[i] = i;
        pthread_create(&threads_philosopher[i], nullptr, philosopher, &id_philosopher[i]); // (wstaźnik na wątek, atrybut domyślny, funkcja rozpoczynająca wątek, argument do funkcji)
    }


    // oczekiwanie na zakończenie wątków (nigdy nie nastąpi)
    for (int i = 0; i < numPhilosophers; ++i) {
        pthread_join(threads_philosopher[i], nullptr);
    }

    // usunięcie zaalokowanej pamięci (nigdy nie nastąpi)
    delete[] threads_philosopher;
    delete[] id_philosopher;

    return 0;
}

