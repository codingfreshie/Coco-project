/**
 * Authors:
 * Siddhant Attavar: 2023A7TS0659P
 * Karthik Tenneti: 2023A7PS0650P
 * Neeharika Maram: 2023A7PS0644P
 * Kunal Verma: 2023A7PS0565P
 * Samhita Ganduri: 2023A7PS0046P
 * Aashritha Lakshmi Mallampati: 2023A7PS0583P
 **/

#include <stdio.h>
#include <string.h>
#include "utils.h"

// DFA related structures
int transition_table[DFA_STATES][CHAR_GROUPS];
int accept[DFA_STATES], token_info[DFA_STATES];

/*
 * Fills an array with a given value
 */
void fill(int arr[], int val, int len) {
	for (int i = 0; i < len; i++) {
		arr[i] = val;
	}
}

/**
 * Initialize the DFA
 * Add transition rules and mark accept states
 */
void init_dfa() {
	memset(accept, -1, sizeof(accept));
	memset(token_info, -1, sizeof(token_info));
	for (int i = 0; i < DFA_STATES; i++) {
		memset(transition_table[i], -1, sizeof(transition_table[i]));
	}

	// "other" transition rules
	fill(transition_table[1], 2, CHAR_GROUPS);
	fill(transition_table[17], 18, CHAR_GROUPS);
	fill(transition_table[22], 23, CHAR_GROUPS);
	fill(transition_table[24], 23, CHAR_GROUPS);
	fill(transition_table[25], 27, CHAR_GROUPS);
	fill(transition_table[26], 27, CHAR_GROUPS);
	fill(transition_table[28], 29, CHAR_GROUPS);
	fill(transition_table[32], 33, CHAR_GROUPS);
	fill(transition_table[34], 35, CHAR_GROUPS);
	fill(transition_table[42], 43, CHAR_GROUPS);
	fill(transition_table[44], 45, CHAR_GROUPS);
	fill(transition_table[47], 48, CHAR_GROUPS);

	// DFA transition rules
	transition_table[0]['!'] = 20;
	transition_table[0][','] = 51;
	transition_table[0]['#'] = 46;
	transition_table[0]['%'] = 7;
	transition_table[0]['&'] = 8;
	transition_table[0]['('] = 55;
	transition_table[0][')'] = 56;
	transition_table[0]['*'] = 59;
	transition_table[0]['+'] = 57;
	transition_table[0]['-'] = 58;
	transition_table[0]['.'] = 54;
	transition_table[0]['/'] = 60;
	transition_table[0][':'] = 53;
	transition_table[0]['<'] = 1;
	transition_table[0]['='] = 15;
	transition_table[0]['>'] = 17;
	transition_table[0]['@'] = 11;
	transition_table[0]['['] = 49;
	transition_table[0][']'] = 50;
	transition_table[0]['_'] = 41;
	transition_table[0]['~'] = 14;
	transition_table[0][127] = 22;
	transition_table[0][128] = 24;
	transition_table[0][129] = 28;
	transition_table[0][130] = 28;
	transition_table[0][';'] = 52;
	transition_table[1]['-'] = 4;
	transition_table[1]['='] = 3;
	transition_table[11]['@'] = 12;
	transition_table[12]['@'] = 13;
	transition_table[15]['='] = 16;
	transition_table[17]['='] = 19;
	transition_table[17][132] = 18;
	transition_table[20]['='] = 21;
	transition_table[22][127] = 22;
	transition_table[22][128] = 22;
	transition_table[24][127] = 22;
	transition_table[24][128] = 22;
	transition_table[24][129] = 25;
	transition_table[24][132] = 23;
	transition_table[25][128] = 25;
	transition_table[25][129] = 26;
	transition_table[25][132] = 27;
	transition_table[26][129] = 26;
	transition_table[26][132] = 27;
	transition_table[28]['.'] = 30;
	transition_table[28][129] = 28;
	transition_table[28][130] = 28;
	transition_table[28][132] = 29;
	transition_table[30][129] = 31;
	transition_table[30][130] = 31;
	transition_table[31][129] = 32;
	transition_table[31][130] = 32;
	transition_table[32][131] = 34;
	transition_table[32][132] = 33;
	transition_table[34]['+'] = 38;
	transition_table[34]['-'] = 38;
	transition_table[34][129] = 36;
	transition_table[34][130] = 36;
	transition_table[34][132] = 35;
	transition_table[36][129] = 37;
	transition_table[36][130] = 37;
	transition_table[38][129] = 39;
	transition_table[38][130] = 39;
	transition_table[39][129] = 40;
	transition_table[39][130] = 40;
	transition_table[4]['-'] = 5;
	transition_table[41][127] = 42;
	transition_table[41][128] = 42;
	transition_table[41][131] = 42;
	transition_table[41][132] = 42;
	transition_table[42][127] = 42;
	transition_table[42][128] = 42;
	transition_table[42][129] = 44;
	transition_table[42][130] = 44;
	transition_table[42][131] = 42;
	transition_table[42][132] = 42;
	transition_table[42][132] = 43;
	transition_table[44][129] = 44;
	transition_table[44][130] = 44;
	transition_table[44][132] = 45;
	transition_table[46][127] = 47;
	transition_table[46][128] = 47;
	transition_table[47][127] = 47;
	transition_table[47][128] = 47;
	transition_table[47][132] = 48;
	transition_table[5]['-'] = 6;
	transition_table[8]['&'] = 9;
	transition_table[9]['&'] = 10;

	// DFA state acceptance rules
	accept[51] = 0;
	accept[7] = 0;
	accept[55] = 0;
	accept[56] = 0;
	accept[59] = 0;
	accept[57] = 0;
	accept[58] = 0;
	accept[54] = 0;
	accept[60] = 0;
	accept[53] = 0;
	accept[49] = 0;
	accept[50] = 0;
	accept[14] = 0;
	accept[52] = 0;
	accept[3] = 0;
	accept[2] = 1;
	accept[13] = 0;
	accept[16] = 0;
	accept[19] = 0;
	accept[18] = 1;
	accept[21] = 0;
	accept[23] = 1;
	accept[27] = 1;
	accept[29] = 1;
	accept[33] = 1;
	accept[35] = 2;
	accept[37] = 0;
	accept[40] = 0;
	accept[43] = 1;
	accept[45] = 1;
	accept[48] = 1;
	accept[6] = 0;
	accept[10] = 0;

	// DFA token_info for accepted states
	token_info[51] = TK_COMMA;
	token_info[7] = TK_COMMENT;
	token_info[55] = TK_OP;
	token_info[56] = TK_CL;
	token_info[59] = TK_MUL;
	token_info[57] = TK_PLUS;
	token_info[58] = TK_MINUS;
	token_info[54] = TK_DOT;
	token_info[60] = TK_DIV;
	token_info[53] = TK_COLON;
	token_info[49] = TK_SQL;
	token_info[50] = TK_SQR;
	token_info[14] = TK_NOT;
	token_info[52] = TK_SEM;
	token_info[3] = TK_LE;
	token_info[2] = TK_LT;
	token_info[13] = TK_OR;
	token_info[16] = TK_EQ;
	token_info[19] = TK_GE;
	token_info[18] = TK_GT;
	token_info[21] = TK_NE;
	token_info[23] = TK_FIELDID;
	token_info[27] = TK_ID;
	token_info[29] = TK_NUM;
	token_info[33] = TK_RNUM;
	token_info[35] = TK_RNUM;
	token_info[37] = TK_RNUM;
	token_info[40] = TK_RNUM;
	token_info[43] = TK_FUNID;
	token_info[45] = TK_FUNID;
	token_info[48] = TK_RUID;
	token_info[6] = TK_ASSIGNOP;
	token_info[10] = TK_AND;
}

