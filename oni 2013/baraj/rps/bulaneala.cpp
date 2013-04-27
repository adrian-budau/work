#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Championship {
  public:
    Championship() {
        root = new Node();
    }

    void addParticipant(const string& play_list) {
        addParticipant_(root, play_list.begin(), play_list.end());
    }

    string bestPlay(const int &length) const {
        Node* current = root;

        string play;
        play.reserve(length);
        for (int i = 0; i < length; ++i) {
            if (current == 0) {
                play.push_back('P');
                continue;
            }

            int best_move = 0;
            int best_score = count_(current -> next[2]) * win_score_ + count_(current -> next[0]) * draw_score_;
            for (int i = 1; i < 3; ++i) {
                int current_score = count_(current -> next[i]) * draw_score_ + count_(current -> next[i - 1]) * win_score_;
                if (current_score > best_score) {
                    best_score = current_score;
                    best_move = i;
                } else
                    if (current_score == best_score && mapToChar_(i) < mapToChar_(best_move))
                        best_move = i;
            }

            play.push_back(mapToChar_(best_move));
            current = current -> next[best_move];
        }

        return play;
    }

  private:
    friend istream& operator>>(istream&, Championship&);

    class Node {
      public:
        Node():
                count(0),
                next() {
        }

        int count;
        Node *next[3];
    };

    template<class Iterator>
    void addParticipant_(Node* const current, Iterator first, Iterator last) {
        if (first == last) {
            current -> count++;
            return;
        }

        Node* &son = current -> next[mapToInt_(*first)];

        // if he doesn't exist
        if (son == 0)
            son = new Node();
        addParticipant_(son, first + 1, last);

        current -> count = 0;
        for (int i = 0; i < 3; ++i)
            current -> count += count_(current -> next[i]);
    }

    // 0 is Rock, 1 is Paper and 2 is Scissors
    static int mapToInt_(const char &play_operation) {
        if (play_operation == 'R')
            return 0;
        if (play_operation == 'P')
            return 1;
        return 2;
    }

    static char mapToChar_(const int &play_operation) {
        if (play_operation == 0)
            return 'R';
        if (play_operation == 1)
            return 'P';
        return 'S';
    }

    int count_(Node* const current) const {
        if (current == 0)
            return 0;
        return current -> count;
    }

    int win_score_, draw_score_; 
    Node *root;
};

istream& operator>>(istream& in_stream, Championship& that) {
    in_stream >> that.win_score_ >> that.draw_score_;
    return in_stream;
}

int main() {
    ifstream cin("rps.in");
    ofstream cout("rps.out");

    // i don't really care about lengths
    int participants, length;
    Championship C;

    cin >> participants >> length >> C;

    for (int i = 0; i < participants; ++i) {
        string new_participant; cin >> new_participant;
        C.addParticipant(new_participant);

        // or maybe I do
        cout << C.bestPlay(length) << "\n";
    }
}
