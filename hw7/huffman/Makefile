
.PHONY: all
all: huff_enc huff_dec

huff_enc: huff_encoding.o huffman.o
	gcc -Wall -o huff_enc huff_encoding.o huffman.o -lm

huff_dec: huff_decoding.o huffman.o
	gcc -Wall -o huff_dec huff_decoding.o huffman.o

huffman.o: huffman.c huffman.h
	gcc -Wall -c -o huffman.o huffman.c

huff_encoding.o: huff_encoding.c huffman.h
	gcc -Wall -c -o huff_encoding.o huff_encoding.c

huff_decoding.o: huff_decoding.c huffman.h
	gcc -Wall -c -o huff_decoding.o huff_decoding.c

.PHONY: clean
clean:
	rm *.o huff_enc huff_dec
