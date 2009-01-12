#ifndef PIMEXCEPTION_H
#define PIMEXCEPTION_H

#include <stdexcept>

class PimException
    : public std::logic_error
{
public:
    PimException(const char* file, int line, const std::string& msg)
        : std::logic_error(msg), m_file(file), m_line(line)
    {}
    virtual ~PimException() throw()
    {}
    const char* file() const
    {   return m_file;  }
    int line() const
    {   return m_line;  }
private:
    const char* m_file;
    int m_line;
};

#define PIMEXCEPTION(x) PimException(__FILE__, __LINE__, x)

#endif /* PIMEXCEPTION_H */

