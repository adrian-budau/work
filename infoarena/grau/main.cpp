#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

static const int kInfinite = numeric_limits<int>::max() / 2;

struct Node {
    Node(int _x, int _y, Node *_left = nullptr, Node *_right = nullptr):
            x(_x), y(_y), left(_left), right(_right),
            minX(x), maxX(x), minY(y), maxY(y), size(1) {
        update();
   }

    void update() {
        minX = x;
        maxX = x;
        minY = y;
        maxY = y;
        size = 1;
        if (left) {
            minX = min(minX, left->minX);
            minY = min(minY, left->minY);
            maxX = max(maxX, left->maxX);
            maxY = max(maxY, left->maxY);
            size += left->size;
        }
        if (right) {
            minX = min(minX, right->minX);
            minY = min(minY, right->minY);
            maxX = max(maxX, right->maxX);
            maxY = max(maxY, right->maxY);
            size += right->size;
        }
    }

    int x, y;

    Node *left, *right;
    int minX, maxX, minY, maxY, size;
};

template<class T>
Node* replace(T begin, T end, int direction = 0) {
    if (begin == end)
        return nullptr;

    if (end - begin == 1)
        return new Node(begin->first, begin->second, nullptr, nullptr);

    int N = end - begin;
    if (direction == 0)
        nth_element(begin, begin + N / 2, end);
    else
        nth_element(begin, begin + N / 2, end, [&](pair<int, int> a, pair<int, int> b) {
            if (a.second != b.second)
                return a.second < b.second;
            return a.first < b.first;
        });
    return new Node(
        (begin + N / 2)->first,
        (begin + N / 2)->second,
        replace(begin, begin + N / 2, 1 - direction),
        replace(begin + N / 2 + 1, end, 1 - direction)
    );
}

bool lower(int x1, int y1, int x2, int y2) {
    if (x1 != x2)
        return x1 < x2;
    return y1 < y2;
}

int closest(Node *node, int x, int y, int answer, int direction = 0) {
    if (node == nullptr)
        return answer;
    if (node->maxX <= x - answer || node->minX >= x + answer)
        return answer;
    if (node->maxY <= y - answer || node->minY >= y + answer)
        return answer;

    answer = min(answer, max(abs(x - node->x), abs(y - node->y)));

    if (direction == 0)
        if (lower(x, y, node->x, node->y)) {
            answer = closest(node->left, x, y, answer, 1 - direction);
            return closest(node->right, x, y, answer, 1 - direction);
        } else {
            answer = closest(node->right, x, y, answer, 1 - direction);
            return closest(node->left, x, y, answer, 1 - direction);
        }
    else
        if (lower(y, x, node->y, node->x)) {
            answer = closest(node->left, x, y, answer, 1 - direction);
            return closest(node->right, x, y, answer, 1 - direction);
        } else {
            answer = closest(node->right, x, y, answer, 1 - direction);
            return closest(node->left, x, y, answer, 1 - direction);
        }
}

pair<int, int> smallestX(Node * node) {
    int left = kInfinite, current = node->x, right = kInfinite;
    if (node->left)
        left = node->left->minX;
    if (node->right)
        right = node->right->minX;
    if (left <= current && left <= right)
        return smallestX(node->left);
    if (current <= right)
        return make_pair(node->x, node->y);
    return smallestX(node->right);
}

pair<int, int> largestX(Node * node) {
    int left = -kInfinite, current = node->x, right = -kInfinite;
    if (node->left)
        left = node->left->maxX;
    if (node->right)
        right = node->right->maxX;
    if (right >= current && right >= left)
        return largestX(node->right);
    if (current >= left)
        return make_pair(node->x, node->y);
    return largestX(node->left);
}

pair<int, int> smallestY(Node * node) {
    int left = kInfinite, current = node->y, right = kInfinite;
    if (node->left)
        left = node->left->minY;
    if (node->right)
        right = node->right->minY;
    if (left <= current && left <= right)
        return smallestY(node->left);
    if (current <= right)
        return make_pair(node->x, node->y);
    return smallestY(node->right);
}

