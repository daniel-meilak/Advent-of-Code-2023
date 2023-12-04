#include<algorithm>
#include<cmath>
#include<cstdlib>
#include<iostream>
#include<string>
#include<vector>
#include"utils.h"

std::size_t winningNumbersSize = 10;

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {" ", "Card", ":"};
    std::vector<std::vector<std::string>> input = read_input_2D("input", delimiters);

    // keep track of how many of each card we have
    std::vector<int> numCards(input.size(), 1);

    long part1{0}, part2{0};
    for (std::size_t i = 0; i < input.size(); i++){
        auto &line = input[i];

        int matches{0};

        auto scratchCard = split_input(line, "|");

        // find how many matches there are
        for (std::size_t j = 1; j < scratchCard[0].size(); j++){
            if (std::ranges::find(scratchCard[1], scratchCard[0][j]) != scratchCard[1].end()){
                matches++;
            }
        }

        part1 += static_cast<long>(std::pow(2, matches-1));

        // get copies of cards according to number of matches
        for (std::size_t j = 1; j <= static_cast<std::size_t>(matches); j++){
            numCards[i+j] += numCards[i];
        }

        part2 += numCards[i];
    }

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}
