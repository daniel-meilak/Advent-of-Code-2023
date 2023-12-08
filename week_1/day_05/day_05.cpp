#include<climits>
#include<cstdlib>
#include<iostream>
#include<string>
#include<vector>
#include"utils.h"

struct Interval {
    long destinationStart;
    long sourceStart;
    long range;
};

struct Interval_map {

    // expects to recieve a vector of strings with 3 elements
    void insert(std::vector<std::string> input){
        intervals.emplace_back(std::stol(input[0]), std::stol(input[1]), std::stol(input[2]));
    }

    long operator[](long value) const{

        for (const auto& interval : intervals){
            if (value >= interval.sourceStart && value < interval.sourceStart + interval.range){
                return interval.destinationStart + (value - interval.sourceStart);
            }
        }

        return value;
    }

    std::vector<Interval> intervals;
};

void splitIntervals(const Interval& toSieve, const std::vector<Interval>& sieves, std::vector<Interval>& intervals);

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {" ", "seeds:"};
    std::vector<std::vector<std::string>> input = read_input_2D("input", delimiters);

    std::vector<long> seeds = input_to_int<long>(input[0]);

    std::size_t index{1};

    std::vector<Interval_map> maps;

    while (index < input.size()){

        // skip empty line & map name
        index += 2;
        
        Interval_map map;

        while (!input[index].empty()){
            map.insert(input[index]);
            index++;
        }

        maps.push_back(map);
    }

    auto find_location = [&maps](long input){ 
        for (auto map : maps){
            input = map[input];
        }

        return input;
    };

    // find smallest location
    long part1{LONG_MAX};
    for (auto seed : seeds){
        part1 = std::min(part1, find_location(seed));
    }

    // Interval A{0, 0, 7}, B{100, 0, 5};
    // std::vector<Interval> intervals;
    // splitIntervals(A, B, intervals);
    // for (auto interval : intervals){
    //     std::cout << interval.destinationStart << " " << interval.sourceStart << " " << interval.range << std::endl;
    // }
    
    // create intervals from seed positions and ranges
    std::vector<Interval> intervals;
    for (std::size_t i = 0; i < seeds.size(); i += 2){
        intervals.emplace_back(0, seeds[i], seeds[i+1]);
    }

    // pass intervals through maps and split as required
    for (auto map : maps){
        std::vector<Interval> newIntervals;
        for (auto interval : intervals){
            splitIntervals(interval, map.intervals, newIntervals);

            // // print all destinationStarts in intervals
            // for (auto interval : newIntervals){
            //     std::cout << interval.sourceStart << std::endl;
            // }
        }
        intervals = newIntervals;

        // print all destinationStarts in intervals
        for (auto interval : newIntervals){
            std::cout << interval.destinationStart << " " << interval.sourceStart << " " << interval.range << std::endl;
        }
        std::cout << std::endl;
    }



    // iterate through the last intervals and find the smallest location
    long part2{LONG_MAX};
    for (auto interval : intervals){
        part2 = std::min(part2, interval.sourceStart);
    }

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}

void splitIntervals(const Interval& toSieve, const std::vector<Interval>& sieves, std::vector<Interval>& intervals){
    std::vector<Interval> result;
    Interval temp = toSieve;

    for (std::size_t i = 0; i < sieves.size(); i++){
        for (std::size_t j = i + 1; j < sieves.size(); j++){
            if ((sieves[i].destinationStart >= sieves[j].destinationStart && sieves[i].destinationStart < sieves[j].destinationStart + sieves[j].range) || (sieves[j].destinationStart >= sieves[i].destinationStart && sieves[j].destinationStart < sieves[i].destinationStart + sieves[i].range)){
                std::cout << "problem here=========================================================================" << std::endl;   
            }
        }
    }

    for (auto sieve : sieves){

        // compare every pair of intervals in result for overlap
        for (std::size_t i = 0; i < result.size(); i++){
            for (std::size_t j = i + 1; j < result.size(); j++){
                if ((result[i].sourceStart >= result[j].sourceStart && result[i].sourceStart < result[j].sourceStart + result[j].range) || (result[j].sourceStart >= result[i].sourceStart && result[j].sourceStart < result[i].sourceStart + result[i].range)){
                    std::cout << "problem here<---------------------------------------------------------------------------" << std::endl;   
                }
            }
        }

        if (temp.sourceStart < sieve.sourceStart){
            // initial interval is split, need to continue sifting part of it 
            if (temp.sourceStart + temp.range > sieve.sourceStart){
                result.emplace_back(0, sieve.destinationStart, temp.sourceStart + temp.range - sieve.sourceStart);
                temp = Interval{0, temp.sourceStart, sieve.sourceStart - temp.sourceStart};
            }
        }
        else if (temp.sourceStart < sieve.sourceStart + sieve.range){
            if (temp.sourceStart + temp.range <= sieve.sourceStart + sieve.range){
                result.emplace_back(0, sieve.destinationStart + temp.sourceStart - sieve.sourceStart, temp.range);
                temp.range = 0;
                break; // fully sifted initial interval
            }
            else {
                // initial interval is split, need to continue sifting part of it
                result.emplace_back(0, sieve.destinationStart + temp.sourceStart - sieve.sourceStart, sieve.sourceStart + sieve.range - temp.sourceStart);
                temp = Interval{0, sieve.sourceStart + sieve.range, temp.sourceStart + temp.range - sieve.sourceStart - sieve.range};
            }
        }
        else { continue; }

        // for (auto r : result){
        //     std::cout << r.destinationStart << " " << r.sourceStart << " " << r.range << std::endl;
        // }
        // std::cout << std::endl;
    }

    // add leftover interval
    result.push_back(temp);

    // remove intervals of 0 range
    for (auto interval : result){
        if (interval.range != 0){
            intervals.push_back(interval);
        }
    }
}
