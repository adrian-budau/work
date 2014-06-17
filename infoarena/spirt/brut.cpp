#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>

using namespace std;

// Numai cazul simplu, N, M <= 100

const int kMaxN = 105;
const int kModulo = 666013;
const int kMaxM = 105;

int N, M;
vector<int> E[kMaxN];

int dpSons[kMaxN][kMaxM][kMaxM], dp[kMaxN][kMaxM][kMaxM];
int aux[kMaxN][kMaxM][kMaxM];

void solve(int node) {
    if (E[node].empty()) { // nu are fii
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                if (i != j)
                    dp[node][i][j] = 1;
        return;
    }

    for (auto &fiu : E[node])
        solve(fiu);

    // mai intai rezolvam vectorul de fii
    // caz de genu ( ... ) ( .... ) .... ( .. ) unde avem rezolvata fiecare paranteze

    for (int i = 0; i < int(E[node].size()); ++i)
        for (int j = 0; j < M; ++j)
            for (int k = 0; k < M; ++k)
                aux[i][j][k] = 0;
 
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < M; ++j)
            aux[0][i][j] = dp[E[node][0]][i][j];

    // aux[i][j][k] inseamna in cate feluri putem colora primii ii fii astfel incat 
    // paranteza deschisa a primului fiu sa fie j si 
    // parantea inchisa a celui de-al i-lea fiu sa fie k

    // automat aux[0] e dinamica primului fiu
    // aux[i][j][k] = suma de aux[i - 1][j][p] * dp[al i-lea fiu][l][k] unde p != l
    // asta se poate face cu sume partiale mai rapid, demonstrativ las asa
    for (int i = 1; i < int(E[node].size()); ++i)
        for (int j = 0; j < M; ++j)
            for (int k = 0; k < M; ++k)
                for (int p = 0; p < M; ++p)
                    for (int l = 0; l < M; ++l)
                        if (p != l)
                            aux[i][j][k] = (aux[i][j][k] + 1LL * aux[i - 1][j][p] * dp[E[node][i]][l][k]) % kModulo;


    for (int i = 0; i < M; ++i)
        for (int j = 0; j < M; ++j)
            dpSons[node][i][j] = aux[E[node].size() - 1][i][j];

    // acum dinamica nodului
    // si aici putem pune sume partiale
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < M; ++j)
            if (i != j)
                for (int k = 0; k < M; ++k)
                    for (int l = 0; l < M; ++l)
                        if (k != i and l != j)
                            dp[node][i][j] = (dp[node][i][j] + dpSons[node][k][l]) % kModulo;
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
    for (int left = 0; left < M; ++left)
        for (int right = 0; right < M; ++right)
            answer = (answer + dpSons[0][left][right]) % kModulo;

    cout << answer << "\n";
}
