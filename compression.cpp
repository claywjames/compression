#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include "huffman.hpp"

long long getFileSize(std::string);

int main(int argc, char * argv[]) {

    if (argc != 3) {
        std::cerr << "Incorrect number of arguments.\n";
        return 1;
    }

    std::string filename(argv[2]);
    std::string basename = filename.substr(0, filename.rfind('.'));

    std::ifstream input(filename, std::ios::in | std::ios::binary);
    if (!input.is_open()) {
            std::cerr << "Unable to open file.\n";
            return 1;
    }

    std::ofstream output;

    if (argv[1][1] == 'c') {

        long long filesize = getFileSize(filename);

        std::cout << "Input file size: " << filesize / 1024 << "kb\n";

        output.open(filename + ".huff", std::ios::out | std::ios::binary);

        compress(input, output);

        output.close(); //committing changes so size can be measured
        output.open(filename + ".huff", std::ios::in | std::ios::binary);
        
        long long compressedSize = getFileSize(filename + ".huff");

        std::cout << "Compressed file size: " << compressedSize / 1024 << "kb\n";

    } else if (argv[1][1] == 'd') {

        output.open(basename, std::ios::out | std::ios::binary);
        decompress(input, output);

    } else {
        std::cerr << "Invalid arguments.\n";
        return 1;
    }

    input.close();
    output.close();

    std::remove(filename.c_str());

    return 0;
}

long long getFileSize(std::string f) {
    struct stat stat_buf;
    int e = stat(f.c_str(), &stat_buf);
    return e == 0 ? stat_buf.st_size : -1;
}


