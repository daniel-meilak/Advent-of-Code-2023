#include<algorithm>
#include<iostream>
#include<string>
#include<string_view>
#include<unordered_map>
#include<vector>
#include"utils.h"

std::unordered_map<std::string_view, int> digitMap = {
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9}
};

std::vector<std::string_view> numbers = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

bool isDigit(const char c);
int findSpelledDigit(std::string::const_iterator it);
int findSpelledDigit(std::string::const_reverse_iterator it);
int findAllDigits(const std::string& line);

int main(){

    // read input into vector of strings.
    std::vector<std::string> delimiters = {};
    std::vector<std::string> input = read_input("input", delimiters);

    long part1{0};
    long part2{0};
    for (auto &line : input){
        
        // first & last digit
        int first = *std::find_if(line.begin(), line.end(), isDigit) - '0';
        int last  = *std::find_if(line.rbegin(), line.rend(), isDigit) - '0';

        part1 += first * 10 + last;

        // first & last spelled out digit
        part2 += findAllDigits(line);
    }


    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}

// custom isDigit that doesn't need unsigned char conversion like the std
bool isDigit(const char c){
    return c >= '1' && c <= '9';
}

// check if iterator points to a spelled out digit
int findSpelledDigit(std::string::const_iterator it){
    for (auto number : numbers){

        // std::equal checks if the range [number.begin(), number.end()) is equal to the range [it, it + number.size())
        if (std::equal(number.begin(), number.end(), it)){
            return digitMap[number];
        }
    }

    return 0;
}

int findSpelledDigit(std::string::const_reverse_iterator it){
    for (auto number : numbers){
        if (std::equal(number.rbegin(), number.rend(), it)){
            return digitMap[number];
        }
    }

    return 0;
}

// find first and last digit in a string (spelled out or not)
int findAllDigits(const std::string& line){
    int first{0}, last{0};

    for (auto it = line.begin(); it != line.end(); ++it){
        if (isDigit(*it)){
            first = *it - '0';
            break;
        }

        first = findSpelledDigit(it);
        if (first){ break; }
    }

    for (auto it = line.rbegin(); it != line.rend(); ++it){
        if (isDigit(*it)){
            last = *it - '0';
            break;
        }

        last = findSpelledDigit(it);
        if (last){ break; }
    }

    return first * 10 + last;
}