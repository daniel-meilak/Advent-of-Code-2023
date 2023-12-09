#include<algorithm>
#include<cstdlib>
#include<iostream>
#include<string>
#include<vector>
#include"utils.h"

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {" "};
    std::vector<std::vector<int>> input = input_to_int_2D(read_input_2D("input", delimiters));

    auto allZero= [](int i){
        return i == 0;
    };

    long part1{0}, part2{0};
    for (auto& line : input){
        std::vector<std::vector<int>> series{{line}};

        while (!std::ranges::all_of(series.back(), allZero)){
            std::vector<int> nextSeries;

            for (std::size_t i = 1; i < series.back().size(); i++){
                nextSeries.push_back(series.back()[i] - series.back()[i-1]);
            }

            series.push_back(nextSeries);
        }

        std::size_t i = series.size() - 1;
        while (i-- != 0){
            series[i].push_back(series[i].back() + series[i+1].back());
            series[i].insert(series[i].begin(), series[i][0] - series[i+1][0]);
        }

        part1 += series[0].back();
        part2 += series[0][0];
    }

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}

