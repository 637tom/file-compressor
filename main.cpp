#include <iostream>
#include <vector>
#include <string>
#include "Encoder.h"
#include "Decoder.h"
#include <fstream>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

std::vector<uint8_t> read_file(fs::path& file_path) {
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
    return data;
}

std::vector<uint8_t> pack_file(fs::path& file_path, fs::path& cut_file_path) {
    std::vector<uint8_t> data = read_file(file_path);
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
    fs::path parent_path = folder_path.parent_path();
    std::vector<uint8_t> res;
    std::string folder_name = folder_path.filename().string();
    unsigned int folder_name_size = folder_name.size();
    uint8_t* folder_name_size_bytes = (uint8_t*)&folder_name_size;
    for(int i = 0; i < 4; i++) {
        res.push_back(folder_name_size_bytes[i]);
    }
    for(int i = 0; i < folder_name_size; i++) {
        res.push_back(folder_name[i]);
    }

    for(auto& file : fs::recursive_directory_iterator(folder_path)) {
        if(fs::is_regular_file(file)) {
            fs::path file_path = file.path();
            fs::path cut_file_path = file.path();
            cut_file_path = cut_file_path.lexically_relative(parent_path);
            std::vector<uint8_t> curr = pack_file(file_path, cut_file_path);
            res.insert(res.end(), curr.begin(), curr.end());
        }
    }
    return res;
}

void unpack_file(const fs::path& file_path, const fs::path& destination_path) {
    std::vector<uint8_t> data = read_file(file_path);
    if(data.size() == 0) {
        std::cout << "Empty file\n";
        return;
    }
    int wsk = 0;
    unsigned int folder_name_size = 0;
    uint8_t* folder_name_size_bytes = (uint8_t*)&folder_name_size;
    for(int i = 0; i < 4; i++) {
        folder_name_size_bytes[i] = data[wsk++];
    }
    std::string folder_name = "";
    for(int i = 0; i < folder_name_size; i++) {
        folder_name += data[wsk++];
    }
    fs::path folder_path = destination_path / folder_name;
    if(fs::exists(folder_path)) {
        std::cout << "Folder already exists\n";
        return;
    }
    fs::create_directories(folder_path);

    while(wsk < data.size()) {
        unsigned int curr_file_path_size = 0;
        uint8_t* curr_file_path_size_bytes = (uint8_t*)&curr_file_path_size;
        for(int i = 0; i < 4; i++) {
            curr_file_path_size_bytes[i] = data[wsk++];
        }
        std::string curr_file_path_str = "";
        for(int i = 0; i < curr_file_path_size; i++) {
            curr_file_path_str += data[wsk++];
        }
        fs::path curr_file_path = curr_file_path_str;
        fs::path curr_destination_path = destination_path / curr_file_path; 
        fs::path curr_parent = curr_destination_path.parent_path();
        if(!fs::exists(curr_parent)) {
            fs::create_directories(curr_parent);
        }
        unsigned int curr_file_size = 0;
        uint8_t* curr_file_size_bytes = (uint8_t*)&curr_file_size;
        for(int i = 0; i < 4; i++) {
            curr_file_size_bytes[i] = data[wsk++];
        }
        std::vector<uint8_t> curr_file_data(curr_file_size);
        for(int i = 0; i < curr_file_size; i++) {
            curr_file_data[i] = data[wsk++];
        }
        HuffmanDecoder decoder(curr_file_data);
        std::vector<uint8_t> decompressed = decoder.decompress();
        std::ofstream file(curr_destination_path, std::ios::binary);
        file.write((char*)decompressed.data(), decompressed.size());
        file.close();
    }
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