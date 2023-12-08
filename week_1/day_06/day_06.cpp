#include<cstdlib>
#include<iostream>
#include<string>
#include<vector>
#include"utils.h"

long calculateWinningWays(const std::string& stringTime, const std::string& stringDistance);

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {" ", "Time:", "Distance:"};
    std::vector<std::vector<std::string>> input = read_input_2D("input", delimiters);

    std::string concatTime, concatDistance;

    long part1{1};
    for (std::size_t i = 0; i < input[0].size(); i++){
        part1 *= calculateWinningWays(input[0][i], input[1][i]);
        concatTime += input[0][i];
        concatDistance += input[1][i];
    }

    long part2 = calculateWinningWays(concatTime, concatDistance);

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}

long calculateWinningWays(const std::string& stringTime, const std::string& stringDistance){
    long time = std::stol(stringTime);
    long distance = std::stol(stringDistance);

    long i{0};
    while (i * (time - i) <= distance){ i++; }

    return time - (i * 2) + 1;
}