/*
 * Check if a TK_ID lexeme is actually a keyword
 * Returns the keyword token ID if matched, else TK_ID
 */
Token_ID get_keyword_token(int dfa, const char *lexeme) {
	struct { const char *word; Token_ID id; } keywords[] = {
		{"with", TK_WITH},
		{"parameters", TK_PARAMETERS},
		{"end", TK_END},
		{"while", TK_WHILE},
		{"union", TK_UNION},
		{"endunion", TK_ENDUNION},
		{"definetype", TK_DEFINETYPE},
		{"as", TK_AS},
		{"type", TK_TYPE},
		{"_main", TK_MAIN},
		{"global", TK_GLOBAL},
		{"parameter", TK_PARAMETER},
		{"list", TK_LIST},
		{"input", TK_INPUT},
		{"output", TK_OUTPUT},
		{"int", TK_INT},
		{"real", TK_REAL},
		{"endwhile", TK_ENDWHILE},
		{"if", TK_IF},
		{"then", TK_THEN},
		{"endif", TK_ENDIF},
		{"read", TK_READ},
		{"write", TK_WRITE},
		{"return", TK_RETURN},
		{"call", TK_CALL},
		{"record", TK_RECORD},
		{"endrecord", TK_ENDRECORD},
		{"else", TK_ELSE},
	};
	int n = sizeof(keywords) / sizeof(keywords[0]);
	for (int i = 0; i < n; i++) {
		if (strcmp(lexeme, keywords[i].word) == 0) {
			return keywords[i].id;
		}
	}
	return token_info[dfa];
}

