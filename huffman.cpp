#include "huffman.hpp"
#include "bytebuffer.hpp"

void recordFrequencies(std::istream & uncompressed, unsigned long long frequencies[]) {

    char nextChar;
    uncompressed.read(&nextChar, 1);
    while (!uncompressed.eof()) {
        ++frequencies[(unsigned char) nextChar];
        uncompressed.read(&nextChar, 1);
    }

}

hnode * buildTree(const unsigned long long frequencies[]) {

    std::priority_queue< hnode*, std::vector<hnode*>, hnodeCmp > trees;

    for (int character = 0; character < possibleBytes; ++character) {
        if (frequencies[character]) {
            trees.push(new leaf_hnode(frequencies[character], (char) character));
        }
    }
    
    while (trees.size() > 1) {

        hnode * leftChild = trees.top();
        trees.pop();

        hnode * rightChild = trees.top();
        trees.pop();

        trees.push(new internal_hnode(leftChild, rightChild));

    }
    
    if (trees.size())
        return trees.top();
    else
        return nullptr;

}

void generateHuffCodes(const hnode * node, const huffCode & prefix, huffCodeMap & codes) {

    //if node is a leaf node
    if (const leaf_hnode * leaf = dynamic_cast<const leaf_hnode*>(node)) {

        if (prefix.size()) {
            codes[leaf->character] = prefix;
        } else {
            //in the case of there being only one specific byte in a file
            codes[leaf->character] = {false};
        }

    //else if node is an internal node
    } else if (const internal_hnode * parent = dynamic_cast<const internal_hnode*>(node)) {

        huffCode leftPrefix = prefix;
        leftPrefix.push_back(false);
        generateHuffCodes(parent->left, leftPrefix, codes);

        huffCode rightPrefix = prefix;
        rightPrefix.push_back(true);
        generateHuffCodes(parent->right, rightPrefix, codes);

    }

}

void compress(std::istream & uncompressed, std::ostream & compressed) {
    
    unsigned long long frequencies[possibleBytes] = {0};
    recordFrequencies(uncompressed, frequencies);

    hnode * huffTree = buildTree(frequencies);

    if (!huffTree) {
        std::cerr << "cannot compress empty file\n";
        return;
    }

    huffCodeMap codes;
    generateHuffCodes(huffTree, huffCode(), codes);
    
    compressed.write(programIdentifier.c_str(), programIdentifier.size()); //used to verify this file as product of this program
    
    std::string numBytes = std::to_string(huffTree->frequency);
    compressed.write(numBytes.c_str(), numBytes.size());

    compressed.write(&seperatorByte, 1);

    byteBuffer b;

    huffTree->writeTree(b, compressed);

    //reseting the file since it was already read once to record frequencies
    uncompressed.clear();
    uncompressed.seekg(0);

    char nextChar;
    uncompressed.read(&nextChar, 1);
    while (!uncompressed.eof()) {
        for (bool bit : codes[(unsigned char) nextChar]) {
            b.write(bit, compressed);
        }
        uncompressed.read(&nextChar, 1);
    }

    b.outputBuffer(compressed);

    delete huffTree;

}

hnode * decodeTree(std::istream & in, byteBuffer & b) {
    
    hnode * h = nullptr;
    
    if (b.read(in)) {
        char byte = b.readByte(in);
        h = new leaf_hnode(0, byte); //frequency doesn't matter for decompression
    } else {
        hnode * left = decodeTree(in, b);
        hnode * right = decodeTree(in, b);
        h = new internal_hnode(left, right);
    }

    return h;

}

char decodeNextByte(std::istream & in, byteBuffer & b, hnode * h) {

    hnode * hTraversor = h;
    if (const internal_hnode * parent = dynamic_cast<const internal_hnode*>(hTraversor)) {
        if (b.read(in)) {
            return decodeNextByte(in, b, parent->right);
        } else {
            return decodeNextByte(in, b, parent->left);
        }
    } else {
        const leaf_hnode * leaf = dynamic_cast<const leaf_hnode*>(hTraversor);
        return leaf->character;
    }

}

void decompress(std::istream & compressed, std::ostream & uncompressed) {
    
    //checking that the compressed file was compressed by this program
    char * verification = new char[5];
    compressed.read(verification, 5);
    if (std::string(verification) != programIdentifier) { 
        std::cerr << "file was not compressed with this program.\n";
        return;
    }

    //checking the number of bytes in the original file
    unsigned long long numBytes;
    std::string s;
    char digit;
    compressed.read(&digit, 1);
    while (digit) {
        s += digit;
        compressed.read(&digit, 1);
    }
    numBytes = std::stoull(s);

    byteBuffer b;
    //recreating the huffman tree
    hnode * huffTree = decodeTree(compressed, b);

    //decompressing file
    for(unsigned long long bytesWritten = 0; bytesWritten < numBytes; ++bytesWritten) {
        char nextByte = decodeNextByte(compressed, b, huffTree);
        uncompressed.write(&nextByte, 1);
    }

    delete huffTree;
    
}