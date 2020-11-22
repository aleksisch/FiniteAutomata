//
// Created by aleksey on 01.10.2020.
//

#ifndef ALGO_AUTOMATA_H
#define ALGO_AUTOMATA_H

#include <iostream>
#include <vector>
#include <set>

struct edge {
    edge(int from = 0, int to = 0, char letter = 0);
    bool operator < (const edge& other) const;

    int from;
    int to;
    char letter;
};

class Automata {
public:
    Automata(std::vector<std::vector<edge>> edges,
             std::set<int> finishState, int start,
             std::vector<char> sigma);


    Automata() = default;
    int AddEdge(edge new_edge);
    int AddVertex();
    void CreateDKA();
    void Minimize();
    void Inverse();
    void MakeFull();
    friend std::ostream& operator << (std::ostream& out, const Automata& automata);
    friend std::istream& operator >>  (std::istream& out, Automata& automata);

private:
    std::vector<std::vector<edge>> edges;
    std::set<int> finish_state;
    int start;
    std::vector<char> alphabet;

};

#endif //ALGO_AUTOMATA_H