/*
 * Convert a character to a character group
 * Some large character groups are clumped together
 * Single character groups are addressed directly via their ASCII value
 * Largest character with a special group has ASCII value 126
 */
int char_to_group(char c) {
	if (c == 'a' || c >= 'e' && c <= 'z') return MAX_ASCII + 1; // [a|e-z]
	if (c >= 'b' && c <= 'd') return MAX_ASCII + 2; // [b-d]
	if (c >= '2' && c <= '7') return MAX_ASCII + 3; // [2-7]
	if (c >= '0' && c <= '1' || c >= '8' && c <= '9') return MAX_ASCII + 4; // [0-1|8-9]
	if (c >= 'A' && c <= 'D' || c >= 'F' && c <= 'Z') return MAX_ASCII + 5; // [A-D|F-Z]
	if (c > MAX_ASCII) return MAX_ASCII + 6; // Other
	return c;
}

/* 
 * Advances DFA by one step
 * Returns -2 if invalid character
 * Returns -1 if not immediately accepted
 * Returns x (x >= 0) if token accepted where x is the number of retractions
 */
int advance_dfa(char c, int *dfa, char *curr_lexeme, int lexeme_length) {
	// printf("%c %d %d\n", c, *dfa, char_to_group(c));
	curr_lexeme[lexeme_length] = c;
	lexeme_length++;

	*dfa = transition_table[*dfa][char_to_group(c)];
	if (*dfa == -1) {
		return -2;
	}
	return accept[*dfa];
}

/*
 * Creates new token from current dfa state
 */
void create_token(Token *token, int dfa, char *curr_lexeme, int lexeme_length, int lineno) {
	curr_lexeme[lexeme_length - accept[dfa]] = '\0';
	token->token_id = get_keyword_token(dfa, curr_lexeme);
	token->lineno = lineno;
	strcpy(token->lexeme, curr_lexeme);
	printf("Token at line %d: %s -> '%s'\n", token->lineno, token_name[token->token_id], token->lexeme);
	curr_lexeme[0] = '\0';
}

/*
 * Remove comments from source code in file_buf and put 
 * cleaned code in new_file
 */
void remove_comments(char *file_buf, char *new_file) {
	int length = strlen(file_buf), in_comment = 0, empty_line = 1, j = 0;
	for (int i = 0; i < length; i++) {
		switch(file_buf[i]) {
		case '%':
			in_comment = 1;
			break;
		case '\r':
		case '\n':
			in_comment = 0;
			if (empty_line) {
				break;
			}
			empty_line = 1;
		default:
			if (!in_comment) {
				empty_line = 0;
				new_file[j++] = file_buf[i];
			}
		}
	}
	new_file[j] = '\0';
}

/*
 * Get token stream
 * Returns 1 if tokenization was successful else 0
 */
int get_tokens(char *file_buf, Token tokens[]) {
	// Start tokenization
	printf("Starting lexical analysis\n");
	int lineno = 1, in_comment = 0, token_no = 0, length = strlen(file_buf);
	char curr_lexeme[TOK_LEN];
	int dfa = 0, lexeme_length = 0, i = 0, error = 0;
	for (int i = 0; i < length; i++) {
		switch(file_buf[i]) {
		case '\n':
			if (dfa == 0) {
				lineno++;
			}
			if (in_comment) {
				in_comment = 0;
			}
		case ' ':
		case '\t':
		case '\r':
		case '\v':
		case '\f':
			if (dfa == 0) {
				continue;
			}
		default:
			if (in_comment) {
				continue;
			}

			// pass to DFA, increment token_no if new token, exit if rejected
			int status = advance_dfa(file_buf[i], &dfa, curr_lexeme, lexeme_length);
			lexeme_length++;
			if (status == -2) {
				curr_lexeme[lexeme_length] = '\0';
				printf("Error on line %d. Invalid lexeme %s\n", lineno, curr_lexeme);
				dfa = 0;
				lexeme_length = 0;
				error = 1;
				continue;
			}
			if (status == -1) {
				continue;
			}

			create_token(&tokens[token_no], dfa, curr_lexeme, lexeme_length, lineno);
			i -= status;
			dfa = 0;
			lexeme_length = 0;
			token_no++;
			in_comment = file_buf[i] == '%';
		}
	}
	if (dfa != 0) {
		printf("Error on line %d. Invalid lexeme %s\n", lineno, curr_lexeme);
		error = 1;
	}

	printf("Lexical analysis complete\n");
	return !error;
}

