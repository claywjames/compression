# compression
This program uses huffman encoding to compress and decompress files.

To build the program run "make compression"

To compress a file run "./compress -c filename"

To decompress a file run "./compress -d filename"


This program works on all files, but since many files are already compressed (pictures, mp3s, pdfs) the results aren't always impressive.  However,
for uncompressed text files the results are sizeable.  The huckfinn.txt file in the testfiles folder will compress to less than 60% its original size.