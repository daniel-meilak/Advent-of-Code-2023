#include<algorithm>
#include<cstdlib>
#include<iostream>
#include<queue>
#include<string>
#include<tuple>
#include<unordered_set>
#include<vector>
#include"point.h"
#include"utils.h"

void findNeighbourNodes(point node, const std::unordered_set<point>& nodes, std::unordered_map<point,std::unordered_map<point,int>>& nodeMap, const std::vector<std::string>& input, bool part2);
int findLongestPath(point start, point end, const std::unordered_map<point,std::unordered_map<point,int>>& nodeMap);

int main(){

    // read input into vector of strings.
    std::vector<std::string> delimiters = {};
    std::vector<std::string> input = read_input("input", delimiters);

    pad(input, '#');

    point start{2,1};
    point end{static_cast<int>(input[0].size()-3), static_cast<int>(input.size()-2)};

    // BFS to find all nodes along the path
    std::unordered_set<point> nodes;

    std::queue<point> queue;
    queue.push(start);

    std::unordered_set<point> visited;

    while (!queue.empty()){
        point current = queue.front();
        queue.pop();

        if (visited.contains(current)){ continue; }
        visited.insert(current);

        if (current == end){ continue; }

        std::vector<point> neighbours;
        for (const auto& dir : {current.up(), current.down(), current.left(), current.right()}){
            
            if (input[dir.y][dir.x] == '#'){ continue; }
            neighbours.push_back(dir);
        }

        // only add intesections to nodes
        if (neighbours.size() > 2){ nodes.insert(current); }

        for (const auto& neighbour : neighbours){ queue.push(neighbour); }
    }

    // add start and end to nodes
    nodes.insert(start);
    nodes.insert(end);

    // find neighbours of each node
    std::unordered_map<point,std::unordered_map<point,int>> nodeMapPart1, nodeMapPart2;
    for (const auto& node : nodes){ findNeighbourNodes(node, nodes, nodeMapPart1, input, false); }
    for (const auto& node : nodes){ findNeighbourNodes(node, nodes, nodeMapPart2, input, true ); }

    // find longest path from start to end
    int part1 = findLongestPath(start, end, nodeMapPart1);
    int part2 = findLongestPath(start, end, nodeMapPart2);    

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}

void findNeighbourNodes(point node, const std::unordered_set<point>& nodes, std::unordered_map<point,std::unordered_map<point,int>>& nodeMap, const std::vector<std::string>& input, bool part2){
    std::queue<std::tuple<point,int,std::unordered_set<point>>> queue;
    queue.push({node, 0, {}});

    while (!queue.empty()){
        auto [current, steps, visited] = queue.front();
        queue.pop();

        if (current != node && nodes.contains(current)){
            if (!(nodeMap[node].contains(current) && nodeMap[node][current] >= steps)){ nodeMap[node][current] = steps; }
            continue;
        }

        visited.insert(current);

        if (char tile = input[current.y][current.x]; !part2 && tile != '.'){
            point next;
            if      (tile == '^'){ next = current.up();    }
            else if (tile == '<'){ next = current.left();  }
            else if (tile == '>'){ next = current.right(); }
            else                 { next = current.down();  }
            
            if (!visited.contains(next)){ queue.push({next, steps+1, visited}); }
        }
        else {
            for (const auto& dir : {current.up(), current.down(), current.left(), current.right()}){
                if (input[dir.y][dir.x] == '#' || visited.contains(dir)){ continue; }
                queue.push({dir, steps+1, visited});
            }
        }
    }
}

int findLongestPath(point start, point end, const std::unordered_map<point,std::unordered_map<point,int>>& nodeMap){
    int longestPathLength{0};
    
    std::queue<std::tuple<point,int,std::unordered_set<point>>> queue;
    queue.push({start, 0, {}});

    while (!queue.empty()){
        auto [current, steps, visited] = queue.front();
        queue.pop();

        if (current == end){
            longestPathLength = std::max(longestPathLength, steps);
            continue;
        }

        visited.insert(current);

        for (const auto& [neighbour, distance] : nodeMap.at(current)){
            if (visited.contains(neighbour)){ continue; }
            queue.push({neighbour, steps+distance, visited});
        }
    }

    return longestPathLength;
}