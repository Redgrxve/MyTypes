#include "nstring.h"
#include "debuglog.h"
#include <string>

NString::NString()
    : m_len{0}
{
    m_str = new char[1];
    m_str[0] = '\0';
}

NString::NString(const NString& other)
    : m_len{other.m_len}, m_str{new char[m_len + 1]}
{
    strcpy_s(m_str, m_len + 1, other.m_str);
}

NString::NString(NString &&other) noexcept
    : m_len{other.m_len}, m_str{other.m_str}
{
    DEBUG_LOG("NString(NString &&other)");

    other.m_len = 0;
    other.m_str = nullptr;
}

NString::NString(const char* s)
{
    m_len = strlen(s);
    m_str = new char[m_len + 1];
    strcpy_s(m_str, m_len + 1, s);
}

NString::~NString()
{
    //std::cout << "Destructor: " << (m_str ? m_str : "nullptr") << "\n";
    delete[] m_str;
}

NString& NString::operator=(const NString& other)
{
    DEBUG_LOG("operator=(const NString& other)");

    if (this == &other) return *this;

    delete[] m_str;
    m_len = other.m_len;
    m_str = new char[m_len + 1];
    strcpy_s(m_str, m_len + 1, other.m_str);
    return *this;
}

NString& NString::operator=(NString&& other) noexcept
{
    DEBUG_LOG("operator=(NString&& other)");
    if (this == &other) return *this;

    delete[] m_str;
    m_len = other.m_len;
    m_str = other.m_str;

    other.m_len = 0;
    other.m_str = nullptr;
    return *this;
}

NString &NString::operator=(const char* str)
{
    if (!str)
        throw std::invalid_argument("Nullptr assignment");

    delete[] m_str;
    m_len = strlen(str);
    m_str = new char[m_len + 1];
    strcpy_s(m_str, m_len + 1, str);
    return *this;
}

char &NString::operator[](size_t i)
{
    if (i >= m_len)
        throw std::out_of_range("Index out of bounds");
    return m_str[i];
}

char NString::operator[](size_t i) const
{
    if (i >= m_len)
        throw std::out_of_range("Index out of bounds");
    return m_str[i];
}

void NString::clear()
{
    delete[] m_str;
    m_str = nullptr;
    m_len = 0;
}

void NString::resize(size_t size)
{
    char* newStr = new char[size];
    strcpy_s(newStr, size, m_str);

    delete[] m_str;
    m_str = newStr;
    m_len = size;
}

void NString::append(const char* str)
{
    if (!str)
        throw std::invalid_argument("Nullptr append");

    size_t newLen = m_len + strlen(str);
    resize(newLen + 1);
    strcat_s(m_str, newLen + 1, str);
    m_len = newLen;
}

void NString::append(char ch)
{
    char* newStr = new char[m_len + 2];
    
    if (m_str)
        strcpy_s(newStr, m_len + 2, m_str);
   
    newStr[m_len] = ch;
    newStr[m_len + 1] = '\0';

    delete[] m_str;
    m_str = newStr;
    ++m_len;
}


bool operator>(const NString& st1, const NString& st2)
{
    return std::strcmp(st1.m_str, st2.m_str) > 0;
}

bool operator<(const NString& st1, const NString& st2)
{
    return std::strcmp(st1.m_str, st2.m_str) < 0;
}

std::ostream& operator<<(std::ostream& os, const NString& st)
{
    return os << st.m_str;
}

std::istream& operator>>(std::istream& is, NString& str)
{
    while (is && std::isspace(is.peek()))
        is.get();

    str.clear();

    size_t capacity = 16;
    str.m_str = new char[capacity];
    size_t pos{};
    char ch{};

    while (is.get(ch) && !std::isspace(ch)) {
        if (pos + 1 >= capacity) {
            capacity *= 2;
            str.resize(capacity);
        }

        str.m_str[pos++] = ch;
    }
    str.m_str[pos] = '\0';
    str.m_len = pos;
    return is;
   /* char temp[NString::CIN_LIM];
    is >> temp;
    if (!is) {
        is.clear();
        is.ignore(255, '\n');
        return is;
    }
    str = temp;
    return is;*/
}

void getline(std::istream& is, NString& str, char delim)
{
    str.clear();
    size_t capacity = 16;
    str.m_str = new char[capacity];
    size_t pos{};
    char ch{};

    while (is.get(ch) && ch != delim) {
        if (pos + 1 >= capacity) {
            capacity *= 2;
            str.resize(capacity);
        }

        str.m_str[pos++] = ch;
    }
    str.m_str[pos] = '\0';
    str.m_len = pos;
}


