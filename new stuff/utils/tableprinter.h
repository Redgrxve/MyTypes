#ifndef TABLEPRINTER_H
#define TABLEPRINTER_H

#include "nvector.h"
#include <cstdint>
#include <ostream>
#include <iomanip>

class TablePrinter
{
public:
    TablePrinter(std::ostream& outStream);

    void addColumn(uint16_t columnWidth);
    void addColumn(const std::string& header, uint16_t columnWidth);
    void printHeader();
    void printHorizontalLine();

    template<typename T>
    TablePrinter& operator<<(const T &value) {
        if (m_currCol == 0)
            m_outStream << "|";

        m_outStream << std::setw(m_columnWidths[m_currCol]) << value;
        m_outStream << "|";
        ++m_currCol;

        if (m_currCol == m_columnsNum) {
            m_outStream << "\n";
            printHorizontalLine();
            m_currCol = 0;
        }

        return *this;
    }

private:
    std::ostream& m_outStream;

    NVector<std::string> m_headers{};
    NVector<int> m_columnWidths{};

    size_t m_columnsNum{};
    size_t m_tableWidth{};

    size_t m_currCol{};
};


#endif // TABLEPRINTER_H
