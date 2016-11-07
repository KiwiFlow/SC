#include "./scc.h"
#include "./Lexer.h"

FILE *fin = NULL;
char *filename;
char *outfile;

int main(int argc, char ** argv)
{
	fin = fopen(argv[1], "rb");
	if (!fin)
	{
		printf("不能打开SC源文件!\n");
		return 0;
	}
	Lexer lexer(fin);
	lexer.test();
	fclose(fin);
	printf("%s 词法分析成功!", argv[1]);
	return 0;
}