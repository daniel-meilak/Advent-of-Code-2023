#include<algorithm>
#include<cstdlib>
#include<iostream>
#include<list>
#include<map>
#include<string>
#include<set>
#include<utility>
#include<vector>
#include"point.h"
#include"utils.h"

int main(){

    // read input into vector of strings.
    std::vector<std::string> delimiters = {};
    std::vector<std::string> input = read_input("input", delimiters);

    int width  = static_cast<int>(input[0].size());
    int height = static_cast<int>(input.size());

    std::map<std::pair<point, point>, std::size_t> starts;

    // insert all starting positions
    for (int x = 0; x < width; x++){
        starts.insert({{point{x, 0}, point{0, 1}}, 0});
        starts.insert({{point{x, height - 1}, point{0,-1}}, 0});
    }

    for (int y = 0; y < height; y++){
        starts.insert({{point{0, y}, point{1, 0}}, 0});
        starts.insert({{point{width - 1, y}, point{-1, 0}}, 0});
    }

    for (auto& [startingPosition, energised] : starts){
        // positions to explore, made up of position and direction being faced
        std::list<std::pair<point, point>> next = {startingPosition};

        std::set<std::pair<point, point>> visited;

        while (!next.empty()){
            auto current = next.front();
            next.pop_front();

            point& position = current.first;
            point& direction = current.second;

            if (visited.contains(current)){ continue; }

            // check if in bounds
            if (position.x < 0 || position.y < 0){ continue; }
            if (position.x >= width || position.y >= height ){ continue; }

            visited.insert(current);

            switch (input[position.y][position.x]){
            case '.':
                next.push_back({position + direction, direction});
                break;
            case '/':
                if      (direction == point{-1, 0}){ next.push_back({position + point{ 0, 1}, point{ 0, 1}}); }
                else if (direction == point{ 1, 0}){ next.push_back({position + point{ 0,-1}, point{ 0,-1}}); }
                else if (direction == point{ 0,-1}){ next.push_back({position + point{ 1, 0}, point{ 1, 0}}); }
                else if (direction == point{ 0, 1}){ next.push_back({position + point{-1, 0}, point{-1, 0}}); }
                break;
            case '\\':
                if      (direction == point{-1, 0}){ next.push_back({position + point{ 0,-1}, point{ 0,-1}}); }
                else if (direction == point{ 1, 0}){ next.push_back({position + point{ 0, 1}, point{ 0, 1}}); }
                else if (direction == point{ 0,-1}){ next.push_back({position + point{-1, 0}, point{-1, 0}}); }
                else if (direction == point{ 0, 1}){ next.push_back({position + point{ 1, 0}, point{ 1, 0}}); }
                break;
            case '|':
                if (direction.x == 0){ next.push_back({position + direction, direction}); }
                else {
                    next.push_back({position + point{0, 1}, point{0, 1}});
                    next.push_back({position + point{0,-1}, point{0,-1}});
                }
                break;
            case '-':
                if (direction.y == 0){ next.push_back({position + direction, direction}); }
                else {
                    next.push_back({position + point{ 1,0}, point{ 1,0}});
                    next.push_back({position + point{-1,0}, point{-1,0}});
                }
                break;
            }
        }

        std::set<point> uniquePositions;
        for (auto& [position, _] : visited){ uniquePositions.insert(position); }

        energised = uniquePositions.size();
    }

    std::size_t part1 = starts[std::make_pair(point{0, 0}, point{1, 0})];

    // get max energised
    auto [_, part2] = *std::ranges::max_element(starts, std::less{}, [](auto& pair){ return pair.second; });

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}
