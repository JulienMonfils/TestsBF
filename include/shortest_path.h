// Header guard to prevent multiple inclusions.
#ifndef shortest_path_h
#define shortest_path_h

// Standard libraries
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <pthread.h>

// /!\ ATTENTION : needed for compatibility with macOS
#include "portable_endian.h"
#include "portable_semaphore.h"

// Required headers
#include "graph.h"

// Custom type aliases
#define uint uint32_t // 32bit unsigned integer
#define sint int32_t  // 32bit signed integer
#define lsint int64_t  // 64bit signed integer

// Structures
typedef struct thread_args {
    link_info *links;
    uint links_size;
    uint nb_nodes;
    uint nbthreads;
    int (*algorithm)(link_info *, uint, uint, lsint *, uint *, uint);
    FILE *output;
} thread_args_t;

// Function declaration.
int algorithm_choice(link_info *links, uint links_size, uint nb_nodes, uint nbthreads, FILE * output);

void *bellman_all_threads(void *args);

int setup_and_run_thread(link_info *links, uint links_size, uint nb_nodes, uint nbthreads, int (*algorithm)(link_info *, uint, uint, lsint *, uint *, uint), FILE *output);

int bellman_ford(link_info *links, uint links_size, uint s, lsint *dist, uint *path, uint nb_nodes);

int SPFA_queue(link_info *links, uint links_size, uint s, lsint *dist, uint *path, uint nb_nodes);

int SPFA_minheap(link_info *links, uint links_size, uint s, lsint *dist, uint *path, uint nb_nodes);

uint get_max_thread(lsint *dist, uint dist_size, uint s, uint nbr_threads);

void* get_max(void* arguments);

uint get_max_nothread(lsint *dist, uint dist_size, uint s);

uint get_path_length(uint dest, uint s, uint *path);

uint get_path(uint *res, uint pathlength, uint dest, uint *path);

uint write_out(FILE *out, uint source, uint dest, lsint cost, uint len_path, uint * trajet);


#endif /* shortest_path_h */
