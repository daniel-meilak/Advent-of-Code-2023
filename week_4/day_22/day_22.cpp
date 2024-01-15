#include<algorithm>
#include<climits>
#include<cstdlib>
#include<functional>
#include<iostream>
#include<list>
#include<string>
#include<tuple>
#include<unordered_map>
#include<unordered_set>
#include<vector>
#include"hash.h"
#include"utils.h"

struct Brick {
    int xMin, yMin, zMin;
    int xMax, yMax, zMax;

    std::vector<std::tuple<int,int,int>> coordinates{};
    std::unordered_set<Brick*> restsOn{}, supports{};

    bool stationary{false}, removeable{true};
};

int countCollapse(Brick& brick);

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {",","~"};
    std::vector<std::vector<int>> input = input_to_int_2D(read_input_2D("input", delimiters));

    std::vector<Brick> bricks;

    for (const auto& line : input){
        Brick brick(line[0], line[1], line[2], line[3], line[4], line[5]);
        bricks.push_back(brick);
    }

    // sort by height
    std::ranges::sort(bricks, std::less(), &Brick::zMin);

    // break down bricks into coordinates
    for (auto& brick : bricks){
        // std::cout << brick.zMin << std::endl;
        for (int x = brick.xMin; x <= brick.xMax; x++){
            for (int y = brick.yMin; y <= brick.yMax; y++){
                for (int z = brick.zMin; z <= brick.zMax; z++){
                    brick.coordinates.push_back(std::make_tuple(x,y,z));
                }
            }
        }
    }

    // look up of each block location for collisions
    std::unordered_map<std::tuple<int,int,int>, Brick*, TupleHash<int,int,int>> blocks;

    // find resting position for each brick
    for (auto& brick : bricks){
        while (brick.stationary == false){
            // check if already at bottom
            if (brick.zMin == 0){ brick.stationary = true; break;}

            // move brick down
            for (auto& coordinate : brick.coordinates){ std::get<2>(coordinate)--; }
            brick.zMin--;

            // check if brick is resting
            for (auto& coordinate : brick.coordinates){
                if (blocks.contains(coordinate)){
                    brick.stationary = true;

                    blocks[coordinate]->supports.insert(&brick);
                    brick.restsOn.insert(blocks[coordinate]);
                }
            }

            // if brick is resting, move it back up
            if (brick.stationary){
                for (auto& coordinate : brick.coordinates){ std::get<2>(coordinate)++; }
                brick.zMin++;
            }
        }

        for (const auto& coordinate : brick.coordinates){ blocks.emplace(coordinate, &brick); }
    }

    std::unordered_map<const Brick*, int> cache{};

    int part1{0}, part2{0};
    for (auto& brick : bricks){
        for (const auto& supports : brick.supports){
            if (supports->restsOn.size() == 1){ brick.removeable = false; }
        }

        if (brick.removeable){ part1++; }
        else {
            if (!cache.contains(&brick)){ part2 += countCollapse(brick); }
        }
    }

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}

int countCollapse(Brick& brick){

    std::unordered_set<Brick*> collapsed{&brick};

    std::list<Brick*> queue;
    for (auto& supports : brick.supports){ queue.push_back(supports); }

    while (!queue.empty()){
        Brick* current = queue.front();
        queue.pop_front();

        if (std::ranges::all_of(current->restsOn, [&collapsed](Brick* brick){ return collapsed.contains(brick); })){
            collapsed.insert(current);
            for (auto& supports : current->supports){
                if (std::ranges::find(queue, supports) == queue.end()){ queue.push_back(supports); }
            }
        }
    }

    return static_cast<int>(collapsed.size() - 1);
}