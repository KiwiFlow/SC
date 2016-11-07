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
		printf("���ܴ�SCԴ�ļ�!\n");
		return 0;
	}
	Lexer lexer(fin);
	lexer.test();
	fclose(fin);
	printf("%s �ʷ������ɹ�!", argv[1]);
	return 0;
}