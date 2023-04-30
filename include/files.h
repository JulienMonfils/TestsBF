// Header guard to prevent multiple inclusions.
#ifndef files_h
#define files_h

// Standard libraries
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
#include "CUnit/Basic.h"
#include "shortest_path.h"
#include <inttypes.h>
#include "pthread.h"
// /!\ ATTENTION : needed for compatibility with macOS
#include "portable_endian.h"
#include "portable_semaphore.h"

// Custom type aliases
#define uint uint32_t // 32bit unsigned integer
#define sint int32_t  // 32bit signed integer
#define lsint int64_t  // 64bit signed integer

// Structures 
typedef struct node_info{
    /*
    Stores all the informations of a node of the graph
    */
    uint id;
    uint dest;
    int64_t cost;
    uint path_length;
    uint *path;

} node_info;

typedef struct thread_info{
    /*
    Stores all the informations of a thread
    */
    uint id;
    uint nblinks;
    uint offset;
    sint fd;
    uint links_per_thread;
    link_info *links;
} thread_info;

// Function declarations
uint bellman_all(node_info **informations, link_info *links, uint links_size, uint nbnodes, bool verbosity);

uint get_data(char *filename, uint *res);

sint read_file(char *filename, link_info *links, uint nblinks, uint nbthreads);

uint write_out(FILE *out, uint source, uint dest, lsint cost, uint len_path, uint * trajet);

uint write_output(FILE *file, link_info *links, uint links_size, uint nbnodes, bool verbosity);

void * thread_reader(void * ptr);


#endif /* files_h */
