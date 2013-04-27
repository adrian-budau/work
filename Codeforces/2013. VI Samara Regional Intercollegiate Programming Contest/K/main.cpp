#include <iostream>
#include <vector>

using namespace std;

int main() {
    int N; int64_t K;
    cin >> N >> K;

    int desc;
    for (desc = 1; 1LL * desc * (desc - 1) / 2 <= K; ++desc);
    --desc;

    vector<int> answer(desc);
    for (int i = 0; i < desc; ++i)
        answer[i] = desc - i;

    K -= 1LL * desc * (desc - 1) / 2;
    if (desc < N) {
        for (int i = 0; i <= desc; ++i)
            if (desc - i == K) {
                answer.insert(answer.begin() + i, desc + 1);
            }
    }

    for (int i = desc + 2; i <= N; ++i)
        answer.push_back(i);
    
    for (auto &x: answer)
        cout << x << " ";
    cout << "\n";
}
