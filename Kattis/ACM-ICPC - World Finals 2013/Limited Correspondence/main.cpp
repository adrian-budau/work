#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

class by_unite {
  public:
    bool operator()(const pair<string, string> &A, const pair<string, string> &B) {
        string X = A.first + B.first;
        string Y = B.first + A.first;
        if (X != Y)
            return X < Y;
        return A.second + B.second < B.second + A.second;
    }
};

class CardPairer {
  public:
    void addCard(const string &first, const string &second) {
        cards_.push_back(make_pair(first, second));
    }

    string lowestMatch() {
        sort(cards_.begin(), cards_.end(), by_unite());

        int total_first = 0, total_second = 0;
        for (int i = 0; i < int(cards_.size()); ++i) {
            total_first += cards_[i].first.size();
            total_second += cards_[i].second.size();
        }

        int total = max(total_first, total_second);
        first_ = string(total, '*');
        second_ = string(total, '*');

        first_position_ = second_position_ = 0;

        taken_ = vector<int>(cards_.size(), 0);
        backtracking();

        if (not answer_.size())
            return "IMPOSSIBLE\n";
        return answer_;
    }

  private:
    void backtracking() {
        if (answer_.size() and (first_position_ > int(answer_.size()) || second_position_ > int(answer_.size())))
            return;
        if (first_position_ == second_position_) {
            if (first_position_ < int(answer_.size()) || answer_.size() == 0)
                answer_ = first_.substr(0, first_position_);
            else if (first_position_ == int(answer_.size())) {
                string aux = first_.substr(0, first_position_);
                answer_ = min(answer_, aux);
            }
        }

        for (int i = 0; i < int(cards_.size()); ++i)
            if (not taken_[i]) {
                int new_first_position = first_position_ + cards_[i].first.size();
                int new_second_position = second_position_ + cards_[i].second.size();

                copy(cards_[i].first.begin(), cards_[i].first.end(), first_.begin() + first_position_);
                copy(cards_[i].second.begin(), cards_[i].second.end(), second_.begin() + second_position_);

                bool ok = true;
                for (int j = min(first_position_, second_position_); j < min(new_first_position, new_second_position); ++j)
                    if (first_[j] != second_[j]) {
                        ok = false;
                        break;
                    }
                if (not ok)
                    continue;

                swap(first_position_, new_first_position);
                swap(second_position_, new_second_position);
                taken_[i] = true;

                backtracking();

                taken_[i] = false;
                swap(first_position_, new_first_position);
                swap(second_position_, new_second_position);
            }
    }

    vector< pair<string, string> > cards_;
    vector<int> taken_;

    string first_, second_;
    int first_position_, second_position_;

    string answer_;
};

int main() {
    int N;

    int test = 0;
    while (cin >> N) {

        CardPairer P;
        for (int i = 0; i < N; ++i) {
            string A, B; cin >> A >> B;
            P.addCard(A, B);
        }

        cout << "Case " << ++test << ": ";
        cout << P.lowestMatch() << "\n";
    }
}
