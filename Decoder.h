#pragma once 
#include "Huffman.h"
#include <vector>
#include <cstdint>

class HuffmanDecoder {
    private:
        Node* root;
        int freq[256];
        int tot_len;
        std::vector<uint8_t> compressed;
        std::set <std::pair<int, Node*>> s;

        void add(int freq, int symbol, Node* left = nullptr, Node* right = nullptr);
        void remTree(Node* curr);
        void buildTree();
        void readData();
    public:
        HuffmanDecoder(std::vector<uint8_t> inp);
        ~HuffmanDecoder();
        std::vector<uint8_t> decompress();
};