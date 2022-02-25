# File Compression
* A simple file compressing program.

## Huffman Coding
Huffman coding is a lossless data compression algorithm generally useful to compress the data in which there are frequently occurring characters. The idea is to assign variable-length codes to input characters, lengths of the assigned codes are based on the frequencies of corresponding characters. The most frequent character gets the smallest code and the least frequent character gets the largest code.
The variable-length codes assigned to input characters are Prefix Codes, means the codes (bit sequences) are assigned in such a way that the code assigned to one character is not the prefix of code assigned to any other character. This is how Huffman Coding makes sure that there is no ambiguity when decoding the generated bitstream. 

## Creating a huffman tree (Algorithm)
* Create a leaf node for each unique character and build a min heap of all leaf nodes (Min Heap is used as a priority queue. The value of frequency field is used to compare two nodes in min heap. Initially, the least frequent character is at root)
* Extract two nodes with the minimum frequency from the min heap.
* Create a new internal node with a frequency equal to the sum of the two nodes frequencies. Make the first extracted node as its left child and the other extracted node as its right child. Add this node to the min heap.
* Repeat steps#2 and #3 until the heap contains only one node. The remaining node is the root node and the tree is complete.

## Decoding the code
For decoding the code, we can take the code and traverse through the tree to find the character.

## Huffman Coding Complexity
The time complexity for encoding each unique character based on its frequency is O(nlog n).
Extracting minimum frequency from the priority queue takes place 2*(n-1) times and its complexity is O(log n). Thus the overall complexity is O(nlog n).

# 👋   To run this file
**Clone the repo**
```
https://github.com/AAreLaa/File-Compression.git
```
Now, compile and run compress.cpp to compress the file and decompress.cpp to decomprees the compressed file.
