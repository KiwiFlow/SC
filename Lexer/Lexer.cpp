#include"./Lexer.h"
namespace env {
	const int CH_EOF = -1;
}

Lexer::Lexer(FILE * fin)
{
	this->fin = fin;
	getch();
	line_num = 1;
	//将关键字，运算符等插入单词表
	static TkWord keywords[] = {
		{ TK_PLUS,    "+",	NULL,	NULL },
		{ TK_MINUS,	  "-",	NULL,	NULL },
		{ TK_STAR,	  "*",	NULL,	NULL },
		{ TK_DIVIDE,  "/",	NULL,	NULL },
		{ TK_MOD,	  "%",	NULL,	NULL },
		{ TK_EQ,	  "==",	NULL,	NULL },
		{ TK_NEQ,	   "!=",	NULL,	NULL },
		{ TK_LT, 	  "<",	NULL,	NULL },
		{ TK_LEQ,	  "<=",	NULL,	NULL },
		{ TK_GT,	  ">",	NULL,	NULL },
		{ TK_GEQ,	  ">=",	NULL,	NULL },
		{ TK_ASSIGN,  "=",	NULL,	NULL },
		{ TK_POINTSTO, "->",	NULL,	NULL },
		{ TK_DOT,	  ".",	NULL,	NULL },
		{ TK_AND,	  "&",	NULL,	NULL },
		{ TK_OPENPA,  "(",	NULL,	NULL },
		{ TK_CLOSEPA, ")",	NULL,	NULL },
		{ TK_OPENBR,  "[",	NULL,	NULL },
		{ TK_CLOSEBR, "]",	NULL,	NULL },
		{ TK_BEGIN,	 "{",	NULL,	NULL },
		{ TK_END,	  "}",	NULL,	NULL },
		{ TK_SEMICOLON,	";",	NULL,	NULL },
		{ TK_COMMA,	    ",",	NULL,	NULL },
		{ TK_ELLIPSIS,	"...",	NULL,	NULL },
		{ TK_EOF,		"End_Of_File",	NULL,	NULL },

		{ TK_CINT,		"整型常量",	NULL,	NULL },
		{ TK_CCHAR,		"字符常量",	NULL,	NULL },
		{ TK_CSTR,	"字符串常量",	NULL,	NULL },

		{ KW_CHAR,	"char",	NULL,	NULL },
		{ KW_SHORT,	"short",	NULL,	NULL },
		{ KW_INT,	"int",	NULL,	NULL },
		{ KW_VOID,	"void",	NULL,	NULL },
		{ KW_STRUCT,"struct",	NULL,	NULL },

		{ KW_IF,	"if"	,	NULL,	NULL },
		{ KW_ELSE,	"else",	NULL,	NULL },
		{ KW_FOR,	"for",	NULL,	NULL },
		{ KW_CONTINUE,	"continue",	NULL,	NULL },
		{ KW_BREAK,		"break",	NULL,	NULL },
		{ KW_RETURN,	"return",	NULL,	NULL },
		{ KW_SIZEOF,	"sizeof",	NULL,	NULL },
		{ KW_ALIGN,	 "__align",	NULL,	NULL },
		{ KW_CDECL,		"__cdecl",	NULL,	NULL },
		{ KW_STDCALL,	"__stdcall",	NULL,	NULL },
	};

	for (const TkWord & each : keywords) {
		tk_hashtable[util::elf_hash(each.spelling)].push_back(&each);
	}

	tk_num = sizeof(keywords) / sizeof(keywords[0]);
}

