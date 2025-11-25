#pragma once
#include <queue>
#include <string>
#include <iostream>

// Arduino-like String wrapper using std::string
class String {
public:
    std::string data;

    String() {}
    void reserve(size_t n) { data.reserve(n); }

    void operator+=(char c) { data += c; }
    void operator=(const char* s) { data = s; }
    bool operator==(const String &other) const {
        return data == other.data;
    }
    bool operator==(const char* other) const {
        return data == std::string(other);
    }
    const char* c_str() const { return data.c_str(); }
    operator std::string() const { return data; }
};

// Fake serial object
class FakeSerial {
public:
    std::queue<char> buffer;

    void begin(int) {}

    bool available() const {
        return !buffer.empty();
    }

    char read() {
        char c = buffer.front();
        buffer.pop();
        return c;
    }

    // Arduino-style println()
    void println(const String& s) {
        std::cout << "[Serial] " << s.data << std::endl;
    }

    // inject input from the PC terminal
    void inject(const std::string& s) {
        for (char c : s) buffer.push(c);
        buffer.push('\n'); // exactly what Arduino does
    }
};

extern FakeSerial Serial;