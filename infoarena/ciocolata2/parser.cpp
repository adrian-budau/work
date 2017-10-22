#include <fstream>
#include <cstdio>

using namespace std;

class Writer {
  public:
    Writer(const char *name):
        m_stream(name) {
        memset(m_buffer, 0, sizeof(m_buffer));
        m_pos = 0;
    }

    Writer& operator<<(int a) {
        int many = 0;
        do {
            digit_buffer[many++] = a % 10 + '0';
            a /= 10;
        } while (a > 0);
        for (int i = many - 1; i >= 0; --i)
            putchar(digit_buffer[i]);
        return *this;
    }

    Writer& operator<<(const char *s) {
        for (; *s; ++s)
            putchar(*s);
        return *this;
    }

    ~Writer() {
        m_stream.write(m_buffer, m_pos);
    }

  private:
    void putchar(char c) {
        m_buffer[m_pos++] = c;
        if (m_pos == kBufferSize) {
            m_stream.write(m_buffer, m_pos);
            m_pos = 0;
        }
    }

    static const int kBufferSize = 32768;
    ofstream m_stream;
    char m_buffer[kBufferSize];
    char digit_buffer[30];
    int m_pos;
};


