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
#include <time.h>
#include "lexer.h"
#include "parser.h"

/*
 * Read file and put contents in character buffer
 */
int read_file(char *file_name, char *file_buf) {
	FILE *fptr = fopen(file_name, "r");
	if (fptr == NULL) {
		return 0;
	}
	int i = 0;
	char c;
	while ((c = fgetc(fptr)) != EOF) {
		file_buf[i++] = c;
	}
	file_buf[i] = '\0';
	fclose(fptr);
	return 1;
}

/*
 * Main function
 */
int main(int argc, char *argv[]) {
	// Print project status
	printf("\n\n\n");
	printf("Both lexical and syntax analysis modules implemented\n");
	printf("FIRST and FOLLOW set automated\n");
	printf("Lexical errors reported\n");
	printf("Syntactic errors partially reported\n");
	printf("modules work with testcases 2, 3, 4, 5 \n");
	printf("modules partially work with testcases 1 and 6 (errors reported)\n");

	if (argc != 3) {
		printf("Invalid arguments\n");
		printf("Instructions: \n");
		printf("./stage1exe testcase.txt parserOutFile.txt\n");
		return 0;
	}

	// Initialize the DFA
	init_dfa();
	// Read source file
	char *file_name = argv[1];
	char file_buf[MAX_CHARS];
	printf("Source file name: %s\n", file_name);
	if (!read_file(file_name, file_buf)) {
		printf("File not found\n");
		return 0;
	}

	double parser_cpu_time = -1;
	while (1) {
		// Print menu
		printf("\n\n\n");
		printf("0. Exit\n");
		printf("1. Remove comments\n");
		printf("2. Print token list\n");
		printf("3. Verify syntactic correctness\n");
		printf("4. Display time\n");
		printf("Enter operation: ");

		int operation;
		scanf("%d", &operation);
		printf("\n\n");

		if (operation == 0) {
			break;
		}
		else if (operation == 1) {
			// Remove comments
			char new_buf[MAX_CHARS];
			remove_comments(file_buf, new_buf);
			printf("Cleaned code:\n%s", new_buf);
		}
		else if (operation == 2) {
			// Print token list
			Token tokens[MAX_TOKENS];
			if (get_tokens(file_buf, tokens)) {
				printf("Lexical analysis successful!\n");
			}
			else {
				printf("Lexical analysis unsuccessful! (Errors found)\n");
			}
		}
		else if (operation == 3) {
			// Step 1: run lexer and write output to file
			const char *lex_out = "lex_output.txt";
			FILE *fp = freopen(lex_out, "w", stdout);
			if (!fp) {
				printf("Error: could not open %s for writing\n", lex_out);
				operation = -1;
				continue;
			}
			Token tokens[MAX_TOKENS];
			int lexer_status = get_tokens(file_buf, tokens);
			freopen("/dev/tty", "w", stdout);
			if (lexer_status) {
				printf("Lexical analysis successful!\n");
			}
			else {
				printf("Lexical analysis unsuccessful! (Errors found)\n");
			}

			// Restore stdout

			printf("Lexer output written to '%s'\n", lex_out);

			// Step 2: parse from that file
			clock_t start_time = clock();
			main_parser((char *)lex_out);
			clock_t end_time = clock();
			parser_cpu_time = (double)(end_time - start_time);
			printf("\nParsing completed.\n");
		}
		else if (operation == 4) {
			// Display time
			if (parser_cpu_time == -1) {
				printf("Lexer and/or parser have not run\n");
				printf("Select operation 3 first\n");
			}
			else {
				printf("Lexer and parser perform time taken\n");
				printf("Execution CPU time: %f\n", parser_cpu_time);
				printf("Execution time (seconds): %f\n", parser_cpu_time / CLOCKS_PER_SEC);
			}
		}
		else {
			printf("Invalid operation selected\n");
		}
		operation = -1;
	}
}
