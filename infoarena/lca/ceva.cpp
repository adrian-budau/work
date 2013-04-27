
class A {
    public:
        A(int a, int _b): b(a * _b) {}
int b = 3;
};

int main() {
    A x(2, 3);
    return x.b;
}

