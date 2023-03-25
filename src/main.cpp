#include <iostream>
#include <memory>
#include "param_vc.h"
// #include <random>
// #include <chrono>
#include <stdlib.h>
#include <time.h>

namespace param_vc {

    static int get_random(int min, int max) {
        int r = 0;
        do {
            r = rand() % (max + 1);
        } while( r < min);
        return r;
    }

    void Graph::randomise(int min_v, int max_v) {
        int s = time(0);        
        srand(s);

        V = get_random(min_v,max_v); // get a new V
        
        adj_lists = std::vector<std::list<int>>(V);
        exists = std::vector<bool>(V,true);

        // now generate random edges

        for (int i = 0; i < V; i++) {
            int nedges = get_random(0,V);
            for (int j = 0; j < nedges; j++) {
                add_edge_safe(i,get_random(0,V-1));
            }
        }
    }

    bool Graph::check_vertex_cover(std::list<int> vc) {
        int first = vc.size();
        vc.unique();
        if (first != vc.size() ) {
            std::cout << "Input list was not unique, you probably have an error\n";
            return false;
        }
        int origE = E;
        // delete all the vertices from the cover, if any edges remain we failed
        for(auto i : vc) {
            remove_vertex(i);
        }

        if(E != 0) {
            std::cout << "Bad cover, some edges ( " << E << " out of " << origE << " ) still exist\n";
            return false;
        } else {
            std::cout << "Looks good, " << origE << " edges covered\n";
            return true;
        }
    }

    void Graph::remove_edge(int u, int v) {
        if (u >= V || v >= V) return;
        adj_lists[u].remove(v);
        adj_lists[v].remove(u);
        E--;
    }

    void Graph::remove_vertex(int v) {
        if (v >= V || !exists[v]) return;
        exists[v] = false;
        for (auto i : adj_lists[v]) {
            adj_lists[i].remove(v);
            E--;
        }
    }

    void Graph::add_edge_safe(int u, int v) {
        add_edge(u,v);
        adj_lists[u].sort();
        adj_lists[u].unique();
        add_edge(v,u);
        adj_lists[v].sort();
        adj_lists[v].unique();
    }

    void Graph::add_edge(int u, int v) {
        if (u >= V || v >= V || u == v || !exists[u] || !exists[v]) return;
        adj_lists[u].emplace_back(v);
        adj_lists[v].emplace_back(u);
        E++;
    }

    Graph::Graph(Graph& other) {
        V = other.V;
        E = other.E;
        adj_lists = std::vector<std::list<int>>(V);
        exists = std::vector<bool>(other.exists);
        for (int i = 0; i < V; i++){
            if (exists[i]) adj_lists[i] = std::list<int>(other.adj_lists[i]);
        }
    };

    void Graph::print(){
        std::cout << "V: " << V << ", E " << E << "\n";

        for (int i = 0; i < adj_lists.size(); i++) {
            std::cout << "Node " << i << " adj list: ";
            if (exists[i]) {
                for (auto j : adj_lists[i]){
                    std::cout << " " << j << " ";
                }
            } else {
                std::cout << "[removed, previous size " << adj_lists[i].size() << " ]";
            }
            std::cout << "\n";
        }
    }

    void ParameterizedVertexCover::print(){
        std::cout << "Searching for vertex cover of size " << k << " on graph G:\n";
        g.print();
        if(done) {
            std::cout << "Does a solution exist? " << (has_solution ? "Yes" : "No!") << "\n";
            if(has_solution) {
                print_partial_solution();
            }
        }
    }

    int ParameterizedVertexCover::has_vertex_cover(){
        // TODO
        return g.V > k || g.E > k;
    }

    void ParameterizedVertexCover::print_partial_solution() {
        std::cout << "Proposed Solution has " << partial_solution.size() << " nodes (k is " << k << ", k' is " << kprime << " ): \n";
        for(auto i : partial_solution)
            std::cout << " " << i << " ";
        std::cout << "\n";
    }

    void ParameterizedVertexCover::kernelize() {
        for (int i = 0; i < g.adj_lists.size(); i++) {
            if (kprime < 0) {
                std::cout << "K has fallen below zero, there is no solution for this graph\n";
                done = true;
                has_solution = false; 
                return;
            }
            // delete empty nodes and nodes > k
            if (!g.exists[i]) continue;
            int lsize = g.adj_lists[i].size();
            if (lsize > kprime) {
                std::cout << "Node " << i << " has more than k neighbours. Taking it into the solution\n";
                partial_solution.emplace_back(i);
                kprime--;
                print_partial_solution();
                g.remove_vertex(i);
                g.print();
            }
            if (!lsize) {
                std::cout << "Node " << i << " has no neighbours. Deleting it\n";
                g.remove_vertex(i);
            }
        }
    }

}

#ifndef WITH_GTESTS
int main(int argc, char **argv) {
    param_vc::Graph g = param_vc::Graph();
    g.randomise(0,19);
    param_vc::ParameterizedVertexCover vc = param_vc::ParameterizedVertexCover(g,7);

    vc.print();

    vc.kernelize();
    std::cout << "Post Kernelization\n";

    vc.print();

    return 0;
}
#endif