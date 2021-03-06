//
// Created by norris on 5/25/20.
//

#ifndef DISTRIBUTED_SCC_GRAPHREADER_HPP
#define DISTRIBUTED_SCC_GRAPHREADER_HPP

#include <unordered_set>
#include <vector>
#include <string>

// For mmap
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

//#include "basic.hpp"
#include "../metis-5.1.0/include/metis.h"

using namespace std;
typedef struct {
    idx_t v1;
    idx_t v2;
    int w;
} EdgeType;

typedef struct{
    idx_t v1;
    idx_t v2;
    int w;
    int ins_stat;
} EdgeType4;

class GraphReader {
    public:
    GraphReader() : buffer_size(100){}
    GraphReader(int buf) : buffer_size(buf){}
    void read(string filename);
    void writeBinaryEdgelist(string filename);
    void read4(string filename);
    void writeBinaryEdgelist4(string filename);    


    int get_num_vert() { return this->nvertices; }
    vector<idx_t>& get_adj_ind() { return this->adj_ind; }
    vector<idx_t>& get_adj_vert() { return this->adj_vert; }
    void printGraph();
    uint8_t* openMMap(const string filename);
    private:
    //    void dump_binary(string filename);
    
    private:
    const int buffer_size = 100; // reserve memory in advance
    idx_t nvertices = 0;
    idx_t nedges = 0;
    unordered_set<idx_t> vertices;
    vector<vector<idx_t>> edges;
    
    // For Metis
    vector<idx_t> adj_ind; // Indices of starting points in adjacent array
    vector<idx_t> adj_vert; // Adjacent vertices in consecutive index order
    vector<idx_t> vert_weights;  // Weights of vertices
    
    vector<idx_t> vert_stat;
};


#endif //DISTRIBUTED_SCC_GRAPHREADER_HPP

