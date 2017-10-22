#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

using namespace std;

class Matrix {
  public:
    Matrix(int rows, int cols):
        m_data(rows, vector<bool>(cols, false)) {
    }

    int rows() const {
        return m_data.size();
    }

    int cols() const {
        return m_data[0].size();
    }

    vector<bool>& operator[](const int& index) {
        return m_data[index];
    }

    const vector<bool>& operator[](const int& index) const {
        return m_data[index];
    }

    Matrix operator*(const Matrix& that) const {
        Matrix result(rows(), that.cols());
        for (int i = 0; i < rows(); ++i)
            for (int j = 0; j < cols(); ++j)
                for (int k = 0; k < that.cols(); ++k)
                    if (m_data[i][j] && that[j][k])
                        result[i][k] = true;
        return result;
    }

  private:
    vector< vector<bool> > m_data;
};

int main() {
    ifstream cin("steinsgate.in");
    ofstream cout("steinsgate.out");

    int V, E, K; assert(cin >> V >> E >> K);
    assert(1 <= V && V <= 200);
    assert(1 <= E && E <= 200 * 199);
    assert(1 <= K && K <= 1000 * 1000 * 1000);

    Matrix M(V, V);
    for (int i = 0; i < E; ++i) {
        int x, y; assert(cin >> x >> y);
        assert(1 <= x && x <= V);
        assert(1 <= y && y <= V);
        assert(x != y);
        assert(M[x - 1][y - 1] == false);
        M[x - 1][y - 1] = true;
    }

    vector<int> value(V);
    for (int i = 0; i < V; ++i) {
        assert(cin >> value[i]);
        assert(1 <= value[i] && value[i] <= 1000 * 1000 * 1000);
    }

    auto paths = M;
    --K;
    while (K > 0) {
        if (K % 2)
            paths = paths * M;
        M = M * M;
        K /= 2;
    }

    vector<int> answer(V, 0);
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            if (paths[i][j] == true)
                answer[j] = max(answer[j], value[i]);

    for (int i = 0; i < V; ++i)
        cout << answer[i] << " ";
    cout << "\n";
}
