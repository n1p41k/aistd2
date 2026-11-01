#include "lab4.h"
#include <algorithm>

static long getFileSize(std::ifstream& file) {
    auto pos = file.tellg();
    file.seekg(0, std::ios::end);
    long size = file.tellg();
    file.seekg(pos);
    return size;
}

// Node 
Huffman::Node::Node(int f, unsigned char c)
    : freq(f), ch(c), left(nullptr), right(nullptr), symbolSet(256, false) {
    if (isLeaf()) symbolSet.setBitValue(c, true);
}

Huffman::Node::Node(Node* l, Node* r)
    : freq(l->freq + r->freq), ch(0), left(l), right(r), symbolSet(256, false) {
    // Объединяем множества символов детей
    for (int i = 0; i < 256; ++i) {
        if (l->symbolSet.bitValue(i) || r->symbolSet.bitValue(i))
            symbolSet.setBitValue(i, true);
    }
}
// является ли узел листом
bool Huffman::Node::isLeaf() const {
    return left == nullptr && right == nullptr;
}

// Huffman 
Huffman::Huffman() : root(nullptr) {
    for (int i = 0; i < 256; ++i) codeTable[i] = nullptr;
}

Huffman::~Huffman() {
    clearTree(root);
    clearCodes();
}

void Huffman::clearTree(Node* node) {
    if (!node) return;
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

void Huffman::clearCodes() {
    for (int i = 0; i < 256; ++i) {
        delete codeTable[i];
        codeTable[i] = nullptr;
    }
}

void Huffman::build(const std::string& text) {
    clearTree(root);
    clearCodes();
    root = nullptr;

    int freq[256] = { 0 };
    for (unsigned char c : text)
        ++freq[c];

    buildTreeFromFreq(freq);
}
//Метод строит дерево Хаффмана по массиву частот.
void Huffman::buildTreeFromFreq(const int freq[256]) {
    std::vector<Node*> nodes;
    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0) {
            nodes.push_back(new Node(freq[i], static_cast<unsigned char>(i)));
        }
    }

    if (nodes.empty()) {
        root = nullptr;
        return;
    }

    // Особый случай: только один символ
    if (nodes.size() == 1) {
        root = nodes[0];
        return;
    }

    // Строим дерево, объединяя узлы с наименьшей частотой
    while (nodes.size() > 1) {
        // Находим два узла с минимальной частотой (линейный поиск)
        int min1 = 0;
        for (size_t i = 1; i < nodes.size(); ++i)
            if (nodes[i]->freq < nodes[min1]->freq) min1 = i;
        Node* n1 = nodes[min1];
        nodes[min1] = nodes.back();
        nodes.pop_back();

        int min2 = 0;
        for (size_t i = 1; i < nodes.size(); ++i)
            if (nodes[i]->freq < nodes[min2]->freq) min2 = i;
        Node* n2 = nodes[min2];
        nodes[min2] = nodes.back();
        nodes.pop_back();

        Node* parent = new Node(n1, n2);
        nodes.push_back(parent);
    }

    root = nodes[0];
}

void Huffman::buildCodes(Node* node, const std::string& curCode) {
    if (!node) return;
    if (node->isLeaf()) {
        int len = curCode.length();
        BoolVector* bv = new BoolVector(len);
        for (int i = 0; i < len; ++i)
            bv->setBitValue(i, curCode[i] == '1');
        codeTable[node->ch] = bv;
        return;
    }
    buildCodes(node->left, curCode + "0");
    buildCodes(node->right, curCode + "1");
}

void Huffman::ensureCodes() {
    clearCodes();
    if (!root) return;
    if (root->isLeaf()) {
        // Единственный символ — код "0"
        BoolVector* bv = new BoolVector(1);
        bv->setBitValue(0, false);   // '0'
        codeTable[root->ch] = bv;
    }
    else {
        buildCodes(root, "");
    }
}

double Huffman::encode(const std::string& input, std::string& output) {
    if (input.empty()) return -1.0;
    if (!root) build(input);
    if (!root) return -1.0;
    ensureCodes();

    output.clear();
    for (unsigned char c : input) {
        BoolVector* code = codeTable[c];
        if (!code) return -1.0;   // символ отсутствует в дереве
        for (int i = 0; i < code->length(); ++i)
            output.push_back(code->bitValue(i) ? '1' : '0');
    }

    // Коэффициент = исходные байты / длина закодированной строки (бит хранится как байт)
    return static_cast<double>(input.size()) / output.size();
}

bool Huffman::decode(const std::string& input, std::string& output) {
    if (!root) return false;
    if (root->isLeaf()) {
        for (size_t i = 0; i < input.length(); ++i) { 
            output.push_back(static_cast<char>(root->ch));
        }
        return true;
    }
    output.clear();
    const Node* cur = root;
    for (char bit : input) {
        if (bit == '0')
            cur = cur->left;
        else if (bit == '1')
            cur = cur->right;
        else
            return false;

        if (!cur) return false;

        if (cur->isLeaf()) {
            output.push_back(static_cast<char>(cur->ch));
            cur = root;
        }
    }
    return cur == root;
}

