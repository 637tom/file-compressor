#include <iostream>
#include <set>
#include <utility>
#include <vector>

struct Node {
    Node* left;
    Node* right;
    int symbol;
};

class Huffman {
    private:
        Node* root;
        set <pair<int, Node*>> s;
        std::vector<unsigned char> text;
        int freq[256];

        void remTree(Node* curr) {
            if(curr == nullptr) return;
            remTree(curr->left);
            remTree(curr->right);
            delete curr;
        }
    public:
        Huffman(std::vector<unsigned char> inp) {
            text = inp;
            root = nullptr;
            for(int i = 0; i < 256; i++) {
                freq[i] = 0;
            }
        }
        ~Huffman() {
            remTree(root);
        }
        void cntFreq() {
            for(auto ch : text) {
                freq[ch]++;
            }
        }
        void buildTree() {
        }
};

int main() {

}