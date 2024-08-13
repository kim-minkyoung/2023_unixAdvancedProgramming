# Unix Advanced Programming Assignments

This repository contains assignments HW2 through HW5, focusing on multi-threaded and multi-user programming in C. The assignments cover thread management, shared memory, synchronization using semaphores and mutexes, and real-time communication between processes.

## HW2: Process and Thread Management

### Files
- `read.c`: Contains code for reading data from shared memory or files.
- `write.c`: Contains code for writing data to shared memory or files.

### Structure and Workflow
HW2 demonstrates how to create and manage threads and processes in C. It includes:
- **Thread Creation**: How to create and manage multiple threads.
- **Process Creation**: How to create and manage child processes using `fork()`.

### Usage
To compile and run HW2:

```bash
gcc -o hw2 202155532.c read.c write.c -lpthread
./hw2
```

## HW3: Thread Synchronization and Shared Memory

### Files
- `chatHeader.h`: Header file with shared data structure and function prototypes.
- `chatThreadSync.c`: Implements a chat system using threads and shared memory with mutexes for synchronization.

### Structure and Workflow
HW3 builds upon HW2 by adding thread synchronization and shared memory. It includes:

- **Shared Memory**: Using shared memory to allow communication between processes.
- **Mutexes**: Using mutexes to synchronize access to shared resources.
- **Thread Management**: Managing multiple threads to handle real-time chat functionality.

### Usage
To compile and run HW3:

```bash
gcc -o hw3 chatThreadSync.c -lpthread -lcurses
./hw3 <UserID>
```

## HW4: Semaphore Synchronization and Signal Handling

### Files
- `main.c`: Implements a chat system with semaphore synchronization and signal handling.

### Structure and Workflow
HW4 extends HW3 by incorporating semaphore synchronization and signal handling:

- **Shared Memory**: Using semaphores to manage access to shared resources.
- **Mutexes**: Handling termination signals to clean up resources and exit gracefully.
- **Thread Management**: Creating and managing threads with improved synchronization.

### Usage
To compile and run HW4:

```bash
gcc -o hw4 main.c -lpthread -lcurses -lrt
./hw4 <UserID>
```

## HW5: Concurrent Deposit and Withdrawal with Synchronization

### Files
- `202155526-kim-minkyoung.c`: Contains a simulation of concurrent deposit and withdrawal operations using threads, mutexes, and condition variables.

### Structure and Workflow
HW5 focuses on synchronization in concurrent deposit and withdrawal scenarios:

- **Concurrency**: Managing concurrent deposit and withdrawal operations.
- **Mutexes and Condition Variables**: Using mutexes to protect shared resources and condition variables to signal state changes.
- **Random Operations**: Simulating random deposit and withdrawal amounts.

### Usage
To compile and run HW5:

```bash
gcc -o hw5 202155526-kim-minkyoung.c -lpthread
./hw5
```
