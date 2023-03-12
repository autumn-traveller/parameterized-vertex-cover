#ifndef _PARAM_VC_H
#define _PARAM_VC_H

namespace param_vc {

    class Graph {
        public:
            Graph() {v = 0;e = 0;};
            ~Graph() = default;

            int v;
            int e;

    };

    class ParameterizedVertexCover {
        public:
            ParameterizedVertexCover(Graph& g, int k) : g(g) , k(k) {};
            ~ParameterizedVertexCover() = default;
            
            int has_vertex_cover();

            Graph& g;
            int k;
    };

}

#endif