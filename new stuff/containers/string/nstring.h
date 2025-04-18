#pragma once
#include <iostream>

class NString
{
public:
    NString();
    NString(const NString &other);
    NString(NString &&other) noexcept;
    NString(const char* s);
    ~NString();

public:
    NString &operator=(const NString& other);
    NString &operator=(NString &&other) noexcept;
    NString &operator=(const char* str);

    void operator+=(char ch) { append(ch); }

    char &operator[](size_t i);
    char operator[](size_t i) const;

    friend bool operator>(const NString& str1, const NString& str2);
    friend bool operator<(const NString& str1, const NString& str2);
    friend std::ostream& operator<<(std::ostream& os, const NString& str);
    friend std::istream& operator>>(std::istream& is, NString& str);

public:
    char& at(size_t i)      { return operator[](i); }
    char at(size_t i) const { return operator[](i); }
    
    char* begin()             { return m_str; }
    char* end()               { return m_str + m_len; }
    const char* begin() const { return m_str; }
    const char* end() const   { return m_str + m_len; }

    const char* c_str() const { return m_str; }
    bool empty() const { return m_len == 0; }
    void clear();
    void resize(size_t size);
    size_t length() const { return m_len; }
    void append(char ch);
    void append(const char* str);
    
    friend void getline(std::istream& is, NString& str, char delim = '\n');

private:
    size_t m_len;
    char* m_str;
    static const uint16_t CIN_LIM = 1024;
};


