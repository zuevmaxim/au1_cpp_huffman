#include "huffman_test.h"
#include "autotest.h"

int main() {
    HuffmanTest ht;
    autoTest at;
    ht.runAllTests();
    at.runAllTests();
    HuffmanTest::showFinalResult();
    return 0;
}
