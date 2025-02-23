#pragma once

#include "net/socket.hpp"
#include "types.hpp"
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
    ConnectionType& m_connection;
    char m_in_buffer[buffer_size];
    size_t m_in_pos = 0;
    size_t m_available_data = 0;

    char m_out_buffer[buffer_size];
    size_t m_out_pos = 0;

    void read_more()
    {
        m_connection.wait_for_data();
        int ret = m_connection.socket().recv(m_in_buffer, buffer_size);
        if (ret == -1) {
            throw std::runtime_error("Failed to read from socket");
        }
        m_available_data = (size_t) ret;
        m_in_pos = 0;
    }

    void check_in_boundary()
    {
        if (m_in_pos >= m_available_data) {
            read_more();
        }
    }

    bool flush_out()
    {
        if (m_out_pos == 0) return true;
        int ret = m_connection.socket().send(m_out_buffer, buffer_size);
        if (ret <= 0) return false;
        m_out_pos = 0;
        return true;
    }
    bool check_out_boundary()
    {
        if (m_out_pos >= buffer_size) {
            return flush_out();
        }
        return true;
    }

public:
    explicit NetworkStream(ConnectionType& connection) : m_connection(connection), m_in_buffer{}, m_out_buffer{}
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
        if (flush_out()) {
            return 0;
        }
        return -1;
    }

    // Get area
    std::streamsize showmanyc() override
    {
        return m_available_data - m_in_pos;
    }
    std::streamsize xsgetn(char_type* __s, std::streamsize __n) override
    {
        for (std::streamsize i = 0; i < __n; ++i) {
            check_in_boundary();
            if (showmanyc() == 0) {
                return i;
            }
            __s[i] = m_in_buffer[m_in_pos++];
        }
        return __n;
    }
    int_type underflow() override
    {
        check_in_boundary();
        if (showmanyc() == 0) {
            return traits_type::eof();
        }
        return m_in_buffer[m_in_pos];
    }
    int_type uflow() override
    {
        check_in_boundary();
        if (showmanyc() == 0) {
            return traits_type::eof();
        }
        return m_in_buffer[m_in_pos++];
    }
    int_type pbackfail(int_type) override
    {
        return traits_type::eof();
    }

    // Put Area
    std::streamsize xsputn(const char_type* buf, std::streamsize count) override
    {
        for (std::streamsize i = 0; i < count; ++i) {
            if (overflow(buf[i]) == traits_type::eof()) {
                return i;
            }
        }
        return count;
    }
    int_type overflow(int_type ch) override
    {
        m_out_buffer[m_out_pos++] = ch;
        if (check_out_boundary()) return 1;
        else
            return traits_type::eof();
    }
};

}// namespace espresso