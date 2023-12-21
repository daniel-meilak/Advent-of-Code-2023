#include<cstdlib>
#include<iostream>
#include<string>
#include<vector>
#include"utils.h"

template <typename Input, typename GetLength, typename GetDirection>
long calcArea(Input input, GetLength getLength, GetDirection getDirection){

    double area{1.0}, x{0.0};
    for (const auto& line : input){

        int length = getLength(line);

        int i{0}, j{0};
        switch (getDirection(line)){
        case '3': case 'U': j = -1; break;
        case '1': case 'D': j =  1; break;
        case '2': case 'L': i = -1; break;
        case '0': case 'R': i =  1; break;
        }

        x += i * length;
        area += (j * length * x) + length/2.0; 
    }

    return static_cast<long>(area);
}

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {" ", "(", ")", "#"};
    std::vector<std::vector<std::string>> input = read_input_2D("input", delimiters);

    auto length1 = [](const std::vector<std::string>& line){ return std::stoi(line[1]); };
    auto length2 = [](const std::vector<std::string>& line){ return std::stoi(line[2].substr(0,5), nullptr, 16); };

    auto direction1 = [](const std::vector<std::string>& line){ return line[0][0]; };
    auto direction2 = [](const std::vector<std::string>& line){ return line[2].back(); };

    long part1 = calcArea(input, length1, direction1);
    long part2 = calcArea(input, length2, direction2);

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}
