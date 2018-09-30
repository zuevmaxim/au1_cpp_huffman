#include "huffman.h"
#include <algorithm>

static const size_t N = 256;
static const size_t INF = 1e9;
static const size_t SIZE = 4;

uint32_t readBinary(std::ifstream &in) {
	unsigned char bytes[SIZE];
	in.read(reinterpret_cast<char*>(bytes), SIZE);
	return static_cast<int>(bytes[0]) | 
		  (static_cast<int>(bytes[1]) << 8) |
		  (static_cast<int>(bytes[2]) << 16) |
		  (static_cast<int>(bytes[3]) << 24);
}

void writeBinary(std::ofstream &out, uint32_t x) {
	char bytes[4];
	for (size_t i = 0; i < SIZE; ++i) {
		bytes[i] = static_cast<char>(x & 255), x >>= 8;
	}
	out.write(bytes, SIZE);
}


HuffmanException::HuffmanException(const std::string &file) {
	error = "Wrong file \"" + file + "\".";
}

void HuffmanException::printError() {
	std::cerr << error << '\n';
}


HuffmanArchiver::HuffmanTree::HuffmanTree() : empty(false) { }

void HuffmanArchiver::HuffmanTree::buildHuffmanTree(
		const std::vector<std::pair<size_t, char>> &frequencies) {
	size_t n = frequencies.size();
	if (n == 0) {
		empty = true;
	}
	size_t i = 0;
	for (auto const &x : frequencies) {
		nodes.push_back({i++, INF, INF, x.first, x.second});
	}
	i = 0;
	if (n > 1) {
		while (i < frequencies.size() || newNodes.size() > 1) {
			HuffmanNode m1 = getMin(frequencies, i);
			HuffmanNode m2 = getMin(frequencies, i);
			HuffmanNode tmp = {n, m1.id, m2.id, m1.stat + m2.stat, ' '};
			nodes.push_back(tmp);
			newNodes.push(tmp);
			++n;
		}
	} else if (n == 1) {
		nodes.push_back({n++, 0, 0, 0, ' '});
	}
	root = n - 1;
	v = root;
}

void HuffmanArchiver::HuffmanTree::getCodes(
		std::unordered_map<char, std::string> &codes) {
	if (empty) {
		return;
	}
	if (root == 0) {
		codes[nodes[root].c] = "0";
		return;
	}
	getCodes(codes, root);
}

void HuffmanArchiver::HuffmanTree::getCodes(
		std::unordered_map<char, std::string> &codes, size_t v) {
	if (nodes[v].l == INF) {
		codes[nodes[v].c] = code;
		return;
	}
	code.push_back('1');
	getCodes(codes, nodes[v].l);
	code.pop_back();
	code.push_back('0');
	getCodes(codes, nodes[v].r);
	code.pop_back();
}

bool HuffmanArchiver::HuffmanTree::getKey(char& key, char next) {
	if (next == '1') {
		v = nodes[v].l;
	} else {
		v = nodes[v].r;
	}
	if (nodes[v].l == INF) {
		key = nodes[v].c;
		v = root;
		return true;
	}
	return false;
}

HuffmanArchiver::HuffmanTree::HuffmanNode HuffmanArchiver::HuffmanTree::getMin(
		const std::vector<std::pair<size_t, char>> &frequencies, size_t &i) {
	if (newNodes.empty() || (i < frequencies.size() && 
			frequencies[i].first < newNodes.front().stat)) {
		return nodes[i++];
	}
	HuffmanNode tmp = newNodes.front();
	newNodes.pop();
	return tmp;
}


HuffmanArchiver::HuffmanArchiver() : sizeIn(0), sizeOut(0) { }

void HuffmanArchiver::compress(const std::string &inputFile,
		const std::string &outputFile) {
	std::ifstream in(inputFile, std::ios::binary | std::ios::in);
	if (!in) {
		throw HuffmanException(inputFile);
	}
	std::ofstream out(outputFile, std::ios::binary | std::ios::out);
	if (!out) {
		throw HuffmanException(outputFile);
	}
	sizeIn = calcFrequencies(in);
	HuffmanTree tree;
	tree.buildHuffmanTree(frequencies);
	tree.getCodes(codes);
	sizeExtra = saveCodes(out);
	in.clear(); in.seekg(0);
	sizeOut = saveData(in, out);
	clear();
}

void HuffmanArchiver::decompress(const std::string &inputFile,
		const std::string &outputFile) {
	std::ifstream in(inputFile, std::ios::binary | std::ios::in);
	if (!in) {
		throw HuffmanException(inputFile);
	}
	std::ofstream out(outputFile, std::ios::binary | std::ios::out);
	if (!out) {
		throw HuffmanException(outputFile);
	}
	sizeExtra = loadFrequencies(in);
	tree.buildHuffmanTree(frequencies);
	decode(in, out);
	clear();
}

