// Header guard to prevent multiple inclusions.
#ifndef bellman_ford_test_h
#define bellman_ford_test_h

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

// /!\ ATTENTION : needed for compatibility with macOS
#include "../include/portable_endian.h"
#include "../include/portable_semaphore.h"

#include "../sp.h"

// Required headers
#include "shortest_path.h"
#include "files.h"

// Custom type aliases
#define uint uint32_t // 32bit unsigned integer
#define sint int32_t  // 32bit signed integer
#define lsint int64_t  // 64bit signed integer

// Function declarations
void classicGraphes1Bell(void);
void classicGraphes2Bell(void);
void classicGraphes3Bell(void);
void testNegCycleBell(void);
void testNoeudsIsolesBell(void);
void testPython1Bell(void);
void hugeGrapheBell(void);
void testNormal1GMax(void);
void testInfGMax(void); 
void testNegGMax(void);
void testAllSameGMax(void);
void testHugePLength(void);
void testHugeGPath(void);
void testWritingOut1(void);
void testWritingOut2(void);
void test_sp1(void);
void test_sp2(void);
void test_sp3(void);
void test_sp4(void);
void test_sp5(void);
void test_sp6(void);
void test_sp7(void);
void test_sp8(void);
void test_sp9(void);
void test_sp10(void);

// Test suite and test cases
int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

//Tested graphs

//Classic graph
#define NB_NODES1 5
#define NB_LINKS1 7

sint LINKS1[NB_LINKS1][3] = {
    {2,0,1},
    {0,1,2},
    {0,3,3},
    {0,4,1},
    {1,2,-1},
    {3,1,-2},
    {3,4,5}
    };

//Classic graph
#define NB_NODES2 4
#define NB_LINKS2 5
#define LEPL1503_USE_HEADER_INSTANCE

sint LINKS2[NB_LINKS2][3] = {
    {0, 1, 3}, 
    {0, 2, -1}, 
    {2, 3, 1}, 
    {3, 0, 1503}, 
    {3, 2, 4},
};

//Graph with negative cost cycle
#define NB_NODES3 5
#define NB_LINKS3 6

sint LINKS3[8][3] = {
    {0, 3, 1},
    {0, 2, 2},
    {0, 4, 30},
    {1, 0, -10},
    {3, 2, 1},
    {2, 1, 1}
};

//Graph with isolated nodes
#define NB_NODES4 25
#define NB_LINKS4 5
#define LEPL1503_USE_HEADER_INSTANCE


sint LINKS4[NB_LINKS4][3] = {
    {0, 1, 3}, 
    {0, 2, -1}, 
    {2, 3, 1}, 
    {3, 0, 1503}, 
    {3, 2, 4},
};


//Graphs produced by Python
#define NB_NODES5 5
#define NB_LINKS5 10

sint LINKS5[NB_LINKS5][3] = {
	{0,2,4},
	{0,4,7},
	{0,1,4},
	{2,0,3},
	{1,0,9},
	{1,4,7},
	{1,3,10},
	{1,2,2},
	{4,0,9},
	{3,2,5},
};


#define NB_NODES6 15
#define NB_LINKS6 100

sint LINKS6[NB_LINKS6][3] = {
	{10, 1, 1},
	{10, 11, 9},
	{10, 13, 6},
	{10, 2, 7},
	{10, 14, 9},
	{10, 7, 9},
	{10, 5, 8},
	{10, 7, 4},
	{10, 6, 6},
	{1, 9, 7},
	{1, 6, 2},
	{1, 14, 7},
	{1, 8, 5},
	{1, 12, 1},
	{1, 4, 7},
	{1, 14, 6},
	{1, 11, 8},
	{1, 3, 4},
	{1, 5, 1},
	{1, 11, 1},
	{11, 4, 4},
	{11, 10, 9},
	{11, 7, 9},
	{11, 14, 2},
	{11, 14, 5},
	{11, 4, 7},
	{4, 14, 9},
	{4, 1, 4},
	{3, 2, 2},
	{3, 8, 10},
	{3, 12, 6},
	{3, 10, 5},
	{3, 10, 6},
	{3, 13, 1},
	{3, 10, 8},
	{3, 11, 9},
	{3, 0, 4},
	{3, 14, 9},
	{3, 1, 1},
	{3, 1, 2},
	{2, 5, 6},
	{2, 7, 7},
	{2, 4, 3},
	{2, 8, 8},
	{2, 7, 1},
	{2, 4, 9},
	{9, 4, 1},
	{9, 4, 1},
	{9, 3, 2},
	{9, 10, 3},
	{9, 1, 7},
	{9, 3, 3},
	{9, 8, 4},
	{9, 3, 1},
	{9, 7, 4},
	{0, 1, 4},
	{0, 8, 4},
	{0, 10, 4},
	{8, 11, 4},
	{8, 4, 10},
	{8, 4, 9},
	{8, 12, 3},
	{8, 14, 2},
	{8, 12, 9},
	{8, 3, 5},
	{8, 2, 10},
	{6, 3, 8},
	{6, 4, 8},
	{6, 14, 8},
	{6, 14, 10},
	{6, 5, 4},
	{6, 9, 8},
	{6, 7, 1},
	{12, 2, 7},
	{12, 4, 2},
	{12, 5, 7},
	{12, 4, 6},
	{12, 2, 3},
	{12, 11, 4},
	{12, 7, 7},
	{5, 4, 3},
	{5, 13, 6},
	{5, 12, 3},
	{5, 7, 1},
	{13, 10, 10},
	{13, 3, 2},
	{13, 12, 1},
	{13, 1, 3},
	{13, 0, 2},
	{13, 10, 5},
	{13, 10, 9},
	{13, 1, 9},
	{13, 9, 7},
	{13, 5, 2},
	{14, 13, 6},
	{14, 9, 6},
	{14, 0, 10},
	{14, 13, 5},
	{14, 9, 2},
	{14, 8, 8}
};

#endif /* bellman_ford_test_h */
