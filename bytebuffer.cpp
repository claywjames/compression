#include "bytebuffer.hpp"

void byteBuffer::write(bool bit, std::ostream & output) {

    writeBuffer <<= 1; //make room for new bit

    if (bit) writeBuffer |= 1; //if its a 1 bit, add it

    if (++bitsWritten == 8) {
        output.write(&writeBuffer, 1);
        bitsWritten = 0;
    }

}

void byteBuffer::write(char byte, std::ostream & output) {
    
    int remainingBits = 8 - bitsWritten;

    char first = byte >> bitsWritten; //portion of byte able to be added to current writeBuffer

    writeBuffer <<= remainingBits;
    writeBuffer |= first;

    output.write(&writeBuffer, 1);

    writeBuffer = byte;

}

void byteBuffer::outputBuffer(std::ostream & output) {

    int remainingBits = 8 - bitsWritten;

    writeBuffer <<= remainingBits;

    output.write(&writeBuffer, 1);

    bitsWritten = 0;

}

bool byteBuffer::read(std::istream & input) {
    
    if (bitsRead == 0) {
        input.read(&readBuffer, 1);
    }

    char temp = readBuffer;
    temp >>= 7;

    readBuffer <<= 1;
    
    if (++bitsRead == 8) bitsRead = 0;

    return temp;

}

char byteBuffer::readByte(std::istream & input) {

    char byte = 0;
    for (int i = 0; i < 8; ++i) {

        byte <<= 1;
        if (read(input)) {
            byte |= 1;
        }

    }

    return byte;

}