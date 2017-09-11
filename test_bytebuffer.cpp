#include "bytebuffer.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

int main() {

    {
        byteBuffer bb;
        std::ostringstream out;

        bb.write('a', out);

        assert(out.str() == "a");
    }
    {
        byteBuffer bb;
        std::ostringstream out;

        //writing out 01000001, signed binary for 65 (an ascii A)
        bb.write(false, out);
        bb.write(true, out);
        for (int i = 0; i < 5; ++i) bb.write(false, out);
        bb.write(true, out);

        assert(out.str() == "A");
    }
    {
        byteBuffer bb;
        std::ostringstream out;

        bb.write('c', out);
        bb.write('l', out);
        bb.write('a', out);
        bb.write('y', out);

        assert(out.str() == "clay");
    }
    {
        byteBuffer bb;
        std::ostringstream out;

        //writing out 00100000, an ascii space
        bb.write(false, out);
        bb.write('A', out);

        assert(out.str() == " ");
    }
    {
        byteBuffer bb;
        std::ostringstream out;

        //writing out 01001100, an ascii L
        bb.write(false, out);
        bb.write(true, out);
        bb.write(false, out);
        bb.write('a', out);

        assert(out.str() == "L");

        //buffer begins with left over 00000001 from writing the 'a' byte
        bb.write(true, out); //now buffer is 00000011
        bb.write('H', out);  //now buffer is 00110100, an ascii '4'
        
        assert(out.str() == "L4");
    }
    {
        byteBuffer bb;
        std::ostringstream out;

        //writes out 01110000, an ascii p
        bb.write(false, out);
        bb.write(true, out);
        bb.write(true, out);
        bb.write(true, out);
        bb.outputBuffer(out);

        assert(out.str() == "p");
    }
    {
        byteBuffer bb;
        std::istringstream in("A");

        //checking the first nibble of an ascii A
        assert(!bb.read(in));
        assert(bb.read(in));
        assert(!bb.read(in));
        assert(!bb.read(in));
    }
    {
        byteBuffer bb;
        std::istringstream in("Vitalik Buterin");

        assert(bb.readByte(in) == 'V');
        assert(!bb.read(in));
        assert(bb.read(in));
    }
    {
        byteBuffer bb;
        std::istringstream in("Clay");

        // Cl == 01000011 01101100
        // 00110110 == 54
        assert(!bb.read(in));
        assert(bb.read(in));
        assert(!bb.read(in));
        assert(!bb.read(in));
        assert(bb.readByte(in) == 54);
    }

    std::cout << "Done testing bytebuffer" << std::endl;

    return 0;
}