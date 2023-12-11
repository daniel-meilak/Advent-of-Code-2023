#include<algorithm>
#include<cstdlib>
#include<iostream>
#include<string>
#include<vector>
#include"point.h"
#include"utils.h"

int main(){

    // read input into vector of strings.
    std::vector<std::string> delimiters = {};
    std::vector<std::string> input = read_input("input", delimiters);

    // find all stars
    std::vector<point> stars, expandedStars1, expandedStars2;
    for (std::size_t y = 0; y < input.size(); y++){
        for (std::size_t x = 0; x < input[0].size(); x++){
            if (input[y][x] == '#'){ stars.emplace_back(x, y); }
        }
    }
    expandedStars1 = stars;
    expandedStars2 = stars;


    // expand galaxy horizontal
    for (std::size_t y = 0; y < input.size(); y++){
        auto& line = input[y];
        if (std::all_of(line.begin(), line.end(), [](char c){ return c == '.';})){
            for (std::size_t i = 0; i < stars.size(); i++){
                if (stars[i].y > static_cast<int>(y)){
                    expandedStars1[i].y += 1;
                    expandedStars2[i].y += 999'999;
                }
            }
        }
    }

    // expand galaxy vertical
    for (std::size_t x = 0; x < input[0].size(); x++){
        
        bool empty = true;
        for (std::size_t y = 0; y < input.size(); y++){
            if (input[y][x] != '.'){
                empty = false;
                break;
            }
        }

        if (empty){
            for (std::size_t i = 0; i < stars.size(); i++){
                if (stars[i].x > static_cast<int>(x)){
                    expandedStars1[i].x += 1;
                    expandedStars2[i].x += 999'999;
                }
            }
        }
    }

    long part1{0}, part2{0};
    
    // find manhattan distance between each pair of stars
    for (std::size_t i = 0; i < stars.size(); i++){
        for (std::size_t j = i + 1; j < stars.size(); j++){
            part1 += manhattan(expandedStars1[i], expandedStars1[j]);
            part2 += manhattan(expandedStars2[i], expandedStars2[j]);
        }
    }

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}
