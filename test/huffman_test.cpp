#include "huffman_test.h"
#include "../src/huffman.h"
#include <iostream>
#include <fstream>

int Test::failedNum = 0;
int Test::totalNum = 0;

void Test::check(bool expr, const char *func, const char  *filename, size_t lineNum) {
	if (!expr) {
		std::cout << "Test failed: " << func << " in " << filename << ':' << lineNum << '\n';
		++failedNum;
	}
	++totalNum;
}

void Test::showFinalResult() {
	if (failedNum == 0) {
		std::cout << "All tests passed.\n";
	} else {
		std::cout << "Failed " << failedNum << " of " << totalNum << " tests.\n"; 
	}
}

HuffmanTest::HuffmanTest() : file("test/files/file.txt") { }

void HuffmanTest::runAllTests() {
	CalcFrequenciesTest1();
	CalcFrequenciesTest2();
	CalcFrequenciesTest3();
	BuildHuffmanTreeTest1();
	BuildHuffmanTreeTest2();
	BuildHuffmanTreeTest3();
	GetCodesTest1();
	GetCodesTest2();
	GetCodesTest3();
	getKeyTest1();
	getKeyTest2();
	getKeyTest3();
}

void HuffmanTest::CalcFrequenciesTest1() {
	HuffmanArchiver h;
	std::ofstream out(file);
	out << "abacaba";
	out.close();
	std::ifstream in(file); in >> std::noskipws;
	size_t n = h.calcFrequencies(in);
	DO_CHECK(n == 7 && h.frequencies.size() == 3 &&
			h.frequencies[2].first == 4 && h.frequencies[2].second == 'a' &&
			h.frequencies[1].first == 2 && h.frequencies[1].second == 'b' &&
			h.frequencies[0].first == 1 && h.frequencies[0].second == 'c');
}

void HuffmanTest::CalcFrequenciesTest2() {
	HuffmanArchiver h;
	std::ofstream out(file);
	out << "";
	out.close();
	std::ifstream in(file); in >> std::noskipws;
	size_t n = h.calcFrequencies(in);
	DO_CHECK(n == 0 && h.frequencies.size() == 0);
}

void HuffmanTest::CalcFrequenciesTest3() {
	HuffmanArchiver h;
	std::ofstream out(file);
	out << " ";
	out.close();
	std::ifstream in(file); in >> std::noskipws;
	size_t n = h.calcFrequencies(in);
	DO_CHECK(n == 1 && h.frequencies.size() == 1 &&
			h.frequencies[0].first == 1 && h.frequencies[0].second == ' ');
}

void HuffmanTest::BuildHuffmanTreeTest1() {
	std::vector<std::pair<size_t, char>> freq;
	freq.push_back({1, 'c'});
	freq.push_back({5, 'b'});
	freq.push_back({10, 'a'});
	HuffmanArchiver::HuffmanTree ht;
	ht.buildHuffmanTree(freq);
	DO_CHECK(ht.nodes.size() == 5 && ht.root == 4 && !ht.empty &&
		ht.nodes[4].l == 3 && ht.nodes[4].r == 2 && ht.nodes[4].stat == 16 &&
		ht.nodes[3].l == 0 && ht.nodes[3].r == 1 && ht.nodes[3].stat == 6 &&
		ht.nodes[2].stat == 10 && ht.nodes[2].c == 'a' &&
		ht.nodes[1].stat == 5 && ht.nodes[1].c == 'b' &&
		ht.nodes[0].stat == 1 && ht.nodes[0].c == 'c');
}

void HuffmanTest::BuildHuffmanTreeTest2() {
	std::vector<std::pair<size_t, char>> freq;
	freq.push_back({1, 'c'});
	HuffmanArchiver::HuffmanTree ht;
	ht.buildHuffmanTree(freq);
	DO_CHECK(ht.nodes.size() == 2 && ht.root == 1 && !ht.empty &&
		ht.nodes[0].stat == 1 && ht.nodes[0].c == 'c');
}

void HuffmanTest::BuildHuffmanTreeTest3() {
	std::vector<std::pair<size_t, char>> freq;
	HuffmanArchiver::HuffmanTree ht;
	ht.buildHuffmanTree(freq);
	DO_CHECK(ht.nodes.empty() && ht.empty);
}

void HuffmanTest::GetCodesTest1() {
	HuffmanArchiver h;
	std::ofstream out(file);
	out << "abacaba";
	out.close();
	std::ifstream in(file); in >> std::noskipws;
	h.calcFrequencies(in);
	HuffmanArchiver::HuffmanTree ht;
	ht.buildHuffmanTree(h.frequencies);
	std::unordered_map<char, std::string> codes;
	ht.getCodes(codes);
	DO_CHECK(codes.size() == 3 &&
		codes['a'] == "0" && codes['b'] == "10" && codes['c'] == "11");
}

void HuffmanTest::GetCodesTest2() {
	HuffmanArchiver h;
	std::ofstream out(file);
	out << "a a";
	out.close();
	std::ifstream in(file); in >> std::noskipws;
	h.calcFrequencies(in);
	HuffmanArchiver::HuffmanTree ht;
	ht.buildHuffmanTree(h.frequencies);
	std::unordered_map<char, std::string> codes;
	ht.getCodes(codes);
	DO_CHECK(codes.size() == 2 &&
		codes['a'] == "0" && codes[' '] == "1");
}

void HuffmanTest::GetCodesTest3() {
	HuffmanArchiver h;
	std::ofstream out(file);
	out << "";
	out.close();
	std::ifstream in(file); in >> std::noskipws;
	h.calcFrequencies(in);
	HuffmanArchiver::HuffmanTree ht;
	ht.buildHuffmanTree(h.frequencies);
	std::unordered_map<char, std::string> codes;
	ht.getCodes(codes);
	DO_CHECK(codes.empty());
}

void HuffmanTest::getKeyTest1() {
	HuffmanArchiver h;
	std::ofstream out(file);
	out << "abacaba";
	out.close();
	std::ifstream in(file); in >> std::noskipws;
	h.calcFrequencies(in);
	HuffmanArchiver::HuffmanTree ht;
	ht.buildHuffmanTree(h.frequencies);
	char key;
	DO_CHECK(ht.getKey(key, '0') && key == 'a');
}

void HuffmanTest::getKeyTest2() {
	HuffmanArchiver h;
	std::ofstream out(file);
	out << "abacaba";
	out.close();
	std::ifstream in(file); in >> std::noskipws;
	h.calcFrequencies(in);
	HuffmanArchiver::HuffmanTree ht;
	ht.buildHuffmanTree(h.frequencies);
	char key;
	DO_CHECK(!ht.getKey(key, '1'));
}

void HuffmanTest::getKeyTest3() {
	HuffmanArchiver h;
	std::ofstream out(file);
	out << "abacaba";
	out.close();
	std::ifstream in(file); in >> std::noskipws;
	h.calcFrequencies(in);
	HuffmanArchiver::HuffmanTree ht;
	ht.buildHuffmanTree(h.frequencies);
	char key;
	ht.getKey(key, '1');
	DO_CHECK(ht.getKey(key, '1') && key == 'c');
}

