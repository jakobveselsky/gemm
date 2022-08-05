#include "buffer.h"

void buffer_init(buffer_t* b, int N)
{
    b->entries = (entry_t**) malloc(N * sizeof(entry_t*));
    for (int i=0; i < N; i++){ b->entries[i] = NULL; }
    b->size = N;
    b->head = b->tail = 0;
    pthread_mutex_init(&b->lock, NULL);
    pthread_cond_init (&b->has_space, NULL);
    pthread_cond_init (&b->has_items, NULL);
}

void buffer_put(buffer_t* b, entry_t* item)
{
    pthread_mutex_lock(&b->lock);

    while (b->tail - b->head >= b->size)
    {
        pthread_cond_wait(&b->has_space, &b->lock);
    }

    b->entries[b->tail++ % b->size] = item;
    pthread_cond_signal(&b->has_items);

    pthread_mutex_unlock(&b->lock);
}

entry_t* buffer_get(buffer_t* b)
{
    pthread_mutex_lock(&b->lock);

    while (b->tail == b->head)
    {
        pthread_cond_wait(&b->has_items, &b->lock);
    }

    entry_t* entry = b->entries[b->head % b->size];
    b->entries[b->head++ % b->size] = NULL;
    pthread_cond_signal(&b->has_space);

    pthread_mutex_unlock(&b->lock);
    return entry;
}

int buffer_count(buffer_t* b)
{
    return b->tail - b->head;
}

void buffer_cleanup(buffer_t* b)
{
    int unfreed_count = 0;
    for (int i=0; i < b->size; i++)
        if (b->entries[i] != NULL)
            unfreed_count++;
    free(b->entries);
    pthread_mutex_destroy(&b->lock);
    pthread_cond_destroy(&b->has_space);
    pthread_cond_destroy(&b->has_items);
}