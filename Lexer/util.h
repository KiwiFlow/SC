#ifndef _UTIL.H
#define _UTIL_H
#include"./scc.h"
namespace util {
	/***********************************************************
	* 功能:	计算哈希地址
	* key:哈希关键字(为了与本书中SC语言关键字区分开，此处我们称为哈希关键字)
	* MAXKEY:哈希表长度
	**********************************************************/
	int elf_hash(const std::string &name);
}
#endif // !_UTIL.H
