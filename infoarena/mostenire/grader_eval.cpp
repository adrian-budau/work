#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <algorithm>

using namespace std;

void result(const string& message, int score) {
    cerr << message;
    cout << score;
    exit(0);
}

int main() {
    ifstream out("mostenire.out");
    if (!out.is_open())
        result("Fisier de iesire lipsa", 0);

    ifstream ok("mostenire.ok");
    if (!ok.is_open())
        result("Fisier ok lipsa", 0);

    int N;
    int64_t K;
    if (!(ok >> N >> K))
        result("Fisier ok invalid", 0);

    vector<int> V(N);
    for (int i = 0; i < N; ++i)
        if (!(ok >> V[i]))
            result("Fisier ok invalid", 0);

    int64_t Dok;
    if (!(ok >> Dok))
        result("Fisier ok invalid", 0);

    int64_t Dout;
    if (!(out >> Dout))
        result("Fisier de iesire incomplet", 0);

    vector<int64_t> Vout(N);
    for (int i = 0; i < N; ++i)
        if (!(out >> Vout[i]))
            result("Fisier de iesire lipsa", 0);

    if (Dok != Dout)
        result("Wrong answer", 0);

    int64_t Kout = 0;
    for (auto &v : Vout)
        Kout += v;
    if (Kout != K)
        result("Wrong answer", 0);

    int64_t minimum = numeric_limits<int64_t>::max(), maximum = numeric_limits<int64_t>::min();
    for (int i = 0; i < N; ++i) {
        minimum = min(minimum, Vout[i] / V[i]);
        maximum = max(maximum, Vout[i] / V[i] + int(Vout[i] % V[i] > 0));
    }

    int64_t diff = maximum - minimum;
    diff = max(diff, *max_element(Vout.begin(), Vout.end()) - *min_element(Vout.begin(), Vout.end()));
    if (diff != Dout)
        result("Wrong answer", 1);

    result("OK", 5);
}
