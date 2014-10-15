#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cassert>

using namespace std;

typedef long double double64;

struct point {
    point (long double _x = 0, long double _y = 0):
            x(_x), y(_y) {}
    long double x, y;
};

const double64 eps = 1e-6;

int cmp(double64 a, double64 b) {
    if (a + eps < b)
        return -1;
    if (b + eps < a)
        return 1;
    return 0;
}

int main() {
    int N, M; cin >> N >> M;

    vector<point> A(N);
    for (int i = 0; i < N; ++i)
        cin >> A[i].x >> A[i].y;

    auto orig = A;
    for (int i = 0; i < M; ++i) {
        if (i % 100 == 0) {
            cerr << i << " -> " << A.size() << "\n";
        }

        char type; cin >> type;
        if (type == 'R') {
            double64 left, right, quantity;
            cin >> left >> right >> quantity;

            vector<point> B;
            int start = -1;
            double64 water = 0;
            vector<bool> water_taken(A.size(), false);
            for (int i = 0; i < int(A.size()); ++i) {
                auto p = A[i];
                if (B.empty()) {
                    B.push_back(p);
                    continue;
                }

                if (start == -1)
                    water = 0;

                double64 total = quantity * (min(right, p.x) - max(left, B.back().x));
                if (!water_taken[i] && cmp(total, 0) >= 0) {
                    water += total;
                    water_taken[i] = true;
                }

                if (cmp(p.y, B.back().y) == 1) {
                    if (start == -1) {
                        B.push_back(p);
                        continue;
                    }
                    while (start != -1) {
                        if (cmp(water, 0) <= 0)
                            break;
                        while (B.size() > 1 && cmp(B[B.size() - 2].y, B[B.size() - 1].y) <= 0 && cmp(B.back().y, B[start + 1].y) > 0) {
                            A[--i] = B.back();
                            B.pop_back();
                            p = A[i];
                        }
                        assert(B.back().y + eps < p.y);

                        double64 leftMultiplier = (B[start + 1].x - B[start].x) / (B[start].y - B[start + 1].y);
                        double64 rightMultiplier = (p.x - B.back().x) / (p.y - B.back().y);
                        double64 base = (B.back().x - B[start + 1].x);
                        double64 delta = base * base + (leftMultiplier + rightMultiplier) * water * 2;
                        delta = sqrt(delta);

                        double64 height = (-base + delta) / (leftMultiplier + rightMultiplier);
                        height = min(height, B[start].y - B[start + 1].y);
                        height = min(height, p.y - B.back().y);

                        assert(height >= -eps);
                        water -= height / 2 * (2 * base + height * (leftMultiplier + rightMultiplier));

                        assert(water >= -eps);
                        assert(B[start + 1].y + height - eps <= p.y);

                        if (cmp(height, B[start].y - B[start + 1].y) == 0) { // overflow left
                            double64 newX = rightMultiplier * height + B.back().x;
                            while (int(B.size()) > start + 1) {
                                assert(B.back().y -eps <= B[start + 1].y + height);
                                B.pop_back();
                            }
                            B.push_back(point(newX, B[start].y));
                            while (start >= 0 && cmp(B[start].y, B[start + 1].y) <= 0)
                                --start;
                            continue;
                        }

                        if (cmp(height, p.y - B.back().y) == 0) { // overflow right
                            double64 newX = B[start + 1].x - leftMultiplier * height;
                            while (int(B.size()) > start + 1) {
                                assert(B.back().y - eps <= B[start + 1].y + height);
                                B.pop_back();
                            }
                            B.push_back(point(newX, p.y));
                            break;
                        }

                        // no overflow
                        double64 newY = B[start + 1].y + height;
                        double64 leftX = B[start + 1].x - height * leftMultiplier;
                        double64 rightX = B.back().x + height * rightMultiplier;

                        while (int(B.size()) > start + 1) {
                            assert(B.back().y - eps <= B[start + 1].y + height);
                            B.pop_back();
                        }
                        B.push_back(point(leftX, newY));
                        B.push_back(point(rightX, newY));
                        break;
                    }
                    B.push_back(p);
                    continue;
                }

                if (cmp(p.y, B.back().y) == -1) {
                    start = B.size() - 1;
                    B.push_back(p);
                    continue;
                }

                B.push_back(p);
            }
            swap(A, B);
        }

        if (type == 'Q') {
            double64 X; cin >> X;
            if (cmp(A.front().x, X) >= 0 || cmp(A.back().x, X) <= 0) {
                cout << "0.0\n";
                continue;
            }
            double64 origY = 0, nowY = 0;
            for (int i = 1; i < int(A.size()); ++i)
                if (cmp(A[i - 1].x, X) <= 0 && cmp(X, A[i].x) <= 0) {
                    double64 t = (X - A[i - 1].x) / (A[i].x - A[i - 1].x);
                    nowY = t * (A[i].y - A[i - 1].y) + A[i - 1].y;
                }
             for (int i = 1; i < int(orig.size()); ++i)
                if (cmp(orig[i - 1].x, X) <= 0 && cmp(X, orig[i].x) <= 0) {
                    double64 t = (X - orig[i - 1].x) / (orig[i].x - orig[i - 1].x);
                    origY = t * (orig[i].y - orig[i - 1].y) + orig[i - 1].y;
                }
            assert(origY - eps <= nowY);
            cout.setf(ios::fixed, ios::floatfield);
            cout.precision(9);
            cout << nowY - origY << "\n";
        }
    }
}
