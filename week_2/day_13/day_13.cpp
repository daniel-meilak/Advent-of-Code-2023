#include<algorithm>
#include<cstdlib>
#include<iostream>
#include<string>
#include<vector>
#include"utils.h"

int findMirror(const std::vector<std::string>& grid, int allowedDefects);
bool verticalCompare(const std::vector<std::string>& grid, std::size_t columnA, std::size_t columnB, int& allowedDefects);
bool horizontalCompare(const std::vector<std::string>& grid, std::size_t rowA, std::size_t rowB, int& allowedDefects);

int main(){

    // read input into vector of strings.
    std::vector<std::string> delimiters = {};
    std::vector<std::string> input = read_input("input", delimiters);

    std::vector<std::string> grid;

    int part1{0}, part2{0};
    for (const auto& line : input){
        if (line.empty()){
            part1 += findMirror(grid, 0);
            part2 += findMirror(grid, 1);
            grid.clear();
        }
        else { grid.push_back(line);}
    }
    part1 += findMirror(grid, 0);
    part2 += findMirror(grid, 1);

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}

int findMirror(const std::vector<std::string>& grid, int allowedDefects){

    // check horizontally
    for (int i = 1; i < static_cast<int>(grid.size()); i++){
        int defects = allowedDefects;
        bool foundMirror = true;

        int j = 1;
        while (i - j >= 0 && i + j - 1 < static_cast<int>(grid.size())){
            if (!horizontalCompare(grid, i - j, i + j -1, defects)){
                foundMirror = false;
                break;
            }

            j++;
        }

        if (foundMirror && defects == 0){ return 100 * i; }
    }

    

    // check vertically
    for (int i = 1; i < static_cast<int>(grid[0].size()); i++){
        int defects = allowedDefects;
        bool foundMirror = true;

        int j = 1;
        while (i - j >= 0 && i + j - 1 < static_cast<int>(grid[0].size())){
            if (!verticalCompare(grid, i-j, i + j - 1, defects)){
                foundMirror = false;
                break;
            }

            j++;
        }

        if (foundMirror && defects == 0){ return i; }
    }

    return 0;
}

bool verticalCompare(const std::vector<std::string>& grid, std::size_t columnA, std::size_t columnB, int& allowedDefects){
    int defects{0};
    
    for (std::size_t i = 0; i < grid.size(); i++){
        if (grid[i][columnA] != grid[i][columnB]){ defects++; }
        if (defects > allowedDefects){ return false; }
    }

    allowedDefects -= defects;
    return true;
}

bool horizontalCompare(const std::vector<std::string>& grid, std::size_t rowA, std::size_t rowB, int& allowedDefects){
    int defects{0};

    for (std::size_t i = 0; i < grid[0].size(); i++){
        if (grid[rowA][i] != grid[rowB][i]){ defects++; }
        if (defects > allowedDefects){ return false; }
    }

    allowedDefects -= defects;
    return true;
}