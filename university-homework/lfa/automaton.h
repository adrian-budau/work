#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iterator>
#include <queue>

using namespace std;

class Automaton {
  public:
    enum struct Type {
        GENERAL = 0,
        NON_DETERMINIST = 1,
        DETERMINIST = 2
    };

    Automaton(const int &states = 1, const string& alphabet = ""):
            states_(states),
            alphabet_(alphabet.begin(), alphabet.end()),
            starts_(states, 0),
            finals_(states, 0) {
        for (int i = 0; i < states; ++i)
            states_[i].index(i);
    }

    Automaton(const Automaton& that) {
        *this = that;
    }

    Automaton& operator=(const Automaton& that) {
        alphabet_ = that.alphabet_;
        starts_ = that.starts_;
        finals_ = that.finals_;

        states_ = vector<Node>(that.states_.size());
        for (int i = 0; i < int(that.states_.size()); ++i)
            states_[i].index(i);
        for (int i = 0; i < int(that.states_.size()); ++i)
            for (auto &edge : that.states_[i].edges())
                addTransition(i, edge.second, edge.first);
        return *this;
    }

    void setStart(const int &index) {
        starts_[index] = 1;
    }

    void setFinal(const int &index) {
        finals_[index] = 1;
    }

    bool hasCharacter(const char &character) const {
        return alphabet_.count(character) > 0;
    }

    void addTransition(const int &from, const int &to, const char &character) {
        states_[from].addTransition(to, character);
    }

    Automaton removeEmptyTransitions() const {
        Automaton result(states_.size(), std::string(alphabet_.begin(), alphabet_.end()));

        result.starts_ = starts_;
        result.finals_ = finals_;

        vector< vector<int> > graph(states_.size(), vector<int>(states_.size(), 0));
        for (int i = 0; i < int(states_.size()); ++i) {
            for (auto &edge : states_[i].edges(0))
                graph[i][edge.second] = 1;
        }

        for (int k = 0; k < int(states_.size()); ++k)
            for (int i = 0; i < int(states_.size()); ++i)
                for (int j = 0; j < int(states_.size()); ++j)
                    if (graph[i][k] and graph[k][j])
                        graph[i][j] = 1;

        for (int i = 0; i < int(states_.size()); ++i)
            for (int j = 0; j < int(states_.size()); ++j)
                if (graph[i][j] and finals_[j])
                    result.setFinal(i);

        for (int i = 0; i < int(states_.size()); ++i)
            for (int j = 0; j < int(states_.size()); ++j)
                if (graph[i][j] and finals_[j])
                    result.setFinal(i);

        for (int i = 0; i < int(states_.size()); ++i)
            for (int j = 0; j < int(states_.size()); ++j)
                if (i != j and graph[i][j])
                    for (auto &edge : states_[j].edges())
                        if (edge.first != 0)
                            result.addTransition(i, edge.second, edge.first);

        for (int i = 0; i < int(states_.size()); ++i)
            for (auto &edge : states_[i].edges())
                if (edge.first != 0)
                    result.addTransition(i, edge.second, edge.first);
        return result;
    }

    Automaton makeDeterminist() const {
        if (type() == Type::GENERAL)
            throw new exception();
        Automaton result(1, string(alphabet_.begin(), alphabet_.end()));

        map< vector<int>, int> M;
        queue< vector<int> > Q;

        vector<int> initial = starts_;

        Q.push(initial);
        M[initial] = 0;
        for (int i = 0; i < int(states_.size()); ++i)
            if (starts_[i] and finals_[i])
                result.finals_[0] = 1;

        while (!Q.empty()) {
            auto state = Q.front();
            Q.pop();

            for (auto &it : alphabet_) {
                vector<int> next_state(states_.size(), 0);
                bool is_final = false;
                for (int i = 0; i < int(states_.size()); ++i)
                    if (state[i] == 1)
                        for (auto &edge : states_[i].edges(it)) {
                            next_state[edge.second] = 1;
                            if (finals_[edge.second] == 1)
                                is_final = true;
                        }

                if (next_state == vector<int>(states_.size(), 0))
                    continue;
                if (M.count(next_state) == 0) {
                    M[next_state] = result.push_back(is_final);
                    Q.push(next_state);
                }

                result.addTransition(result[M[state]].index(), result[M[next_state]].index(), it);
            }
        }

        result.starts_[0] = 1;
        return result;
    }

    Automaton minimize() const {
        if (type() != Type::DETERMINIST)
            throw new exception();

        return inverse().makeDeterminist().inverse().makeDeterminist();
    }

    Automaton inverse() const {
        type();
        // we reverse all edges, finals and starts swap
        Automaton result(states_.size(), std::string(alphabet_.begin(), alphabet_.end()));

        result.starts_ = finals_;
        result.finals_ = starts_;
        for (auto &from : states_)
            for (auto &edge : from.edges()) {
                result.addTransition(states_[edge.second].index(), from.index(), edge.first);
            }
        return result;
    }

    void star() {
        for (int i = 0; i < int(states_.size()); ++i)
            for (int j = 0; j < int(states_.size()); ++j)
                if (starts_[i] and finals_[j])
                    addTransition(j, i, 0);
        for (int i = 0; i < int(states_.size()); ++i)
            if (starts_[i] == 1)
                finals_[i] = 1;
    }

