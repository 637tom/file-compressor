#include <iostream>
#include <vector>
#include <string>
#include "Encoder.h"
#include "Decoder.h"
#include <fstream>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

std::vector<uint8_t> pack_file(fs::path& file_path, fs::path& cut_file_path) {
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cout << "Unable to open file" << file_path << "\n";
        return {};
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> data(size);
    file.read((char*)data.data(), size);
    file.close();
    HuffmanEncoder encoder(data);
    std::vector<uint8_t> compressed = encoder.compress();
    std::reverse(compressed.begin(), compressed.end());
    unsigned int comp_size = compressed.size();
    uint8_t* comp_size_bytes = (uint8_t*)&comp_size;
    std::vector<uint8_t> res = compressed;
    for(int i = 3; i >= 0; i --) {
        res.push_back(comp_size_bytes[i]);
    }
    std::string cut_file_path_str = cut_file_path.string();
    for(int i = cut_file_path_str.size() - 1; i >= 0; i--) {
        res.push_back(cut_file_path_str[i]);
    }
    unsigned int l = cut_file_path_str.size();
    uint8_t* l_bytes = (uint8_t*)&l;
    for(int i = 3; i >= 0; i--) {
        res.push_back(l_bytes[i]);
    }
    std::reverse(res.begin(), res.end());
    return res;
}

std::vector<uint8_t> pack_folder(const fs::path& folder_path) {
    std::vector<uint8_t> res;
    for(auto& file : fs::directory_iterator(folder_path)) {
        if(file.is_regular_file()) {
            fs::path file_path = file.path();
            fs::path cut_file_path = file.path();
            cut_file_path = cut_file_path.lexically_relative(folder_path);
            std::vector<uint8_t> curr = pack_file(file_path, cut_file_path);
            res.insert(res.end(), curr.begin(), curr.end());
        }
    }
    return res;
}
int main() {
    std::string testString = "ABR";
    while(testString.size() < 10000) {
        testString += testString;
    }
    std::vector<uint8_t> originalData(testString.begin(), testString.end());
    
    HuffmanEncoder encoder(originalData);
    std::vector<uint8_t> compressed = encoder.compress();

    std::cout << "orginal: " << originalData.size() << " compressed: " << compressed.size() << "\n";

    HuffmanDecoder decoder(compressed);
    std::vector<uint8_t> decompressed = decoder.decompress();
    
    std::string resultString(decompressed.begin(), decompressed.end());

    if (testString == resultString) {
        std::cout << "OK \n";
    } else {
        std::cout << "WRONG \n";
    }

    return 0;
}