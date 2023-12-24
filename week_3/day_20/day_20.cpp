#include<algorithm>
#include<cstdlib>
#include<iostream>
#include<list>
#include<memory>
#include<string>
#include<tuple>
#include<unordered_map>
#include<vector>
#include"utils.h"

enum Pulse { low, high };

long lowPulses{0}, highPulses{0};

struct Module {
    std::string name;
    std::vector<std::string> connections;
    std::unordered_map<std::string, Pulse> memory;

    Module(const std::string& name): name(name) {}

    virtual void receivePulse(Pulse pulse, const std::string&){
        incrementPulses(pulse);
    };

    void incrementPulses(Pulse pulse){
        pulse == Pulse::high ? highPulses++ : lowPulses++;
    }
};

std::unordered_map<std::string, std::shared_ptr<Module>> modules;

std::list<std::tuple<std::string,std::string,Pulse>> pulseQueue;

struct FlipFlop : Module {
    int state{0};
    
    FlipFlop(const std::string& name) : Module(name) {}

    void receivePulse(Pulse pulse, const std::string&) override {
        incrementPulses(pulse);
        
        if (pulse == Pulse::high){ return; }

        state = !state;
        sendPulse();
    }

    void sendPulse(){
        for (const auto& connection : connections){
            pulseQueue.push_back(std::make_tuple(name, connection, state ? Pulse::high : Pulse::low));
        }
    };
};

struct Conjunction : Module {
    Conjunction(const std::string& name) : Module(name) {}

    void receivePulse(Pulse pulse, const std::string& name) override {
        incrementPulses(pulse);
        memory[name] = pulse;
        sendPulse();
    }

    void sendPulse(){
        bool allHigh = std::ranges::all_of(memory, [](const auto& m){ return m.second == Pulse::high; });

        for (const auto& connection : connections){
            pulseQueue.push_back(std::make_tuple(name, connection, allHigh ? Pulse::low : Pulse::high));
        }
    };
};

struct Broadcaster : Module {

    Broadcaster(const std::string& name) : Module(name) {}

    void receivePulse(Pulse pulse, const std::string&){
        incrementPulses(pulse);
        sendPulse(pulse);
    }

    void sendPulse(Pulse pulse){
        for (const auto& connection : connections){
            pulseQueue.push_back(std::make_tuple(name, connection, pulse));
        }
    };
};

void processPulses(const std::string& origin);

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {"->", " ", ","};
    std::vector<std::vector<std::string>> input = read_input_2D("input", delimiters);

    std::vector<std::shared_ptr<FlipFlop>> flipFlops;

    for (auto& line : input){
        std::string& name{line[0]};
        
        switch (name[0]){
        case '%':
            name.erase(0,1);
            flipFlops.push_back(std::make_shared<FlipFlop>(name));
            modules[name] = flipFlops.back();
            break;
        case '&':
            name.erase(0,1);
            modules[name] = std::make_shared<Conjunction>(name);
            break;      
        default:
            if (name == "broadcaster"){ modules[name] = std::make_shared<Broadcaster>(name); }
            else { modules[name] = std::make_shared<Module>(name); }
            break;
        }

        for (auto it = line.begin() + 1; it != line.end(); it++){
            if (!modules.contains(*it)){ modules[*it] = std::make_shared<Module>(*it); }
            modules[name]->connections.push_back(*it);
        }
    }

    // get inputs for conjunctions
    for (const auto& line : input){
        for (auto it = line.begin() + 1; it != line.end(); it++){
            modules[*it]->memory.emplace(line[0], Pulse::low);
        }
    }    
    
    long part1{0};
    for (int buttonPresses = 0; buttonPresses < 1000; buttonPresses++){ processPulses("broadcaster"); }
    part1 = lowPulses * highPulses;

    std::unordered_map<std::string, long> counts;
    for (const auto& module : modules["broadcaster"]->connections){

        for (auto& flipFlop : flipFlops){ flipFlop->state = 0; }
        
        long presses{0};
        while (true){
            presses++;
            processPulses(module);
            
            if (std::ranges::all_of(flipFlops, [](const auto& f){ return f->state == 0; })){
                counts[module] = presses;
                break;
            }
        }
    }

    long part2{1};
    for (const auto& [_, count] : counts){ part2 *= count; }

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}

void processPulses(const std::string& origin){
    pulseQueue.push_back(std::make_tuple("", origin, Pulse::low));

    while (!pulseQueue.empty()){
        auto [sender, receiver, pulse] = pulseQueue.front();
        pulseQueue.pop_front();
        modules[receiver]->receivePulse(pulse, sender);
    }
}