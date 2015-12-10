#include <iostream>
#include <deque>
#include <algorithm>
#include <fstream>
#include <vector>

using namespace std;

class Reader {
  public:
    Reader(const string& filename):
            m_stream(filename),
            m_pos(kBufferSize - 1),
            m_buffer(new char[kBufferSize]) {
        next();
    }

    Reader& operator>>(int& value) {
        value = 0;
        while (current() < '0' || current() > '9')
            next();
        while (current() >= '0' && current() <= '9') {
            value = value * 10 + current() - '0';
            next();
        }
        return *this;
    }

  private:
    const int kBufferSize = 32768;

    char current() {
        return m_buffer[m_pos];
    }

    void next() {
        if (++m_pos == kBufferSize) {
            m_stream.read(m_buffer.get(), kBufferSize);
            m_pos = 0;
        }
    }

    ifstream m_stream;
    int m_pos;
    unique_ptr<char[]> m_buffer;
};

int main() {
    Reader cin("kthvalue.in");
    ofstream cout("kthvalue.out");

    int M; cin >> M;
    deque<int> D;
    for (int i = 0; i < M; ++i) {
        int type; cin >> type;
        if (type == 1) {
            int x; cin >> x;
            D.push_front(x);
        } else if (type == 2) {
            int x; cin >> x;
            D.push_back(x);
        } else if (type == 3) {
            D.pop_front();
        } else if (type == 4) {
            D.pop_back();
        } else {
            int x, y, k; cin >> x >> y >> k;
            vector<int> V(D.begin() + x - 1, D.begin() + y);
            nth_element(V.begin(), V.begin() + k - 1, V.end());
            cout << V[k - 1] << "\n";
        }
    }
}
