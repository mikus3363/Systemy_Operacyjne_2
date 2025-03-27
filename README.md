# **Dokumentacja projektu - Problem Filozofów**

## **Opis problemu**

Problem filozofów to klasyczny problem synchronizacji w programowaniu wielowątkowym. N filozofów siedzi przy okrągłym stole, każdy ma dostęp do dwóch sąsiednich widelców, ale jednocześnie może używać tylko dwóch widelców do jedzenia. Problemem jest synchronizacja dostępu do zasobów (widelców), aby uniknąć zakleszczeń (deadlock) oraz zagłodzenia (starvation) filozofów.

## **Instrukcja uruchomienia**

**Windows**

- Zainstaluj kompilator MinGW

- Skopiuj kod źródłowy do pliku filozofowie.cpp.

- Skompiluj kod w terminalu cmd lub PowerShell:

      g++ filozofowie.cpp -o filozofowie.exe -pthread

- Uruchom program:

      filozofowie.exe <liczba filozofów>

**Linux**

- Upewnij się, że masz zainstalowany kompilator g++:

      sudo apt update && sudo apt install g++

- Skopiuj kod źródłowy do pliku filozofowie.cpp.

- Skompiluj kod:

      g++ filozofowie.cpp -o filozofowie -pthread

- Uruchom program:

      ./filozofowie <liczba filozofów>

## **Wątki i ich funkcjonalność**

- Filozofowie (philosopher) - Każdy filozof działa w osobnym wątku. Cykl życia filozofa to:

  - Myślenie

  - Pobieranie dwóch sąsiednich widelców

  - Jedzenie

  - Odkładanie widelców

- Wątek aktualizacji (updateDisplay) - Osobny wątek monitoruje i wyświetla aktualne stany filozofów w konsoli co sekundę.

## **Sekcje krytyczne i ich rozwiązanie**

- Dostęp do widelców – sekcja krytyczna występuje, gdy filozofowie próbują jednocześnie pobrać dwa widelce. Aby uniknąć zakleszczenia wykorzystamy rozwiązanie asymetryczne:

  - Filozofowie o parzystych numerach pobierają najpierw prawy widelec, a potem lewy.

  - Filozofowie o nieparzystych numerach pobierają najpierw lewy widelec, a potem prawy.

- Zastosowanie zmiennej atomowej – każdy widelec jest zabezpieczony zmienną atomową locked, zapewniającą, że operacje blokowania i odblokowywania są niepodzielne, czyli wykonywane bez możliwości przerwania przez inne wątki.

- Unikanie zagłodzenia – w przypadku braku dostępnych widelców, filozofowie wykonują krótkie losowe opóźnienia. Pozwala to na bardziej sprawiedliwy dostęp do zasobów, a przy okazji niweluje zakleszczenia.

## **Podsumowanie**

Implementacja rozwiązuje problem filozofów, zapewniając synchronizację zasobów i minimalizując ryzyko zakleszczeń. Dzięki zastosowaniu zmiennej atomowej oraz odpowiedniemu porządkowi pobierania widelców, program działa stabilnie i efektywnie zarządza współbieżnymi wątkami.


# **Project Documentation - Dining Philosophers Problem**

## **Problem Description**

The dining philosophers problem is a classic synchronization problem in multithreaded programming. N philosophers sit around a circular table, each having access to two adjacent forks but can only use both forks simultaneously to eat. The challenge is synchronizing access to resources (forks) to prevent deadlocks and starvation.

## **Execution Instructions**

**Windows**

- Install MinGW Compiler

- Copy the source code into a file named philosophers.cpp.

- Compile the code in cmd or PowerShell:

      g++ philosophers.cpp -o philosophers.exe -pthread

- Run the program:

      philosophers.exe <number of philosophers>

**Linux**

- Ensure g++ compiler is installed:

      sudo apt update && sudo apt install g++

- Copy the source code into a file named philosophers.cpp.

- Compile the code:

      g++ philosophers.cpp -o philosophers -pthread

- Run the program:

      ./philosophers <number of philosophers>

## **Threads and Their Functionality**

- Philosophers (philosopher) – Each philosopher runs in a separate thread. The philosopher’s life cycle consists of:

  - Thinking

  - Picking up two adjacent forks

  - Eating

  - Putting down the forks

- Update Thread (updateDisplay) – A separate thread monitors and displays the current states of the philosophers in the console every second.

## **Critical Sections and Their Solutions**

- Access to forks – A critical section occurs when philosophers attempt to pick up two forks simultaneously. To avoid deadlocks, an asymmetric solution is applied:

  - Even-numbered philosophers pick up the right fork first, then the left.

  - Odd-numbered philosophers pick up the left fork first, then the right.

- Using atomic variables – Each fork is protected by an atomic variable locked, ensuring that locking and unlocking operations are indivisible, meaning they are executed without interruption from other threads.

- Avoiding starvation – If forks are unavailable, philosophers introduce short random delays. This allows for fairer resource access and helps prevent deadlocks.

## **Summary**

This implementation effectively solves the dining philosophers problem by ensuring resource synchronization and minimizing the risk of deadlocks. Thanks to atomic variables and an appropriate fork-picking order, the program runs stably and efficiently manages concurrent threads.
