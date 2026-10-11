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

/*
 * Fills an array with a given value
 */
void fill(int arr[], int val, int len);

/**
 * Initialize the DFA
 * Add transition rules and mark accept states
 */
void init_dfa();

/*
 * Check if a TK_ID lexeme is actually a keyword
 * Returns the keyword token ID if matched, else TK_ID
 */
Token_ID get_keyword_token(int dfa, const char *lexeme);

/*
 * Read file and put contents in character buffer
 */
int read_file(char *file_name, char *file_buf);

/*
 * Convert a character to a character group
 * Some large character groups are clumped together
 * Single character groups are addressed directly via their ASCII value
 * Largest character with a special group has ASCII value 126
 */
int char_to_group(char c);

/* 
 * Advances DFA by one step
 * Returns -2 if invalid character
 * Returns -1 if not immediately accepted
 * Returns x (x >= 0) if token accepted where x is the number of retractions
 */
int advance_dfa(char c, int *dfa, char *curr_lexeme, int lexeme_length);

/*
 * Creates new token from current dfa state
 */
void create_token(Token *token, int dfa, char *curr_lexeme, int lexeme_length, int lineno);

/*
 * Remove comments from source code in file_buf and put 
 * cleaned code in new_file
 */
void remove_comments(char *file_buf, char *new_file);

/*
 * Get token stream
 * Returns 1 if tokenization was successful else 0
 */
int get_tokens(char *file_name, Token tokens[]);

