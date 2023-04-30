/*
 * Course: LEPL1503 Project 3
 * Project Name: Efficient Multithreaded Code in C
 * Authors: Lebrun Léa, Polet Romain, Van Gorp Thibaut, Leblanc Christophe, Leemans Jade, Monfils Julien
 * Creation Date: 03/2023


 * File description:
   This C code contains performance tests for the functions implemented in "performance_test.h".
   These tests evaluate the execution time of the Bellman-Ford algorithm and the SPFA_SLF algorithm for finding shortest paths in randomly generated directed weighted graphs.

   The "generate_random_graph" function generates a random graph, and the "write_graph_to_file" function writes it to a file.
   The "run_random_test" function generates a random graph and performs the basic Bellman-Ford algorithm and the SPFA_SLF algorithm on it.
   It then calculates the execution time for each algorithm and returns an array of the average execution times.
   The "write_to_file" function writes the test results to a text file if the user chooses to do so.

   The code also includes the "bellman_ford" function, which implements the basic Bellman-Ford algorithm, and the "SPFA_SLF" function, which implements an optimized version of the algorithm.
*/

#include "../include/performance_test.h"

int main(int argc, char *argv[])
{
    // Initialize the random number generator
    srand(time(NULL));

    // Define the tests parameters
    uint s = 0;
    uint n = 50;

    // Ask the user to choose between manual and automatic mode
    printf("Do you want to run the tests in manual or automatic mode? (m/a) ");
    char user_input;
    scanf(" %c", &user_input);

    // Define graphs parameters
    uint nb_nodes = 50;
    uint links_size = 350;
    sint min_cost = -40;
    sint max_cost = 20;

    if (user_input == 'm' || user_input == 'M')
    {
        printf("Do you want to save the random graphs? (y/n) ");
        scanf(" %c", &user_input);

        bool save_graphs = (user_input == 'y' || user_input == 'Y');
        double executionTime = run_random_test(n, nb_nodes, links_size, s, min_cost, max_cost, save_graphs);

        printf("Do you want to add the test results to a text file? (y/n) ");
        scanf(" %c", &user_input);

        if (user_input == 'y' || user_input == 'Y')
        {
            write_to_file(n, nb_nodes, links_size, s, min_cost, max_cost, executionTime);
            printf("The results have been added to the file 'results.txt'.\n");
        }
    }
    else if (user_input == 'a' || user_input == 'A')
    {
        // Define the ranges for the automatic mode
        uint max_nodes = 10000;
        uint max_links_size = 30000;
        sint min_cost = -50;
        sint max_cost = 50;

        // Run the tests in automatic mode
        run_automatic_tests(max_nodes, max_links_size, min_cost, max_cost);
    }
    else
    {
        printf("Invalid input. Please choose either 'm' for manual mode or 'a' for automatic mode.\n");
        return 1;
    }

    return 0;
}

