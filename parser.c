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
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCTIONS  250
#define MAX_SYMBOLS      150
#define MAX_LEN          100
#define MAX_TREE_NODES   200000
#define STACK_SIZE       10000
#define TOK_LEN          100
#define MAX_TOKENS       10000


typedef struct {
    char token_name[MAX_LEN];
    int  lineno;
    char lexeme[TOK_LEN];
} Token;

/* -------------------------------------------------------
 * Grammar structures
 * ------------------------------------------------------- */
typedef struct {
    char rules[MAX_PRODUCTIONS][MAX_SYMBOLS][MAX_LEN];
    int  productionCount;
} grammar;

typedef struct {
    int  FIRST[MAX_SYMBOLS][MAX_SYMBOLS];
    int  FOLLOW[MAX_SYMBOLS][MAX_SYMBOLS];
    char nonTerminals[MAX_SYMBOLS][MAX_LEN];
    int  ntCount;
    char terminals[MAX_SYMBOLS][MAX_LEN];
    int  tCount;
    int  epsIdx;
    int  dolIdx;
} FirstAndFollow;

typedef int table[MAX_SYMBOLS][MAX_SYMBOLS];

/* -------------------------------------------------------
 * Parse Tree Node
 * ------------------------------------------------------- */
typedef struct ParseTreeNode {
    char symbol[MAX_LEN];
    int  isLeaf;
    Token tokenInfo;
    struct ParseTreeNode *firstChild;
    struct ParseTreeNode *nextSibling;
    struct ParseTreeNode *parent;
} *parseTree;

/* -------------------------------------------------------
 * Globals
 * ------------------------------------------------------- */
grammar        G;
FirstAndFollow FF;
table          T;

static struct ParseTreeNode nodePool[MAX_TREE_NODES];
static int nodePoolIdx = 0;

Token rawTokenStream[MAX_TOKENS];
Token cleanTokenStream[MAX_TOKENS];

/* -------------------------------------------------------
 * load_tokens_from_file
 * Reads lines: "Token at line X: TK_NAME -> 'lexeme'"
 * ------------------------------------------------------- */
int load_tokens_from_file(const char *filename, Token tokens[]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { printf("Error: Could not open %s\n", filename); return 0; }
    char line[512];
    int  count = 0;
    while (fgets(line, sizeof(line), fp)) {
        int  lineno;
        char tok_name[MAX_LEN], lexeme[TOK_LEN];
        if (sscanf(line, "Token at line %d: %s -> '%[^']'", &lineno, tok_name, lexeme) == 3) {
            tokens[count].lineno = lineno;
            strcpy(tokens[count].token_name, tok_name);
            strcpy(tokens[count].lexeme,     lexeme);
            count++;
            if (count >= MAX_TOKENS) { printf("Warning: Max tokens reached.\n"); break; }
        }
    }
    fclose(fp);
    /* sentinel */
    strcpy(tokens[count].token_name, "$");
    strcpy(tokens[count].lexeme,     "$");
    tokens[count].lineno = -1;
    return 1;
}

/* -------------------------------------------------------
 * Parse Tree helpers
 * ------------------------------------------------------- */
parseTree createNode(const char *sym, int isLeaf, parseTree parent) {
    if (nodePoolIdx >= MAX_TREE_NODES) return NULL;
    parseTree n   = &nodePool[nodePoolIdx++];
    strcpy(n->symbol, sym);
    n->isLeaf     = isLeaf;
    n->parent     = parent;
    n->firstChild = n->nextSibling = NULL;
    n->tokenInfo.token_name[0] = '\0';
    n->tokenInfo.lexeme[0]     = '\0';
    n->tokenInfo.lineno        = -1;
    return n;
}

void addChild(parseTree parent, parseTree child) {
    if (!parent->firstChild) { parent->firstChild = child; return; }
    parseTree t = parent->firstChild;
    while (t->nextSibling) t = t->nextSibling;
    t->nextSibling = child;
}

/* -------------------------------------------------------
 * Grammar helpers
 * ------------------------------------------------------- */
int isNT(const char *s) { return s[0] == '<'; }

int getIdx(char arr[][MAX_LEN], int count, const char *s) {
    for (int i = 0; i < count; i++) if (strcmp(arr[i], s) == 0) return i;
    return -1;
}