TkWord* Lexer::get_token()
{
	TkWord *tp = nullptr;
	preprocess();
	switch (ch)
	{
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
	case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
	case 'o': case 'p': case 'q': case 'r': case 's': case 't':
	case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
	case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
	case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
	case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
	case '_':
	{
		tp = parse_identifier();
		break;
	}
	case '0': case '1': case '2': case '3':
	case '4': case '5': case '6': case '7':
	case '8': case '9':
		tp = parse_num();
		break;
	case '+':
		getch();
		tp = new TkWord(TK_PLUS, "+", NULL, NULL);
		break;
	case '-':
		getch();
		if (ch == '>')
		{
			tp = new TkWord(TK_POINTSTO, "->", NULL, NULL);
			getch();
		}
		else
			tp = new TkWord(TK_MINUS, "-", NULL, NULL);
		break;
	case '/':
		tp = new TkWord(TK_DIVIDE, "/", NULL, NULL);
		getch();
		break;
	case '%':
		tp = new TkWord(TK_MOD, "%", NULL, NULL);
		getch();
		break;
	case '=':
		getch();
		if (ch == '=')
		{
			tp = new TkWord(TK_EQ, "==", NULL, NULL);
			getch();
		}
		else
			tp = new TkWord(TK_ASSIGN, "=", NULL, NULL);
		break;
	case '!':
		getch();
		if (ch == '=')
		{
			tp = new TkWord(TK_NEQ, "!=", NULL, NULL);
			getch();
		}
		else
			error("暂不支持'!'(非操作符)");
		break;
	case '<':
		getch();
		if (ch == '=')
		{
			tp = new TkWord(TK_LEQ, "<=", NULL, NULL);
			getch();
		}
		else
			tp = new TkWord(TK_LT, "<", NULL, NULL);
		break;
	case '>':
		getch();
		if (ch == '=')
		{
			tp = new TkWord(TK_GEQ, ">=", NULL, NULL);
			getch();
		}
		else
			tp = new TkWord(TK_GT, ">", NULL, NULL);
		break;
	case '.':
		getch();
		if (ch == '.')
		{
			getch();
			if (ch != '.')
				error("省略号拼写错误");
			else
				tp = new TkWord(TK_ELLIPSIS, "...", NULL, NULL);
			getch();
		}
		else
		{
			tp = new TkWord(TK_DOT, ".", NULL, NULL);
		}
		break;
	case '&':
		tp = new TkWord(TK_AND, "&", NULL, NULL);
		getch();
		break;
	case ';':
		tp = new TkWord(TK_SEMICOLON, ";", NULL, NULL);
		
		getch();
		break;
	case ']':
		tp = new TkWord(TK_CLOSEBR, "]", NULL, NULL);
		getch();
		break;
	case '}':
		tp = new TkWord(TK_END, "}", NULL, NULL);
		getch();
		break;
	case ')':
		tp = new TkWord(TK_CLOSEPA, ")", NULL, NULL);
		
		getch();
		break;
	case '[':
		tp = new TkWord(TK_OPENBR, "[", NULL, NULL);
		getch();
		break;
	case '{':
		tp = new TkWord(TK_BEGIN, "{", NULL, NULL);
		//token = TK_BEGIN;
		getch();
		break;
	case ',':
		tp = new TkWord(TK_COMMA, ",", NULL, NULL);
		//token = TK_COMMA;
		getch();
		break;
	case '(':
		tp = new TkWord(TK_OPENPA, "(", NULL, NULL);
		//token = TK_OPENPA;
		getch();
		break;
	case '*':
		tp = new TkWord(TK_STAR, "*", NULL, NULL);
		//token = TK_STAR;
		getch();
		break;
	case '\'':
	    tp = parse_string(ch);
		break;
	case '\"':
	{
		tp =  parse_string(ch);
		break;
	}
	case EOF:
		tp = new TkWord(TK_EOF, "", NULL, NULL);
		break;
	default:
		error("不认识的字符:\\x%02x", ch); //上面字符以外的字符，只允许出现在源码字符串，不允许出现的源码的其它位置
		getch();
		break;
	}
	return tp;
}

void Lexer::getch()
{
	ch = getc(fin);
}

void Lexer::preprocess()
{
	while (1)
	{
		if (ch == ' ' || ch == '\t' || ch == '\r')
			skip_white_space();
		else if (ch == '/')
		{
			//向前多读一个字节看是否是注释开始符，猜错了把多读的字符再放回去
			getch();
			if (ch == '*')
			{
				parse_comment();
			}
			else
			{
				ungetc(ch, fin); //把一个字符退回到输入流中
				ch = '/';
				break;
			}
		}
		else
			break;
	}

}

