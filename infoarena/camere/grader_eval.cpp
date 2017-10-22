#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

void result(int score, string message) {
    cout << score;
    cerr << message;
    exit(0);
}

int main() {
    ifstream output("camere.out");
    ifstream ok("camere.ok");

    int answer;
    while (ok >> answer) {
        int out_answer;
        if (!(output >> out_answer))
            result(0, "Fisier de iesire incomplet");
        if (answer != out_answer)
            result(0, "Wrong answer");
    }
    result(5, "OK");
}
