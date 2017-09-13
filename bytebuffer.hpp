#ifndef BYTE_BUFFER_HPP
#define BYTE_BUFFER_HPP

#include <fstream>

class byteBuffer {
private:

    unsigned char writeBuffer, readBuffer;
    int bitsWritten, bitsRead;

public:

    byteBuffer() :  writeBuffer(0), readBuffer(0), bitsWritten(0), bitsRead(0) {}

    void write(bool bit, std::ostream & output);
    void write(unsigned char byte, std::ostream & output);

    void outputBuffer(std::ostream & output);

    bool read(std::istream & input);
    unsigned char readByte(std::istream & input);

};

#endif