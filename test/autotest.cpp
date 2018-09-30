#include "autotest.h"
#include "../src/huffman.h"
#include <fstream>
#include <cstdlib>

autoTest::autoTest() : txtIn("test/files/txtIn.txt"),
		txtOut("test/files/txtOut.txt"), bin("test/files/bin.bin") { }

bool autoTest::fileCompare(const std::string &f1, const std::string &f2) {
	std::ifstream in1(f1), in2(f2);
	in1 >> std::noskipws;
	in2 >> std::noskipws;
	char c1, c2;
	while (in1 >> c1 && in2 >> c2) {
		if (c1 != c2)
			return false;
	}
	if (in1 >> c1 || in2 >> c2)
		return false;
	return true;
}

void autoTest::runAllTests() {
	HuffmanTest1();
	HuffmanTest2();
	HuffmanTest3();
	HuffmanTest4();
	HuffmanTest5();
	HuffmanTest6();
	HuffmanTest7();
	HuffmanTest8();
	HuffmanTest9();
}

void autoTest::HuffmanTest1() {
	HuffmanArchiver h;
	std::ofstream out(txtIn);
	out << "abacaba";
	out.close();
	h.compress(txtIn, bin);
	h.decompress(bin, txtOut);
	DO_CHECK(fileCompare(txtIn, txtOut) && h.sizeOut == 7);
}

void autoTest::HuffmanTest2() {
	HuffmanArchiver h;
	std::ofstream out(txtIn);
	out << "";
	out.close();
	h.compress(txtIn, bin);
	h.decompress(bin, txtOut);
	DO_CHECK(fileCompare(txtIn, txtOut));
}

void autoTest::HuffmanTest3() {
	HuffmanArchiver h;
	std::ofstream out(txtIn);
	out << "aaaaaaa";
	out.close();
	h.compress(txtIn, bin);
	h.decompress(bin, txtOut);
	DO_CHECK(fileCompare(txtIn, txtOut) && h.sizeOut == 7);
}

void autoTest::HuffmanTest4() {
	HuffmanArchiver h;
	std::ofstream out(txtIn);
	srand(42);
	size_t n = 5e6;
	char c;
	for (size_t i = 0; i < n; ++i) {
		c = static_cast<char>(rand() % 94 + 33);
		out << c;
	}
	out.close();
	h.compress(txtIn, bin);
	h.decompress(bin, txtOut);
	DO_CHECK(fileCompare(txtIn, txtOut) && h.sizeOut == n);
}

void autoTest::HuffmanTest5() {
	HuffmanArchiver h;
	std::ofstream out(txtIn);
	out << "\n";
	out.close();
	h.compress(txtIn, bin);
	h.decompress(bin, txtOut);
	DO_CHECK(fileCompare(txtIn, txtOut));
}

void autoTest::HuffmanTest6() {
	HuffmanArchiver h;
	std::ofstream out(txtIn);
	out << "\na$\n\t4q	px'\"";
	out.close();
	h.compress(txtIn, bin);
	h.decompress(bin, txtOut);
	DO_CHECK(fileCompare(txtIn, txtOut));
}

void autoTest::HuffmanTest7() {
	HuffmanArchiver h;
	std::ofstream out(txtIn);
	size_t n = 5e2;
	for (size_t i = 0; i < n; ++i) {
		out << 'a';
	}
	out.close();
	h.compress(txtIn, bin);
	h.decompress(bin, txtOut);
	DO_CHECK(fileCompare(txtIn, txtOut) && h.sizeOut == n);
}

void autoTest::HuffmanTest8() {
	for (size_t j = 0; j < 200; ++j) {
		HuffmanArchiver h;
		std::ofstream out(txtIn);
		srand(42);
		size_t n = rand() % 100;
		unsigned char c;
		for (size_t i = 0; i < n; ++i) {
			c = static_cast<unsigned char>(rand() % 256);
			out << c;
		}
		out.close();
		h.compress(txtIn, bin);
		h.decompress(bin, txtOut);
		DO_CHECK(fileCompare(txtIn, txtOut) && h.sizeOut == n);
	}
}

void autoTest::HuffmanTest9() {
	HuffmanArchiver h;
	std::ofstream out(txtIn);
	unsigned char c = 0;
	out << c;
	for (c = 1; c != 0; ++c) 
		out << c;
	out.close();
	h.compress(txtIn, bin);
	h.decompress(bin, txtOut);
	DO_CHECK(fileCompare(txtIn, txtOut) && h.sizeOut == 256);
}
