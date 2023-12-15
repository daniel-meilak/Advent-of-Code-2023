#include<cstdlib>
#include<iostream>
#include<set>
#include<string>
#include<tuple>
#include<unordered_map>
#include<vector>
#include"point.h"
#include"utils.h"

struct PairHash {
    size_t operator()(const std::vector<std::string>& input) const {
        std::hash<std::string> string_hasher;
        size_t seed{0};

        // Hash the strings in the vector
        for (const std::string& line : input) {
            seed ^= string_hasher(line) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }

        return seed;
    }
};

using TiltMap = std::unordered_map<std::vector<std::string>, std::pair<std::vector<std::string>, std::vector<point>>, PairHash>;
TiltMap tiltMap;

std::vector<point> directions = {point{0, -1}, point{-1, 0}, point{0, 1},  point{1, 0}};

void tiltInDirection(std::vector<std::string>& input, std::vector<point>& roundRocks, point direction);
int calculateLoad(const std::vector<point> roundRocks, std::size_t size);

int main(){

    // read input into vector of strings.
    std::vector<std::string> delimiters = {};
    std::vector<std::string> input = read_input("input", delimiters);

    pad(input, 'x');

    // find round rocks
    std::vector<point> roundRocks;

    for (std::size_t y = 1; y < input.size() - 1; y++){
        for (std::size_t x = 1; x < input[0].size() - 1; x++){
            if (input[y][x] == 'O'){
                roundRocks.push_back(point{static_cast<int>(x), static_cast<int>(y)});
            }
        }
    }

    // roll rocks
    tiltInDirection(input, roundRocks, point{0, -1});

    // calculate load
    int part1 = calculateLoad(roundRocks, input.size());

    // get n iterations before loop
    bool found_loop{false};
    std::size_t i{1};
    while (!found_loop){
        tiltInDirection(input, roundRocks, directions[i++ % 4]);
        if (tiltMap.contains(input)){ found_loop = true; }
    }

    // get loop length
    std::size_t j{0};
    auto inputCopy = input;
    do {
        input = tiltMap[input].first;
        j++;
    } while (inputCopy != input);

    // get to billionth cycle
    std::size_t offset = (4'000'000'000 - i) % j;
    for (std::size_t k = 0; k < offset; k++){
        std::tie(input, roundRocks) = tiltMap[input];
    }

    int part2 = calculateLoad(roundRocks, input.size());

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}

void tiltInDirection(std::vector<std::string>& input, std::vector<point>& roundRocks, point direction){
    
    auto inputCopy = input;
    if (tiltMap.contains(input)){
        input = tiltMap[input].first;
        roundRocks = tiltMap[input].second;
        return;
    }

    bool (*compare)(point, point) = nullptr;
    if (direction == point{0, -1}){ compare = [](point a, point b){ return a.y < b.y; }; }
    else if (direction == point{0, 1}){ compare = [](point a, point b){ return a.y > b.y; }; }
    else if (direction == point{-1, 0}){ compare = [](point a, point b){ return a.x < b.x; }; }
    else if (direction == point{1, 0}){ compare = [](point a, point b){ return a.x > b.x; }; }

    // sort rocks
    std::sort(roundRocks.begin(), roundRocks.end(), compare);

    for (auto& rock : roundRocks){
        point newPos = rock + direction;

        while (input[newPos.y][newPos.x] == '.'){ newPos += direction; }
        newPos -= direction;

        input[rock.y][rock.x] = '.';
        input[newPos.y][newPos.x] = 'O';
        rock = newPos;
    }

    tiltMap[inputCopy] = std::make_pair(input, roundRocks);
}

int calculateLoad(const std::vector<point> roundRocks, std::size_t size){
    int result{0};
    for (const auto& rock : roundRocks){
        result += static_cast<int>(size - rock.y - 1);
    }

    return result;
}