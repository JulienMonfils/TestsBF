/*
 * Course: LEPL1503 Project 3
 * Project Name: Efficient Multithreaded Code in C
 * Authors: Lebrun Léa, Polet Romain, Van Gorp Thibaut, Leblanc Christophe, Leemans Jade, Monfils Julien
 * Creation Date: 03/2023
 

 * File description:
   This C code contains unit tests for the Bellman-Ford algorithm used to find the shortest paths in weighted directed graphs. 
   The tests cover various scenarios, including classic graphs, graphs with negative cycles, isolated nodes, and randomly generated graphs using Python.
 
   The tests are written using the CUnit library.
   The tested functions include "bellman_ford," which implements the original Bellman-Ford algorithm, as well as "SPFA_queue," an optimized version of the algorithm.
 
   Additionally, the code includes tests for the "get_max_nothread" function and the "get_path_length" function.
*/

#include "../include/bellman_ford_test.h"

int main() {
  // Initialize the test suite
  if (CUE_SUCCESS != CU_initialize_registry()) {
    return CU_get_error();
  }

  // Add the test suite to the registry
  CU_pSuite suite = CU_add_suite("Suite", init_suite, clean_suite);
  if (suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Add the test cases to the suite
  if (
    CU_add_test(suite, "Bellmanford : Graphes classiques, algo original", classicGraphes1Bell) == NULL ||
    CU_add_test(suite, "Bellmanford : Graphes classiques, algo optimisé SPFA_queue", classicGraphes2Bell) == NULL ||
    CU_add_test(suite, "Bellmanford : Graphes classiques, algo optimisé SPFA_minheap", classicGraphes3Bell) == NULL ||
    CU_add_test(suite, "Bellmanford : Graphes avec cycle négatif, les trois algos", testNegCycleBell) == NULL ||
    CU_add_test(suite, "Bellmanford : Graphes avec noeuds isolés, les trois algos", testNoeudsIsolesBell) == NULL ||
    CU_add_test(suite, "Bellmanford : Graphe généré en python, les trois algos", testPython1Bell) == NULL ||
    CU_add_test(suite, "Bellmanford : Grand graphe généré avec python, les trois algos", hugeGrapheBell) == NULL ||
    CU_add_test(suite, "getmax : Tests pour le cas classique", testNormal1GMax) == NULL ||
    CU_add_test(suite, "getmax : Tests pour des coûts infinies", testInfGMax) == NULL ||
    CU_add_test(suite, "getmax : Tests pour des coûts négatifs", testNegGMax) == NULL ||
    CU_add_test(suite, "getmax : Tests pour des coûts égaux", testAllSameGMax) == NULL||
    CU_add_test(suite, "get_path_length : Test sur un grand graphe généré avec python", testHugePLength) == NULL ||
    CU_add_test(suite, "get_path_length : Test sur un grand graphe généré avec python", testHugeGPath) == NULL ||
    CU_add_test(suite, "Testing write_out : Small path", testWritingOut1) == NULL ||
    CU_add_test(suite, "Testing write_out : Bigger path", testWritingOut2) == NULL ||
    CU_add_test(suite, "testing sp 4 threads : small graph", test_sp1) == NULL ||
    CU_add_test(suite, "testing sp 4 threads : large graph", test_sp2) == NULL ||
    CU_add_test(suite, "testing sp 4 threads : dense graph", test_sp3) == NULL ||
    CU_add_test(suite, "testing sp 4 threads : graph with an isolated node", test_sp4) == NULL ||
    CU_add_test(suite, "testing sp 4 threads : disconnected graph", test_sp5) == NULL ||
    CU_add_test(suite, "testing sp 1 threads : small graph", test_sp6) == NULL ||
    CU_add_test(suite, "testing sp 1 threads : large graph", test_sp7) == NULL ||
    CU_add_test(suite, "testing sp 1 threads : dense graph", test_sp8) == NULL ||
    CU_add_test(suite, "testing sp 1 threads : graph with an isolated node", test_sp9) == NULL ||
    CU_add_test(suite, "testing sp 1 threads : disconnected graph", test_sp10) == NULL ) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Run all tests using the basic interface
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}

//Tests des méthode bellmanford
void classicGraphes1Bell(void){
    /*Test sur des petits graphes classiques avec l'algorithme original*/
    lsint * dist = (lsint *) malloc(NB_NODES1 * sizeof(lsint));
    if (dist == NULL){return;}
    uint * path = (uint *) malloc(NB_NODES1 * sizeof(uint));
    if (path == NULL){
        free(dist);
        return;
    }
    link_info * links  = (link_info *)malloc(sizeof(link_info)*NB_LINKS1);
    if (links==NULL){
        free(path); free(dist);
        return;
    }

    uint i; 
    for (i=0; i<NB_LINKS1; i++){
        links[i].from = LINKS1[i][0]; links[i].to = LINKS1[i][1]; links[i].cost = LINKS1[i][2];
    }
    bellman_ford(links, NB_LINKS1, 0, dist, path, NB_NODES1);

    CU_ASSERT_EQUAL(dist[2], 0);
    CU_ASSERT_EQUAL(dist[1], 1);
    CU_ASSERT_EQUAL(dist[3], 3);

    free(dist);
    free(path);
    free(links);
    lsint * dist2 = (lsint *) malloc(NB_NODES2 * sizeof(lsint));
    if (dist2 == NULL){return;}
    uint * path2 = (uint *) malloc(NB_NODES2 * sizeof(uint));
    if (path2 == NULL){
        free(dist2);
        return;
    }
    link_info * links2  = (link_info *)malloc(sizeof(link_info)*NB_LINKS2);
    if (links2==NULL){
        free(path2); free(dist2);
        return;
    }

    for (i=0; i<NB_LINKS2; i++){
        links2[i].from = LINKS2[i][0]; links2[i].to = LINKS2[i][1]; links2[i].cost = LINKS2[i][2];
    }
    bellman_ford(links2, NB_LINKS2, 0, dist2, path2, NB_NODES2);

    CU_ASSERT_EQUAL(dist2[2], -1);
    CU_ASSERT_EQUAL(path2[2], 0);

    free(dist2);
    free(path2);
    free(links2);
}

void classicGraphes2Bell(void){
    /*Test sur des petits graphes classiques avec l'algorithme SPF_queue*/
    lsint * dist = (lsint *) malloc(NB_NODES1 * sizeof(lsint));
    if (dist == NULL){return;}
    uint * path = (uint *) malloc(NB_NODES1 * sizeof(uint));
    if (path == NULL){
        free(dist);
        return;
    }
    link_info * links  = (link_info *)malloc(sizeof(link_info)*NB_LINKS1);
    if (links==NULL){
        free(path); free(dist);
        return;
    }

    uint i;
    for (i=0; i<NB_LINKS1; i++){
        links[i].from = LINKS1[i][0]; links[i].to = LINKS1[i][1]; links[i].cost = LINKS1[i][2];
    }
    SPFA_queue(links, NB_LINKS1, 0, dist, path, NB_NODES1);

    CU_ASSERT_EQUAL(dist[2], 0);
    CU_ASSERT_EQUAL(dist[1], 1);
    CU_ASSERT_EQUAL(dist[3], 3);

    free(dist);
    free(path);
    free(links);
    lsint * dist2 = (lsint *) malloc(NB_NODES2 * sizeof(lsint));
    if (dist2 == NULL){return;}
    uint * path2 = (uint *) malloc(NB_NODES2 * sizeof(uint));
    if (path2 == NULL){
        free(dist2);
        return;
    }
    link_info * links2  = (link_info *)malloc(sizeof(link_info)*NB_LINKS2);
    if (links2==NULL){
        free(path2); free(dist2);
        return;
    }

    for (i=0; i<NB_LINKS2; i++){
        links2[i].from = LINKS2[i][0]; links2[i].to = LINKS2[i][1]; links2[i].cost = LINKS2[i][2];
    }
    SPFA_queue(links2, NB_LINKS2, 0, dist2, path2, NB_NODES2);

    CU_ASSERT_EQUAL(dist2[2], -1);
    CU_ASSERT_EQUAL(path2[2], 0);

    free(dist2);
    free(path2);
    free(links2);
}

void classicGraphes3Bell(void){
    /*Tests de graphes classiques avec l'algo SPF_minheap*/
    lsint * dist = (lsint *) malloc(NB_NODES1 * sizeof(lsint));
    if (dist == NULL){return;}
    uint * path = (uint *) malloc(NB_NODES1 * sizeof(uint));
    if (path == NULL){
        free(dist);
        return;
    }
    link_info * links  = (link_info *)malloc(sizeof(link_info)*NB_LINKS1);
    if (links==NULL){
        free(path); free(dist);
        return;
    }

    uint i;
    for (i=0; i<NB_LINKS1; i++){
        links[i].from = LINKS1[i][0]; links[i].to = LINKS1[i][1]; links[i].cost = LINKS1[i][2];
    }
    SPFA_queue(links, NB_LINKS1, 0, dist, path, NB_NODES1);

    CU_ASSERT_EQUAL(dist[2], 0);
    CU_ASSERT_EQUAL(dist[1], 1);
    CU_ASSERT_EQUAL(dist[3], 3);

    free(dist);
    free(path);
    free(links);
    lsint * dist2 = (lsint *) malloc(NB_NODES2 * sizeof(lsint));
    if (dist2 == NULL){return;}
    uint * path2 = (uint *) malloc(NB_NODES2 * sizeof(uint));
    if (path2 == NULL){
        free(dist2);
        return;
    }
    link_info * links2  = (link_info *)malloc(sizeof(link_info)*NB_LINKS2);
    if (links2==NULL){
        free(path2); free(dist2);
        return;
    }

    for (i=0; i<NB_LINKS2; i++){
        links2[i].from = LINKS2[i][0]; links2[i].to = LINKS2[i][1]; links2[i].cost = LINKS2[i][2];
    }
    SPFA_queue(links2, NB_LINKS2, 0, dist2, path2, NB_NODES2);

    CU_ASSERT_EQUAL(dist2[2], -1);
    CU_ASSERT_EQUAL(path2[2], 0);

    free(dist2);
    free(path2);
    free(links2);
    
}

void testNegCycleBell(void){
    /*Test sur un graphe comportant un cycle de coût négatif*/
    lsint * dist = (lsint *) malloc(NB_NODES3 * sizeof(lsint));
    if (dist == NULL){return;}
    uint * path = (uint *) malloc(NB_NODES3 * sizeof(uint));
    if (path == NULL){
        free(dist);
        return;
    }
    link_info * links  = (link_info *)malloc(sizeof(link_info)*NB_LINKS3);
    if (links==NULL){
        free(path); free(dist);
        return;
    }
    uint i;
    for (i=0; i<NB_LINKS3; i++){
        links[i].from = LINKS3[i][0]; links[i].to = LINKS3[i][1]; links[i].cost = LINKS3[i][2];
    }
    CU_ASSERT_EQUAL(bellman_ford(links, NB_LINKS3, 0, dist, path, NB_NODES3), 1);

    
    free(dist);
    free(path);
    free(links);
    lsint * dist2 = (lsint *) malloc(NB_NODES3 * sizeof(lsint));
    if (dist2 == NULL){return;}
    uint * path2 = (uint *) malloc(NB_NODES3 * sizeof(uint));
    if (path2 == NULL){
        free(dist2);
        return;
    }
    link_info * links2  = (link_info *)malloc(sizeof(link_info)*NB_LINKS3);
    if (links2==NULL){
        free(path2); free(dist2);
        return;
    }

    for (i=0; i<NB_LINKS3; i++){
        links2[i].from = LINKS3[i][0]; links2[i].to = LINKS3[i][1]; links2[i].cost = LINKS3[i][2];
    }
    CU_ASSERT_EQUAL(SPFA_queue(links2, NB_LINKS3, 0, dist2, path2, NB_NODES3), 1);

    free(dist2);
    free(path2);
    free(links2);
    lsint * dist3 = (lsint *) malloc(NB_NODES3 * sizeof(lsint));
    if (dist3 == NULL){return;}
    uint * path3 = (uint *) malloc(NB_NODES3 * sizeof(uint));
    if (path3 == NULL){
        free(dist3);
        return;
    }
    link_info * links3  = (link_info *)malloc(sizeof(link_info)*NB_LINKS3);
    if (links3==NULL){
        free(path3); free(dist3);
        return;
    }
    for (i=0; i<NB_LINKS3; i++){
        links3[i].from = LINKS3[i][0]; links3[i].to = LINKS3[i][1]; links3[i].cost = LINKS3[i][2];
    }
    CU_ASSERT_EQUAL(SPFA_minheap(links3, NB_LINKS3, 0, dist3, path3, NB_NODES3), 1);

    free(dist3);
    free(path3);
    free(links3);
}

void testNoeudsIsolesBell(void){
    /*Test sur un graphe avec des noeuds isolés*/
    lsint * dist = (lsint *) malloc(NB_NODES4 * sizeof(lsint));
    if (dist == NULL){return;}
    uint * path = (uint *) malloc(NB_NODES4 * sizeof(uint));
    if (path == NULL){
        free(dist);
        return;
    }
    link_info * links  = (link_info *)malloc(sizeof(link_info)*NB_LINKS4);
    if (links==NULL){
        free(path); free(dist);
        return;
    }

    uint i;
    for (i=0; i<NB_LINKS4; i++){
        links[i].from = LINKS4[i][0]; links[i].to = LINKS4[i][1]; links[i].cost = LINKS4[i][2];
    }
    CU_ASSERT_EQUAL(bellman_ford(links, NB_LINKS4, 0, dist, path, NB_NODES4), 0);

    CU_ASSERT_EQUAL(dist[2], -1);
    CU_ASSERT_EQUAL(path[2], 0);
  
    free(dist);
    free(path);

    lsint * dist2 = (lsint *) malloc(NB_NODES4 * sizeof(lsint));
    if (dist2 == NULL){
        free(links);
        return;
    }
    uint * path2 = (uint *) malloc(NB_NODES4 * sizeof(uint));
    if (path2 == NULL){
        free(dist2);free(links);
        return;
    }
    
    CU_ASSERT_EQUAL(SPFA_queue(links, NB_LINKS4, 0, dist2, path2, NB_NODES4), 0);

    CU_ASSERT_EQUAL(dist2[2], -1);
    CU_ASSERT_EQUAL(path2[2], 0);
  
    free(dist2);
    free(path2);

    lsint * dist3 = (lsint *) malloc(NB_NODES4 * sizeof(lsint));
    if (dist3 == NULL){
        free(links);
        return;
    }
    uint * path3= (uint *) malloc(NB_NODES4 * sizeof(uint));
    if (path3 == NULL){
        free(dist3);free(links);
        return;
    }

    CU_ASSERT_EQUAL(SPFA_minheap(links, NB_LINKS4, 0, dist3, path3, NB_NODES4), 0);

    CU_ASSERT_EQUAL(dist3[2], -1);
    CU_ASSERT_EQUAL(path3[2], 0);
  
    free(dist3);
    free(path3);
    free(links);
}

void testPython1Bell(void){
    /*Test sur un graphe généré aléatoirement avec python*/
    uint NB_NODES = NB_NODES5;
    uint NB_LINKS = NB_LINKS5;

    uint i;

    lsint * dist = (lsint *) malloc((NB_NODES) * sizeof(lsint));
    if (dist == NULL){return;}
    uint * path = (uint *) malloc((NB_NODES) * sizeof(uint));
    if (path == NULL){
        free(dist);
        return;
    }
    link_info * links  = (link_info *)malloc(sizeof(link_info)*NB_LINKS5);
    if (links==NULL){
        free(path); free(dist);
        return;
    }
    for (i=0; i<NB_LINKS5; i++){
        links[i].from = LINKS5[i][0]; links[i].to = LINKS5[i][1]; links[i].cost = LINKS5[i][2];
    }
    CU_ASSERT_EQUAL(bellman_ford(links, NB_LINKS, 1, dist, path, NB_NODES), 0);

    uint32_t expected[] = {5, 0, 2, 10, 7};

    for (i = 0; i < NB_NODES; i++){
      CU_ASSERT_EQUAL(expected[i], dist[i]);
    }

    free(dist);
    free(path);

    lsint * dist2 = (lsint *) malloc((NB_NODES) * sizeof(lsint));
    if (dist2 == NULL){
        free(links);
        return;
    }
    uint * path2 = (uint *) malloc((NB_NODES) * sizeof(uint));
    if (path2 == NULL){
        free(dist2);free(links);
        return;
    }
    
    CU_ASSERT_EQUAL(SPFA_queue(links, NB_LINKS, 1, dist2, path2, NB_NODES), 0);


    for (i = 0; i < NB_NODES; i++){
      CU_ASSERT_EQUAL(expected[i], dist2[i]);
    }

    free(dist2);
    free(path2);

    lsint * dist3 = (lsint *) malloc((NB_NODES) * sizeof(lsint));
    if (dist3 == NULL){
        free(links);
        return;
    }
    uint * path3 = (uint *) malloc((NB_NODES) * sizeof(uint));
    if (path3 == NULL){
        free(dist3);free(links);
        return;
    }
    
    CU_ASSERT_EQUAL(SPFA_minheap(links, NB_LINKS, 1, dist3, path3, NB_NODES), 0);


    for (i = 0; i < NB_NODES; i++){
      CU_ASSERT_EQUAL(expected[i], dist3[i]);
    }

    free(dist3);
    free(path3);
    free(links);
}

void hugeGrapheBell(void){
    uint NB_NODES = NB_NODES6;
    uint NB_LINKS = NB_LINKS6;

    uint i;

    lsint * dist = (lsint *) malloc((NB_NODES) * sizeof(lsint));
    if (dist == NULL){return;}
    uint * path = (uint *) malloc((NB_NODES) * sizeof(uint));
    if (path == NULL){
        free(dist);
        return;
    }
    link_info * links  = (link_info *)malloc(sizeof(link_info)*NB_LINKS6);
    if (links==NULL){
        free(path); free(dist);
        return;
    }

    for (i=0; i<NB_LINKS6; i++){
        links[i].from = LINKS6[i][0]; links[i].to = LINKS6[i][1]; links[i].cost = LINKS6[i][2];
    }
    CU_ASSERT_EQUAL(bellman_ford(links, NB_LINKS, 13, dist, path, NB_NODES), 0);

    uint expected[] = {2, 3, 4, 2, 3, 2, 5, 3, 6, 7, 5, 4, 1, 0, 6};

    for (i = 0; i < NB_NODES; i++){
      CU_ASSERT_EQUAL(expected[i], dist[i]);
    }

    free(dist);
    free(path);

    lsint * dist2 = (lsint *) malloc((NB_NODES) * sizeof(lsint));
    if (dist2 == NULL){
        free(links);
        return;
    }
    uint * path2 = (uint *) malloc((NB_NODES) * sizeof(uint));
    if (path2 == NULL){
        free(dist2);free(links);
        return;
    }
    
    CU_ASSERT_EQUAL(SPFA_queue(links, NB_LINKS, 13, dist2, path2, NB_NODES), 0);

    for (i = 0; i < NB_NODES; i++){
      CU_ASSERT_EQUAL(expected[i], dist2[i]);
    }

    free(dist2);
    free(path2);

    lsint * dist3 = (lsint *) malloc((NB_NODES) * sizeof(lsint));
    if (dist3 == NULL){
        free(links);    
        return;
    }
    uint * path3 = (uint *) malloc((NB_NODES) * sizeof(uint));
    if (path3 == NULL){
        free(dist3);free(links);
        return;
    }
    
    CU_ASSERT_EQUAL(SPFA_minheap(links, NB_LINKS, 13, dist3, path3, NB_NODES), 0);

    for (i = 0; i < NB_NODES; i++){
      CU_ASSERT_EQUAL(expected[i], dist3[i]);
    }

    free(dist3);
    free(path3);
    free(links);
}


//Test de la méthode getmax
void testNormal1GMax(void){
    //Test pour le cas classique
    lsint dist1[] = {15,2,3,5,0,1};
    uint s1 = 4;
    uint dist_size1 = 6;

    CU_ASSERT_EQUAL(get_max_nothread(dist1, dist_size1, s1), 0);
}

void testInfGMax(void){
    //Test pour le cas où tous les coûts sont infinis
    lsint dist2[] = {INT64_MAX, 0, INT64_MAX, INT64_MAX};
    uint s2 = 1;
    uint dist_size2 = 4;

    CU_ASSERT_EQUAL(get_max_nothread(dist2, dist_size2, s2), 1);

}

void testNegGMax(void){
    //Test avec des coûts négatifs
    lsint dist1[] = {-15,2,-3,5,0,-1};
    uint s1 = 4;
    uint dist_size1 = 6;

    CU_ASSERT_EQUAL(get_max_nothread(dist1, dist_size1, s1), 3);
    
    lsint dist2[] = {-15,-2,-3,-5,0,-1};
    uint s2 = 4;
    uint dist_size2 = 6;

    CU_ASSERT_EQUAL(get_max_nothread(dist2, dist_size2, s2), 5);
    
}


void testAllSameGMax(void){
    //Test avec des coûts égaux
    lsint dist1[] = {15, 0, 15, 15};
    uint s1 = 1;
    uint dist_size1 = 4;

    CU_ASSERT_EQUAL(get_max_nothread(dist1, dist_size1, s1), 0);



    lsint dist2[] = {-5,-5,-5,-5,0,-5};
    uint s2 = 4;
    uint dist_size2 = 6;

    CU_ASSERT_EQUAL(get_max_nothread(dist2, dist_size2, s2), 0);
}



//Test de path_length

void testHugePLength(void){
    /*Test pour un grand graphe généré avec python*/
    uint NB_NODES = NB_NODES6;
    uint NB_LINKS = NB_LINKS6;
    
    uint i;

    lsint * dist = (lsint *) malloc((NB_NODES) * sizeof(lsint));
    if (dist == NULL){return;}
    uint * path = (uint *) malloc((NB_NODES) * sizeof(uint));
    if (path == NULL){
        free(dist);
        return;
    }
    link_info * links  = (link_info *)malloc(sizeof(link_info)*NB_LINKS6);
    if (links==NULL){
        free(path); free(dist);
        return;
    }

    for (i=0; i<NB_LINKS6; i++){
        links[i].from = LINKS6[i][0]; links[i].to = LINKS6[i][1]; links[i].cost = LINKS6[i][2];
    }
    CU_ASSERT_EQUAL(bellman_ford(links, NB_LINKS, 13, dist, path, NB_NODES), 0);

    uint expected[] = {2, 3, 4, 2, 3, 2, 5, 3, 6, 7, 5, 4, 1, 0, 6};

    for (i = 0; i < NB_NODES; i++){
      CU_ASSERT_EQUAL(expected[i], dist[i]);
    }
    uint max = get_max_nothread(dist, NB_NODES, 13);
    CU_ASSERT_EQUAL(max, 9);

    CU_ASSERT_EQUAL(get_path_length(max, 13, path), 2);

    free(dist);
    free(path);
    free(links);
}


//Test de get_path

void testHugeGPath(void){
    /*Test pour un grand graphe généré avec python*/
    uint NB_NODES = NB_NODES6;
    uint NB_LINKS = NB_LINKS6;

    uint i;

    lsint * dist = (lsint *) malloc((NB_NODES) * sizeof(lsint));
    if (dist == NULL){return;}
    uint * path = (uint *) malloc((NB_NODES) * sizeof(uint));
    if (path == NULL){
        free(dist);
        return;
    }
    link_info * links  = (link_info *)malloc(sizeof(link_info)*NB_LINKS6);
    if (links==NULL){
        free(path); free(dist);
        return;
    }
    for (i=0; i<NB_LINKS6; i++){
        links[i].from = LINKS6[i][0]; links[i].to = LINKS6[i][1]; links[i].cost = LINKS6[i][2];
    }
    CU_ASSERT_EQUAL(bellman_ford(links, NB_LINKS, 13, dist, path, NB_NODES), 0);

    uint expected[] = {2, 3, 4, 2, 3, 2, 5, 3, 6, 7, 5, 4, 1, 0, 6};

    for (i = 0; i < NB_NODES; i++){
      CU_ASSERT_EQUAL(expected[i], dist[i]);
    }
    uint max = get_max_nothread(dist, NB_NODES, 13);
    CU_ASSERT_EQUAL(max, 9);

    uint plength = get_path_length(max, 13, path);
    CU_ASSERT_EQUAL(plength, 2);

    uint * dest = (uint * ) malloc(plength * sizeof(uint));
    if (dest == NULL){
        free(dist);
        free(path); free(links);
        return;
    }
    get_path(dest, plength, max, path);

    uint expectedResult[] = {13, 9};
    for (i = 0; i < plength; i++){
        CU_ASSERT_EQUAL(dest[i], expectedResult[i]);
    }
    free(dist);
    free(path);
    free(dest);
    free(links);
}










//Testing if the output file of the "write_out" method is correct

bool read_small_file(FILE * ptr, u_int32_t sRef, u_int32_t destRef, int64_t cRef, u_int32_t lenRef, uint * trajetRef){
    /*
    Return true if the line written by write_out is correct.
    Return false otherwise.
    */
    bool errorDetected = false;

    if (ptr == NULL){
        return true;
    }

    u_int32_t s; u_int32_t dest; int64_t c; u_int32_t len;
    u_int32_t s2; u_int32_t dest2; int64_t c2; u_int32_t len2;
    fread(&s, sizeof(u_int32_t), 1, ptr);   //On lit l'élément, on le récupère et on avance de un élément dans le fichier
    fread(&dest, sizeof(u_int32_t), 1, ptr);
    fread(&c, sizeof(int64_t), 1, ptr);
    fread(&len, sizeof(u_int32_t), 1, ptr);

    
    s2 = be32toh(s);
    dest2 = be32toh(dest);
    c2 = be64toh(c);
    len2 = be32toh(len);

    

    if (s2 != sRef || dest2 != destRef || c2 != cRef || len2 != lenRef){
        errorDetected = true;
        return errorDetected;
    }

    

    for (int j=0; j<len2; j++){
        u_int32_t node;
        fread(&node, sizeof(u_int32_t), 1, ptr);
        node = be32toh(node);
        if (node != trajetRef[j]){
            errorDetected = true;
        }
    }
    return errorDetected;
}

//Writing a small line
void testWritingOut1(void){
    u_int32_t s = 3;
    u_int32_t destRef = 2;
    int64_t cRef = 194;
    u_int32_t lenRef = 10;
    uint trajetRef[10] = {1,5,7,6,2,4,6,1,2,3};

    FILE * ptr = fopen("test/test.bin", "wb+"); //On ouvre le fichier pour écrire dedans
    if (ptr == NULL){CU_ASSERT_EQUAL(false, true);}
    
    write_out(ptr, s, destRef, cRef, lenRef, trajetRef);
    
    fclose(ptr);
    bool result = false;
    
    ptr = fopen("test/test.bin", "rb+");
    if (ptr == NULL){return;}
    result = read_small_file(ptr, s, destRef, cRef, lenRef, trajetRef);
    
    CU_ASSERT_EQUAL(result, false);
    fclose(ptr); 
}

//Writing a bigger line
void testWritingOut2(void){
    u_int32_t s = 30;
    u_int32_t destRef = 48;
    int64_t cRef = 194;
    u_int32_t lenRef = 150;
    uint trajetRef[150] = {1,8,5,7,6,2,4,15,3,8,5,1,7,9,2,1,8,5,7,6,2,4,15,3,8,5,1,7,9,2,1,8,5,7,6,2,4,15,3,8,5,1,7,9,2,1,8,5,7,6,2,4,15,3,8,5,1,7,9,2,1,8,5,7,6,2,4,15,3,8,5,1,7,9,2,1,8,5,7,6,2,4,15,3,8,5,1,7,9,2,1,8,5,7,6,2,4,15,3,8,5,1,7,9,2,1,8,5,7,6,2,4,15,3,8,5,1,7,9,2,1,8,5,7,6,2,4,15,3,8,5,1,7,9,2,1,8,5,7,6,2,4,15,3,8,5,1,7,9,2};
    
    FILE * ptr = fopen("test/test.bin", "wb+"); //On ouvre le fichier pour écrire dedans
    if (ptr == NULL){CU_ASSERT_EQUAL(false, true);}
    write_out(ptr, s, destRef, cRef, lenRef, trajetRef);
    fclose(ptr);
    bool result = false;

    ptr = fopen("test/test.bin", "rb+");
    if (ptr == NULL){return;}
    result = read_small_file(ptr, s, destRef, cRef, lenRef, trajetRef);
    CU_ASSERT_EQUAL(result, false);
    fclose(ptr);
}


bool Compare_files(char* filenamePython, char* filenameC){
    /*Return true if the two files are different
      Return false if the two files are the same
      */
    bool e = false;
    FILE * ptrPy = fopen(filenamePython, "rb+"); //Reference file
    if (ptrPy ==NULL){
        printf("pb with the file :-(\n");
        return true;
    }

    FILE * ptrC = fopen(filenameC, "rb+"); //File created by sp
    if (ptrC == NULL){
        printf("pb with the file :-(\n");
        fclose(ptrPy);
        return true;
    }

    u_int32_t nbnodesPy;
    fread(&nbnodesPy, sizeof(u_int32_t), 1, ptrPy); //On récupère le nombre de noeuds du graphe
    nbnodesPy = be32toh(nbnodesPy);     //On converti dans le bon type

    u_int32_t nbnodesC;
    fread(&nbnodesC, sizeof(u_int32_t), 1, ptrC);
    nbnodesC = be32toh(nbnodesC);
    //printf("Nb nodes : %d, %d\n", nbnodesPy, nbnodesC);

    if (nbnodesPy != nbnodesC){
        fclose(ptrPy);
        fclose(ptrC);
        return true;
    }

    long position = ftell(ptrC); 
    u_int32_t sC; u_int32_t destC; int64_t cC; u_int32_t lenC;
    for (int i=0; i< nbnodesPy; i++){
        u_int32_t sPy; u_int32_t destPy; int64_t cPy; u_int32_t lenPy;
        fread(&sPy, sizeof(u_int32_t), 1, ptrPy);   //On lit l'élément, on le récupère et on avance de un élément dans le fichier
        fread(&destPy, sizeof(u_int32_t), 1, ptrPy);
        fread(&cPy, sizeof(int64_t), 1, ptrPy);
        fread(&lenPy, sizeof(u_int32_t), 1, ptrPy);
        sPy = be32toh(sPy);
        destPy = be32toh(destPy);
        cPy = be64toh(cPy);
        lenPy = be32toh(lenPy);

        fread(&sC, sizeof(u_int32_t), 1, ptrC);   //On lit l'élément, on le récupère et on avance de un élément dans le fichier
        fread(&destC, sizeof(u_int32_t), 1, ptrC);
        fread(&cC, sizeof(int64_t), 1, ptrC);
        fread(&lenC, sizeof(u_int32_t), 1, ptrC);
        sC = be32toh(sC);
        destC = be32toh(destC);
        cC = be64toh(cC);
        lenC = be32toh(lenC);
        

        while (sC != sPy){
            for (int j=0; j<lenC; j++){
            u_int32_t nodeC;
            fread(&nodeC, sizeof(u_int32_t), 1, ptrC);
            nodeC = be32toh(nodeC);
        }
            fread(&sC, sizeof(u_int32_t), 1, ptrC);   //On lit l'élément, on le récupère et on avance de un élément dans le fichier
            fread(&destC, sizeof(u_int32_t), 1, ptrC);
            fread(&cC, sizeof(int64_t), 1, ptrC);
            fread(&lenC, sizeof(u_int32_t), 1, ptrC);
            sC = be32toh(sC);
            destC = be32toh(destC);
            cC = be64toh(cC);
            lenC = be32toh(lenC);
        }

        

        //printf("Source : %d, %d\n", sPy, sC);
        //printf("Dest : %d, %d\n", destPy, destC);
        //printf("Cout : %ld, %ld\n", cPy, cC);
        //printf("Taille de trajet : %d, %d\n", lenPy, lenC);

        if (sPy != sC || destPy != destC || cPy != cC || lenPy != lenC){
            printf("ici \n");
            fclose(ptrPy);
            fclose(ptrC);
            return true;}

        
        //printf("Trajet : [");
        for (int j=0; j<lenPy; j++){
            u_int32_t nodePy;
            u_int32_t nodeC;
            fread(&nodePy, sizeof(u_int32_t), 1, ptrPy);
            fread(&nodeC, sizeof(u_int32_t), 1, ptrC);
            nodePy = be32toh(nodePy);
            nodeC = be32toh(nodeC);
            //printf(" %d, %d", nodePy, nodeC);
            
            if (nodePy != nodeC){
                fclose(ptrPy);
                fclose(ptrC);
                return true;}
        }
        //printf("]\n------------------------------------------\n");
        fseek(ptrC, position, SEEK_SET);
    }
    fclose(ptrPy);
    fclose(ptrC);
    return e;
}

//Testing sp for a small graph with 4 threads
void test_sp1(void){
    bool result = false;
    result = Compare_files("test/ReferenceOutputFiles/petitOutput.bin", "test/OutputFiles/petitTest1.bin");
    CU_ASSERT_EQUAL(result, false);
}

//Testing sp for a large graph with 4 threads
void test_sp2(void){
    bool result = false;
    result = Compare_files("test/ReferenceOutputFiles/outputGrand.bin", "test/OutputFiles/grandTest2.bin");
    CU_ASSERT_EQUAL(result, false);
}

//Testing sp for a dense graph with 4 threads
void test_sp3(void){
    bool result = false;
    result = Compare_files("test/ReferenceOutputFiles/denseOutput.bin", "test/OutputFiles/denseTest3.bin");
    CU_ASSERT_EQUAL(result, false);
}

//Testing sp for a graph with an isolated node with 4 threads
void test_sp4(void){
    bool result = false;
    result = Compare_files("test/ReferenceOutputFiles/IsoleOutput.bin", "test/OutputFiles/IsoleTest4.bin");
    CU_ASSERT_EQUAL(result, false);
}

//Testing sp for a disconnected graph with 4 threads
void test_sp5(void){
    bool result = false;
    result = Compare_files("test/ReferenceOutputFiles/Disconnected.bin", "test/OutputFiles/NonConnexeTest5.bin");
    CU_ASSERT_EQUAL(result, false);
}



//Testing sp for a small graph with 1 threads
void test_sp6(void){
    bool result = false;
    result = Compare_files("test/ReferenceOutputFiles/petitOutput.bin", "test/OutputFiles/petitTest1thread.bin");
    CU_ASSERT_EQUAL(result, false);
}

//Testing sp for a large graph with 1 threads
void test_sp7(void){
    bool result = false;
    result = Compare_files("test/ReferenceOutputFiles/outputGrand.bin", "test/OutputFiles/grandTest1thread.bin");
    CU_ASSERT_EQUAL(result, false);
}

//Testing sp for a dense graph with 1 threads
void test_sp8(void){
    bool result = false;
    result = Compare_files("test/ReferenceOutputFiles/denseOutput.bin", "test/OutputFiles/denseTest1thread.bin");
    CU_ASSERT_EQUAL(result, false);
}

//Testing sp for a graph with an isolated node with 1 threads
void test_sp9(void){
    bool result = false;
    result = Compare_files("test/ReferenceOutputFiles/IsoleOutput.bin", "test/OutputFiles/IsoleTest1thread.bin");
    CU_ASSERT_EQUAL(result, false);
}

//Testing sp for a disconnected graph with 1 threads
void test_sp10(void){
    bool result = false;
    result = Compare_files("test/ReferenceOutputFiles/Disconnected.bin", "test/OutputFiles/NonConnexeTest1thread.bin");
    CU_ASSERT_EQUAL(result, false);
}


