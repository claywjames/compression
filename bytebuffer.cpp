#include "bytebuffer.hpp"

void byteBuffer::write(bool bit, std::ostream & output) {

    writeBuffer <<= 1; //make room for new bit

    if (bit) writeBuffer |= 1; //if its a 1 bit, add it

    if (++bitsWritten == 8) {
        output.write( (char*) &writeBuffer, 1);
        bitsWritten = 0;
    }

}

void byteBuffer::write(unsigned char byte, std::ostream & output) {
    
    int remainingBits = 8 - bitsWritten;

    unsigned char first = byte >> bitsWritten; //portion of byte able to be added to current writeBuffer

    writeBuffer <<= remainingBits;
    writeBuffer |= first;

    output.write( (char*) &writeBuffer, 1);

    writeBuffer = byte;

}

void byteBuffer::outputBuffer(std::ostream & output) {

    int remainingBits = 8 - bitsWritten;

    writeBuffer <<= remainingBits;

    output.write( (char*) &writeBuffer, 1);

    bitsWritten = 0;

}

bool byteBuffer::read(std::istream & input) {
    
    if (bitsRead == 0) {
        input.read( (char*)&readBuffer, 1);
    }

    unsigned char temp = readBuffer;
    temp >>= 7;

    readBuffer <<= 1;
    
    if (++bitsRead == 8) bitsRead = 0;

    return temp;

}

unsigned char byteBuffer::readByte(std::istream & input) {

    unsigned char byte = 0;
    for (int i = 0; i < 8; ++i) {

        byte <<= 1;
        if (read(input)) {
            byte |= 1;
        }

    }

    return byte;

}