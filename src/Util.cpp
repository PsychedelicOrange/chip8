//
// Created by parth on 26/5/25.
//

#include "Util.h"

#include <cstdint>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

namespace util {

    std::string read_string_from_file(char* filePath)
    {
         // fine for small files
        std::ifstream file(filePath);
        std::stringstream st;
        st << file.rdbuf();
        return st.str();
    }

    std::vector<uint8_t> read_byte_array_from_file(char* filePath)
    {
        std::ifstream file;
        file.open(filePath, std::ios::in | std::ios::binary);
        file.seekg(0, std::ios_base::end);
        auto length = file.tellg();
        file.seekg(0, std::ios_base::beg);

        std::vector<uint8_t> bytes(length);

        file.read(reinterpret_cast<std::istream::char_type*>(bytes.data()), length);

        file.close();
        return bytes;
    }
} // util