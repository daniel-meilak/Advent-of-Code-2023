#include<cstdlib>
#include<iostream>
#include<string>
#include<numeric>
#include<unordered_map>
#include<utility>
#include<vector>
#include"utils.h"

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {" ", "=", "(", ")", ","};
    std::vector<std::vector<std::string>> input = read_input_2D("input", delimiters);

    std::string directions{input[0][0]};

    std::unordered_map<std::string, std::pair<std::string,std::string>> nodes;
    for (std::size_t i = 2; i < input.size(); i++){
        nodes[input[i][0]] = std::make_pair(input[i][1], input[i][2]);
    }

    auto nextPosition = [&nodes, &directions](std::string& currentPosition, long& step){
        switch (directions[step % directions.size()]){
        case 'L':
            currentPosition = nodes[currentPosition].first;
            return;
        default:
            currentPosition = nodes[currentPosition].second;
            return;
        }
    };

    std::string currentPosition{"AAA"};
    long part1{0};
    while (currentPosition != "ZZZ"){
        nextPosition(currentPosition, part1);
        part1++;
    }

    std::vector<std::string> simultaneousPositions;
    for (std::size_t i = 2; i < input.size(); i++){
        if (input[i][0][2] == 'A'){
            simultaneousPositions.push_back(input[i][0]);
        }
    }

    long steps{0};
    std::vector<long> stepsToZ;
    while (!simultaneousPositions.empty()){
        for (std::size_t i = 0; i < simultaneousPositions.size(); i++){
            auto& position = simultaneousPositions[i];
            nextPosition(position, steps);

            if (position[2] == 'Z'){
                stepsToZ.push_back(steps+1);
                simultaneousPositions.erase(simultaneousPositions.begin() + i);
                i--;
            }
        }
        steps++;
    }

    long part2{1};
    for (long s : stepsToZ){ part2 = std::lcm(part2, s); } 


    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}