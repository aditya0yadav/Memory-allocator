#include <stdio.h>
#include <pthread.h>
#include <unistd.h>  // For sbrk

typedef char ALIGN[16];

struct head_t {
    size_t size;
    unsigned is_free;
    struct head_t *next;
};

typedef struct head_t head_t;

head_t *head, *tail;

pthread_mutex_t global_malloc_lock;

head_t *get_free_block(size_t size) {
    head_t *curr = head;
    while (curr) {
        if (curr->is_free && curr->size >= size) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

void *malloc(size_t size) {
    size_t total_size;
    void *block;
    head_t *header;

    if (!size) {
        return NULL;
    }

    pthread_mutex_lock(&global_malloc_lock);
    header = get_free_block(size);

    if (header) {
        header->is_free = 0;
        pthread_mutex_unlock(&global_malloc_lock);
        return (void *)(header + 1);
    }

    total_size = sizeof(head_t) + size;
    block = sbrk(total_size);

    if (block == (void *) -1) {
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }

    header = (head_t *)block;
    header->size = size;
    header->is_free = 0;
    header->next = NULL;

    if (!head) {
        head = header;
    }

    if (tail) {
        tail->next = header;
    }

    pthread_mutex_unlock(&global_malloc_lock);
    return (void *)(header + 1);
}

int main() {
    void *ptr1 = malloc(100);
    void *ptr2 = malloc(50);

    printf("Allocated memory at %p and %p\n", ptr1, ptr2);

    return 0;
}
