#include<algorithm>
#include<cstdlib>
#include<iostream>
#include<list>
#include<string>
#include<unordered_map>
#include<unordered_set>
#include<vector>
#include"hash.h"
#include"point.h"
#include"utils.h"

using Step = std::pair<point,int>;

int main(){

    // read input into vector of strings.
    std::vector<std::string> delimiters = {};
    std::vector<std::string> input = read_input("input", delimiters);

    int width = static_cast<int>(input[0].size());
    int height = static_cast<int>(input.size());

    auto S = find_in_grid(input, 'S');
    point start = {static_cast<int>(S.first), static_cast<int>(S.second)};

    // precompute all neighbours
    std::unordered_map<point,std::vector<point>> neighbours; 
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            
            
            std::vector<point> next, directions = {dir::up, dir::down, dir::left, dir::right};
            for (const auto& direction : directions){
                next.emplace_back(mod(x + direction.x, width - 1), mod(y + direction.y, height - 1));
            }

            for (int i = 0; i < 4; i++){
                if (input[next[i].y][next[i].x] != '#'){ neighbours[{x,y}].push_back(directions[i]); }
            }
        }
    }

    int maxDistance{start.x + (width * 2)};

    // possible positions at each step
    std::vector<std::unordered_set<point>> allPossiblePositions(maxDistance + 1);
    allPossiblePositions[0].insert(start);

    for (int i = 1; i < maxDistance + 1; i++){
        for (const auto& position : allPossiblePositions[i-1]){

            point reduced = {mod(position.x,width), mod(position.y,height)};

            for (const auto& next : neighbours[reduced]){
                allPossiblePositions[i].insert(position + next);
            }
        }
    }

    std::size_t part1 = allPossiblePositions[64].size();

    long goal{26501365};
    long a0, a1, a2, b0, b1, b2, n;
    
    a0 = allPossiblePositions[start.x].size();
    a1 = allPossiblePositions[start.x + width].size();
    a2 = allPossiblePositions[start.x + (width * 2)].size();

    b0 = a0;
    b1 = a1-a0;
    b2 = a2-a1;
    n = goal / width;
    
    long part2 = b0 + (b1 * n) + (n * (n - 1)/2) * (b2-b1);

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}