void write_to_file(uint n, uint nb_nodes, uint links_size, uint s, sint min_cost, sint max_cost, double executionTime)
{
    const char *file_name = "test/results.txt";
    bool file_exists = access(file_name, F_OK) != -1;

    FILE *file = fopen(file_name, "a");
    if (file == NULL)
    {
        printf("Error opening the file %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    if (!file_exists)
    {
        fprintf(file, "Number of iterations, Number of nodes, Links size, Source node, Minimum cost, Maximum cost, Execution Tim, Density (links/node)\n");
    }
    
    double ratio = (double)links_size/ (double)nb_nodes;
    fprintf(file, "%" PRIu32 " %" PRIu32 " %" PRIu32 " %" PRIu32 " %" PRId32 " %" PRId32 " %f %f\n", n, nb_nodes, links_size, s, min_cost, max_cost, executionTime, ratio);

    fclose(file);
}

double run_random_test(uint n, uint nb_nodes, uint links_size, uint s, sint min_cost, sint max_cost, bool save_graphs)
{
    lsint *dist = malloc(nb_nodes * sizeof(lsint));
    uint *path = malloc(nb_nodes * sizeof(uint));

    struct timeval tvStart1, tvEnd1;
    double executionTime = 0;

    uint i;

    // Delete the existing file if save_graphs is true
    if (save_graphs)
    {
        const char *graphs_file_name = "test/random_graphs.txt";
        FILE *fp = fopen(graphs_file_name, "w");
        if (fp == NULL)
        {
            perror("Error opening the file.");
        }
        else
        {
            printf("The contents of the existing file %s have been erased.\n", graphs_file_name);
            fclose(fp);
        }
    }

    // Loop for n iterations
    for (i = 0; i < n; i++)
    {
        // Generate a random graph
        link_info *links = (link_info*) malloc(sizeof(link_info) *links_size);
        if (links==NULL){
            return 1;
        }
        generate_random_graph(links, links_size, nb_nodes, min_cost, max_cost);

        // Save the random graph to a file if save_graphs is true
        if (save_graphs && i == 0) // Adding the condition i == 0 to write the header only once
        {
            char file_name[32];
            snprintf(file_name, sizeof(file_name), "test/random_graphs.txt");
            write_graph_to_file(file_name, links, links_size, n, nb_nodes, s, min_cost, max_cost);
        }
        else if (save_graphs) //
        {
            char file_name[32];
            snprintf(file_name, sizeof(file_name), "test/random_graphs.txt");
            write_graph_to_file(file_name, links, links_size, 0, nb_nodes, s, min_cost, max_cost);
        }
        // Apply the algorithm of the optimized version of Bellman-Ford (algorithm_choice) on the random graph
        sint e = gettimeofday(&tvStart1, NULL);
        if (e != 0)
        {
            free(links);
            exit(EXIT_FAILURE);
        }
        bellman_ford(links, links_size, s, dist, path, nb_nodes);
        e = gettimeofday(&tvEnd1, NULL);
        if (e != 0)
        {
            free(links);
            exit(EXIT_FAILURE);
        }
        free(links);
        executionTime += ((tvEnd1.tv_sec - tvStart1.tv_sec) + (tvEnd1.tv_usec - tvStart1.tv_usec) / 1000000.0) / n;
    }

    free(dist);
    free(path);
    
    return executionTime;
}

void run_automatic_tests(uint max_nodes, uint max_links_size, sint min_cost, sint max_cost)
{
    // Declare variables to ajust the number of tests per loop
    int n; 

    // Declare variables to track the number of nodes and links for each test
    uint nb_nodes;
    uint links_size;

    // Declare variables to control the increment of nodes and links for each test
    uint link_increment;
    uint node_increment;

    // Initialize the number of nodes and links to 1
    nb_nodes = 1;
    links_size = 1;

    // Loop over different combinations of nodes and links
    while (nb_nodes <= max_nodes)
    {   
        // This loop will continue as long as links_size is less than or equal to 5 times the number of nodes. 
        // This ensures that tests for densities higher than 5 aren't possible.
        while (links_size <= max_links_size && links_size <= 5*nb_nodes)
        { 
            // Adjust the number of iterations, the links increment and the nodes increment 
            // based on the current values of nb_nodes and links_size
            if (nb_nodes == 1 || links_size == 1) {
                n = 1000;
                link_increment = 9;
                node_increment = 9;
            } else if (nb_nodes < 100 || links_size < 100) {
                n = 1000;
                link_increment = 10;
                node_increment = 10;
            } else if (nb_nodes < 1000 || links_size < 1000) {
                n = 100;
                link_increment = 100;
                node_increment = 100;
            } else if (nb_nodes < 10000 || links_size < 10000) {
                n = 10;
                link_increment = 1000;
                node_increment = 1000;
            } else {
                n = 1;
                link_increment = 10000;
                node_increment = 10000;
            }
            
            // Run the test with the current combination of parameters
            double executionTime = run_random_test(n, nb_nodes, links_size, 0, min_cost, max_cost, false);

            // Write the results to the file
            write_to_file(n, nb_nodes, links_size, 0, min_cost, max_cost, executionTime);

            links_size += link_increment;
        }
        
        nb_nodes += node_increment;

        // Reset the links_size to 1 for the next iteration
        links_size = 1;
    }

}

// Generate a random graph
void write_graph_to_file(const char *file_name, link_info*links, uint links_size, uint n, uint nb_nodes, uint s, sint min_cost, sint max_cost)
{
    FILE *file = fopen(file_name, "a");
    if (file == NULL)
    {
        printf("Error opening the file %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    if (n != 0) // Add the condition to write the header only once
    {
        fprintf(file, "%" PRIu32 " %" PRIu32 " %" PRIu32 " %" PRIu32 " %" PRId32 " %" PRId32 "\n", n, nb_nodes, links_size, s, min_cost, max_cost);
    }

    for (uint i = 0; i < links_size; i++)
    {
        fprintf(file, "%" PRId32 " %" PRId32 " %" PRId32 "\n", links[i].from, links[i].to, links[i].cost);
    }
    fprintf(file, "=====\n");

    fclose(file);
}

// Generate a random graph
void generate_random_graph(link_info*links, uint links_size, uint nb_nodes, sint min_cost, sint max_cost)
{
    for (uint i = 0; i < links_size; i++)
    {
        links[i].from = rand() % nb_nodes;
        links[i].to = rand() % nb_nodes;
        links[i].cost = min_cost + rand() % (max_cost - min_cost + 1);
    }
}

