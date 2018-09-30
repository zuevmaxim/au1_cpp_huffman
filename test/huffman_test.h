#ifndef _HUFFMAN_TEST_H_
#define _HUFFMAN_TEST_H_ 

#include <stddef.h>
#include <string>

#define DO_CHECK(EXPR) check(EXPR, __FUNCTION__, __FILE__, __LINE__)

class Test {
protected:
	static int failedNum; 
	static int totalNum; 
public:
	static void check(bool expr, const char *func, const char  *filename, size_t lineNum); 
	static void showFinalResult();
	virtual void runAllTests() = 0;
};


class HuffmanTest : public Test {
public:
	HuffmanTest();
	void runAllTests();
	void CalcFrequenciesTest1();
	void CalcFrequenciesTest2();
	void CalcFrequenciesTest3();
	void BuildHuffmanTreeTest1();
	void BuildHuffmanTreeTest2();
	void BuildHuffmanTreeTest3();
	void GetCodesTest1();
	void GetCodesTest2();
	void GetCodesTest3();
	void getKeyTest1();
	void getKeyTest2();
	void getKeyTest3();
private:
	std::string file;
};

#endif
