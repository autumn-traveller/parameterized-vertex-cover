#include <iostream>
#include <memory>
#include "param_vc.h"
// #include <random>
// #include <chrono>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

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

    bool Graph::check_vertex_cover(std::vector<int> vc) {
        // delete all the vertices from the cover, if any edges remain we failed
        for(auto i : vc) {
            remove_vertex(i);
        }
        bool worked = true;
        for (auto list : adj_lists) {
            for (auto j : list) {
                if (exists[j] && !adj_lists[j].empty()) {
                    std::cout << "Uh oh- node " << j << " is still in the graph but it's adjacency list isnt empty:";
                    for (auto k : adj_lists[j]) {
                        std::cout << " " << k << " ";
                    }
                    std::cout << "are still in the list\n";
                    worked = false;
                }
            }
        }
        if(!worked) {
            std::cout << "Bad cover, some edges still exist\n";
        } else {
            std::cout << "Looks good, all edges are covered\n";
        }

        std::cout << "Undeleting vertex cover\n";

        for(auto i : vc) {
            undelete_vertex(i);
        }

        return worked;
    }

    // void Graph::remove_edge(int u, int v) {
    //     if (u >= V || v >= V)
    //         return;
    //     if(exists[u])
    //         adj_lists[u].remove(v);
    //     if(exists[v])
    //         adj_lists[v].remove(u);
    //     E--;
    // }

    void Graph::remove_vertex(int v) {
        if (v >= V || !exists[v])
            return;
        exists[v] = false;
        for (auto i : adj_lists[v]) {
            assert(i != v);
            if(exists[i]) {
                adj_lists[i].remove(v);
                E--;
            }
        }
    }

    void Graph::add_edge_safe(int u, int v) {
        add_edge(u,v);
        adj_lists[u].sort();
        adj_lists[u].unique();
        add_edge(v,u);
        adj_lists[v].sort();
        adj_lists[v].unique();
        E--; // sad attempt to prevent duplicate counting in add_edge() (it can still occur)
    }

    void Graph::add_edge(int u, int v) {
        if (u >= V || v >= V || u == v || !exists[u] || !exists[v])
            return;
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
            if (exists[i])
                adj_lists[i] = std::list<int>(other.adj_lists[i]);
        }
    }

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

    void Graph::undelete_vertex(int i) {
        for(auto j : adj_lists[i]) {
            adj_lists[j].emplace_back(i);
            adj_lists[j].sort();
            adj_lists[j].unique();
        }
    }

    void Graph::recover_deleted_vertices() {
        for (int i = 0; i < V; i++) {
            if (!exists[i]) {
                std::cout << "Recovering node " << i << ":";
                print_list(i);
                std::cout << "\n";
            }
            exists[i] = true;
            undelete_vertex(i);
        }
    }

    void Graph::print_list(int i) {
        for (auto j : adj_lists[i]) {
            std::cout << " " << j << " ";
        }
    }

    void ParameterizedVertexCover::print(){
        std::cout << "Searching for vertex cover of size " << K << " on graph G:\n";
        g.print();
        if(done) {
            std::cout << "Does a solution exist? " << (has_solution ? "Yes" : "No!") << "\n";
            if(has_solution) {
                print_partial_solution();
            }
        }
    }

    bool ParameterizedVertexCover::has_vertex_cover(){
        kernelize();
        if (!done) // kernelization can sometimes just solve the probolem (i.e. star graph)
            branch(0);
        done = true; // branch() always resets this value to false
        return has_solution;
    }

    void ParameterizedVertexCover::print_partial_solution() {
        std::cout << "Proposed Solution has " << partial_solution.size() << " nodes (k is " << K << ", k' is " << kprime << " ): \n";
        for(auto i : partial_solution)
            std::cout << " " << i << " ";
        std::cout << "\n";
    }

    void ParameterizedVertexCover::kernelize() {
        assert(kprime >= 0);
        for (int i = 0; i < g.V; i++) {
            // delete empty nodes and nodes > k
            if (!g.exists[i])
                continue;
            int lsize = g.adj_lists[i].size();
            if (lsize > kprime) {
                std::cout << "Node " << i << " has more than k neighbours. Taking it into the solution\n";
                partial_solution.emplace_back(i);
                print_partial_solution();
                g.remove_vertex(i);
                // g.print();
                kprime--;
            } else if (!lsize) {
                std::cout << "Node " << i << " has no neighbours. Deleting it\n";
                g.remove_vertex(i);
            }
            if (kprime < 0) {
                std::cout << "k has fallen below zero, there is no solution for this graph\n";
                done = true;
                has_solution = false; 
                return;
            }
        }
        finished_check();
        if (!has_solution && K > 0) {
            done = false;
        }
    }
    
    bool ParameterizedVertexCover::finished_check(){
        done = true;
        return (has_solution = g.check_vertex_cover(partial_solution) && partial_solution.size() <= K);
    }

    bool ParameterizedVertexCover::branch(int start){
        if (kprime <= 0) {
            finished_check();
            done = false;
            return has_solution;
        }

        bool worked = false;
        for(int i = start; i < g.V; i++){
            if (!g.exists[i])
                continue;
            
            int nbhood_size = g.adj_lists[i].size();

            if (kprime <= 0 && nbhood_size > 0) {
                std::cout << "(Sub)Graph does not accept a vertex cover (k is <= 0) but node " << i << " still has neighbours:";
                g.print_list(i);
                return false;
            }

            // first we consider taking all of the neighbours of i into our VC (trim the search tree faster)
            if (!nbhood_size) {
                g.remove_vertex(i); // remove nodes with no neighbours
                start++;
                continue;
            }
            if (nbhood_size == 1) {
                partial_solution.push_back(g.adj_lists[i].front()); // if a node only has one neighbour, always take the neighbour into the cover, now remove both nodes and continue
                g.remove_vertex(g.adj_lists[i].front());
                g.remove_vertex(i);
                kprime--;
                start++;
                continue;
            }
            if (kprime >= nbhood_size) {
                for(auto j : g.adj_lists[i]) {
                    partial_solution.emplace_back(j);
                    g.remove_vertex(j);
                }
                g.remove_vertex(i); // all its neighbours are in the VC, we can delete i too
                kprime -= nbhood_size;
                if(branch(start + 1)) {
                    return true;
                } else {
                    // undo everything
                    kprime += nbhood_size;
                    g.undelete_vertex(i);
                    for(int j = 0; j < nbhood_size; j++){
                        g.undelete_vertex(partial_solution.back());
                        partial_solution.pop_back();
                    }
                }
            }

            // if that fails we branch on taking i
            if (kprime >= 1) {
                g.remove_vertex(i);
                partial_solution.emplace_back(i);
                kprime--;
                if(branch(start + 1)) {
                    return true;
                } else {
                    kprime++;
                    g.undelete_vertex(i);
                    partial_solution.pop_back();
                }
            }

        }

        finished_check();
        done = false;
        return has_solution;
    }

}

#ifndef WITH_GTESTS
int main(int argc, char **argv) {
    param_vc::Graph g = param_vc::Graph();
    g.randomise(0,19);
    param_vc::ParameterizedVertexCover vc = param_vc::ParameterizedVertexCover(g,7);

    vc.print();

    if (vc.has_vertex_cover())
        std::cout << "Solved it!!!! Awww yeauuh";
    else
        std::cout << "No solution possible! Sorry :/";

    vc.print();

    return 0;
}
#endif