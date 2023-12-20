#include<cstdlib>
#include<iostream>
#include<queue>
#include<string>
#include<tuple>
#include<unordered_set>
#include<vector>
#include"hash.h"
#include"point.h"
#include"utils.h"

using State = std::tuple<int,point,point>;

struct stateCompare {
    bool operator()(const State& left, const State& right){
        return std::get<0>(left) > std::get<0>(right);
    }
};

std::vector<point> directions(point& p);

int leastHeatLoss(const std::vector<std::string>& input, int minLine, int maxLine);

int main(){

    // read input into vector of strings.
    std::vector<std::string> delimiters = {};
    std::vector<std::string> input = read_input("input", delimiters);

    int part1 = leastHeatLoss(input, 1, 3);
    int part2 = leastHeatLoss(input, 4, 10);    

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}

int leastHeatLoss(const std::vector<std::string>& input, int minLine, int maxLine){
    int width = static_cast<int>(input[0].size());
    int height = static_cast<int>(input.size());

    point start{0,0};
    point end{width-1, height-1};

    std::unordered_set<std::pair<point,point>, PairHash<point,point>> visited;

    std::priority_queue<State, std::vector<State>, stateCompare> frontier;
    frontier.push(std::make_tuple(0, start, point{1,0}));
    frontier.push(std::make_tuple(0, start, point{0,1}));

    while (!frontier.empty()){
        auto [heatLoss, position, lastDirection] = frontier.top();
        frontier.pop();

        if (position == end){return heatLoss; }

        if (visited.contains({position, lastDirection})){ continue; }
        visited.insert({position, lastDirection});

        for (auto& direction : directions(lastDirection)){
            
            
            for (int i = minLine; i <= maxLine; i++){
                point next = position + direction * i;

                if (next.x < 0 || next.x >= width || next.y < 0 || next.y >= height){ break; }
                
                int nextLoss{0};
                for (int j = 1; j <= i; j++){
                    point p = position + direction * j;
                    nextLoss += input[p.y][p.x] - '0';
                }

                frontier.push(std::make_tuple(heatLoss + nextLoss, next, direction));
            }
        }
    }

    return 0;
}

std::vector<point> directions(point& p){
    return {{p.y, -p.x}, {-p.y, p.x}};
}