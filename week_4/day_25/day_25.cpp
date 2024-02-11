#include<cstdlib>
#include<iostream>
#include<iterator>
#include<list>
#include<queue>
#include<string>
#include<unordered_map>
#include<unordered_set>
#include<vector>
#include"utils.h"

std::unordered_map<std::string,std::string> findPath(const std::unordered_map<std::string, std::unordered_set<std::string>>& nodes, const std::string& start, const std::string& end);

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {" ", ":"};
    std::vector<std::vector<std::string>> input = read_input_2D("input", delimiters);

    std::unordered_set<std::string> allNodes;
    std::unordered_map<std::string, std::unordered_set<std::string>> nodes;
    for (const auto& line : input){
        allNodes.insert(line[0]);

        for (std::size_t i = 1; i < line.size(); i++){
            nodes[line[0]].insert(line[i]);
            nodes[line[i]].insert(line[0]);
            allNodes.insert(line[i]);
        }
    }

    std::unordered_map<std::string, int> mostVisited;

    int limit{10'000}, n{0};
    for (auto it1 = allNodes.begin(); it1 != allNodes.end(); it1++){
        if (n >= limit){ break; }
        for (auto it2 = std::next(it1); it2 != allNodes.end(); it2++){
            auto path = findPath(nodes, *it1, *it2);

            std::string first  = *it2;
            std::string second = path[*it2];
            while (first != *it1){
                if (first < second){ mostVisited[first + second]++; }
                else { mostVisited[second + first]++; }
                
                first = second;
                second = path[second];
            }

            n++;
        }
    }

    std::priority_queue<std::pair<int, std::string>> orderedQueue;

    for (const auto& pair : mostVisited){ orderedQueue.emplace(pair.second, pair.first); }

    for (int i = 0; i < 3; i++){
        std::string first = orderedQueue.top().second.substr(0, 3);
        std::string second = orderedQueue.top().second.substr(3, 3);

        nodes[first].erase(second);
        nodes[second].erase(first);

        orderedQueue.pop();
    }

    std::vector<std::unordered_set<std::string>> groups;

    for (const auto& [node, connections] : nodes){
        std::vector<std::string> temp;
        temp.push_back(node);
        temp.insert(temp.end(), connections.begin(), connections.end());

        int groupIndex{-1};
        for (std::size_t i = 0; i < groups.size(); i++){
            for (std::size_t j = 0; j < temp.size(); j++){
                if (groups[i].contains(temp[j])){
                    groupIndex = static_cast<int>(i);
                    break;
                }
            }

            if (groupIndex != -1){ break; }
        }

        if (groupIndex == -1){
            groups.push_back({temp.begin(), temp.end()});
        }
        else {
            groups[groupIndex].insert(temp.begin(), temp.end());
        }
    }

    std::size_t part1 = groups[0].size() * groups[1].size();

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Merry Christmas" << std::endl;

    return 0;
}

std::unordered_map<std::string,std::string> findPath(const std::unordered_map<std::string, std::unordered_set<std::string>>& nodes, const std::string& start, const std::string& end){
    std::list<std::string> queue{start};
    std::unordered_map<std::string,std::string> cameFrom;

    while (!queue.empty()){
        std::string current = queue.front();
        queue.pop_front();

        if (current == end){ break; }

        for (const auto& node : nodes.at(current)){
            if (cameFrom.contains(node)){ continue; }

            queue.push_back(node);
            cameFrom[node] = current;
        }
    }

    return cameFrom;
}