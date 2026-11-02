#include "lab4.h"
#include <iostream>
#include <string>

int main() {
    Huffman huff;
    std::string original = "Hello, Huffman!";
    std::string encoded;
    double ratio = huff.encode(original, encoded);
    if (ratio > 0) {
        std::cout << "Original: " << original << std::endl;
        std::cout << "Encoded: " << encoded << std::endl;
        std::cout << "Ratio: " << ratio << std::endl;

        std::string decoded;
        if (huff.decode(encoded, decoded)) {
            std::cout << "Decoded: " << decoded << std::endl;
        }
    }
    return 0;
}