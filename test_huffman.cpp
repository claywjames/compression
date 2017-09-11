#include "huffman.hpp"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {

    {
        unsigned long long f[possibleBytes] = {0};
        std::istringstream in("");

        recordFrequencies(in, f);

        for (int n : f) assert(n == 0);

        //done testing recordFrequencies

        hnode * t = buildTree(f);

        assert(t == nullptr);

        //done testing buildTree

        huffCodeMap c;
        generateHuffCodes(t, huffCode(), c);

        assert(c.empty());

        //done testing generateHuffCodes
    }
    {
        unsigned long long f[possibleBytes] = {0};
        std::istringstream in("a");

        recordFrequencies(in, f);

        assert(f[97] == 1);

        //done testing recordFrequencies

        hnode * t = buildTree(f);

        assert(t->frequency == 1);

        //done testing buildTree

        huffCodeMap c;
        generateHuffCodes(t, huffCode(), c);

        huffCode h = {false};
        assert(c['a'] == h);

        //done testing generateHuffCodes
    }
    {
        unsigned long long f[possibleBytes] = {0};
        std::istringstream in("rracecar");

        recordFrequencies(in, f);

        assert(f[97] == 2);
        assert(f[99] == 2);
        assert(f[101] == 1);
        assert(f[114] == 3);
        assert(f[4] == 0);

        //done testing recordFrequencies

        internal_hnode * t = dynamic_cast<internal_hnode*>(buildTree(f));
        
        assert(t->left->frequency == 3);
        assert(t->right->frequency == 5);

        internal_hnode * u = dynamic_cast<internal_hnode*>(t->right);

        assert(u->left->frequency == 2);
        assert(u->right->frequency == 3);

        internal_hnode * v = dynamic_cast<internal_hnode*>(t->left);

        assert(v->left->frequency == 1);
        assert(v->right->frequency == 2);

        //done testing buildTree

        huffCodeMap c;
        generateHuffCodes(t, huffCode(), c);

        huffCode r = {true, true};
        assert(c['r'] == r);

        //this test assures that the huffman coding is as follows:
        //  a 01, c 10, e 00, r 11
        //this is not the only optimal solution but it is the one that should
        //result from this program
    }

    std::cout << "Done testing huffman coding" << std::endl;

    return 0;
}