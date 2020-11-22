//
// Created by aleksey on 01.10.2020.
//

#include <queue>
#include <set>
#include <map>
#include<algorithm>
#include <utility>
#include "Automata.h"

Automata::Automata(std::vector<std::vector<edge>> edges, std::set<int> finishState, int start,
                   std::vector<char> sigma) : edges(std::move(edges)), finish_state(std::move(finishState)), start(start),
                                              alphabet(std::move(sigma)) {}


using QueueOfVertexSet = std::queue<std::set<int>>;

struct vertex_state {
    vertex_state(int from, std::map<char, int> transitions) :
                            from(from),
                            transitions(std::move(transitions)) {}
    vertex_state() = default;

    bool operator<(const vertex_state &other) const {
        if (from < other.from)
            return true;
        if (other.from < from)
            return false;
        return transitions < other.transitions;
    }

    int from;
    std::map<char, int> transitions;
};

void Automata::CreateDKA() {
    QueueOfVertexSet to_visit;
    std::map<std::set<int>, int> table;
    std::set<int> tmp;
    tmp.insert(this->start);
    table[tmp] = 0;
    to_visit.push(tmp);
    std::set<int> visited;
    std::vector<std::vector<edge>> new_edges;
    std::set<int> new_finish;
    if (this->finish_state.count(this->start)) {
        new_finish.insert(this->start);
    }
    new_edges.emplace_back();
    while (!to_visit.empty()) {
        std::set<int> current_vertex = to_visit.front();
        to_visit.pop();
        for (char& letter : this->alphabet) {
            std::set<int> reachable_vertex;
            bool is_finish = false;
            for (auto& vertex: current_vertex) {
                for (edge edge_in_vertex : this->edges[vertex]) {
                    if (edge_in_vertex.letter == letter) {
                        reachable_vertex.insert(edge_in_vertex.to);
                        is_finish |= this->finish_state.count(edge_in_vertex.to);
                    }
                }
            }
            if (!reachable_vertex.empty()) {
                if (table.find(reachable_vertex) == table.end()) {
                    table[reachable_vertex] = table.size();
                    new_edges.emplace_back();
                    to_visit.push(reachable_vertex);
                }
                edge new_edge(table[current_vertex], table[reachable_vertex], letter);
                new_edges[table[current_vertex]].emplace_back(new_edge);
                if (is_finish) {
                    new_finish.insert(table[reachable_vertex]);
                }
            }
        }
        visited.insert(table[current_vertex]);
    }
    this->edges = new_edges;
    this->finish_state = new_finish;
}

int Automata::AddEdge(edge new_edge) {
    if (this->edges.size() < new_edge.from && this->edges.size() < new_edge.to) {
        this->edges[new_edge.from].push_back(new_edge);
        return 0;
    } else {
        return -1;
    }
}

int Automata::AddVertex() {
    int new_vertex_num = this->edges.size();
    this->edges.emplace_back();
    return new_vertex_num;
}

std::ostream &operator<<(std::ostream &out, const Automata &automata) {
    std::cout << automata.edges.size() << std::endl;
    for (auto& edges_vec : automata.edges)
        for (auto& edge: edges_vec)
            std::cout << edge.from << " " << edge.to << " " << edge.letter << std::endl;
    std::cout << automata.finish_state.size() << std::endl;
    for (const auto& state : automata.finish_state) {
        std::cout << state << " ";
    }
    std::cout << std::endl;
    return out;
}

void Automata::Minimize() {
    std::vector<int> group(this->edges.size());
    for (int i = 0; i < group.size(); i++) {
        group[i] = this->finish_state.count(i);
    }
    std::vector<int> new_group = group;

    do {
        group = new_group;
        std::vector<std::map<char, int>> states(this->edges.size());
        std::map<vertex_state, int> used_state;
        for (int i = 0; i < this->edges.size(); i++) {
            for (const auto &edge : this->edges[i]) {
                states[i][edge.letter] = group[edge.to];
            }
            vertex_state current(group[i], states[i]);
            if (used_state.count(current) == 0) {
                used_state[current] = used_state.size();
            }
            new_group[i] = used_state[current];
        }
    } while (group != new_group);

    int new_states_number = *std::max_element(group.begin(), group.end()) + 1;
    std::vector<std::vector<edge>> new_edges(new_states_number);
    std::set<int> new_finish;
    for (const auto& edge : this->finish_state) {
        new_finish.insert(group[edge]);
    }
    std::set<edge> used;
    for (const auto & edges_vec : this->edges) {
        for (const auto& current_edge : edges_vec) {
            edge tmp = current_edge;
            tmp.from = group[tmp.from];
            tmp.to = group[tmp.to];
            if (!used.count(tmp)) {
                new_edges[tmp.from].push_back(tmp);
                used.insert(tmp);
            }
        }
    }
    this->edges = new_edges;
    this->finish_state = new_finish;
}

void Automata::Inverse() {
    std::set<int> new_finish;
    int last_not_finish_num = 0;
    for (const auto& current_state : this->finish_state) {
        for (int i = last_not_finish_num; i < current_state; i++) {
            new_finish.insert(i);
        }
        last_not_finish_num = current_state + 1;
    }
    for (int i = last_not_finish_num; i < this->edges.size(); i++) {
        new_finish.insert(i);
    }
    this->finish_state = std::move(new_finish);
}

void Automata::MakeFull() {
    int fake_vertex_num = this->AddVertex();
    for (int i = 0; i < this->edges.size(); i++) {
        std::set<char> visited;
        for (const auto& e : this->edges[i]) {
            visited.insert(e.letter);
        }
        for (const auto& symbol : this->alphabet) {
            if (!visited.count(symbol)) {
                edge new_edge(i, fake_vertex_num, symbol);
                this->edges[i].emplace_back(new_edge);
            }
        }
    }
    for (const auto& symbol : this->alphabet) {
        edge new_edge(fake_vertex_num, fake_vertex_num, symbol);
        this->edges[fake_vertex_num].emplace_back(new_edge);
    }
}

std::istream &operator>>(std::istream &in, Automata &automata) {
    in >> automata.start;

    int number = 0;
    in >> number;
    while (number--) {
        int tmp;
        in >> tmp;
        automata.finish_state.insert(tmp);
    }
    int alphabet_size;
    in >> alphabet_size;
    for (int i = 0; i < alphabet_size; i++) {
        char l;
        in >> l;
        automata.alphabet.push_back(l);
    }
    int n, m;
    in >> n >> m;
    automata.edges = std::vector<std::vector<edge>>(n);
    for (int i = 0; i < m; i++) {
        edge current_edge;
        in >> current_edge.from >> current_edge.to >> current_edge.letter;
        automata.edges[current_edge.from].emplace_back(current_edge);
    }
    return in;
}

edge::edge(int from, int to, char letter) : from(from), to(to), letter(letter){}

bool edge::operator<(const edge& other) const {
    if (this->from < other.from) {
        return true;
    } else if (this->from == other.from) {
        if (this->to < other.to) {
            return true;
        } else if (this->to == other.to) {
            if (this->letter < other.letter) {
                return true;
            }
        }
    }
    return false;
}