void readGrammar(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { printf("Error: Could not open grammar %s\n", filename); exit(1); }
    char line[512];
    G.productionCount = 0;
    while (fgets(line, sizeof(line), fp)) {
        char *arrow = strstr(line, "===>");
        if (!arrow) continue;
        *arrow = '\0';
        char lhs[MAX_LEN]; sscanf(line, "%s", lhs);
        char *rhsPart = arrow + 4;
        char *tok = strtok(rhsPart, " \t\n\r");
        int symIdx = 0;
        strcpy(G.rules[G.productionCount][symIdx++], lhs);
        while (tok) {
            if (strcmp(tok, "|") == 0) {
                G.productionCount++;
                symIdx = 0;
                strcpy(G.rules[G.productionCount][symIdx++], lhs);
            } else {
                strcpy(G.rules[G.productionCount][symIdx++], tok);
            }
            tok = strtok(NULL, " \t\n\r");
        }
        G.productionCount++;
    }
    fclose(fp);
}

/* -------------------------------------------------------
 * ComputeFirstAndFollowSets
 * ------------------------------------------------------- */
FirstAndFollow ComputeFirstAndFollowSets(grammar g) {
    FirstAndFollow f;
    memset(&f, 0, sizeof(f));
    f.epsIdx = -1; f.dolIdx = -1;

    for (int i = 0; i < g.productionCount; i++) {
        if (getIdx(f.nonTerminals, f.ntCount, g.rules[i][0]) == -1)
            strcpy(f.nonTerminals[f.ntCount++], g.rules[i][0]);
        for (int j = 1; strlen(g.rules[i][j]) > 0; j++) {
            char *s = g.rules[i][j];
            if (isNT(s)) {
                if (getIdx(f.nonTerminals, f.ntCount, s) == -1)
                    strcpy(f.nonTerminals[f.ntCount++], s);
            } else if (strcmp(s, "eps") != 0 && getIdx(f.terminals, f.tCount, s) == -1) {
                strcpy(f.terminals[f.tCount++], s);
            }
        }
    }
    strcpy(f.terminals[f.tCount], "eps"); f.epsIdx = f.tCount++;
    strcpy(f.terminals[f.tCount], "$");   f.dolIdx = f.tCount++;

    int changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < g.productionCount; i++) {
            int lhs = getIdx(f.nonTerminals, f.ntCount, g.rules[i][0]);
            int nullable = 1;
            for (int j = 1; strlen(g.rules[i][j]) > 0; j++) {
                char *s = g.rules[i][j];
                if (strcmp(s, "eps") == 0) break;
                if (!isNT(s)) {
                    int tid = getIdx(f.terminals, f.tCount, s);
                    if (!f.FIRST[lhs][tid]) { f.FIRST[lhs][tid] = 1; changed = 1; }
                    nullable = 0; break;
                }
                int rhs = getIdx(f.nonTerminals, f.ntCount, s);
                for (int k = 0; k < f.tCount; k++)
                    if (k != f.epsIdx && f.FIRST[rhs][k] && !f.FIRST[lhs][k]) { f.FIRST[lhs][k] = 1; changed = 1; }
                if (!f.FIRST[rhs][f.epsIdx]) { nullable = 0; break; }
            }
            if (nullable && !f.FIRST[lhs][f.epsIdx]) { f.FIRST[lhs][f.epsIdx] = 1; changed = 1; }
        }
    }

    f.FOLLOW[0][f.dolIdx] = 1;
    changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < g.productionCount; i++) {
            int lhs = getIdx(f.nonTerminals, f.ntCount, g.rules[i][0]);
            for (int j = 1; strlen(g.rules[i][j]) > 0; j++) {
                if (!isNT(g.rules[i][j])) continue;
                int B = getIdx(f.nonTerminals, f.ntCount, g.rules[i][j]);
                int k = j + 1, nullable = 1;
                while (strlen(g.rules[i][k]) > 0) {
                    if (!isNT(g.rules[i][k])) {
                        int tid = getIdx(f.terminals, f.tCount, g.rules[i][k]);
                        if (!f.FOLLOW[B][tid]) { f.FOLLOW[B][tid] = 1; changed = 1; }
                        nullable = 0; break;
                    }
                    int C = getIdx(f.nonTerminals, f.ntCount, g.rules[i][k]);
                    for (int t = 0; t < f.tCount; t++)
                        if (t != f.epsIdx && f.FIRST[C][t] && !f.FOLLOW[B][t]) { f.FOLLOW[B][t] = 1; changed = 1; }
                    if (!f.FIRST[C][f.epsIdx]) { nullable = 0; break; }
                    k++;
                }
                if (nullable)
                    for (int t = 0; t < f.tCount; t++)
                        if (f.FOLLOW[lhs][t] && !f.FOLLOW[B][t]) { f.FOLLOW[B][t] = 1; changed = 1; }
            }
        }
    }
    return f;
}

