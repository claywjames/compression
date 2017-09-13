#ifndef HUFFMAN_HPP_
#define HUFFMAN_HPP_

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <map>
#include <string>
#include <iterator>
#include "bytebuffer.hpp"

typedef std::vector<bool> huffCode;
typedef std::map< unsigned char, huffCode > huffCodeMap;

const int possibleBytes = 256;
const std::string programIdentifier = "16927";  //1952nd prime; used to verify files compressed with this program
const char seperatorByte = 0;

class hnode {
protected:

    hnode(unsigned long long f) : frequency(f) {}

public:

    const unsigned long long frequency;
    virtual ~hnode() {}

    virtual void writeTree(byteBuffer & b, std::ostream & out) const {};

};

class internal_hnode : public hnode {
public:

    hnode * left;
    hnode * right;

    internal_hnode(hnode * left, hnode * right) : hnode(left->frequency + right->frequency), left(left), right(right) {}
    ~internal_hnode() { delete left; delete right; } //recursive postorder traversal

    //encodes a preorder traversal of the tree to out
    void writeTree(byteBuffer & b, std::ostream & out) const {

        b.write(false, out);

        if (left) {
            left->writeTree(b, out);
        }

        if (right) {
            right->writeTree(b, out);
        }

    }

};

class leaf_hnode : public hnode {
public:

    const unsigned char character;
    leaf_hnode(unsigned long long f, unsigned char c) : hnode(f), character(c) {}

    //encodes a preorder traversal of the tree to out
    void writeTree(byteBuffer & b, std::ostream & out) const {

        b.write(true, out);
        b.write(character, out);

    }

};

void recordFrequencies(std::istream &, unsigned long long[]);

//used with priority queue to sort bytes by most frequent
struct hnodeCmp {
    bool operator()(const hnode * lhs, const hnode * rhs) {
        return lhs->frequency > rhs->frequency;
    }
};

hnode * buildTree(const unsigned long long[]);

void generateHuffCodes(const hnode*, const huffCode&, huffCodeMap&);

hnode * decodeTree(std::istream &, byteBuffer &);
char decodeNextByte(std::istream &, byteBuffer &, hnode *);

void compress(std::istream &, std::ostream &);
void decompress(std::istream &, std::ostream &);


#endif