#include<cctype>
#include<iostream>
#include<string>
#include<unordered_set>
#include<vector>
#include"math_func.h"
#include"point.h"
#include"utils.h"

bool is_digit(const char c);

int main(){

    // read grid into vector of strings.
    std::vector<std::string> delimiters = {};
    std::vector<std::string> grid = read_input("input", delimiters);

    pad(grid, '.');

    int width = static_cast<int>(grid[0].size());
    int height = static_cast<int>(grid.size());

    long part1{0};

    // for part2, remember all partNumbers linked to a gear
    std::unordered_map<point, std::vector<int>> gearNumbers;

    for (int y = 1; y < height-1; y++){
        for (int x = 1; x < width-1; x++){

            char current = grid[y][x];
            if (!is_digit(current)){ continue; }

            point start{x-1,y};

            // get full number
            std::string partNumber{current};
            while (is_digit(grid[y][++x])){
                partNumber.push_back(grid[y][x]);
            }

            point end{x,y};

            // get neighbours
            std::vector<point> neighbours = {start, end};
            while (start != (end + point{1,0})){
                neighbours.push_back(start + point{0,-1});
                neighbours.push_back(start + point{0,1});
                start += {1,0};
            }

            // check if the number is connected to a special character
            for (auto neighbour : neighbours){
                char check = grid[neighbour.y][neighbour.x];
                if (!is_digit(check) && check != '.'){
                    part1 += std::stoi(partNumber);

                    if (check == '*'){
                        gearNumbers[neighbour].push_back(std::stoi(partNumber));
                    }             
                }
            }
        }
    }

    long part2{0};
    for (auto [gear, numbers] : gearNumbers){
        if (numbers.size() == 2){
            part2 += numbers[0] * numbers[1];
        }
    }

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}

// custom isDigit that doesn't need unsigned char conversion like the std
bool is_digit(const char c){
    return c >= '0' && c <= '9';
}