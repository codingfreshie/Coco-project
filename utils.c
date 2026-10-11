/**
 * Authors:
 * Siddhant Attavar: 2023A7TS0659P
 * Karthik Tenneti: 2023A7PS0650P
 * Neeharika Maram: 2023A7PS0644P
 * Kunal Verma: 2023A7PS0565P
 * Samhita Ganduri: 2023A7PS0046P
 * Aashritha Lakshmi Mallampati: 2023A7PS0583P
 **/

#define MAX_CHARS 1000000
#define TOK_LEN 100
#define MAX_LINE 1000
#define MAX_TOKENS 10000
#define MAX_ASCII 126
#define CHAR_GROUPS 133
#define DFA_STATES 61

/*
 * Enum containing all token types
 */
typedef enum {
	TK_LT,
	TK_LE,
	TK_ASSIGNOP,
	TK_COMMENT,
	TK_AND,
	TK_OR,
	TK_NOT,
	TK_EQ,
	TK_GT,
	TK_GE,
	TK_NE,
	TK_ID,
	TK_NUM,
	TK_RNUM,
	TK_FUNID,
	TK_RUID,
	TK_SQL,
	TK_SQR,
	TK_COMMA,
	TK_SEM,
	TK_COLON,
	TK_DOT,
	TK_OP,
	TK_CL,
	TK_PLUS,
	TK_MINUS,
	TK_MUL,
	TK_DIV,
	TK_FIELDID,
	TK_WITH,
	TK_PARAMETERS,
	TK_END,
	TK_WHILE,
	TK_UNION,
	TK_ENDUNION,
	TK_DEFINETYPE,
	TK_AS,
	TK_TYPE,
	TK_MAIN,
	TK_GLOBAL,
	TK_PARAMETER,
	TK_LIST,
	TK_INPUT,
	TK_OUTPUT,
	TK_INT,
	TK_REAL,
	TK_ENDWHILE,
	TK_IF,
	TK_THEN,
	TK_ENDIF,
	TK_READ,
	TK_WRITE,
	TK_RETURN,
	TK_CALL,
	TK_RECORD,
	TK_ENDRECORD,
	TK_ELSE,
} Token_ID;

/*
 * Token structure
 */
typedef struct {
	Token_ID token_id;
	int lineno;
	char lexeme[TOK_LEN];
} Token;

// Token names
const char* token_name[] = {
	"TK_LT",
	"TK_LE",
	"TK_ASSIGNOP",
	"TK_COMMENT",
	"TK_AND",
	"TK_OR",
	"TK_NOT",
	"TK_EQ",
	"TK_GT",
	"TK_GE",
	"TK_NE",
	"TK_ID",
	"TK_NUM",
	"TK_RNUM",
	"TK_FUNID",
	"TK_RUID",
	"TK_SQL",
	"TK_SQR",
	"TK_COMMA",
	"TK_SEM",
	"TK_COLON",
	"TK_DOT",
	"TK_OP",
	"TK_CL",
	"TK_PLUS",
	"TK_MINUS",
	"TK_MUL",
	"TK_DIV",
	"TK_FIELDID",
	"TK_WITH",
	"TK_PARAMETERS",
	"TK_END",
	"TK_WHILE",
	"TK_UNION",
	"TK_ENDUNION",
	"TK_DEFINETYPE",
	"TK_AS",
	"TK_TYPE",
	"TK_MAIN",
	"TK_GLOBAL",
	"TK_PARAMETER",
	"TK_LIST",
	"TK_INPUT",
	"TK_OUTPUT",
	"TK_INT",
	"TK_REAL",
	"TK_ENDWHILE",
	"TK_IF",
	"TK_THEN",
	"TK_ENDIF",
	"TK_READ",
	"TK_WRITE",
	"TK_RETURN",
	"TK_CALL",
	"TK_RECORD",
	"TK_ENDRECORD",
	"TK_ELSE",
};
