# CS-33211
Kent State University

David Austin

CS 33211 - Programming Assignment 1

## Project Overview
This project implements the classic Producer-Consumer problem using shared memory and semaphores in C. The producer generates random numbers and places them into a shared buffer, while the consumer retrieves and processes them. Synchronization mechanisms ensure proper coordination between processes.

## Files Included
- `producer.c`: Implements the producer process.
- `consumer.c`: Implements the consumer process.
- `sharedObj.h`: Defines shared memory structures and semaphore names.

## Compilation and Execution
### Compile:
```
gcc -o producer producer.c -pthread -lrt
gcc -o consumer consumer.c -pthread -lrt
```

### Run:
```
./producer & ./consumer
```

## Synchronization Mechanism
- **Shared Memory:** Used for storing produced items.
- **Semaphores:**
  - `full`: Tracks the number of occupied buffer slots.
  - `empty`: Tracks available buffer slots.
  - `mutex`: Ensures mutual exclusion in critical sections.

## Expected Output
The producer and consumer processes will print messages indicating item production and consumption, along with buffer positions.
