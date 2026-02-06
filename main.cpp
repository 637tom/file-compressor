#include <iostream>
#include <vector>
#include <string>
#include "Encoder.h"
#include "Decoder.h"
#include <fstream>
#include <algorithm>

std::vector<uint8_t> pack_file(const std::string& file_path, const std::string& cut_file_path) {
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
    for(int i = 0; i < 4; i ++) {
        res.push_back(comp_size_bytes[i]);
    }
    for(int i = 0; i < cut_file_path.size(); i++) {
        res.push_back(cut_file_path[i]);
    }
    std::reverse(res.begin(), res.end());
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