#pragma once

#include <algorithm>
#include <boost/asio/ip/address_v4.hpp>
#include <istream>


namespace network {

class ipv4_header {
    unsigned char rep[60];

public:
    ipv4_header();

    unsigned char version() const;
    unsigned short header_length() const;
    unsigned char type_of_service() const;
    unsigned short total_length() const;
    unsigned short identification() const;
    bool dont_fragment() const;
    bool more_fragments() const;
    unsigned short fragment_offset() const;
    unsigned int time_to_live() const;
    unsigned char protocol() const;
    unsigned short header_checksum() const;

    boost::asio::ip::address_v4 source_address() const {
        boost::asio::ip::address_v4::bytes_type bytes = { { rep[12], rep[13], rep[14], rep[15] } };
    
        return boost::asio::ip::address_v4(bytes);
    }

    boost::asio::ip::address_v4 destination_address() const {
        boost::asio::ip::address_v4::bytes_type bytes = { { rep[16], rep[17], rep[18], rep[19] } };

        return boost::asio::ip::address_v4(bytes);
    }

    friend std::istream& operator>>(std::istream& is, ipv4_header& header) {
        is.read(reinterpret_cast<char*>(header.rep), 20);

        if (header.version() != 4)
            is.setstate(std::ios::failbit);

        std::streamsize options_length = header.header_length() - 20;

        if (options_length < 0 || options_length > 40)
            is.setstate(std::ios::failbit);
        else
            is.read(reinterpret_cast<char*>(header.rep) + 20, options_length);

        return is;
    }

private:
    unsigned short decode(int a, int b) const;
};

}