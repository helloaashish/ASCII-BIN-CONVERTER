//
//  readBin.cpp
//  bin_ascii_convert
//
//  Created by Aashish Pandey on 2/4/21.
//  Copyright © 2021 Aashish Pandey. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "graphReader.hpp"

using namespace std;

//reader with four tuple edgelist
void readBinary4(string strFile)
{
    ifstream binaryFile;
    int size = 0;
    binaryFile.open(strFile, ios::in | ios::binary);
    binaryFile.seekg(0, ios::end);
    size = int(binaryFile.tellg());
    binaryFile.seekg(0, ios::beg);
    int m = size / sizeof(EdgeType4);
    EdgeType4 edges[m];
    for (int i = 0; i < m; i++)
    {
        binaryFile.read(reinterpret_cast<char *>(&edges), sizeof(edges));
        cout << edges[i].v1 << " " << edges[i].v2 << " " << edges[i].w << " " << edges[i].ins_stat << endl;
    }

    binaryFile.close();
}

// read the triplets edgelist (node1, node2, weights)
void readBinary(string strFile)
{
    ifstream binaryFile;
    int size = 0;
    binaryFile.open(strFile, ios::in | ios::binary);
    binaryFile.seekg(0, ios::end);
    size = int(binaryFile.tellg());
    binaryFile.seekg(0, ios::beg);
    int m = size / sizeof(EdgeType);
    EdgeType edges[m];
    for (int i = 0; i < m; i++)
    {
        binaryFile.read(reinterpret_cast<char *>(&edges), sizeof(edges));
        cout << edges[i].v1 << " " << edges[i].v2 << " " << edges[i].w << endl;
    }
    binaryFile.close();
}

int main()
{
    //FILE NAME
    cout << "Num of Parms: " << endl;
    int parms = 0;
    cin >> parms;
    if (parms == 4)
    {
        string filename = "asciiEdgeList4.txt.edgelist.bin";
        readBinary4(filename);
    }
    else if (parms == 3)
    {
        string filename = "asciiEdgeList.txt.edgelist.bin";
        readBinary(filename);
    }
    else
    {
        cout << "Wrong File Format";
    }

    return 0;
}
