

//
// Created by norris on 5/25/20.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include "graphReader.hpp"

uint8_t *GraphReader::openMMap(const string filename)
{
    int m_fd;
    struct stat statbuf;
    uint8_t *m_ptr_begin;

    if ((m_fd = open(filename.c_str(), O_RDONLY)) < 0)
        perror("can't open file for reading");

    if (fstat(m_fd, &statbuf) < 0)
        perror("fstat in openMMap failed");
    auto size = statbuf.st_size;
#if DEBUG >= 1
    cout << "Memory-mapping file " << filename << " of size " << size << endl;
#endif

    if ((m_ptr_begin = (uint8_t *)mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, m_fd, 0)) == MAP_FAILED)
        perror("mmap in openMMap failed");

    uint8_t *m_ptr = m_ptr_begin;
    size = statbuf.st_size;

    return m_ptr;

    /*
     //To read:
     uint8_t *  mmfile = openMMap("my_file", length);
     uint32_t * memblockmm;
     memblockmm = (uint32_t *)mmfile; //cast file to uint32 array
     uint32_t data = memblockmm[0]; // an integer
     mmfile += 4; //increment by 4 since we read a 32 bit entry and each entry in mmfile is 8 bits.
     */
}

void GraphReader::read(string filename)
{
    std::ifstream file;
    int capacity = this->buffer_size;
    adj_vert.reserve(capacity);
    vert_weights.reserve(capacity);
    idx_t maxvertex = 0;
    bool binary = false;

    if (filename.substr(filename.rfind('.') + 1) == "bin")
        binary = true;

    if (access(filename.c_str(), F_OK) == -1)
    {
        cerr << "Specified input file " << filename << " does not exist." << endl;
        exit(1);
    }
    //file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    idx_t current_index = 0;
    try
    {
        file.open(filename);
        std::string line;
        idx_t previous_vertex = 0; // start indexing vertices at 0
        adj_ind.push_back(previous_vertex);

        while (file.peek() != EOF)
        {
            idx_t e[3];
            if (binary)
            {
                EdgeType edge_info;
                file.readsome(reinterpret_cast<char *>(&edge_info), sizeof(EdgeType));
                if (maxvertex < edge_info.v1)
                    maxvertex = edge_info.v1;
                if (maxvertex < edge_info.v2)
                    maxvertex = edge_info.v2;
                adj_vert.push_back(edge_info.v2);
                vert_weights.push_back(edge_info.w);
                e[0] = edge_info.v1;
            }
            else
            {
                std::getline(file, line);
                if (line.empty())
                    continue;
                //cout << line << endl;
                // line contains the current line
                std::stringstream lineStream(line);
                if (line[0] == '%' || line[0] == '#')
                    continue; // skip comments

                int value;
                int i = 0;
                // Read an integer at a time from the line
                while (lineStream >> value)
                {
                    // Add the integers from a line to a 1D array (vector)
                    if (i < 2)
                    {
                        e[i] = value; // this is a vertex index
                        if (maxvertex < value)
                            maxvertex = value;
                    }
                    // after the vertices, the remaining numbers are the weights
                    if (i == 2)
                        vert_weights.push_back(value);
                    i++;
                }
                if (i < 2)
                    cerr << "ERROR: Invalid edge list line encountered (edge " << current_index << "):  " << line << endl;
                adj_vert.push_back(e[1]);
            }

            if (previous_vertex != e[0])
            {
                // Starting edges for new vertex; assumes sorted edge lists!!
                adj_ind.push_back(current_index);
                previous_vertex = e[0];
            }
            current_index++;
            if (current_index >= capacity)
            {
                capacity += this->buffer_size;
                adj_vert.reserve(capacity);
                vert_weights.reserve(capacity);
            }
        }
        file.close();
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "GraphReader::read: Exception opening/reading/closing file" << filename << endl;
    }
    adj_ind.push_back(current_index);
    this->nedges = current_index;
    adj_vert.shrink_to_fit();
    this->nvertices = maxvertex + 1; // vertex indices start at 0
    //edges.shrink_to_fit();
#if DEBUG >= 2
    this->printGraph();
#endif
}

