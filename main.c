#include <stdio.h>
#include "main.h"

#include <stdlib.h>

int main(void) {
    int num = 200000000;
    struct HashMap hashMap = makeHashMap(num);
    for (int i = 0; i < num; i++) {
        addNode(i, (int)(rand() * 10), &hashMap);
    }
    // for (int i = 0; i < hashMap.numBuckets; i++) {
    //     printf("Bucket start \n");
    //     printBucket(&hashMap, i);
    // }
    printf("Buffer size: %zu", hashMap.size);
    freeHashMap(&hashMap);
}

typedef struct Node * Bucket;

struct HashMap makeHashMap(int num) {
    struct HashMap hashMap;
    hashMap.numNodes = num;
    hashMap.numBuckets = num / 3;
    hashMap.size = sizeof(Bucket) * hashMap.numBuckets +
        sizeof(struct Node) * hashMap.numNodes;
    hashMap.buffer = malloc(hashMap.size);
    hashMap.unalloc = hashMap.buffer + sizeof(struct Node *) * hashMap.numBuckets;
    
    // struct Node ** because there are pointers being stored, therefore we need pointers to those pointers
    for (int i = 0; i < hashMap.numBuckets; i++) {
        ((Bucket *) (hashMap.buffer))[i] = NULL;
    }

    return hashMap;
}

int addNode(int key, int val, struct HashMap *hashMap) {
    struct Node node;
    node.key = key;
    node.value = val;
    node.next = NULL;
    // Add the size of the buffer to the start of the buffer and ensure
    // that it is greater than the unallocated slot with an additional space
    if (hashMap->buffer + hashMap->size > (void *)(hashMap->unalloc + 1)) {
        *(hashMap -> unalloc) = node;

        struct Node *currentNode = hashMap->unalloc;
        hashMap->unalloc++;
        // ADDING THE NODE TO A BUCKET
        struct Node **head = getHeadNode(key, hashMap);
            //The head is of type Node ** because it represents a pointer to the start of the buffer, containing pointers to the nodes

        // if there is no head node, make a pointer to the node that is made
        if (*head == NULL) {
            // head is dereferenced to check if the Node *, which head points to, points to a node
            *head = currentNode;
        }
        // if there is a head node, use a while to traverse from node to node
        else {
            struct Node *current = *head;
            if (current == NULL) return 1;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = currentNode;
        }
        return 0;
    }
    return 1;
}

struct Node *getNode(int key, struct HashMap *hashMap) {
    struct Node **head = getHeadNode(key, hashMap);
    if (*head == NULL) {
        return NULL;
    }
    struct Node *current = *head;
    while (current != NULL) {
        if (current->key == key) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

struct Result getValue(int key, struct HashMap *hashMap) {
    struct Result result;
    struct Node *node = getNode(key, hashMap);
    if (node == NULL) {
        result.foundValue = false;
        result.value = 0;
        return result;
    }
    result.foundValue = true;
    result.value = node->value;
    return result;
}

int setValue(int key, int newVal, struct HashMap *hashMap) {
    struct Node *node = getNode(key, hashMap);
    if (node == NULL) {
        return 1;
    }
    node->value = newVal;
    return 0;
}

void printNodeByKey(int key, struct HashMap *hashMap) {
    struct Node *head = getNode(key, hashMap);
    if (head == NULL) {
        printf("Null ");
        return;
    }
    struct Node *current = head;
    while (head->next != NULL) {
        current = head->next;
        if (current->key == key) {
            break;
        }
    }
    printNode(current);
}

void printNode(struct Node *node) {
    printf("Address: %p, ", node);

    printf("Key: %d, ", node->key);
    printf("Value: %d, ", node->value);

    printf("Next: %p \n", node->next);
}

void printHashMap(struct HashMap *hashmap) {
    struct Node **heads = (struct Node **)hashmap->buffer;
    printf("Number of buckets: %d", hashmap->numBuckets);
    for (int i = 0; i < hashmap->numBuckets; i++) {
        printf("Pointer: %p \n", *(heads + i));
    }
}

void printBucket(struct HashMap *hashmap, int bucketNum) {
    struct Node **head = (struct Node **)hashmap->buffer + bucketNum;
    struct Node *current = *head;
    if (current == NULL) return;
    while (current != NULL) {
        printNode(current);
        current = current->next;
    }
}

Bucket *getHeadNode(int key, struct HashMap *hashMap) {
    Bucket * buff = (struct Node **)(hashMap->buffer);
    int offset = key % (hashMap->numBuckets);
    return buff + offset;
}

void freeHashMap(struct HashMap *hashMap) {
    free(hashMap->buffer);
    hashMap->buffer = NULL;
    hashMap->size = 0;
    hashMap->unalloc = NULL;
    hashMap->numNodes = 0;
    hashMap->numBuckets = 0;
}