#include "shared.h"

int main() {
    // Open shared memory with read and write permissions
    int fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);

    // Set the size of the shared memory object
    ftruncate(fd, SIZE);

    // Map the shared memory object into the address space
    struct table *share = mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

    // Check if the memory mapping failed
    if(share == MAP_FAILED){
        perror("Error: Failed to map shared memory");
        return EXIT_FAILURE;
    }

    // Initialize the buffer indices to 0
    share->in = 0;
    share->out = 0;

    // Open the semaphores with appropriate permissions and initial values
    sem_t *full = sem_open(semFullName, O_CREAT, 0666, 0); // Semaphore for full items (initially 0)
    sem_t *empty = sem_open(semEmptyName, O_CREAT, 0666, bufferSize); // Semaphore for empty slots (initialized to buffer size)
    sem_t *mutex = sem_open(semMutexName, O_CREAT, 0666, 1); // Mutex semaphore to ensure mutual exclusion (initialized to 1)

    // Initialize random number generator
    time_t t;
    srand((unsigned) time(&t));

    // Set the number of iterations (productions)
    int loop = 10;

    // Print a message indicating that the producer process has started
    printf("\n[Producer] Process has started\n");

    // Producer loop to produce items and add them to the shared buffer
    while(loop--) {
        // Wait for an empty slot to be available
        sem_wait(empty);

        // Simulate some processing delay with sleep
        sleep(rand() % 3);

        // Wait for the mutex to ensure no other process is modifying the buffer
        sem_wait(mutex);

        // Simulate some processing delay
        sleep(rand() % 3);

        // Produce a new item (random number between 0 and 99) and store it in the buffer
        share->buffer[share->in] = rand() % 100;

        // Print the produced item and its position in the buffer
        printf("item produced: %d in pos: %d\n", share->buffer[share->in], share->in);

        // Update the position for the next production
        share->in = (share->in + 1) % bufferSize;

        // Release the mutex after modifying the buffer
        sem_post(mutex);

        // Signal that a new item is available
        sem_post(full);
    }

    // Print a message indicating that the producer process has completed
    printf("[Producer] Process completed\n");

    // Close and unlink the semaphores
    sem_close(full);
    sem_close(empty);
    sem_close(mutex);
    sem_unlink(semFullName);
    sem_unlink(semEmptyName);
    sem_unlink(semMutexName);

    // Unmap the shared memory and close the file descriptor
    munmap(share, SIZE);
    close(fd);

    // Unlink the shared memory object
    shm_unlink(NAME);

    return EXIT_SUCCESS;
}
