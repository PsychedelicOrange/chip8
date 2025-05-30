//
// Created by parth on 26/5/25.
//

#include "Util.h"

#include <cstdint>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
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

    std::vector<uint8_t> read_byte_array_from_file(const char* filePath)
    {
        std::ifstream file;
        file.open(filePath, std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
            std::cerr << "file doesn't exist!" << std::endl;
            exit(1);
        }
        file.seekg(0, std::ios_base::end);
        auto length = file.tellg();
        file.seekg(0, std::ios_base::beg);

        std::vector<uint8_t> bytes(length);

        file.read(reinterpret_cast<std::istream::char_type*>(bytes.data()), length);

        file.close();
        return bytes;
    }

    uint8_t get_random_byte()
    {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(0, 255);
            return static_cast<uint8_t>(distrib(gen));
    }
} // util