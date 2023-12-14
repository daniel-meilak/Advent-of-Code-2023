#include<cstdlib>
#include<iostream>
#include<list>
#include<numeric>
#include<string>
#include<tuple>
#include<unordered_map>
#include<vector>
#include"utils.h"

long long countPossibilities(const std::string& row, std::list<int> counts, long long possibilities = 0);

struct TupleHash {
    size_t operator()(const std::tuple<std::string, std::list<int>>& t) const {
        std::hash<std::string> string_hasher;
        std::hash<int> int_hasher;
        size_t seed = string_hasher(std::get<0>(t));
        for (int i : std::get<1>(t)) {
            seed ^= int_hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        return seed;
    }
};

std::unordered_map<std::tuple<std::string, std::list<int>>, long long, TupleHash> cache;

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {" ", ","};
    std::vector<std::vector<std::string>> input = read_input_2D("input", delimiters);

    long long part1{0}, part2{0};
    for (auto& line : input){
        std::string row = line[0];
        std::list<int> counts;
        for (std::size_t i = 1; i < line.size(); i++){
            counts.push_back(std::stoi(line[i]));
        }

        part1 += countPossibilities(row, counts);

        std::string row2{row};
        std::list<int> counts2{counts};
        for (int i = 1; i < 5; i++){
            row2 += '?' + row;
            counts2.insert(counts2.end(), counts.begin(), counts.end());
        }

        part2 += countPossibilities(row2, counts2);
    }

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}

long long countPossibilities(const std::string& row, std::list<int> counts, long long possibilities){
    // check for cached result
    auto parameters = std::make_tuple(row, counts);
    if (cache.find(parameters) != cache.end()){ return cache[parameters]; }

    if (counts.empty()){ return row.find('#') == std::string::npos; }

    int current = counts.front();
    counts.pop_front();

    auto totalDamaged = std::accumulate(counts.begin(), counts.end(), 0);
    for (std::size_t i = 0; i < (row.size() - totalDamaged - counts.size() - current + 1); i++){
        if (row.substr(0, i).find('#') != std::string::npos){
            break;
        }

        int next = static_cast<int>(i) + current;
        if (current <= static_cast<int>(row.size()) && row.substr(i, current).find('.') == std::string::npos && row[next] != '#'){
            if (next + 1 >= static_cast<int>(row.size())){ possibilities++; }
            else { possibilities += countPossibilities(row.substr(next + 1), counts); }
        }
    }

    cache[parameters] = possibilities;
    return possibilities;
}