//A-edit implementation of read4

void GraphReader::read4(string filename)
{
    std::ifstream file;
    int capacity = this->buffer_size;
    adj_vert.reserve(capacity);
    vert_weights.reserve(capacity);
    vert_stat.reserve(capacity); //A-edit
    idx_t maxvertex = 0;
    bool binary = false;

    if (filename.substr(filename.rfind('.') + 1) == "bin")
        binary = true;

    if (access(filename.c_str(), F_OK) == -1)
    {
        cerr << "Specified input file " << filename << " does not exist." << endl;
        exit(1);
    }
    //file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    idx_t current_index = 0;
    try
    {
        file.open(filename);
        std::string line;
        idx_t previous_vertex = 0; // start indexing vertices at 0
        adj_ind.push_back(previous_vertex);

        while (file.peek() != EOF)
        {
            idx_t e[4];   //A-edit
            if (binary)
            {
                EdgeType4 edge_info;  //A-edit
                file.readsome(reinterpret_cast<char *>(&edge_info), sizeof(EdgeType4));  //A-edit
                if (maxvertex < edge_info.v1)
                    maxvertex = edge_info.v1;
                if (maxvertex < edge_info.v2)
                    maxvertex = edge_info.v2;
                adj_vert.push_back(edge_info.v2);
                vert_weights.push_back(edge_info.w);
                vert_stat.push_back(edge_info.ins_stat);   //A-edit
                e[0] = edge_info.v1;
            }
            else
            {
                std::getline(file, line);
                if (line.empty())
                    continue;
                //cout << line << endl;
                // line contains the current line
                std::stringstream lineStream(line);
                if (line[0] == '%' || line[0] == '#')
                    continue; // skip comments

                int value;
                int i = 0;
                // Read an integer at a time from the line
                while (lineStream >> value)
                {
                    // Add the integers from a line to a 1D array (vector)
                    if (i < 2)
                    {
                        e[i] = value; // this is a vertex index
                        if (maxvertex < value)
                            maxvertex = value;
                    }
                    // after the vertices, the remaining numbers are the weights
                    if (i == 2)
                        vert_weights.push_back(value);
                                        // after the vertices, the remaining numbers are the weights
                    if (i == 3)
                        vert_stat.push_back(value);
                    i++;
                }
                if (i < 2)
                    cerr << "ERROR: Invalid edge list line encountered (edge " << current_index << "):  " << line << endl;
                adj_vert.push_back(e[1]);
            }

            if (previous_vertex != e[0])
            {
                // Starting edges for new vertex; assumes sorted edge lists!!
                adj_ind.push_back(current_index);
                previous_vertex = e[0];
            }
            current_index++;
            if (current_index >= capacity)
            {
                capacity += this->buffer_size;
                adj_vert.reserve(capacity);
                vert_weights.reserve(capacity);
                vert_stat.reserve(capacity);      //A-edit
            }
        }
        file.close();
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "GraphReader::read: Exception opening/reading/closing file" << filename << endl;
    }
    adj_ind.push_back(current_index);
    this->nedges = current_index;
    adj_vert.shrink_to_fit();
    this->nvertices = maxvertex + 1; // vertex indices start at 0
    //edges.shrink_to_fit();
#if DEBUG >= 2
    this->printGraph();
#endif
}
// DONE UPDATE OF READ4 


