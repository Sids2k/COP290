#include "../include/mythread.h"
#include "../include/list.h"
#include "../include/hm.h"

int hash(const char* key)
{
    int h = 0;
    for(int i = 0; key[i] != '\0'; i++)
        h = (h * 31 + key[i]) % SZ;
    return h;
}

int hashmap_create(struct hashmap_s *const out_hashmap)
{
    for(int i = 0; i < SZ; i++)
    {
        out_hashmap->table[i] = list_new();
        // out_hashmap->lk[i] = lock_new();
    }
    return 0;
}

void* hashmap_get(struct hashmap_s *const hashmap, const char* key)
{
    int h = hash(key);
    struct listentry* entry = hashmap->table[h]->head;
    while(entry != NULL)
    {
        struct hashmap_element_s* element = entry->data;
        if(strcmp(element->key, key) == 0)
            return element->data;
        entry = entry->next;
    }
    return NULL;
}

int hashmap_put(struct hashmap_s *const hashmap, const char* key, void* data)
{
    int h = hash(key);
    struct hashmap_element_s* element = malloc(sizeof(struct hashmap_element_s));
    element->key = malloc(sizeof(char) * 100);
    strcpy(element->key, key);
    element->data = data;

    list_add(hashmap->table[h], element);
    return 0;
}

void hashmap_iterator(struct hashmap_s* const hashmap, int (*f)(struct hashmap_element_s *const))  // Execute argument function on each key-value pair in hashmap
{
    for(int i = 0; i < SZ; i++)
    {
        struct listentry* entry = hashmap->table[i]->head;
        while(entry != NULL)
        {
            struct hashmap_element_s* element = entry->data;
            f(element);
            entry = entry->next;
        }
    }
}