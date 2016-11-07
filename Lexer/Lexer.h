#ifndef _LEXER_H
#define _LEXER_H
#include"./scc.h"
#include"./util.h"
/* ���ʱ��� */
enum e_TokenCode
{
	/* ��������ָ��� */
	TK_PLUS,		// + �Ӻ�
	TK_MINUS,		// - ����
	TK_STAR,		// * �Ǻ�
	TK_DIVIDE,		// / ����
	TK_MOD,			// % ���������
	TK_EQ,			// == ���ں�
	TK_NEQ,			// != �����ں�
	TK_LT,			// < С�ں�
	TK_LEQ,			// <= С�ڵ��ں�
	TK_GT,			// > ���ں�
	TK_GEQ,			// >= ���ڵ��ں�
	TK_ASSIGN,		// = ��ֵ����� 
	TK_POINTSTO,	// -> ָ��ṹ���Ա�����
	TK_DOT,			// . �ṹ���Ա�����
	TK_AND,         // & ��ַ�������
	TK_OPENPA,		// ( ��Բ����
	TK_CLOSEPA,		// ) ��Բ����
	TK_OPENBR,		// [ ��������
	TK_CLOSEBR,		// ] ��Բ����
	TK_BEGIN,		// { �������
	TK_END,			// } �Ҵ�����
	TK_SEMICOLON,	// ; �ֺ�    
	TK_COMMA,		// , ����
	TK_ELLIPSIS,	// ... ʡ�Ժ�
	TK_EOF,			// �ļ�������

	/* ���� */
	TK_CINT,		// ���ͳ���
	TK_CCHAR,		// �ַ�����
	TK_CSTR,		// �ַ�������

	/* �ؼ��� */
	KW_CHAR,		// char�ؼ���
	KW_SHORT,		// short�ؼ���
	KW_INT,			// int�ؼ���
	KW_VOID,		// void�ؼ���  
	KW_STRUCT,		// struct�ؼ���   
	KW_IF,			// if�ؼ���
	KW_ELSE,		// else�ؼ���
	KW_FOR,			// for�ؼ���
	KW_CONTINUE,	// continue�ؼ���
	KW_BREAK,		// break�ؼ���   
	KW_RETURN,		// return�ؼ���
	KW_SIZEOF,		// sizeof�ؼ���

	KW_ALIGN,		// __align�ؼ���	
	KW_CDECL,		// __cdecl�ؼ��� standard c call
	KW_STDCALL,     // __stdcall�ؼ��� pascal c call
	/* ��ʶ�� */
	TK_IDENT
};

/* �ʷ�״̬ */
enum e_LexState
{
	LEX_NORMAL,
	LEX_SEP
};


/* ���ʴ洢�ṹ���� */
struct TkWord
{
	int  tkcode;					// ���ʱ���
	std::string spelling;			// �����ַ���
	struct Symbol *sym_struct;		// ָ�򵥴�����ʾ�Ľṹ����
	struct Symbol *sym_identifier;	// ָ�򵥴�����ʾ�ı�ʶ��
	TkWord(int i, const std::string &j, struct Symbol * m, struct Symbol *n) :tkcode(i), spelling(j), sym_struct(m), sym_identifier(n) {}
};

class Lexer {
public:
	Lexer(FILE *fin);
	TkWord *get_token();
	void test();
private:
	void getch();
	void preprocess();
	void parse_comment();
	TkWord *insert_token(const std::string & str, int token_code);
	void skip_white_space();
	int is_nodigit(char c);
	int is_digit(char c);
	TkWord *parse_identifier();
	TkWord *parse_num();
	TkWord *parse_string(char sep);
	
	void color_token(TkWord * tp);

	FILE *fin;
	char ch;
	int line_num;
	int tk_num;
	std::vector<TkWord*> tktable;
	std::map<int, std::vector<const TkWord*> > tk_hashtable;
};
#endif //_Lexer_H