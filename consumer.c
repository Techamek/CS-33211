#include "sharedObj.h"

int main() {
    int fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SIZE);

    struct table *share = mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (share == MAP_FAILED) {
        perror("Error: Failed to map shared memory");
        return EXIT_FAILURE;
    }

    sem_t *full = sem_open(semFullName, O_CREAT, 0666, 0);
    sem_t *empty = sem_open(semEmptyName, O_CREAT, 0666, bufferSize);
    sem_t *mutex = sem_open(semMutexName, O_CREAT, 0666, 1);

    srand((unsigned) time(NULL));
    int loop = 10;

    printf("\n[Consumer] Process has started\n");

    while (loop--) {
        sem_wait(full);
        sleep(rand() % 3);

        sem_wait(mutex);
        sleep(rand() % 3);

        printf("Consumed item: %d at index: %d\n", share->buffer[share->out], share->out);
        share->out = (share->out + 1) % bufferSize;

        sem_post(mutex);
        sem_post(empty);
    }

    printf("[Consumer] Process completed\n");

    sem_close(full);
    sem_close(empty);
    sem_close(mutex);
    sem_unlink(semFullName);
    sem_unlink(semEmptyName);
    sem_unlink(semMutexName);
    munmap(share, SIZE);
    close(fd);
    shm_unlink(NAME);

    return EXIT_SUCCESS;
}

I adjusted the messages for better clarity and con
