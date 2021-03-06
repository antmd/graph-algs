//
// George 'papanikge' Papanikolaou
// CEID Advance Algorithm Desing Course 2014
// Library of Efficient Data types and Algorithms (LEDA) testing
//
// Strong Components identifier and checker implementation
//

#include "basic.h"
#include <LEDA/core/set.h>

using namespace leda;

/*
 * My implementation of Kosaraju's algorithm for finding SCC.
 * Maybe we should leave the graph reversed to save some time.
 */
int my_STRONG_COMPONENTS(graph& G, node_array<int>& compnum)
{
    int i;
    int scc_count = 0;
    int n = G.number_of_nodes();
    node  v;
    node* Stack = new node[n+1];
    list<node> S;
    node_array<int> useless(G, 0);
    node_array<bool> reached(G, false);

    // first DFS over the graph, to get the completion times
    DFS_NUM(G, useless, compnum);

    // create a sorted array of the completion times. The [0] position is vacant.
    forall_nodes(v,G) {
        Stack[compnum[v]] = v;
    }

    // reverse graph
    G.rev_all_edges();
    // running DFS on the reversed graph starting from the top of the Stack
    for (i = n; i > 0; i--) {
        // we are performing DFS on the nodes in the Stack. We are using the
        // reached array to perform it only once for every one.
        if (!reached[Stack[i]]) {
            S = DFS(G, Stack[i], reached);
            // S now is the biggest strong component that contains Stack[i]
            // Reporting and updating arrays and counters
            forall(v, S)
                compnum[v] = scc_count;
            // writing the SCC nums on top of the compnums, which is kinda awkward.
            scc_count++;
        }
    }

    delete[] Stack;
    G.rev_all_edges();
    return scc_count;
}

/*
 * Checker function for the SCC above implementation based on the fact that
 * all the nodes can reach the others when they are in the same SCC
 */
bool STRONG_COMPONENTS_checker(leda::graph& G, leda::node_array<int>& check_nums)
{
    node n, v;
    set<int> S;
    int orig_counter, rev_counter;
    list<node> LN1, LN2;
    bool currently = true;

    forall_nodes(n, G) {
        /* we need to perform the test for one node in each clique */
        if (S.member(check_nums[n]))
            continue;
        else
            S.insert(check_nums[n]);

        orig_counter = 0;
        rev_counter  = 0;

        node_array<int> for_bfs(G, -1);
        /* Performing BFS on the original and on the reverse graph.*/
        LN1 = BFS(G, n, for_bfs);
        forall(v, LN1) {
            if (check_nums[n] == check_nums[v])
                orig_counter++;
        }

        G.rev_all_edges();
        for_bfs.init(G, -1);
        LN2 = BFS(G, n, for_bfs);
        forall(v, LN2) {
            if (check_nums[n] == check_nums[v])
                rev_counter++;
        }

        /* Checking the summing of the found nodes. They need to be the same. */
        if (rev_counter != orig_counter) {
            currently = false;
            break;
        }
    }

    return currently;
}
