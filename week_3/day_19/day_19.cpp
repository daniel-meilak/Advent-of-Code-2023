#include<cstdlib>
#include<iostream>
#include<string>
#include<unordered_map>
#include<utility>
#include<vector>
#include"utils.h"

struct range {
    int min{1};
    int max{4000};
};

using Workflows = std::unordered_map<std::string, std::vector<std::pair<std::string,std::string>>>;
using AcceptedPart = std::unordered_map<char, range>;

void findAcceptableParts(const Workflows& workflows, const std::string& next, std::vector<AcceptedPart>& parts, AcceptedPart& part);
bool acceptablePart(const std::unordered_map<char, int>& part, std::vector<AcceptedPart>& acceptedParts);

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {"{", "}", ",", "=", ":"};
    std::vector<std::vector<std::string>> input = read_input_2D("input", delimiters);

    Workflows workflows;

    // build workflows
    std::size_t i;
    for (i = 0; i < input.size(); i++){
        const auto& line = input[i];
        if (line.empty()){ break; }

        for (std::size_t j = 1; j < line.size() - 2; j+=2){
            workflows[line[0]].emplace_back(line[j], line[j+1]);
        }

        workflows[line[0]].emplace_back("", line.back());
    }

    std::vector<std::unordered_map<char, int>> parts;

    for (i = i + 1; i < input.size(); i++){
        const auto& line = input[i];
        
        parts.push_back({});
        for (std::size_t j = 0; j < 8; j+=2){
            parts.back()[line[j][0]] = std::stoi(line[j+1]);
        }
    }

    std::vector<AcceptedPart> acceptedParts;
    AcceptedPart part{{'x',{}}, {'m',{}}, {'a',{}}, {'s',{}}};
    findAcceptableParts(workflows, "in", acceptedParts, part);


    int part1{0};
    for (const auto& part : parts){
        if (acceptablePart(part, acceptedParts)){
            for (const auto& [_, rating]: part){ part1 += rating; }
        }
    }

    long part2{0};
    for (const auto& criterea : acceptedParts){
        long total{1};
        for (const auto& [_, range] : criterea){ total *= (range.max - range.min + 1); }
        part2 += total;
    }

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}

void findAcceptableParts(const Workflows& workflows, const std::string& next, std::vector<AcceptedPart>& parts, AcceptedPart& part){

    if (next == "A"){
        parts.push_back(part);
        return;
    }
    else if (next == "R"){ return; }
    
    for (const auto& [check, destination] : workflows.at(next)){    
        if (check.empty()){
            findAcceptableParts(workflows, destination, parts, part);
            break;
        }

        auto partCopy = part;
        int limit = std::stoi(check.substr(2));

        if (check[1] == '>'){
            partCopy[check[0]].min = limit + 1; 
            part[check[0]].max = limit;
        }
        else {
            partCopy[check[0]].max = limit - 1;
            part[check[0]].min = limit;
        }

        findAcceptableParts(workflows, destination, parts, partCopy);
    }
}

bool acceptablePart(const std::unordered_map<char, int>& part, std::vector<AcceptedPart>& acceptedParts){
    for (const auto& criterea : acceptedParts){
        bool accepted{true};
        for (const auto& [key, range] : criterea){
            if (part.at(key) < range.min || part.at(key) > range.max){ accepted = false; }
        }

        if (accepted){ return true; }
    }

    return false;
}