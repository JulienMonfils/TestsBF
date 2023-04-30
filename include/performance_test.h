// Header guard to prevent multiple inclusions.
#ifndef performance_test_h
#define performance_test_h

// Standard libraries
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "CUnit/Basic.h"
#include <time.h>
#include <limits.h>
#include <math.h>

// /!\ ATTENTION : needed for compatibility with macOS
#include "portable_endian.h"
#include "portable_semaphore.h"

// Required headers
#include "../include/shortest_path.h"

// Custom type aliases
#define uint uint32_t // 32bit unsigned integer
#define sint int32_t  // 32bit signed integer
#define lsint int64_t  // 64bit signed integer

// Function declarations
int shortest_path(link_info *links, uint links_size, uint s, lsint *dist, uint *path, uint nb_nodes);

int bellman_ford(link_info *links, uint links_size, uint s, lsint *dist, uint *path, uint nb_nodes); 

int SPFA_queue(link_info *links, uint links_size, uint s, lsint *dist, uint *path, uint nb_nodes); 

void write_to_file(uint n, uint nb_nodes, uint links_size, uint s, sint min_cost, sint max_cost, double executionTime);

double run_random_test(uint n, uint nb_nodes, uint links_size, uint s, sint min_cost, sint max_cost, bool save_graphs);

void run_automatic_tests(uint max_nodes, uint max_links_size, sint min_cost, sint max_cost);

void write_graph_to_file(const char *file_name, link_info *links, uint links_size, uint n, uint nb_nodes, uint s, sint min_cost, sint max_cost);

void generate_random_graph(link_info *links, uint links_size, uint nb_nodes, sint min_cost, sint max_cost);

#endif /* performance_test_h */
