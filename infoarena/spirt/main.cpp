#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>

using namespace std;

// Numai cazul simplu, N, M <= 100

const int kMaxN = 100005;
const int kModulo = 666013;
const int kMaxM = 2; // cazul 0 -> colurile sunt 0 si 0, cazul 1 -> culorile sunt 0 si 1, te poti uita ca si cum astea sunt colurile
                     // nu doar caz 0 -> egal, caz 1  -> diferit

int N, M;
vector<int> E[kMaxN];

int dpSons[kMaxN][kMaxM], dp[kMaxN][kMaxM];
int aux[kMaxN][kMaxM];

void solve(int node) {
    if (E[node].empty()) { // nu are fii
        dp[node][0] = 0;
        dp[node][1] = 1;
        return;
    }

    for (auto &fiu : E[node])
        solve(fiu);

    // mai intai rezolvam vectorul de fii
    // caz de genu ( ... ) ( .... ) .... ( .. ) unde avem rezolvata fiecare paranteze

    for (int i = 0; i < int(E[node].size()); ++i)
        for (int j = 0; j < 2; ++j)
            aux[i][j] = 0;

    for (int i = 0; i < 2; ++i)
        aux[0][i] = dp[E[node][0]][i];

    for (int i = 1; i < int(E[node].size()); ++i) {
        // caz 1 -> (0 .. ... x) (y  0)
        //       -> (0 .. ....0) (1  0) -> 0  e fixat, doar 1 poate lua valori (M - 1) posibiliati
        //       -> (0 .. ....1) (2  0) -> si 1 si 2 pot lua valori (M - 1) * (M - 2) posibilitati
        aux[i][0] = (1LL * aux[i - 1][0] * dp[E[node][i]][1] % kModulo * (M - 1) +
                     1LL * aux[i - 1][1] * dp[E[node][i]][1] % kModulo * (1LL * (M - 1) * (M - 2) % kModulo))
                    % kModulo;

        // caz 2 -> (0 ..... .x) (y  1)
        //       -> (0 .. ... 0) (2  1) -> 0 si 1 sunt fixate, doar 2 poate lua valori (M - 2) posibilitati
        //       -> (0 ... .. 1) (0  1) -> o singura posibilitate
        //       -> (0 .... ..1) (2  1) -> 0 si 1 sunt fixate  doar 2 poate lua valori (M - 2) posibilitati
        //       -> (0 ...... 2) (0  1) -> 0 si 1 sunt fixate doar 2 poate lua valori (M - 2) posiblitati
        //       -> (0 ...... 2) (3  1) -> 0 si 1 sunt fixate 2 si 3 pot lua valori (M - 2) * (M - 3) posibilitati

        int temp1 = 0, temp2 = 0, temp3 = 0; // temp1 sunt alea inmultite cu 1, temp2 alea inmultite cu (M - 2) si temp3 ...
        temp1 = (1LL * aux[i - 1][1] * dp[E[node][i]][1]) % kModulo;
        temp2 = (1LL * aux[i - 1][0] * dp[E[node][i]][1] + 2LL * aux[i - 1][1] * dp[E[node][i]][1]) % kModulo;
        temp3 = (1LL * aux[i - 1][1] * dp[E[node][i]][1]) % kModulo;
        aux[i][1] = (
            temp1 + 
            1LL * temp2 * (M - 2) + 
            1LL * temp3 * (1LL * (M - 2) * (M - 3) % kModulo)
        ) % kModulo;
    }

    for (int i = 0; i < 2; ++i)
        dpSons[node][i] = aux[E[node].size() - 1][i];

    // dp[node][0] nu are sens deci cazul 1 nu exista

    // caz 2 -> (0 (x   y) 1)
    //       -> (0 (1   0) 1) o singura posibilitate
    //       -> (0 (1   2) 1) unde 2 poate lua M - 2 valori
    //       -> (0 (2   0) 1) unde 2 poate lua M - 2 valori
    //       -> (0 (2   3) 1) unde 2 si 3 pot lua valori, (M - 2) * (M - 3) posiblitati
    //       -> (0 (2   2) 1) unde 2 poate lua (M - 2) valori
    dp[node][1] = (
        dpSons[node][1] + 
        2LL * dpSons[node][1] * (M - 2) + 
        1LL * dpSons[node][0] * (M - 2) +
        1LL * dpSons[node][1] * (1LL * (M - 2) * (M - 3) % kModulo)
    ) % kModulo;
}

int main() {
    ifstream cin("spirt.in");
    ofstream cout("spirt.out");

    cin >> N >> M;
    string S; cin >> S;
    N /= 2;

    stack<int> st;
    st.push(0);
    int current_node = 0;

    // am facut graful
    for (int i = 0; i < 2 * N; ++i)
        if (S[i] == '(') {
            ++current_node;
            E[st.top()].push_back(current_node);
            st.push(current_node);
        } else {
            st.pop();
        }

    solve(0);

    int answer = 0;
    answer = (
        1LL * dpSons[0][0] * M +
        1LL * dpSons[0][1] * (1LL * M * (M - 1) % kModulo)
    ) % kModulo;

    cout << answer << "\n";
}
