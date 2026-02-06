#pragma once 
#include "Huffman.h"
#include <vector>
#include <cstdint>
#include <set>

class HuffmanDecoder {
    private:
        Node* root;
        unsigned int freq[256];
        unsigned int tot_len;
        std::vector<uint8_t> compressed;
        std::set <std::pair<unsigned int, Node*>> s;

        void add(unsigned int freq, uint8_t symbol, Node* left = nullptr, Node* right = nullptr);
        void remTree(Node* curr);
        void buildTree();
        void readData();
    public:
        HuffmanDecoder(std::vector<uint8_t> inp);
        ~HuffmanDecoder();
        std::vector<uint8_t> decompress();
};