/* -------------------------------------------------------
 * createParseTable
 * ------------------------------------------------------- */
void createParseTable(FirstAndFollow f, table t) {
    for (int i = 0; i < MAX_SYMBOLS; i++)
        for (int j = 0; j < MAX_SYMBOLS; j++) t[i][j] = -1;
    for (int i = 0; i < G.productionCount; i++) {
        int u = getIdx(f.nonTerminals, f.ntCount, G.rules[i][0]);
        int firstSet[MAX_SYMBOLS] = {0};
        int isNullable = 1;
        for (int j = 1; strlen(G.rules[i][j]) > 0; j++) {
            char *s = G.rules[i][j];
            if (strcmp(s, "eps") == 0) break;
            if (!isNT(s)) {
                firstSet[getIdx(f.terminals, f.tCount, s)] = 1;
                isNullable = 0; break;
            }
            int v = getIdx(f.nonTerminals, f.ntCount, s);
            for (int k = 0; k < f.tCount; k++) if (k != f.epsIdx && f.FIRST[v][k]) firstSet[k] = 1;
            if (!f.FIRST[v][f.epsIdx]) { isNullable = 0; break; }
        }
        for (int k = 0; k < f.tCount; k++) if (firstSet[k]) t[u][k] = i;
        if (isNullable)
            for (int k = 0; k < f.tCount; k++)
                if (f.FOLLOW[u][k] && t[u][k] == -1) t[u][k] = i;
    }
}

/* -------------------------------------------------------
 * parseInputSourceCode
 * ------------------------------------------------------- */
typedef struct { parseTree items[STACK_SIZE]; int top; } Stack;
void push(Stack *s, parseTree n) { s->items[++s->top] = n; }
parseTree pop(Stack *s) { return s->items[s->top--]; }

parseTree parseInputSourceCode(char *lexFile, table t) {

    if (!load_tokens_from_file(lexFile, rawTokenStream))
        return NULL;

    /* Filter comments */
    int validCount = 0;
    for (int i = 0; strlen(rawTokenStream[i].token_name) > 0; i++) {
        if (strcmp(rawTokenStream[i].token_name, "TK_COMMENT") == 0)
            continue;
        cleanTokenStream[validCount++] = rawTokenStream[i];
    }

    strcpy(cleanTokenStream[validCount].token_name, "$");
    strcpy(cleanTokenStream[validCount].lexeme, "$");
    cleanTokenStream[validCount].lineno = -1;

    nodePoolIdx = 0;

    Stack s;
    s.top = -1;

    parseTree root   = createNode(FF.nonTerminals[0], 0, NULL);
    parseTree dollar = createNode("$", 1, NULL);

    push(&s, dollar);
    push(&s, root);

    int tokenPtr = 0;
    int syntaxErrors = 0;

    while (s.top >= 0) {

        parseTree X = pop(&s);
        Token *lookahead = &cleanTokenStream[tokenPtr];

        /* EOF protection */
        if (strcmp(lookahead->token_name, "$") == 0 && s.top <= 0)
            break;

        /* -------------------- TERMINAL -------------------- */
        if (X->isLeaf) {

            if (strcmp(X->symbol, "eps") == 0)
                continue;

            if (strcmp(X->symbol, lookahead->token_name) == 0) {

                /* MATCH */
                X->tokenInfo = *lookahead;
                tokenPtr++;

            } else {

                /* TERMINAL PANIC MODE */
                fprintf(stderr,
                    "Line %d: Error: The token %s for lexeme %s does not match expected token %s\n",
                    lookahead->lineno,
                    lookahead->token_name,
                    lookahead->lexeme,
                    X->symbol);

                syntaxErrors++;

                /* Skip bad token */
                tokenPtr++;

                /* If not EOF, retry same terminal */
                if (strcmp(cleanTokenStream[tokenPtr].token_name, "$") != 0)
                    push(&s, X);
            }
        }

        /* -------------------- NON-TERMINAL -------------------- */
        else {

            int u = getIdx(FF.nonTerminals, FF.ntCount, X->symbol);
            int v = getIdx(FF.terminals, FF.tCount, lookahead->token_name);

            int ruleIdx = (u != -1 && v != -1) ? t[u][v] : -1;

            if (ruleIdx == -1) {

                /* NON-TERMINAL PANIC MODE */
                fprintf(stderr,
                    "Line %d: Error: Unexpected token %s ('%s') while parsing %s\n",
                    lookahead->lineno,
                    lookahead->token_name,
                    lookahead->lexeme,
                    X->symbol);

                syntaxErrors++;

                int skipped = 0;

                while (1) {

                    int followIdx = getIdx(
                        FF.terminals,
                        FF.tCount,
                        cleanTokenStream[tokenPtr].token_name
                    );

                    /* Stop if token in FOLLOW(X) and at least one skipped */
                    if (followIdx != -1 &&
                        FF.FOLLOW[u][followIdx] &&
                        skipped > 0)
                        break;

                    if (strcmp(cleanTokenStream[tokenPtr].token_name, "$") == 0)
                        break;

                    tokenPtr++;
                    skipped++;
                }

                /* Pop X automatically (do not expand it) */
                continue;
            }

            /* -------------------- APPLY PRODUCTION -------------------- */

            int rhsCount = 0;
            while (strlen(G.rules[ruleIdx][rhsCount + 1]) > 0)
                rhsCount++;

            parseTree children[MAX_SYMBOLS];

            for (int i = 0; i < rhsCount; i++) {
                char *sym = G.rules[ruleIdx][i + 1];
                children[i] = createNode(sym, !isNT(sym), X);
                addChild(X, children[i]);
            }

            for (int i = rhsCount - 1; i >= 0; i--)
                push(&s, children[i]);
        }
    }

    /* FINAL STATUS */
    if (syntaxErrors == 0)
        printf("Input source code is syntactically correct...........\n");
    else
        printf("Parsing completed with %d syntax errors.\n", syntaxErrors);

    return root;
}

