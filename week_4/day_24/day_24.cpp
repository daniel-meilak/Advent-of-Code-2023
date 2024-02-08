#include<array>
#include<cstdlib>
#include<iostream>
#include<string>
#include<tuple>
#include<utility>
#include<vector>
#include"utils.h"

using point = std::array<long long,3>;

long long minRange = 200000000000000;
long long maxRange = 400000000000000;

point operator+(const point& x, const point& y){
    return {x[0]+y[0], x[1]+y[1], x[2]+y[2]};
}

point operator-(const point& x, const point& y){
    return {x[0]-y[0], x[1]-y[1], x[2]-y[2]};
}

struct Line {
    point origin;
    point velocity; 
};

std::pair<bool,point> findIntersection(const Line& left, const Line& right);
std::pair<bool,point> findCommonIntersection(const std::vector<Line>& lines, point rockVelocity);
bool inBounds(point intersection);
bool inFuture(const Line& left, const Line& right, point intersection);
long long intersectAllLines(const std::vector<Line>& lines);

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {",", " ", "@"};
    std::vector<std::vector<std::string>> input = read_input_2D("input", delimiters);

    std::vector<Line> lines;
    for (const auto& line : input){
        Line temp;
        for (std::size_t i = 0; i < 3; i++){ temp.origin[i] = std::stoll(line[i]); }
        for (std::size_t i = 3; i < 6; i++){ temp.velocity[i-3] = std::stoll(line[i]); }
        lines.emplace_back(temp);
    }

    long long part1 = intersectAllLines(lines);

    // find a rock velocity such that all hailstone velocities minus the rock velocity intersect at the same point
    // we can do this by brute force ignoring the z-component of the hailstone velocities
    bool allIntersect{false};
    point commonIntersection, rockVelocity{-500,-500,0};

    auto increment = [&rockVelocity](){ rockVelocity[1] < 500 ? rockVelocity[1]++ : (rockVelocity[1] = -500, rockVelocity[0]++); };

    while (!allIntersect){
        increment();
        std::tie(allIntersect, commonIntersection) = findCommonIntersection(lines, rockVelocity);
    }

    // find the z-component of the rock velocity
    long long t1 = (commonIntersection[0] - lines[0].origin[0])/(lines[0].velocity[0] - rockVelocity[0]);
    long long t2 = (commonIntersection[0] - lines[1].origin[0])/(lines[1].velocity[0] - rockVelocity[0]);    

    rockVelocity[2] = (lines[0].origin[2] - lines[1].origin[2] + t1*lines[0].velocity[2] - t2*lines[1].velocity[2])/(t1 - t2);

    // find the z-component of the commonIntersection
    long long t = (commonIntersection[0] - lines[0].origin[0])/(lines[0].velocity[0] - rockVelocity[0]);
    commonIntersection[2] = lines[0].origin[2] + t*(lines[0].velocity[2] - rockVelocity[2]);

    long long part2 = commonIntersection[0] + commonIntersection[1] + commonIntersection[2];

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}


std::pair<bool,point> findIntersection(const Line& left, const Line& right){    
    long long det = left.velocity[0] * right.velocity[1] - left.velocity[1] * right.velocity[0];

    if (det == 0) {
        // The lines are parallel in x,y
        return {false, {}};
    }
    else {
        long long t = (right.velocity[1] * (right.origin[0] - left.origin[0]) + right.velocity[0] * (left.origin[1] - right.origin[1])) / det;
        return {true, {left.origin[0] + t * left.velocity[0], left.origin[1] + t * left.velocity[1], 0}};
    }
}

bool inBounds(point intersection){
    auto inRange = [](long long value){ return value >= minRange && value <= maxRange; };
    return inRange(intersection[0]) && inRange(intersection[1]);
}

bool inFuture(const Line& left, const Line& right, point intersection){
    // find lines form intersection to origin of each line
    point leftLine = intersection - left.origin;
    point rightLine = intersection - right.origin;

    long long leftDot = leftLine[0]*left.velocity[0]+leftLine[1]*left.velocity[1];
    long long rightDot = rightLine[0]*right.velocity[0]+rightLine[1]*right.velocity[1];

    return leftDot > 0 && rightDot > 0;
}

long long intersectAllLines(const std::vector<Line>& lines){
    long long count{0};
    
    for (std::size_t i = 0; i < lines.size(); i++){
        Line A = lines[i];

        for (std::size_t j = i+1; j < lines.size(); j++){    
            Line B = lines[j];

            auto [intersect, intersection] = findIntersection(A, B);

            if (intersect && inBounds(intersection) && inFuture(A, B, intersection)){ count++; }   
        }
    }

    return count;
}

std::pair<bool,point> findCommonIntersection(const std::vector<Line>& lines, point rockVelocity){
    bool startComparison{true};
    point commonIntersection;

    for (std::size_t i = 0; i < lines.size(); i++){
        Line A = lines[i];
        A.velocity = A.velocity - rockVelocity;

        for (std::size_t j = i+1; j < lines.size(); j++){    
            Line B = lines[j];
            B.velocity = B.velocity - rockVelocity;

            auto [intersect, intersection] = findIntersection(A, B);

            if (startComparison){
                commonIntersection = intersection;
                startComparison = false;
            }
            else if (intersect && intersection != commonIntersection){
                return {false, {}};
            }
        }
    }

    return {true, commonIntersection};
}