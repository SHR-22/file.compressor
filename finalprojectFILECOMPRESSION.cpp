#include<bits/stdc++.h>

using namespace std;

// Structure for a node in Huffman Tree
struct HuffmanNode {
    unsigned char data;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(unsigned char data, int frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

// Comparator for Huffman nodes
struct CompareNodes {
    bool operator()(HuffmanNode* const& lhs, HuffmanNode* const& rhs) {
        return lhs->frequency > rhs->frequency;
    }
};

// Function prototypes
vector<unsigned char> readFile(const string& filename);
void writeFile(const string& filename, const vector<unsigned char>& data);
vector<unsigned char> compress(const vector<unsigned char>& data);
vector<unsigned char> decompress(const vector<unsigned char>& compressedData, const unordered_map<unsigned char, string>& huffmanCodes);

// Main class for Huffman Compression
class HuffCompression {
private:
    unordered_map<unsigned char, string> huffmap;

    // Create Huffman Tree
    HuffmanNode* createHuffmanTree(const unordered_map<unsigned char, int>& freqMap) {
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes> minHeap;
        for (auto& pair : freqMap) {
            minHeap.push(new HuffmanNode(pair.first, pair.second));
        }

        while (minHeap.size() > 1) {
            HuffmanNode* left = minHeap.top(); minHeap.pop();
            HuffmanNode* right = minHeap.top(); minHeap.pop();
            HuffmanNode* parent = new HuffmanNode(0, left->frequency + right->frequency);
            parent->left = left;
            parent->right = right;
            minHeap.push(parent);
        }

        return minHeap.top();
    }

    // Generate Huffman Codes
    void generateHuffCodes(HuffmanNode* root, string code) {
        if (root == nullptr) return;
        if (root->data != 0) {
            huffmap[root->data] = code;
        }
        generateHuffCodes(root->left, code + "0");
        generateHuffCodes(root->right, code + "1");
    }

public:
    // Compress data
    vector<unsigned char> compressData(const vector<unsigned char>& data) {
        unordered_map<unsigned char, int> freqMap;
        for (auto& ch : data) {
            freqMap[ch]++;
        }

        HuffmanNode* root = createHuffmanTree(freqMap);
        generateHuffCodes(root, "");

        vector<unsigned char> compressedData;
        string bits;
        for (auto& ch : data) {
            bits += huffmap[ch];
            while (bits.length() >= 8) {
                bitset<8> bits8(bits.substr(0, 8));
                compressedData.push_back(static_cast<unsigned char>(bits8.to_ulong()));
                bits.erase(0, 8);
            }
        }
        if (!bits.empty()) {
            bits.append(8 - bits.length(), '0');
            bitset<8> bits8(bits);
            compressedData.push_back(static_cast<unsigned char>(bits8.to_ulong()));
        }

        return compressedData;
    }

    // Decompress data
    vector<unsigned char> decompressData(const vector<unsigned char>& compressedData) {
        string bits;
        for (auto& byte : compressedData) {
            bits += bitset<8>(byte).to_string();
        }

        string code;
        vector<unsigned char> decompressedData;
        for (auto& bit : bits) {
            code += bit;
            for (auto& pair : huffmap) {
                if (pair.second == code) {
                    decompressedData.push_back(pair.first);
                    code.clear();
                    break;
                }
            }
        }
        return decompressedData;
    }
};

// Helper functions to read and write files
vector<unsigned char> readFile(const string& filename) {
    ifstream file(filename, ios::binary | ios::ate);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }
    streampos size = file.tellg();
    vector<unsigned char> buffer(size);
    file.seekg(0, ios::beg);
    file.read(reinterpret_cast<char*>(&buffer[0]), size);
    file.close();
    return buffer;
}

void writeFile(const string& filename, const vector<unsigned char>& data) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }
    file.write(reinterpret_cast<const char*>(&data[0]), data.size());
    file.close();
}

int main() {
    string inputFile = "C:\\Users\\shreyansh\\OneDrive\\Desktop\\New Text Document.txt";
    string compressedFile = "C:\\Users\\shreyansh\\OneDrive\\Desktop\\vishwas";
    string decompressedFile = "C:\\Users\\shreyansh\\OneDrive\\Desktop\\decomvishwas";

    // Read input data from file
    vector<unsigned char> inputData = readFile(inputFile);

    // Huffman Compression
    HuffCompression huff;
    vector<unsigned char> compressedData = huff.compressData(inputData);

    // Write compressed data to file
    writeFile(compressedFile, compressedData);

    // Decompression
    vector<unsigned char> decompressedData = huff.decompressData(compressedData);

    // Write decompressed data to file
    writeFile(decompressedFile, decompressedData);

    return 0;
}
