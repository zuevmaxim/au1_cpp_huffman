#include "huffman.h"
#include <iostream>
#include <cstring>

enum Task { COMPRESS, DECOMPRESS, UNDEFIND };

int main(int argc, char* argv[]) {
	Task task = UNDEFIND;
	std::string inputFile, outputFile;
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-c") == 0)
			task = COMPRESS;
		else if (strcmp(argv[i], "-u") == 0)
			task = DECOMPRESS;
		else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0)
			inputFile = argv[++i];
		else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0)
			outputFile = argv[++i];
		else {
			std::cerr << "Wrong argument " << argv[i] << '\n';
			return 1;
		}
	}
	if (task == UNDEFIND) {
		std::cerr << "No task -c or -u was given.\n";
		return 1;
	}
	try {
		HuffmanArchiver h;
		if (task == COMPRESS)
			h.compress(inputFile, outputFile);
		else
			h.decompress(inputFile, outputFile);
		h.printStatistics();
	} catch (HuffmanException& e) {
		e.printError();
		return 1;
	}
	return 0;
}


