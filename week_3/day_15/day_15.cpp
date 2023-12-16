#include<algorithm>
#include<cstdlib>
#include<iostream>
#include<string>
#include<unordered_map>
#include<vector>
#include"utils.h"

int hash(const std::string& input);

struct Lens {
    std::string label;
    int focalLength;
};

int main(){

    // read input into vector of strings
    std::vector<std::string> delimiters = {","};
    std::vector<std::string> input = read_input("input", delimiters);

    int part1{0};
    for (const auto& line : input){
        part1 += hash(line);
    }

    std::unordered_map<int, std::vector<Lens>> boxes;
    for (const auto& line : input){
        auto operatorIndex = line.find_first_of("-=");
        std::string label = line.substr(0, operatorIndex);
        auto& box = boxes[hash(label)];
        auto it = std::ranges::find(box, label, &Lens::label);

        switch (line[operatorIndex]){
        case '-':
            if (it != box.end()){ box.erase(it); }
            break;
        default :
            auto focalLength = std::stoi(line.substr(operatorIndex + 1));
            if (it != box.end()){ it->focalLength = focalLength; }
            else { box.emplace_back(label, focalLength); }
            break;
        }
    }

    int part2{0};
    for (const auto& [box, lens] : boxes){
        for (std::size_t i = 0; i < lens.size(); i++){
            part2 += (box + 1) * (static_cast<int>(i) + 1) * lens[i].focalLength;
        }
    }

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}

int hash(const std::string& input){
    int currentValue{0};
    
    for (const char& c : input){
        currentValue += static_cast<int>(c);
        currentValue *= 17;
        currentValue %= 256;
    }

    return currentValue;
}