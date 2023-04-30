

// Header guard to prevent multiple inclusions.
#ifndef graph_h
#define graph_h

// Standard libraries
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

// /!\ ATTENTION : needed for compatibility with macOS
#include "portable_endian.h"
#include "portable_semaphore.h"

// Custom type aliases
#define uint uint32_t // 32bit unsigned integer
#define sint int32_t  // 32bit signed integer
#define lsint int64_t  // 64bit signed integer

// Structures
typedef struct link_info{
    uint from;
    uint to;
    sint cost;
} link_info;

//*****Queue data structure used by the SPFA_SLF algorithm*****//
typedef struct node{
    struct node* next;
    struct node* prev;
    sint value;
} node_t;

typedef struct queue{
    node_t* head;
    node_t* tail;
    sint size;
} queue_t;

//*****Adjacency list data structure representing graphs used by the SPFA_minheap algorithm*****//
typedef struct adjacency_node
{
    uint node_to;
    lsint cost;
    struct adjacency_node *next;
} adjacency_node_t;

//*****Binary min-heap data structure representing graphs used by the SPFA_minheap algorithm*****//
typedef struct minheap {
    int* arr;
    // Current Size of the Heap
    int size;
    // Maximum capacity of the heap
    int capacity;
} minheap;


// Function declarations

//*****Queue data structure used by the SPFA_SLF algorithm*****//
queue_t* create_queue();

int is_empty(queue_t* queue);

void enqueue(queue_t* queue, sint value);

int dequeue(queue_t* queue);

void move_to_front(queue_t* queue, sint value);

void free_queue(queue_t *queue);

//*****Adjacency list data structure representing graphs used by the SPFA_minheap algorithm*****//
adjacency_node_t **create_adjacency_list(uint nb_nodes, link_info *links, uint links_size);

void free_adjacency_list(adjacency_node_t **adj_list, uint nb_nodes);

//*****Binary min-heap data structure representing graphs used by the SPFA_minheap algorithm*****//
int get_min(minheap* heap);

minheap* create_minheap(uint capacity);

minheap* insert_minheap(minheap* heap, sint element);

minheap* heapify(minheap* heap, uint index);

minheap* delete_minimum(minheap* heap);

minheap* delete_element(minheap* heap, uint index);

void free_minheap(minheap* heap);

#endif /* graph_h */
