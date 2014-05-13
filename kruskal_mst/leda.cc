//
// George 'papanikge' Papanikolaou
// CEID Advance Algorithm Design Course 2014
// Benchmarking of Minimum Spanning Tree algorithms with Boost & LEDA
//

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#include <LEDA/graph/graph.h>
#include <LEDA/graph/graph_gen.h>
#include <LEDA/graph/min_span.h>
#include <LEDA/system/timer.h>

/*
 * Wrapper around undirected random graph generation with random edge weights.
 * Calculates number-of-nodes to be the theoretical maximum so the graph would
 * be fully connected (aka complete).
 * TODO: eventually we need to be able to generate boost *and* LEDA graphs, so we
 * may need to make this a class of some sort (?)
 */
void connected_random_generator(leda::graph& G, int number_of_nodes, leda::edge_array<int>& cost)
{
    leda::edge e;
    int number_of_edges = 2 * number_of_nodes * log10(number_of_nodes);

    leda::random_simple_undirected_graph(G, number_of_nodes, number_of_edges);
    // assigning random weights
    cost.init(G, 0);
    srand(time(NULL));
    forall_edges(e, G) {
        cost[e] = rand() % 10000 + 1;
    }

    std::cout << "Graph generated. Nodes: " << number_of_nodes << std::endl;
    return;
}

/*
 * Main function to run all the MST versions and benchmark their time
 */
void benchmark_implementations(leda::graph& G, leda::edge_array<int>& cost)
{
    float T;

    T = leda::used_time();
    leda::MIN_SPANNING_TREE(G, cost);
    std::cout << "\t\tLEDA MST calculation time: " << leda::used_time(T) << std::endl;
}

int main(int argc, char **argv)
{
    int i, n;
    leda::graph G;
    leda::edge e; //// TODO: REMOVE THIS
    leda::edge_array<int> cost;
    int N[] = { 10000, 40000, 70000 };

    if (argc > 2 && !strcmp(argv[1], "-n")) {
        n = atoi(argv[2]);
        connected_random_generator(G, n, cost);
        benchmark_implementations(G, cost);
    } else {
        std::cout << "\n>>> Minimum Spanning Tree Benchmarking <<<\n";
        std::cout << "Give -n <number of nodes> if you want custom nodes\n";
        std::cout << "Moving on with the default number of nodes...\n\n";
        for (i = 0; i < 3; i++) {
            std::cout << ">>> Random graphs...";
            connected_random_generator(G, N[i], cost);
            benchmark_implementations(G, cost);

            // clean up
            G.clear();
        }
        N[0] = 100;
        N[1] = 200;
        N[2] = 300;
        for (i = 0; i < 3; i++) {
            std::cout << ">>> Grid graphs...";
            leda::grid_graph(G, N[i]);
            // this is a hack, LEDA cannot produce an undirected grid
            // graph, so we make them bidirectional.
            G.make_bidirected();

            // assigning random weights
            // TODO: these need to get on their own function
            cost.init(G, 0);
            srand(time(NULL));
            forall_edges(e, G) {
                cost[e] = rand() % 10000 + 1;
            }
            benchmark_implementations(G, cost);

            // clean up
            G.clear();
        }
    }

    return 0;
}
