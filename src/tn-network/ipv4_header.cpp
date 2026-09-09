#include "ipv4_header.hpp"

using namespace network;


unsigned char ipv4_header::version() const { return (rep[0] >> 4) & 0xF; }
unsigned short ipv4_header::header_length() const { return (rep[0] & 0xF) * 4; }
unsigned char ipv4_header::type_of_service() const { return rep[1]; }
unsigned short ipv4_header::total_length() const { return decode(2, 3); }
unsigned short ipv4_header::identification() const { return decode(4, 5); }
bool ipv4_header::dont_fragment() const { return (rep[6] & 0x40) != 0; }
bool ipv4_header::more_fragments() const { return (rep[6] & 0x20) != 0; }
unsigned short ipv4_header::fragment_offset() const { return decode(6, 7) & 0x1FFF; }
unsigned int ipv4_header::time_to_live() const { return rep[8]; }
unsigned char ipv4_header::protocol() const { return rep[9]; }
unsigned short ipv4_header::header_checksum() const { return decode(10, 11); }

unsigned short ipv4_header::decode(int a, int b) const {
    return (rep[a] << 8) + rep[b];
}