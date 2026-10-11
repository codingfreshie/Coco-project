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
#include <ctype.h>
#include "lexer.h"
#include <stdlib.h>

#define MAX_PRODUCTIONS 100
#define MAX_SYMBOLS 130
#define MAX_LEN 80

typedef struct {
    char rules[MAX_PRODUCTIONS][MAX_SYMBOLS][MAX_LEN];
    int productionCount;
} Grammar;

typedef struct {
    int FIRST[MAX_SYMBOLS][MAX_SYMBOLS];
    int FOLLOW[MAX_SYMBOLS][MAX_SYMBOLS];

    char nonTerminals[MAX_SYMBOLS][MAX_LEN];
    int nonTerminalCount;

    char terminals[MAX_SYMBOLS][MAX_LEN];
    int terminalCount;

    int epsilonIndex;
    int dollarIndex;
} FirstAndFollow;

/* ================= HELPERS ================= */

int isNonTerminal(char *symbol);

int exists(char arr[][MAX_LEN], int count, char *symbol);

void trim(char *str);

/* ================= READ GRAMMAR ================= */

void readGrammar(FILE *fp);

/* ================= SYMBOL COLLECTION ================= */

void collectSymbols(Grammar G, FirstAndFollow *FF);

/* ================= FIRST COMPUTATION ================= */

int getNonTerminalIndex(FirstAndFollow *FF, char *symbol);

int getTerminalIndex(FirstAndFollow *FF, char *symbol);

void initializeFIRST(FirstAndFollow *FF);

void computeFIRST(Grammar G, FirstAndFollow *FF);

//======================FOLLOW==================================
void initializeFOLLOW(FirstAndFollow *FF);

int addFirstOfSequence(Grammar G, FirstAndFollow *FF,
                       int B, int prodIndex, int startPos); 

void computeFOLLOW(Grammar G, FirstAndFollow *FF);


void printFirstAndFollow(FirstAndFollow *FF);

//==================Parse Table===========
int addFirstOfRHS(Grammar G, FirstAndFollow *FF,
                  int prodIndex, int *result);

void createParseTable(Grammar G,
                      FirstAndFollow *FF,
                      int parseTable[MAX_SYMBOLS][MAX_SYMBOLS]);

void printParseTable(FirstAndFollow *FF,int parseTable[MAX_SYMBOLS][MAX_SYMBOLS]);

//================== preditive parser =========

typedef struct parseTreeNode {
    int isLeaf;                 // 1 = terminal, 0 = non-terminal
    int symbol;                 // terminal or non-terminal id
    Token tk;                   // valid if leaf
    struct parseTreeNode *parent;
    struct parseTreeNode *child;
    struct parseTreeNode *sibling;
} parseTreeNode;

typedef struct StackNode {
    char symbol[MAX_LEN];
    struct StackNode *next;
} StackNode;

StackNode* push(StackNode *top, char *symbol);

StackNode* pop(StackNode *top);

int isTerminalSymbol(FirstAndFollow *FF, char *symbol);

void parseInputSourceCode(char *testcaseFile,
                          Grammar G,
                          FirstAndFollow *FF,
                          int parseTable[MAX_SYMBOLS][MAX_SYMBOLS]);

/* ================= MAIN ================= */

void ComputeFirstAndFollowSets(Grammar G, FirstAndFollow *FF);

int main_parser();

