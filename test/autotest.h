#ifndef _AUTO_HUFFMAN_TEST_H_
#define _AUTO_HUFFMAN_TEST_H_ 

#include <stddef.h>
#include <string>
#include "huffman_test.h"

class autoTest : public Test {
public:
	autoTest();
	void runAllTests();
	void HuffmanTest1();
	void HuffmanTest2();
	void HuffmanTest3();
	void HuffmanTest4();
	void HuffmanTest5();
	void HuffmanTest6();
	void HuffmanTest7();
	void HuffmanTest8();
	void HuffmanTest9();
private:
	bool fileCompare(const std::string &f1, const std::string &f2);
	std::string txtIn;
	std::string txtOut;
	std::string bin;
};


#endif
