// author: Mate Botond Nemeth
// Last modified: 30/11/2022
// Artificial Intelligence Coursework Option A Node file

#include <iostream>
#include <memory>
#include <vector>
#include "cave.h"

using namespace std;

Node::Node(int value)
{
    Node::node_num = value;
    Node::g_score = 1000000;
    Node::f_score = 1000000;
}
