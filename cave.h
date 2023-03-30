// author: Mate Botond Nemeth
// Last modified: 30/11/2022
// Artificial Intelligence Coursework Option A Node header file

#pragma once
#include <iostream>
#include <memory>
#include <vector>

class Node
{
public:
    // Constructor for the node
    Node(int value);
    // Name of the node
    int node_num;
    // The two coordinates
    int coord_x;
    int coord_y;
    // G score for the A* search algorithm
    float g_score;
    // F score for the A* search algorithm
    float f_score;
    // Nodes connected to this node
    std::vector<int> connections;
};
