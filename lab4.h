#pragma once

#include "C:\Users\nikna\Nikitos\aistd2\lab4\lab4\oop3.h"
#include <string>
#include <vector>
#include <fstream>

class Huffman {
public:
    Huffman();
    ~Huffman();

    // Работа со строками
    void build(const std::string& text);
    double encode(const std::string& input, std::string& output);
    bool decode(const std::string& input, std::string& output);

    // Работа с файлами 
    double encodeFile(const std::string& inFile, const std::string& outFile);
    bool decodeFile(const std::string& inFile, const std::string& outFile);

    // Экспорт/импорт дерева
    bool exportTree(const std::string& filename) const;
    bool importTree(const std::string& filename);

private:
    struct Node {
        int freq;               // частота 
        unsigned char ch;       // символ (только для листа)
        Node* left;
        Node* right;
        BoolVector symbolSet;   // какие символы есть в поддереве 

        Node(int f, unsigned char c = 0);
        Node(Node* l, Node* r);
        bool isLeaf() const;
    };

    Node* root;                 // корень дерева Хаффмана
    BoolVector* codeTable[256]; // коды символов (индекс = код символа)

    void clearTree(Node* node);
    void clearCodes();
    void buildTreeFromFreq(const int freq[256]);
    void buildCodes(Node* node, const std::string& curCode);
    void ensureCodes();          // построить codeTable по текущему дереву
    void writeTree(Node* node, std::ofstream& out) const;
    Node* readTree(std::ifstream& in);
};
