#pragma once

#include <cassert>
#include <iostream>
#include <string>

class cint {
  private:
    int m_i;

  public:
    cint() = delete;
    cint(int i) : m_i{i} {
        ++instances;
        std::cout << "add " << i << std::endl;
    }

    cint(const cint& src) : m_i{src.m_i} {
        ++instances;
        std::cout << "copy " << m_i << std::endl;
    }

    cint(cint&& src) : m_i{src.m_i} {
        ++instances;
        std::cout << "move " << m_i << std::endl;
    }

    cint& operator=(const cint& src) {
        m_i = src.m_i;
        std::cout << "copy= " << m_i << std::endl;
        return *this;
    }

    cint& operator=(cint&& src) {
        m_i = src.m_i;
        std::cout << "move= " << m_i << std::endl;
        return *this;
    }

    ~cint() {
        --instances;
        std::cout << "~" << m_i << std::endl;
    }

    int get_i() const noexcept {
        return m_i;
    }

    void set_i(int i) noexcept {
        m_i = i;
    }

    static int instances;
};

inline std::ostream& operator<<(std::ostream& out, const cint& point) {
    out << "cint_" << point.get_i();
    return out;
}
