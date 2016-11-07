#include "./util.h"
namespace env {
	const int MAXKEY = 1024;
}
namespace util {
	int elf_hash(const std::string &key) {
		int h = 0, g = 0;
		for (const char &c : key) {
			h = (h << 4) + c;
			g = h & 0xf0000000;
			if (g)
				h ^= g >> 24;
			h &= ~g;
		}
		return h%(env::MAXKEY);
	}
}