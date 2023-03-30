// author: Mate Botond Nemeth
// Last modified: 30/11/2022
// Artificial Intelligence Coursework Option A

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include <queue>
#include <map>
#include <tuple>
#include <cmath>
#include <algorithm>
#include "cave.h"

using namespace std;

// Operator overloading

bool operator<(const Node &node1, const Node &node2)
{
    return node1.f_score < node2.f_score;
}

bool operator<=(const Node &node1, const Node &node2)
{
    return node1.f_score <= node2.f_score;
}

bool operator==(const Node &node1, const Node &node2)
{
    return node1.f_score == node2.f_score;
}

bool operator>(const Node &node1, const Node &node2)
{
    return node1.f_score > node2.f_score;
}

bool operator>=(const Node &node1, const Node &node2)
{
    return node1.f_score >= node2.f_score;
}

// Processes the file, takes in a file name as a parameter
void file_processing (char** argv);

// Print all nodes to test reading in from the file
void test_print (vector<Node> nodes);

// Calculates the distance between two nodes
float get_distance (Node node1, Node node2);

// Prints the route found by the A* algorithm
void get_route (vector<Node> nodes, Node start, Node end, map<int, int>& came_from, char** argv)
{
    string file_name = argv[1];
    file_name += ".csn";
    ofstream result(file_name);

    // Route cost
    // float route_cost = 0;

    int x = end.node_num;
    int y = 1000000;

    vector<int> route;
    route.push_back(x);

    // Prints all nodes and calculates route cost
    while(y != 1)
    {
        y = came_from.at(x);
        // Calculating the route cost, not a requirement of the output, so it is commented out to speed up the program
        // route_cost += get_distance(nodes[x-1], nodes[y-1]);
        route.push_back(y);
        x = y;
    }

    reverse(route.begin(), route.end());
    string route_result = "";

    for (int i : route)
    {
        string s = to_string(i);
        if (i == end.node_num)
        {
            route_result += s;
        }
        else
        {
            route_result += s + " ";
        }
    }

    result << route_result;
    result.close();
    route.clear();
}

// Gets called if A* doesn't find a route
void no_route (char** argv)
{
    string file_name = argv[1];
    file_name += ".csn";
    ofstream result(file_name);
    result << "0";
    result.close();
}

// A* path finding algorithm. Takes in a list of nodes, a starting node, a goal node and an integer, integer map as a parameter.
void A_star (vector<Node>& nodes, Node& start, Node& end, map<int, int>& came_from, char** argv)
{
    // Priority queue for the algorithm
    // Takes in a tuple of float and Node
    // Have to pass in the float f score values as negative, since I was not able to change the priority list default settings
    // That's why it uses a tuple as well
    priority_queue<tuple<float, Node>> frontier;

    start.g_score = 0;
    start.f_score = get_distance(start, end);

    // The came from list has value pairs, each key (node number) has the number of the node it came from
    came_from[start.node_num] = start.node_num;

    // Pushing the start node to the frontier
    frontier.push(make_tuple(-start.f_score, start));

    while (!frontier.empty())
    {
        // Getting and poping the node with the lowest f score
        Node current = get<1>(frontier.top());
        frontier.pop();

        if (current.node_num == end.node_num)
        {
            get_route(nodes, start, end, came_from, argv);
            return;
        }

        // Goes through all connected nodes
        for (int next_num : current.connections)
        {
            Node next = nodes[next_num - 1];
            // Calculates new cost
            double new_cost = current.g_score + get_distance(current, next);
            if (new_cost < nodes[next.node_num - 1].g_score)
            {
                // Updates scores
                nodes[next.node_num - 1].g_score = new_cost;
                next.g_score = new_cost;
                nodes[next.node_num - 1].f_score = new_cost + get_distance(next, end);
                next.f_score = new_cost + get_distance(next, end);
                // Pushes new node to the frontier with the updated negative of the f score
                frontier.push(make_tuple(-next.f_score, nodes[next.node_num - 1]));
                // Sets came from node number
                came_from[next.node_num] = current.node_num;
            }
        }
    }
    // If no route found, calls this function
    no_route(argv);
}

int main (int argc , char ** argv)
{
    file_processing(argv);
}

void file_processing (char** argv)
{
    string file_name = argv[1];
    file_name += ".cav";

    // File name passed as an argument when executing the program
    ifstream file(file_name);

    // Reading from the file
    if ( file )
    {
        stringstream line;

        line << file.rdbuf();

        file.close();

        string segment;
        vector<string> segments;

        int num;

        // The number of nodes in the graph
        int cave_sum;
        // A vector storing all nodes
        vector<Node> nodes;

        // Get all numbers separated by a ","
        while(getline(line, segment, ','))
        {
            segments.push_back(segment);
        }

        // The total number of caves
        cave_sum = stoi(segments[0]);
        int cavern_num = 1;

        // Get node coordinates
        for(int i = 1; i <= cave_sum * 2; i = i + 2)
        {
            Node node = Node(cavern_num);
            node.coord_x = stoi(segments[i]);
            node.coord_y = stoi(segments[i + 1]);
            nodes.push_back(node);
            cavern_num++;
        }

        int row = 0;
        int col = 0;

        // Gets connections
        for (int i = cave_sum * 2 + 1; i < segments.size(); i++)
        {
            int num = stoi(segments[i]);
            if (num == 1)
            {
                nodes[col].connections.push_back(nodes[row].node_num);
            }
            col++;
            if (col == cave_sum)
            {
                col = 0;
                row++;
            }
        }

        map<int, int> came_from;

        A_star(nodes, nodes[0], nodes[nodes.size() - 1], came_from, argv);

        // Clears memory
        nodes.clear();
        segments.clear();
        came_from.clear();
    }
}

void test_print (vector<Node> nodes)
{
    for(Node node : nodes)
    {
        cout << "Node: " << node.node_num << "\t coord x:" << node.coord_x << "\t coord y: " << node.coord_y << endl;
        cout << "Number of Nodes connected" << node.connections.size() << endl;
    }
}

float get_distance (Node node1, Node node2)
{
    return sqrt(pow((node2.coord_x - node1.coord_x), 2) + pow((node2.coord_y - node1.coord_y), 2));
}