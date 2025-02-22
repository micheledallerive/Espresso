#pragma once

#include "net/socket.hpp"
#include <net/connection.hpp>
#include <streambuf>
#include <unistd.h>

namespace espresso {

/**
 * Stream that retrieves characters from the newtork.
*/
template<typename ConnectionType>
class NetworkStream : public std::basic_streambuf<char> {
private:
    static constexpr size_t buffer_size = 1024;
    ConnectionType &m_connection;
    char m_buffer[buffer_size];
    int m_pos = 0;
    ssize_t m_available_data = 0;

    void read_more()
    {
        m_connection.wait_for_data();
        m_available_data = m_connection.socket().recv(m_buffer, buffer_size);
        if (m_available_data == -1) {
            throw std::runtime_error("Failed to read from socket");
        }
        m_pos = 0;
    }

    void check_boundary()
    {
        if (m_pos >= m_available_data) {
            read_more();
        }
    }

public:
    explicit NetworkStream(ConnectionType &connection) : m_connection(connection), m_buffer{}
    {
    }
    ~NetworkStream() override = default;

protected:
    void imbue(const std::locale&) override
    {
    }

    // Positioning
    basic_streambuf<char_type, std::char_traits<char>>* setbuf(char_type*, std::streamsize) override
    {
        return nullptr;
    }
    pos_type seekoff(off_type, std::ios_base::seekdir, std::ios_base::openmode) override
    {
        return pos_type(off_type(-1));
    }
    pos_type seekpos(pos_type, std::ios_base::openmode) override
    {
        return pos_type(off_type(-1));
    }
    int sync() override
    {
        return 0;
    }

    // Get area
    std::streamsize showmanyc() override
    {
        return m_available_data - m_pos;
    }
    std::streamsize xsgetn(char_type* __s, std::streamsize __n) override
    {
        for (std::streamsize i = 0; i < __n; ++i) {
            check_boundary();
            if (showmanyc() == 0) {
                return i;
            }
            __s[i] = m_buffer[m_pos++];
        }
        return __n;
    }
    int_type underflow() override
    {
        check_boundary();
        if (showmanyc() == 0) {
            return traits_type::eof();
        }
        return m_buffer[m_pos];
    }
    int_type uflow() override
    {
        check_boundary();
        if (showmanyc() == 0) {
            return traits_type::eof();
        }
        return m_buffer[m_pos++];
    }
    int_type pbackfail(int_type) override
    {
        return traits_type::eof();
    }

    // Put Area
    std::streamsize xsputn(const char_type*, std::streamsize) override
    {
        return 0;
    }
    int_type overflow(int_type) override
    {
        return traits_type::eof();
    }
};

}// namespace espresso