void GraphReader::writeBinaryEdgelist(string filename)
{
    // Binary format: store the graph in edgelist form, where each edge is represented as pair of idx_t vertex IDs
    // and weights are integers. So, for a graph with N edges, we have N triplets <idx_t, idx_t, int>
    ofstream outfile(filename, ios::out | ios::binary);
    idx_t i = 0, current = 0;
    cout << "Writing binary graph; nv=" << this->nvertices << ", nedges=" << this->nedges << endl;
    for (idx_t v1 = 0; v1 < this->nvertices; v1++)
    {
        // Edge is between current adn adj_ind[i]
        if (adj_ind[i] >= this->nedges)
            break; // done
        for (idx_t v2_ind = adj_ind[i]; v2_ind < adj_ind[i + 1]; v2_ind++)
        {
            EdgeType edge = {v1, adj_vert[v2_ind], vert_weights[v2_ind]};
            outfile.write(reinterpret_cast<char *>(&edge), sizeof(edge));
            cout << edge.v1 << " " << edge.v2 << " " << edge.w << endl;
        }
        i++;
    }
    outfile.close();

#if DEBUG >= 2
    // Read the binary format to verify correctness
    ifstream fin(filename, ios::in | ios::binary);
    fin.seekg(0, ifstream::end);
    int size = fin.tellg() / sizeof(EdgeType);
    EdgeType tmp[size];
    fin.seekg(0, ifstream::beg);
    fin.read(reinterpret_cast<char *>(&tmp), sizeof(tmp));
    fin.close();
    cout << "Edgelist read from binary: " << endl;
    for (int j = 0; j < size; j++)
        cout << tmp[j].v1 << " " << tmp[j].v2 << " " << tmp[j].w << endl;
#endif
}


//A-edit update the writeBinaryEdgelist to write graph with 4 parms
void GraphReader::writeBinaryEdgelist4(string filename)
{
    // Binary format: store the graph in edgelist form, where each edge is represented as pair of idx_t vertex IDs
    // and weights are integers. So, for a graph with N edges, we have N triplets <idx_t, idx_t, int>
    ofstream outfile(filename, ios::out | ios::binary);
    idx_t i = 0, current = 0;
    cout << "Writing binary graph; nv=" << this->nvertices << ", nedges=" << this->nedges << endl;
    for (idx_t v1 = 0; v1 < this->nvertices; v1++)
    {
        // Edge is between current adn adj_ind[i]
        if (adj_ind[i] >= this->nedges)
            break; // done
        for (idx_t v2_ind = adj_ind[i]; v2_ind < adj_ind[i + 1]; v2_ind++)
        {
            EdgeType4 edge = {v1, adj_vert[v2_ind], vert_weights[v2_ind], vert_stat[v2_ind]}; //A-edit
            outfile.write(reinterpret_cast<char *>(&edge), sizeof(edge));
            cout << edge.v1 << " " << edge.v2 << " " << edge.w << " "<< edge.ins_stat<<endl;  //A-edit
        }
        i++;
    }
    outfile.close();

#if DEBUG >= 2
    // Read the binary format to verify correctness
    ifstream fin(filename, ios::in | ios::binary);
    fin.seekg(0, ifstream::end);
    int size = fin.tellg() / sizeof(EdgeType4);
    EdgeType4 tmp[size];
    fin.seekg(0, ifstream::beg);
    fin.read(reinterpret_cast<char *>(&tmp), sizeof(tmp));
    fin.close();
    cout << "Edgelist read from binary: " << endl;
    for (int j = 0; j < size; j++)
        cout << tmp[j].v1 << " " << tmp[j].v2 << " " << tmp[j].w << " "<<tmp[j].ins_stat<<endl;
#endif
}


void GraphReader::printGraph()
{
    cout << "Graph with nvertices=" << this->nvertices << ", nedges=" << this->nedges << ", Adjacency indices: " << endl;
    for (auto it = adj_ind.begin(); it != adj_ind.end(); it++)
        cout << *it << " ";
    cout << "\nAdjacent vertices in consecutive index order: " << endl;
    for (int i = 0; i < adj_vert.size(); i++)
        cout << adj_vert[i] << " ";
    cout << endl;
    for (int i = 0; i < vert_weights.size(); i++)
        cout << vert_weights[i] << " ";
    cout << endl
         << "End of graph.\n";
}
