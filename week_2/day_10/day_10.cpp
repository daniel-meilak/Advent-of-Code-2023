#include<cstdlib>
#include<iostream>
#include<list>
#include<string>
#include<unordered_set>
#include<utility>
#include<vector>
#include"point.h"
#include"utils.h"

enum dir {up, down, left, right};

int main(){

    // read input into vector of strings.
    std::vector<std::string> delimiters = {};
    std::vector<std::string> input = read_input("input", delimiters);

    pad(input, '.');

    // find the start
    point start;
    for (std::size_t y = 0; y < input.size(); y++){
        if (auto pos = input[y].find('S'); pos != std::string::npos){
            start = point{static_cast<int>(pos), static_cast<int>(y)};
            break;
        }
    }

    // each next position is a point and the direction from which it was entered
    std::list<std::pair<point, dir>> toExplore;
    int startType{0}; // 4 bits up, down, left, right

    // find the connected pipes
    if (char up = input[start.y-1][start.x]; up == '|' || up == '7' || up == 'F'){
        toExplore.emplace_back(start.up(), dir::up);
        startType += 8;
    }
    if (char down = input[start.y+1][start.x]; down == '|' || down == 'J' || down == 'L'){
        toExplore.emplace_back(start.down(), dir::down);
        startType += 4;
    }
    if (char left = input[start.y][start.x-1]; left == '-' || left == 'F' || left == 'L'){
        toExplore.emplace_back(start.left(), dir::left);
        startType += 2;
    }
    if (char right = input[start.y][start.x+1]; right == '-' || right == 'J' || right == '7'){
        toExplore.emplace_back(start.right(), dir::right);
        startType += 1;
    }

    // replace the start with appropriate pipe
    switch (startType){
    case 12: input[start.y][start.x] = '|'; break; // 1100
    case 10: input[start.y][start.x] = 'J'; break; // 1010
    case  9: input[start.y][start.x] = 'L'; break; // 1001
    case  6: input[start.y][start.x] = '7'; break; // 0110
    case  5: input[start.y][start.x] = 'F'; break; // 0101
    default: input[start.y][start.x] = '-'; break; // 0011
    }

    std::unordered_set<point> visited{start};


    int totalMoves{1};
    while (!toExplore.empty()){
        std::pair<point,dir> current = toExplore.front();
        toExplore.pop_front();

        if (visited.contains(current.first)){ break; }

        visited.insert(current.first);

        switch (input[current.first.y][current.first.x]){
        case '|':
            if (current.second == dir::up){ toExplore.emplace_back(current.first.up(), dir::up); }
            else { toExplore.emplace_back(current.first.down(), dir::down); }
            break;
        case '-':
            if (current.second == dir::left){ toExplore.emplace_back(current.first.left(), dir::left); }
            else { toExplore.emplace_back(current.first.right(), dir::right); }
            break;
        case 'L':
            if (current.second == dir::left){ toExplore.emplace_back(current.first.up(), dir::up); }
            else { toExplore.emplace_back(current.first.right(), dir::right); }
            break;
        case 'J':
            if (current.second == dir::right){ toExplore.emplace_back(current.first.up(), dir::up); }
            else { toExplore.emplace_back(current.first.left(), dir::left); }
            break;
        case '7':
            if (current.second == dir::up){ toExplore.emplace_back(current.first.left(), dir::left); }
            else { toExplore.emplace_back(current.first.down(), dir::down); }
            break;
        default: // 'F'
            if (current.second == dir::up){ toExplore.emplace_back(current.first.right(), dir::right); }
            else { toExplore.emplace_back(current.first.down(), dir::down); }
            break;
        }

        totalMoves++;
    }

    // when we hit the same position twice, we have found the end
    int part1 = totalMoves/2;

    int part2{0};
    for (std::size_t y = 1; y < input.size()-1; y++){
        
        bool inside{false};
        char lastTurn;
        for (std::size_t x = 1; x < input[0].size()-1; x++){
            point current{static_cast<int>(x), static_cast<int>(y)};
            char type = input[y][x];

            if (visited.contains(current)){
                switch (type){
                case '|':
                    inside = !inside;
                    break;
                case 'L':
                case 'F':
                    lastTurn = type;
                    break;
                case '7':
                    // an L followed by a 7 is equavalent to passing a |
                    if (lastTurn == 'L'){ inside = !inside; }
                    break;
                case 'J':
                    // an F followed by a J is equavalent to passing a |
                    if (lastTurn == 'F'){ inside = !inside; }
                    break;
                default:
                    break;
                }   
            }
            else if (inside){ part2++; }
        }
    }

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}
