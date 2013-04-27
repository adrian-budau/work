#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <tr1/unordered_map>

using namespace std;
using namespace tr1;

class Championship {
  public:
    Championship(const int &length = 1, const int &number = 1, const int &win_score = 0, const int &draw_score = 0) {
        win_score_ = win_score;
        draw_score_ = draw_score;
        for (cheat_ = 1, cheat_length_ = 0; cheat_ * cheat_ <= length && // we don't do more then lenght operations on a query
                                            cheat_ * cheat_ * cheat_length_<= length * number // we don't take too much time to add edges
                                            ; cheat_ *= 3, cheat_length_++);
        cheat_ /= 3;
        cheat_length_--;

        roots_.resize(cheat_);
        better_.resize(cheat_);
        for (int i = 0; i < cheat_; ++i)
            roots_[i] = new Node();
       
        cheat_string_.resize(cheat_);
        for (int i = 0; i < cheat_; ++i) {
            int now = i;
            string current_play;
            for (int k = 0; k < cheat_length_; ++k, now /= 3)
                current_play.push_back(mapToChar_(now % 3));
            cheat_string_[i] = current_play;
        }

        for (int i = 0; i < cheat_; ++i)
            for (int j = 0; j < cheat_; ++j)
                if (j != i) {
                    int now = i;
                    int next = j;
                    for (int k = 0; k < cheat_length_; ++k) {
                        int nowv = now % 3;
                        int nextv = next % 3;
                        
                        now /= 3;
                        next /= 3;

                        if (nowv == (nextv + 1) % 3) {
                            better_[i].push_back(j);
                            break;
                        }

                        if ((nowv + 1) % 3 == nextv)
                            break;
                    }
                }
    }

    void addParticipant(const string& play_list) {
        string optimize = play_list.substr(0, cheat_length_);
        int number = 0;
        for (string::const_reverse_iterator it = optimize.rbegin(); it != optimize.rend(); ++it)
            number = number * 3 + mapToInt_(*it);

        addParticipant_(roots_[number], play_list.begin(), play_list.end());
    }

    string bestPlay(const int &length) const {
        // first let's choose roots
        int best_move = -1;
        int best_score = -1000000000;
        for (int i = 0; i < cheat_; ++i) {
            int current_score = 0;
            for (vector<int>::const_iterator it = better_[i].begin(); it != better_[i].end(); ++it)
                current_score += count_(roots_[*it]) * win_score_;
            current_score += count_(roots_[i]) * draw_score_;

            if (current_score > best_score) {
                best_score = current_score;
                best_move = i;
            } else
                if (current_score == best_score) {
                    if (best_move == -1 || cheat_string_[i] < cheat_string_[best_move])
                        best_move = i;
                }
        }

        Node* current = roots_[best_move];

        string play(cheat_string_[best_move]);
        play.reserve(length);
        for (int i = 0; i < length - cheat_length_; ++i) {
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

    int cheat_, cheat_length_;
    vector<string> cheat_string_;
    int win_score_, draw_score_; 
    vector< vector<int> > better_;
    vector<Node *> roots_;
};

int main() {
    ifstream cin("rps.in");
    ofstream cout("rps.out");

    // i don't really care about lengths
    int participants, length, win_score, draw_score;

    cin >> participants >> length >> win_score >> draw_score;

    if (win_score == 0 && draw_score > 0) {
        unordered_map<string, int> Words;

        int best = -1;
        string where;
        for (int i = 0; i < participants; ++i) {
            string X; cin >> X;
            if (++Words[X] > best) {
                where = X;
                best = Words[X];
            }

            if (Words[X] == best && X < where)
                where = X;
            cout << where << "\n";
        }


        return 0;
    }

    Championship C(length, participants, win_score, draw_score);

    for (int i = 0; i < participants; ++i) {
        string new_participant; cin >> new_participant;
        C.addParticipant(new_participant);

        // or maybe I do
        cout << C.bestPlay(length) << "\n";
    }
}
