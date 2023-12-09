#include<cstdlib>
#include<functional>
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include"utils.h"

bool withJokers{false};

enum HandType {HighCard, OnePair, TwoPair, ThreeOfAKind, FullHouse, FourOfAKind, FiveOfAKind};

int getCardCost(char card);
std::map<int,int> getHand(const std::string& hand);
HandType getHandType(const std::map<int, int>& hand);

struct handSort {
    bool operator()(const std::vector<std::string>& left, const std::vector<std::string>& right) const {
        
        std::map<int, int> leftHand{getHand(left[0])}, rightHand{getHand(right[0])};
        HandType leftHandType{getHandType(leftHand)}, rightHandType{getHandType(rightHand)};

        if (leftHandType != rightHandType){
            return leftHandType < rightHandType;
        }
        else {
            int i{0};
            while (left[0][i] == right[0][i]){ i++; }
            return getCardCost(left[0][i]) < getCardCost(right[0][i]);
        }
    }
};

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {" "};
    std::vector<std::vector<std::string>> input = read_input_2D("input", delimiters);

    std::sort(input.begin(), input.end(), handSort());

    int part1{0};
    for (std::size_t i = 0; i < input.size(); i++){
        part1 += (static_cast<int>(i) + 1) * std::stoi(input[i][1]);
    }

    int part2{0};
    withJokers = true;
    std::sort(input.begin(), input.end(), handSort());

    for (std::size_t i = 0; i < input.size(); i++){
        part2 += (static_cast<int>(i) + 1) * std::stoi(input[i][1]);
    }

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}

HandType getHandType(const std::map<int, int>& hand){

    if (withJokers && hand.contains(1)){
        switch (hand.size()){
        case 1:
        case 2:
            return HandType::FiveOfAKind;
        case 3:
            if (hand.at(1) == 1 && (hand.begin()->second == 2 || (++hand.begin())->second == 2)){ return HandType::FullHouse; }
            else{ return HandType::FourOfAKind; }
        case 4:
            return HandType::ThreeOfAKind;
        case 5:
            return HandType::OnePair;
        default:
            std::cout << "Error: hand size is not between 1 and 5.\n";
            exit(1);
        }
    }
    else {
        switch (hand.size()){
        case 1:
            return HandType::FiveOfAKind;
        case 2:
            if (hand.begin()->second == 1 || hand.begin()->second == 4){ return HandType::FourOfAKind; }
            else{ return HandType::FullHouse; }
        case 3:
            for (const auto& [_, count] : hand){ if (count == 3){ return HandType::ThreeOfAKind; }}
            return HandType::TwoPair;
        case 4:
            return HandType::OnePair;
        case 5:
            return HandType::HighCard;
        default:
            std::cout << "Error: hand size is not between 1 and 5.\n";
            exit(1);
        }
    }
    
}

std::map<int,int> getHand(const std::string& hand){
    std::map<int, int> handMap;

    for (const auto& card : hand){
        handMap[getCardCost(card)]++;
    }

    return handMap;
}

int getCardCost(char card){
    if (card == 'T'){ return 10; }
    else if (card == 'J'){ return withJokers ? 1 : 11; }
    else if (card == 'Q'){ return 12; }
    else if (card == 'K'){ return 13; }
    else if (card == 'A'){ return 14; }
    else{ return card - '0'; }
}