    Automaton& operator+=(const Automaton& that) {
        int N = states_.size();
        int M = that.states_.size();

        for (int i = 0; i < M; ++i)
            push_back(0);

        for (int i = 0; i < M; ++i)
            for (auto &edge : that.states_[i].edges())
                addTransition(i + N, edge.second + N, edge.first);

        for (int i = 0; i < N; ++i)
            if (finals_[i])
                for (int j = 0; j < M; ++j)
                    if (that.starts_[j])
                        addTransition(i, j + N, 0);
        for (int i = 0; i < N; ++i)
            if (finals_[i])
                finals_[i] = 0;
        for (int i = 0; i < M; ++i)
            if (that.finals_[i])
                finals_[i + N] = 1;
        return *this;
    }

    Automaton& operator|=(const Automaton &that) {
        int N = states_.size();
        int M = that.states_.size();

        for (int i = 0; i < M; ++i)
            push_back(0);
        for (int i = 0; i < M; ++i)
            for (auto &edge : that.states_[i].edges())
                addTransition(i + N, edge.second + N, edge.first);

        for (int i = 0; i < M; ++i) {
            if (that.starts_[i])
                starts_[i + N] = 1;
            if (that.finals_[i])
                finals_[i + N] = 1;
        }

        return *this;
    }

    bool check(const string &S) const {
        if (type() == Type::GENERAL)
            throw new exception();

        vector<int> start = starts_;

        for (auto &c : S) {
            vector<int> next(states_.size(), 0);
            for (int i = 0; i < int(states_.size()); ++i)
                if (start[i])
                    for (auto &edge : states_[i].edges(c))
                        next[edge.second] = 1;
            swap(next, start);
        }

        for (int i = 0; i < int(states_.size()); ++i)
            if (start[i] and finals_[i])
                return true;
        return false;
    }

    Type type() const {
        for (auto &node : states_)
            if (node.edges(0).size())
                return Type::GENERAL;

        for(auto &node : states_)
            for (auto &edge : node.edges())
                if (node.count(edge.first) > 1)
                    return Type::NON_DETERMINIST;
        return Type::DETERMINIST;
    }

    friend istream& operator>>(istream&, Automaton&);
    friend ostream& operator<<(ostream&, const Automaton&);

  private:
    class Node {
      public:
        Node(int index = 0):
                index_(index) {
        }

        const int &index() const {
            return index_;
        }

        Node& index(const int &newIndex) {
            index_ = newIndex;
            return *this;
        }

        void addTransition(int to, const char &character) {
            next_.insert({character, to});
        }

        vector< pair<char, int> > edges() const {
            return vector< pair<char, int> >(next_.begin(), next_.end());
        }

        vector< pair<char, int> > edges(const char& character) const {
            auto range = next_.equal_range(character);
            return vector< pair<char, int> >(range.first, range.second);
        }

        int count(const char &character) const {
            return next_.count(character);
        }

      private:
        multimap<char, int> next_;
        int index_;
    };

    int push_back(bool is_final) {
        if (is_final)
            finals_.push_back(1);
        else
            finals_.push_back(0);

        starts_.push_back(0);
        states_.push_back(Node(states_.size()));
        return states_.back().index();
    }

    Node& operator[](const int& n) {
       return states_[n];
    }

    const Node& operator[](const int& n) const {
        return states_[n];
    }

    vector<Node> states_;
    set<char> alphabet_;

    vector<int> starts_;
    vector<int> finals_;
};

istream& operator>>(istream& stream, Automaton& object) {
    int states;
    string alphabet;
    stream >> states;
    stream >> alphabet;

    object = Automaton(states, alphabet);
    int start_states;
    stream >> start_states;
    for (int i = 0; i < start_states; ++i) {
        int node; stream >> node;
        object.setStart(node);
    }


    int accepting_states; stream >> accepting_states;
    for (int i = 0; i < accepting_states; ++i) {
        int node; stream >> node;

        object.setFinal(node);
    }

    int transitions; stream >> transitions;
    for (int i = 0; i < transitions; ++i) {
        int from, to; char character;
        stream >> from >> to >> character;
        if (object.hasCharacter(character))
            object.addTransition(from, to, character);
        else
            object.addTransition(from, to, 0);
    }

    return stream;
}

ostream& operator<<(ostream& stream, const Automaton& object) {
    stream << "Q(states) = {0";
    for (int i = 1; i < int(object.states_.size()); ++i)
        stream << ", " << i;
    stream << "}\n";

    stream << "Σ(alphabet) = {";
    if (object.alphabet_.size()) {
        bool start = true;
        for (auto &it : object.alphabet_) {
            if (not start)
                stream << ", ";
            else
                start = false;
            stream << '\'' << it << '\'';
        }
    }

    stream << "}\n";

    stream << "Q(start statse) = {";
    bool start = true;
    for (int i = 0; i < int(object.states_.size()); ++i)
        if (object.starts_[i]) {
            if (not start)
                stream << ", ";
            else
                start = false;
            stream << i;
        }
    stream << "}\n";
 
    stream << "F(final states) = {";
    start = true;
    for (int i = 0; i < int(object.states_.size()); ++i)
        if (object.finals_[i]) {
            if (not start)
                stream << ", ";
            else
                start = false;
            stream << i;
        }
    stream << "}\n";
    stream << "δ(transitions): \n";
    for (auto &from : object.states_)
        for (auto &edge : from.edges()) {
            string move;
            if (edge.first)
                move += edge.first;
            if (!move.size())
                move = "ε";
            stream << from.index() << " to " << object[edge.second].index() << " with " << move << "\n";
        }
    return stream;
}

