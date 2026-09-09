#include "icmp_header.hpp"

using namespace network;


icmp_header::icmp_header() {
    std::fill(rep, rep + sizeof(rep), 0);
}

unsigned char icmp_header::type() const { return rep[0]; }
unsigned char icmp_header::code() const { return rep[1]; }
unsigned short icmp_header::check_sum() const { return decode(2, 3); }
unsigned short icmp_header::id() const { return decode(4, 5); }
unsigned short icmp_header::seq_num() const { return decode(6, 7); }

void icmp_header::type(unsigned char n) { rep[0] = n; }
void icmp_header::code(unsigned char n) { rep[1] = n; }
void icmp_header::check_sum(unsigned short n) { return encode(2, 3, n); }
void icmp_header::id(unsigned short n) { return encode(4, 5, n); }
void icmp_header::seq_num(unsigned short n) { return encode(6, 7, n); }

unsigned short icmp_header::decode(int a, int b) const {
    return (rep[a] << 8) + rep[b];
}
 
void icmp_header::encode(int a, int b, unsigned short n) {
    rep[a] = static_cast<unsigned char>(n >> 8);
    rep[b] = static_cast<unsigned char>(n & 0xFF);
}


template<typename Iterator>
void compute_check_sum(icmp_header& header, Iterator begin, Iterator end) {
    unsigned int sum{
        (header.type() << 8) + header.code() + header.id() + header.seq_num()
    };

    Iterator body_iter{begin};

    while(body_iter != end) {
        sum += (static_case<unsigned char>(*body_iter++) << 8);

        if(body_iter != end)
            sum += static_case<unsigned char>(*body_iter++);
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);

    header.check_sum(static_case<unsigned short>(~sum));
}