#pragma once

#include <iostream>
#include <algorithm>

namespace network {

class icmp_header {
    unsigned char rep[8];

public:
    enum {
        echo_reply = 0, destination_unreachable = 3, source_quench = 4,
        redirect = 5, echo_request = 8, time_exceeded = 11, parameter_problem = 12,
        timestamp_request = 13, timestamp_reply = 14, info_request = 15,
        info_reply = 16, address_request = 17, address_reply = 18 
    };

    icmp_header();

    unsigned char type() const;
    unsigned char code() const;
    unsigned short check_sum() const;
    unsigned short id() const;
    unsigned short seq_num() const;

    void type(unsigned char n);
    void code(unsigned char n);
    void check_sum(unsigned short n);
    void id(unsigned short n);
    void seq_num(unsigned short n);

    friend std::istream& operator>>(std::istream& is, icmp_header& header) { 
        return is.read(reinterpret_cast<char*>(header.rep), 8); 
    }

    friend std::ostream& operator<<(std::ostream& os, const icmp_header& header) { 
        return os.write(reinterpret_cast<const char*>(header.rep), 8); 
    }


private:
    unsigned short decode(int a, int b) const;
    void encode(int a, int b, unsigned short n);
};


template<typename Iterator>
void compute_check_sum(icmp_header& header, Iterator begin, Iterator end);

unsigned short get_id();

}