void HuffmanArchiver::printStatistics() {
	std::cout << sizeIn << '\n';
	std::cout << sizeOut << '\n';
	std::cout << sizeExtra << '\n';
}

void HuffmanArchiver::clear() {
	frequencies.clear();
	codes.clear();
}

size_t HuffmanArchiver::calcFrequencies(std::ifstream &in) {
	size_t n = 0;
	char c;
	frequencies.resize(N);
	for (size_t i = 0; i < N; ++i) {
		frequencies[i] = {0, static_cast<char>(i)};
	}
	in.seekg(0, std::ios_base::end);
	size_t len = in.tellg();
	in.clear(); in.seekg(0);
	while (len--) {
		in.read(&c, 1);
		++frequencies[static_cast<unsigned char>(c)].first;
		++n;
	}
	auto it = partition(frequencies.begin(), frequencies.end(), 
		[](const std::pair<size_t, char> &x) { return x.first != 0; } );
	frequencies.resize(it - frequencies.begin());
	sort(frequencies.begin(), frequencies.end());
	return n;
}

size_t HuffmanArchiver::saveCodes(std::ofstream &out) {
	writeBinary(out, sizeIn);
	writeBinary(out, frequencies.size());
	for (auto &x :  frequencies) {
		out.write(&x.second, sizeof(char));
		writeBinary(out, x.first);
	}
	return 2 * sizeof(frequencies.size()) + frequencies.size() * 
		(sizeof(char) + sizeof(size_t));
}

size_t HuffmanArchiver::saveData(std::ifstream &in, std::ofstream &out) {
	BinaryOut Out(out);
	size_t start = in.tellg();
	in.seekg(0, std::ios_base::end);
	size_t len = in.tellg();
	len -= start;
	in.clear(); in.seekg(start);
	char c;
	while (len--) {
		in.read(&c, 1);
		Out.write(codes[c]);
	}
	return Out.count();
}

size_t HuffmanArchiver::loadFrequencies(std::ifstream &in) {
	sizeOut = readBinary(in);
	size_t n = readBinary(in);
	char c;
	for (size_t i = 0; i < n; ++i) {
		in.read(&c, sizeof(char));
		frequencies.push_back({readBinary(in), c});
	}
	return 2 * sizeof(size_t) + n * (sizeof(char) + sizeof(size_t));
}

void HuffmanArchiver::decode(std::ifstream &in, std::ofstream &out) {
	BinaryIn In(in);
	size_t k = 0;
	char x, c;
	std::string s(sizeOut, ' ');
	sizeIn = In.count();
	while (k < sizeOut) {
		x = In.read();
		if (tree.getKey(c, x)) {
			s[k++] = c;
		}
	}
	out.write(reinterpret_cast<char*>(&s[0]), s.length());
}


HuffmanArchiver::BinaryOut::BinaryOut(std::ofstream &out) : out(out), buf(0), 
		k(0), n(0), bufSize(8) { }

void HuffmanArchiver::BinaryOut::flush() {
	if (k != 0) {
		buffer.push_back(buf), ++n;
	}
	out.write(reinterpret_cast<char*>(&buffer[0]), n * sizeof(char));
}

void HuffmanArchiver::BinaryOut::write(const std::string &s) {
	size_t i = 0;
	while (i < s.length()) {
		while (k < bufSize && i < s.length()) {
			buf += (1 << k++) * (s[i++] == '1' ? 1 : 0);
		}
		if (k == bufSize) {
			buffer.push_back(buf);
			buf = 0;
			k = 0;
			++n;
		}
	}
}

size_t HuffmanArchiver::BinaryOut::count() {
	flush();
	return n;
}


HuffmanArchiver::BinaryIn::BinaryIn(std::ifstream &in) : in(in), buf(0), 
		n(0), bufSize(8), k(bufSize), i(0) {
	size_t start = in.tellg();
	in.seekg(0, std::ios_base::end);
	n = in.tellg();
	n -= start;
	in.clear(); in.seekg(start);
	buffer.resize(n / sizeof(char) + 1);
	in.read(reinterpret_cast<char*>(&buffer[0]), n);
}

char HuffmanArchiver::BinaryIn::read() {
	if (k == bufSize) {
		k = 0;
		buf = buffer[i++];
	}
	return (buf & (1 << k++)) ? '1' : '0';
}

size_t HuffmanArchiver::BinaryIn::count() {
	return n;
}