void Lexer::skip_white_space()
{
	while (ch == ' ' || ch == '\t' || ch == '\r')      // 忽略空格,和TAB ch =='\n' ||
	{
		if (ch == '\r')
		{
			getch();
			if (ch != '\n')
				return;
			line_num++;
		}
		printf("%c", ch); //这句话，决定是否打印空格，如果不输出空格，源码中空格将被去掉，所有源码挤在一起
		getch();
	}
}

int Lexer::is_nodigit(char c)
{
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		c == '_';
}

int Lexer::is_digit(char c) {
	return c >= '0' && c <= '9';
}

void Lexer::parse_comment() {
	getch();
	do
	{
		do
		{
			if (ch == '\n' || ch == '*' || ch == env::CH_EOF)
				break;
			else
				getch();
		} while (1);
		if (ch == '\n')
		{
			line_num++;
			getch();
		}
		else if (ch == '*')
		{
			getch();
			if (ch == '/')
			{
				getch();
				return;
			}
		}
		else
		{
			error("一直到文件尾未看到配对的注释结束符");
			return;
		}
	} while (1);
}

TkWord *Lexer::insert_token(const std::string &str,int token_code) {
	int key = util::elf_hash(str);
	TkWord *tp = nullptr;


	std::vector<const TkWord *>::iterator it;
	if (tk_hashtable.count(key) &&
		(it = std::find_if(tk_hashtable[key].begin(),
			tk_hashtable[key].end(),
			[&str](const TkWord *m) -> bool { return m->spelling == str; })) != tk_hashtable[key].end()) {
		tp = new TkWord((*it)->tkcode, (*it)->spelling, (*it)->sym_struct, (*it)->sym_identifier);
	}
	else {
		tk_hashtable[key].push_back(tp = new TkWord(tk_num++, str, NULL, NULL));
	}
	return tp;
}

TkWord* Lexer::parse_identifier()
{
	std::string str;
	str.push_back(ch);
	getch();
	while (is_nodigit(ch) || is_digit(ch))
	{
		str.push_back(ch); // str += ch;
		getch();
	}
	return insert_token(str,TK_IDENT);
}

TkWord* Lexer::parse_num() {
	std::string str;
	do {
		str.push_back(ch);
		getch();
	} while (is_digit(ch));
	return insert_token(str,TK_CINT);
}

TkWord* Lexer::parse_string(char sep) {
	std::string str;
	char c;
	getch();
	for (;;)
	{
		if (ch == sep)
			break;
		else if (ch == '\\')
		{
			str.push_back(ch);
			getch();
			switch (ch) // 解析转义字符
			{
			case '0':
				c = '\0';
				break;
			case 'a':
				c = '\a';
				break;
			case 'b':
				c = '\b';
				break;
			case 't':
				c = '\t';
				break;
			case 'n':
				c = '\n';
				break;
			case 'v':
				c = '\v';
				break;
			case 'f':
				c = '\f';
				break;
			case 'r':
				c = '\r';
				break;
			case '\"':
				c = '\"';
				break;
			case '\'':
				c = '\'';
				break;
			case '\\':
				c = '\\';
				break;
			default:
				c = ch;
				if (c >= '!' && c <= '~')
					warning("非法转义字符: \'\\%c\'", c); // 33-126 0x21-0x7E可显示字符部分
				else
					warning("非法转义字符: \'\\0x%x\'", c);
				break;
			}
			str.push_back(c);
			getch();
		}
		else
		{
			str.push_back(ch);
			getch();
		}
	}
	getch();
	return insert_token(str, sep == '\''?TK_CCHAR:TK_CSTR);
}

void Lexer::test() {
	TkWord * tp = nullptr;
	while (true) {
		tp = get_token();
		if (tp->tkcode == TK_EOF) {
			break;
		}
		else {
			color_token(tp);
		}
	}
	printf("\n代码行数: %d行\n", line_num);
}

void Lexer::color_token(TkWord *tp) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	if (tp->tkcode >= TK_IDENT)
		SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);
	else if (tp->tkcode >= KW_CHAR)
		SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	else if (tp->tkcode >= TK_CINT)
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN);
	else
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("%s", tp->spelling.c_str());
}
