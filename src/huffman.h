#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_


#include <vector>
#include <queue>
#include <unordered_map>
#include <map>
#include <string>
#include <cstdint>
#include <iostream>
#include <fstream>

class HuffmanException {
public:
	HuffmanException(const std::string& file);
	void printError();
private:
	std::string error;
};


class HuffmanArchiver {
public:
	friend class HuffmanTest;
	friend class autoTest;
	HuffmanArchiver();
	void compress(const std::string &inputFile, const std::string &outputFile);
	void decompress(const std::string &inputFile,
			const std::string &outputFile);
	void printStatistics();
private:
	void clear();
	size_t calcFrequencies(std::ifstream &in);
	size_t saveCodes(std::ofstream &out);
	size_t saveData(std::ifstream &in, std::ofstream &out);
	size_t loadFrequencies(std::ifstream &in);
	void decode(std::ifstream &in, std::ofstream &out);
	
	class HuffmanTree {
	public:
		friend class HuffmanTest;
		HuffmanTree();
		void buildHuffmanTree(
				const std::vector<std::pair<size_t, char>> &frequencies);
		void getCodes(std::unordered_map<char, std::string> &codes);
		bool getKey(char& key, char next);
	private:
		struct HuffmanNode {
			size_t id, l, r, stat;
			char c;
		};
		HuffmanNode getMin(
				const std::vector<std::pair<size_t, char>> &frequencies,
				size_t& i);
		void getCodes(std::unordered_map<char, std::string> &codes, size_t v);
		size_t v;
		bool empty;
		size_t root;
		std::string code;
		std::vector <HuffmanNode> nodes;
		std::queue <HuffmanNode> newNodes;
	};
	
	class BinaryOut {
	public:
		BinaryOut(std::ofstream &out);
		void flush();
		void write(const std::string &s);
		size_t count();
	private:
		std::ofstream &out;
		char buf;
		std::vector<char> buffer;
		size_t k;
		size_t n, bufSize;
	};
	
	class BinaryIn {
	public:
		BinaryIn(std::ifstream &in);
		char read();
		size_t count();
	private:
		std::ifstream &in;
		char buf;
		std::vector<char> buffer;
		size_t n, bufSize;
		size_t k;
		size_t i;
	};
	
	std::vector<std::pair<size_t, char>> frequencies;
	std::unordered_map <char, std::string> codes;
	HuffmanTree tree;
	uint32_t sizeIn, sizeOut, sizeExtra;
};

#endif