double Huffman::encodeFile(const std::string& inFile, const std::string& outFile) {
    // Открываем входной файл и читаем все данные
    std::ifstream in(inFile, std::ios::binary);
    if (!in.is_open()) return -1.0;

    long fileSize = getFileSize(in);
    if (fileSize <= 0) return -1.0;

    std::vector<unsigned char> data(fileSize);
    in.read(reinterpret_cast<char*>(data.data()), fileSize);
    in.close();

    // Подсчёт частот
    int freq[256] = { 0 };
    for (unsigned char c : data) ++freq[c];

    // Построение дерева
    clearTree(root);
    clearCodes();
    root = nullptr;
    buildTreeFromFreq(freq);
    if (!root) return -1.0;
    ensureCodes();

    // Вычисляем общее количество бит в закодированных данных
    unsigned int totalBits = 0;
    for (unsigned char c : data) {
        BoolVector* code = codeTable[c];
        if (!code) return -1.0;
        totalBits += code->length();
    }

    // Упаковываем биты в байты
    int bytesToWrite = (totalBits + 7) / 8;
    std::vector<unsigned char> outBytes(bytesToWrite, 0);
    unsigned int bitPos = 0;
    for (unsigned char c : data) {
        BoolVector* code = codeTable[c];
        for (int i = 0; i < code->length(); ++i) {
            if (code->bitValue(i)) {
                int byteIdx = bitPos / 8;
                int bitIdx = bitPos % 8;
                outBytes[byteIdx] |= (1 << (7 - bitIdx));
            }
            ++bitPos;
        }
    }

    // Запись в выходной файл
    std::ofstream out(outFile, std::ios::binary);
    if (!out.is_open()) return -1.0;

    out.write(reinterpret_cast<const char*>(&totalBits), sizeof(totalBits));
    writeTree(root, out);
    out.write(reinterpret_cast<const char*>(outBytes.data()), bytesToWrite);
    out.close();

    return static_cast<double>(fileSize) / bytesToWrite;
}

bool Huffman::decodeFile(const std::string& inFile, const std::string& outFile) {
    std::ifstream in(inFile, std::ios::binary);
    if (!in.is_open()) return false;
    

    // Читаем количество бит
    unsigned int totalBits;
    in.read(reinterpret_cast<char*>(&totalBits), sizeof(totalBits));
    if (!in) return false;

    // Читаем дерево
    Node* newRoot = readTree(in);
    if (!newRoot) return false;

    // Читаем упакованные данные
    int bytesToRead = (totalBits + 7) / 8;
    std::vector<unsigned char> encData(bytesToRead);
    in.read(reinterpret_cast<char*>(encData.data()), bytesToRead);
    if (!in) {
        clearTree(newRoot);
        return false;
    }
    in.close();

    // Декодируем биты
    std::string decoded;
    const Node* cur = newRoot;
    for (unsigned int i = 0; i < totalBits; ++i) {
        int byteIdx = i / 8;
        int bitIdx = i % 8;
        bool bit = (encData[byteIdx] >> (7 - bitIdx)) & 1;

        if (bit)
            cur = cur->right;
        else
            cur = cur->left;

        if (!cur) {
            clearTree(newRoot);
            return false;
        }

        if (cur->isLeaf()) {
            decoded.push_back(static_cast<char>(cur->ch));
            cur = newRoot;
        }
    }

    if (cur != newRoot) {
        clearTree(newRoot);
        return false;
    }

    // Запись декодированных данных
    std::ofstream out(outFile, std::ios::binary);
    if (!out.is_open()) {
        clearTree(newRoot);
        return false;
    }
    out.write(decoded.data(), decoded.size());
    out.close();

    // Заменяем текущее дерево на восстановленное
    clearTree(root);
    clearCodes();
    root = newRoot;
    ensureCodes();   // строим таблицу кодов для нового дерева

    return true;
}

bool Huffman::exportTree(const std::string& filename) const {
    if (!root) return false;
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) return false;
    writeTree(root, out);
    out.close();
    return true;
}

bool Huffman::importTree(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) return false;
    Node* newRoot = readTree(in);
    in.close();
    if (!newRoot) return false;
    clearTree(root);
    clearCodes();
    root = newRoot;
    ensureCodes();
    return true;
}

void Huffman::writeTree(Node* node, std::ofstream& out) const {
    if (!node) return;
    if (node->isLeaf()) {
        unsigned char marker = 1;
        out.write(reinterpret_cast<const char*>(&marker), 1);
        out.write(reinterpret_cast<const char*>(&node->ch), 1);
    }
    else {
        unsigned char marker = 0;
        out.write(reinterpret_cast<const char*>(&marker), 1);
        writeTree(node->left, out);
        writeTree(node->right, out);
    }
}

Huffman::Node* Huffman::readTree(std::ifstream& in) {
    unsigned char marker;
    if (!in.read(reinterpret_cast<char*>(&marker), 1)) return nullptr;
    if (marker == 1) {   // лист
        unsigned char ch;
        if (!in.read(reinterpret_cast<char*>(&ch), 1)) return nullptr;
        return new Node(0, ch);   // частота не важна
    }
    else if (marker == 0) {     // внутренний узел
        Node* left = readTree(in);
        Node* right = readTree(in);
        if (!left || !right) {
            delete left;
            delete right;
            return nullptr;
        }
        return new Node(left, right);
    }
    return nullptr;
}