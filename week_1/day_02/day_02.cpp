#include<algorithm>
#include<iostream>
#include<string>
#include<vector>
#include"utils.h"

struct Game {
    int red{0};
    int green{0};
    int blue{0};
};

Game maxCubes{12,13,14};

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {"Game", ",", ";", " "};
    std::vector<std::vector<std::string>> input = read_input_2D("input", delimiters);

    std::vector<Game> games;

    for (auto &line : input){
        games.push_back({});

        for (std::size_t i = 2; i < line.size(); i += 2){
            
            if      (line[i] == "red"  ){ games.back().red = std::max(games.back().red, std::stoi(line[i-1]));   }
            else if (line[i] == "green"){ games.back().green = std::max(games.back().green, std::stoi(line[i-1])); }
            else if (line[i] == "blue" ){ games.back().blue = std::max(games.back().blue, std::stoi(line[i-1]));   }
        }
    }

    long part1{0}, part2{0};
    for (std::size_t i = 0; i < games.size(); i++){
        if (games[i].red <= maxCubes.red && games[i].green <= maxCubes.green && games[i].blue <= maxCubes.blue){
            part1 += i+1;
        }

        part2 += games[i].red * games[i].green * games[i].blue;
    }

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}
