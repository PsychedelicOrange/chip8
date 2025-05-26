//
// Created by parth on 26/5/25.
//

#ifndef UTIL_H
#define UTIL_H
#include <cstdint>
#include <string>
#include <vector>

namespace util {

    std::string read_string_from_file(char* filePath);
    std::vector<uint8_t> read_byte_array_from_file(char* filePath);

} // util

#endif //UTIL_H
