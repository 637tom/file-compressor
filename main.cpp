#include <iostream>
#include <vector>
#include <string>
#include "Encoder.h"
#include "Decoder.h"

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