# Variables
CPP          = clang++
CPP_FLAGS    = -g -Wall -W -Wunused -Wuninitialized -Wshadow -std=c++11 

#===========================================================================
TESTS =  test_bytebuffer test_huffman

#===========================================================================
msg:
	@echo 'Targets for compiling test programs:'
	@echo '   tests'
	@echo '   clean'
	@echo '   compression'

#===========================================================================
bytebuffer.o:	bytebuffer.hpp  bytebuffer.cpp
	${CPP} ${CPP_FLAGS} -c bytebuffer.cpp

#===========================================================================
huffman.o:	huffman.hpp  huffman.cpp
	${CPP} ${CPP_FLAGS} -c huffman.cpp

#===========================================================================
# Compile test programs

test_%: bytebuffer.o huffman.o test_%.o
	${CPP} ${CPP_FLAGS} bytebuffer.o huffman.o test_$*.o -o test_$*

test_%.o: bytebuffer.hpp huffman.hpp test_%.cpp
	${CPP} ${CPP_FLAGS} ${INCLUDE_OPT} -c test_$*.cpp


#===========================================================================
# Run test programs

tests: bytebuffer.hpp huffman.hpp ${TESTS}
	./test_bytebuffer
	./test_huffman

#===========================================================================
# Compile huffman_compress

compression.o: compression.cpp huffman.hpp bytebuffer.hpp
	${CPP} ${CPP_FLAGS} ${INCLUDE_OPT} -c compression.cpp

compression: compression.o huffman.o bytebuffer.o
	${CPP} ${CPP_FLAGS} huffman.o compression.o  bytebuffer.o -o compress



#============================================================================
clean:
	rm -f *.o
	rm -f *.huff 
	rm -f compress
	rm -f $(TESTS)


