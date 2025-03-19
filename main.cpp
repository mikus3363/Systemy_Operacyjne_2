#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <atomic>
#include <cstdlib>

using namespace std;

class Fork{
public:
    atomic<bool> locked; // zastosowanie zmiennej atomowej aby uniknąć przerwania przez inne wątki, operacje są wykonywane w sposób niepodzielny

    Fork() {
        locked = false; // domyślnie widelec jest odblokowany
    }

    bool tryLock() {
        return !locked.exchange(true); // próba zablokowania widelec; zwraca true, jeśli uda się zablokować (czyli widelec był odblokowany)
    }

    void unlock() {
        locked = false; // odblokowanie widelec
    }
};

Fork* forks;

struct PhilosopherData {
    int id; // id filozofa
    int numPhilosophers; // liczba filozofów
    string state; // stan filozofa myśli/je
};

void* philosopher(void* arg) {
    PhilosopherData* data = (PhilosopherData*)arg; // pobranie danych filozofa
    int id = data->id; // identyfikator filozofa przypisanie
    int numPhilosophers = data->numPhilosophers; // liczba filozofów przypisanie

    while (true) {
        data->state = "mysli     "; // ustawienie stanu filozofa
        usleep(1000000); // pauza na 1 sekundę

        // wzięcie widelców
        if(id % 2 == 0){ // warunek jeżeli filozof ma parzysty numer
            int rightForkId = id; // id widelca po prawej stronie
            while (!forks[rightForkId].tryLock()) { // próba zablokowania widelca po prawej stronie filozofa
                usleep(rand() % 1000 + 500); // jeżeli widelec jest zajęty to krótka pauza zależna od rand dla zabezpieczenia przed deadlockiem
            }
            int leftForkId = (id + 1) % numPhilosophers; // id widelca po lewej stronie
            while (!forks[leftForkId].tryLock()) { // próba zablokowania widelca po lewej stronie filozofa
                usleep(rand() % 1000 + 500); // jeżeli widelec jest zajęty to krótka pauza zależna od rand dla zabezpieczenia przed deadlockiem
            }
        } else{ // dla filozofów o nieparzystym numerze
            int leftForkId = id; // id widelca po lewej stronie
            while (!forks[leftForkId].tryLock()) { // próba zablokowania widelca po lewej stronie filozofa
                usleep(rand() % 1000 + 500); // jeżeli widelec jest zajęty to krótka pauza zależna od rand dla zabezpieczenia przed deadlockiem
            }
            int rightForkId = (id + 1) % numPhilosophers; // id widelca po prawej stronie
            while (!forks[rightForkId].tryLock()) { // próba zablokowania widelca po prawej stronie filozofa
                usleep(rand() % 1000 + 500); // jeżeli widelec jest zajęty to krótka pauza zależna od rand dla zabezpieczenia przed deadlockiem
            }
        }
        data->state = "je        "; // ustawienie stanu filozofa
        usleep(1000000); // pauza na 1 sekundę

        // odłożenie widelców
        if(id % 2 == 0) { // jeżeli filozof ma parzysty numer
            int leftForkId = (id + 1) % numPhilosophers; // id widelca po lewej stronie
            forks[leftForkId].unlock(); // odblokowanie widelca po lewej stronie
            forks[id].unlock(); // odblokowanie widelca po prawej stronie
        } else{ // jeżeli filozof ma nieparzysty numer
            int rightForkId = (id + 1) % numPhilosophers; // id widelca po prawej stronie
            forks[rightForkId].unlock(); // odblokowanie widelca po prawej stronie
            forks[id].unlock(); // odblokowanie widelca po lewej stronie
        }
    }

    return nullptr; // nigdy nie jest zwracane, bo pętla nieskończona
}

void* updateDisplay(void* arg) {
    // wątek do aktualizaji wyświetlania
    PhilosopherData* data = (PhilosopherData*)arg; // dane o filozofach
    int numPhilosophers = data[0].numPhilosophers; // liczba filozofów

    while (true) {
        for (int i = 0; i < numPhilosophers; ++i) {
            if(i % numPhilosophers == 0 && i != 0){
                cout<<endl; // co liczbę filozofów nowa linia
            }
            cout << "Filozof " << i << ": " << data[i].state; // wyświetlenie co robią filozofowie
        }
        cout << endl;
        cout << endl;
        usleep(1000000); // pauza na 1 sekundę
    }

    return nullptr; // nigdy nie jest zwracane, bo pętla nieskończona
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Uzycie: " << argv[0] << " <liczba filozofow>" << endl;
        return 1;
    }

    int numPhilosophers = atoi(argv[1]); // liczba filozofów
    if (numPhilosophers < 2) {
        cout << "Liczba filozofow musi byc wieksza niz 1!" << endl;
        return 1;
    }

    forks = new Fork[numPhilosophers];

    // utworzenie wątków filozofów
    pthread_t* threads_philosopher = new pthread_t[numPhilosophers]; // tablica wątków
    PhilosopherData* data = new PhilosopherData[numPhilosophers]; // tablica danych filozofów

    for (int i = 0; i < numPhilosophers; ++i) {
        data[i].id = i; // ustawienie id filozofa
        data[i].numPhilosophers = numPhilosophers; // ustawienie liczby filozofów
        pthread_create(&threads_philosopher[i], nullptr, philosopher, &data[i]); // (wstaźnik na wątek, atrybut domyślny, funkcja rozpoczynająca wątek, argument do funkcji)
    }

    pthread_t updateThread; // wątek do aktualizacji wyświetlania
    pthread_create(&updateThread, nullptr, updateDisplay, data); // utworzenie aktualnego wątku do wyświetlania. Konstrukcja tak jak wyżej

    // oczekiwanie na zakończenie wątków (nigdy nie nastąpi)
    for (int i = 0; i < numPhilosophers; ++i) {
        pthread_join(threads_philosopher[i], nullptr);
    }

    pthread_join(updateThread, nullptr); // czekaj na zakończenie wątku update

    // usunięcie zaalokowanej pamięci (nigdy nie nastąpi)
    delete[] threads_philosopher;
    delete[] data;
    delete[] forks;

    return 0;
}
