#include <cstddef> /* NULL */
/* #include <metis.h>*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "graphReader.hpp"

// Convert ASCII edgelist graph to binary edgelist with triplets for each edge (v1, v2, weight) -- all ints
//// Convert ASCII edgelist graph to binary edgelist with four tuple for each edge (v1, v2, weight, ins_stat) -- all ints
// To run:  ./convert 
//hardcoded filename

int main(int argc, char *argv[])
{
    /*
    if (argc < 2) {
        cerr << "Usage: ./convert inputgraph" << endl;
        exit(1);
    }
    string filename = argv[1];
    */
    int fileformat = 0;
    cout << "No of Parms: 3/4: ";
    cin >> fileformat;
    string filename;

    // fileformat==4? filename="asciiEdgeList4.txt": filename="asciiEdgeList.txt";
     GraphReader reader(1000); // buffer size is 1000


    switch (fileformat){
        case 3:
        {       
        filename= "asciiEdgeList.txt";
        reader.read(filename);
        reader.writeBinaryEdgelist(filename + ".edgelist.bin");
        cout << "Binary graph: " << filename + ".edgelist.bin" << endl;
        break;}

        case 4:
        {       
        filename= "asciiEdgeList4.txt";
        reader.read4(filename);
        reader.writeBinaryEdgelist4(filename + ".edgelist.bin");
        cout << "Binary graph: " << filename + ".edgelist.bin" << endl;
        break;}

    }
    



    return 0;
}
