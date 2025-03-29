#include "shared.h"

int main() {
    int fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SIZE);

    struct table *share = mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (share == MAP_FAILED) {
        perror("Error: Shared memory mapping failed");
        return EXIT_FAILURE;
    }

    share->in = 0;
    share->out = 0;

    sem_t *full = sem_open(semFullName, O_CREAT, 0666, 0);
    sem_t *empty = sem_open(semEmptyName, O_CREAT, 0666, bufferSize);
    sem_t *mutex = sem_open(semMutexName, O_CREAT, 0666, 1);

    srand((unsigned) time(NULL));
    int loop = 10;

    printf("\n[Producer] Process has started\n");

    while (loop--) {
        sem_wait(empty);
        sleep(rand() % 3);

        sem_wait(mutex);
        sleep(rand() % 3);

        share->buffer[share->in] = rand() % 100;
        printf("Produced item: %d at index: %d\n", share->buffer[share->in], share->in);
        share->in = (share->in + 1) % bufferSize;

        sem_post(mutex);
        sem_post(full);
    }

    printf("[Producer] Process completed\n");

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
