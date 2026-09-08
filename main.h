//
// Created by yuanx on 5/3/2026.
//

#include <stdbool.h>

struct Node {
    struct Node *next;
    int key;
    int value;
} ;

struct HashMap {
    void *buffer;
    size_t size;
    struct Node *unalloc;
    int numNodes;
    int numBuckets;
};

struct Result {
    int value;
    bool foundValue;
};

struct Node **getHeadNode(int key, struct HashMap *hashMap);
struct HashMap makeHashMap(int num);
int addNode(int key, int val, struct HashMap *hashMap);
struct Result getValue(int key, struct HashMap *hashMap);
void printNode(struct Node *node);
void printNodeByKey(int key, struct HashMap *hashMap);
void printHashMap(struct HashMap *hashmap);
void printBucket(struct HashMap *hashmap, int bucketNum);
void freeHashmap(struct HashMap *hashmap);
#ifndef HASHMAP_MAIN_H
#define HASHMAP_MAIN_H

#endif //HASHMAP_MAIN_H
