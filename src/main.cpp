#include <iostream>
#include "param_vc.h"

// namespace param_vc {

//     class Graph {
//         public:
//             Graph() {};
//             virtual ~Graph() = default;

//             int v;
//             int e;

//     };

//     class ParameterizedVertexCover {
//         public:
//             ParameterizedVertexCover(Graph& g, int k) : graph(g) , k(k) {};
//             virtual ~ParameterizedVertexCover() = default;
            
//             Graph& graph;
//             int k;
//     };

// }

namespace param_vc {

    int ParameterizedVertexCover::has_vertex_cover(){

        return g.v > k;// || this.g.e > this.k;

    }

}

#ifndef WITH_GTESTS
int main(int argc, char **argv) {
    std::cout << "Hello there\n...\nGeneral Kenobi\n";

    param_vc::Graph g = param_vc::Graph();
    param_vc::ParameterizedVertexCover vc = param_vc::ParameterizedVertexCover(g,5);

    std::cout << vc.has_vertex_cover() << "\n";
    return 0;
}
#endif