pair<int, int> largestY(Node *node) {
    int left = -kInfinite, current = node->y, right = -kInfinite;
    if (node->left)
        left = node->left->maxY;
    if (node->right)
        right = node->right->maxY;
    if (right >= current && right >= left)
        return largestY(node->right);
    if (current >= left)
        return make_pair(node->x, node->y);
    return largestY(node->left);
}


bool find(Node *node, int x, int y, int direction = 0) {
    if (node == nullptr)
        return false;
    if (node->x == x && node->y == y)
        return true;
    if (direction == 0)
        if (lower(x, y, node->x, node->y))
            return find(node->left, x, y, 1 - direction);
        else
            return find(node->right, x, y, 1 - direction);
    else
        if (lower(y, x, node->y, node->x))
            return find(node->left, x, y, 1 - direction);
        else
            return find(node->right, x, y, 1 - direction);
}

void traverseDelete(Node* node, vector< pair<int, int> > &elements) {
    if (node == nullptr)
        return;
    traverseDelete(node->left, elements);
    elements.emplace_back(node->x, node->y);
    traverseDelete(node->right, elements);
    delete node;
}

Node *insert(Node *node, int x, int y, int direction = 0) {
    if (node == nullptr)
        return new Node(x, y, nullptr, nullptr);

    if (rand() % (2 * node->size) == node->size - 1) {
        // replace this part
        vector< pair<int, int> > elements;
        elements.emplace_back(x, y);
        traverseDelete(node, elements);
        return replace(elements.begin(), elements.end(), direction);
    }

    if (direction == 0)
        if (lower(x, y, node->x, node->y))
            node->left = insert(node->left, x, y, 1 - direction);
        else
            node->right = insert(node->right, x, y, 1 - direction);
    else
        if (lower(y, x, node->y, node->x))
            node->left = insert(node->left, x, y, 1 - direction);
        else
            node->right = insert(node->right, x, y, 1 - direction);
    node->update();
    return node;
}

Node *remove(Node* node, int x, int y, int direction = 0) {
    if (node == nullptr)
        return node;

    if (node->x == x && node->y == y) {
        if (!node->left && !node->right) {
            delete node;
            return nullptr;
        }

        pair<int, int> next;
        if (node->right)
            if (direction == 0)
                next = smallestX(node->right);
            else
                next = smallestY(node->right);
        else
            if (direction == 0)
                next = largestX(node->left);
            else
                next = largestY(node->left);
        auto result = remove(node, next.first, next.second, direction);
        result->x = next.first;
        result->y = next.second;
        result->update();
        return result;
    }
    if (direction == 0)
        if (lower(x, y, node->x, node->y))
            node->left = remove(node->left, x, y, 1 - direction);
        else
            node->right = remove(node->right, x, y, 1 - direction);
    else
        if (lower(y, x, node->y, node->x))
            node->left = remove(node->left, x, y, 1 - direction);
        else
            node->right = remove(node->right, x, y, 1 - direction);
    node->update();
    return node;
}

int main() {
    srand(16051993);
    ifstream cin("grau.in");
    ofstream cout("grau.out");

    int N; cin >> N;

    vector< pair<int, int> > points(N);
    for (int i = 0; i < N; ++i)
        cin >> points[i].first >> points[i].second;
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    for (auto &p : points)
        p = make_pair(p.first + p.second, p.first - p.second);
    Node *root = replace(points.begin(), points.end());

    int M; cin >> M;
    for (int i = 0; i < M; ++i) {
        int type, x, y; cin >> type >> x >> y;
        if (type == 0) {
            cout << closest(root, x + y, x - y, kInfinite) << "\n";
        } else if (type == 1) {
            if (not find(root, x + y, x - y))
                root = insert(root, x + y, x - y);
        } else {
            root = remove(root, x + y, x - y);
        }
    }
}
