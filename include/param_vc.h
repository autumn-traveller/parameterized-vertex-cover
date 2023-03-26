#ifndef _PARAM_VC_H
#define _PARAM_VC_H

#include <list>
#include <memory>
#include <vector>

namespace param_vc {

    class Graph {
        public:
            Graph() : V(0), E(0) {};
            ~Graph() = default;

            // Graph(int v);
            Graph(int v) : V(v), E(0), adj_lists(v) , exists(v,true) { adj_lists.shrink_to_fit(); exists.shrink_to_fit(); };

            Graph(Graph& other); 

            // non-constant runtime ( O(V) ) but makes sure v is not already in u's adj. list or vice versa
            void add_edge_safe(int u, int v);

            // blindly inserts v into u's adjacency list- danger of duplicate entries
            void add_edge(int u, int v);

            // void remove_edge(int u, int v);

            void remove_vertex(int v);

            void randomise(int min_v, int max_v);

            bool check_vertex_cover(std::vector<int> vc);

            void print();
            void print_list(int i);

            void undelete_vertex(int i);

            // rebuild the "old" graph, before vertices were removed- edges can't be recovered...
            void recover_deleted_vertices();

            int V;
            int E;
            std::vector<bool> exists;
            std::vector<std::list<int>> adj_lists;

            // represent the graph as an array of V adjacency lists, allow for dynamic deletion by tracking which nodes still remain in the graph via the "exists" array
    };

    class ParameterizedVertexCover {
        public:
            ParameterizedVertexCover(Graph& g, int k) : done(false), g(g) , K(k) , kprime(k), partial_solution(0) {};
            ~ParameterizedVertexCover() = default;
            
            void kernelize();
            
            bool finished_check();
            
            void print();
            void print_partial_solution();

            bool has_vertex_cover();

            bool branch(int start);

            Graph& g;
            int K;
            int kprime;
            std::vector<int> partial_solution;
            bool has_solution;
            bool done;
    };

}

#endif //_PARAM_VC_H