/* -------------------------------------------------------
 * printParseTree — inorder traversal
 * ------------------------------------------------------- */
void inorder(parseTree node, FILE *out) {
    if (!node) return;
    if (node->firstChild) inorder(node->firstChild, out);

    char lexeme[MAX_LEN], lineStr[16], tokenName[MAX_LEN];
    char valueStr[MAX_LEN], parentSym[MAX_LEN], nodeSym[MAX_LEN];

    if (node->isLeaf && strcmp(node->symbol, "eps") != 0) {
        strcpy(lexeme,    node->tokenInfo.lexeme);
        sprintf(lineStr,  "%d", node->tokenInfo.lineno);
        strcpy(tokenName, node->tokenInfo.token_name);
        if (strcmp(node->tokenInfo.token_name, "TK_NUM")  == 0 ||
            strcmp(node->tokenInfo.token_name, "TK_RNUM") == 0)
            strcpy(valueStr, node->tokenInfo.lexeme);
        else
            strcpy(valueStr, "----");
        strcpy(nodeSym, "----");
    } else {
        strcpy(lexeme,    "----");
        strcpy(lineStr,   "----");
        strcpy(tokenName, "----");
        strcpy(valueStr,  "----");
        strcpy(nodeSym,   node->symbol);
    }
    strcpy(parentSym, node->parent ? node->parent->symbol : "ROOT");

    fprintf(out, "%-20s | %-6s | %-20s | %-13s | %-26s | %-6s | %-25s\n",
            lexeme, lineStr, tokenName, valueStr, parentSym,
            node->isLeaf ? "yes" : "no", nodeSym);

    if (node->nextSibling) inorder(node->nextSibling, out);
}

void printParseTree(parseTree PT, char *outfile) {
    FILE *out = fopen(outfile, "w");
    if (!out) { printf("Failed to create output file.\n"); return; }

    fprintf(out, "%-20s | %-6s | %-20s | %-13s | %-26s | %-6s | %-25s\n",
            "lexeme", "lineno", "tokenName", "valueIfNumber",
            "parentNodeSymbol", "isLeaf", "NodeSymbol");
    fprintf(out, "%.20s-+-%.6s-+-%.20s-+-%.13s-+-%.26s-+-%.6s-+-%.25s\n",
            "--------------------","------","--------------------",
            "-------------","--------------------------","------",
            "-------------------------");

    inorder(PT, out);
    fclose(out);
    printf("Parse tree written to 'parsetree_output.txt'\n");
}

/* -------------------------------------------------------
 * main_parser — called from driver.c with lex output file path
 * ------------------------------------------------------- */
void main_parser(char *lexFile) {
    readGrammar("grammar.txt");
    FF = ComputeFirstAndFollowSets(G);
    createParseTable(FF, T);

    printf("\n--- Parsing ---\n");
    parseTree tree = parseInputSourceCode(lexFile, T);
    if (tree) printParseTree(tree, "parsetree_output.txt");
}
