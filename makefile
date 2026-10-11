# Authors:
# Siddhant Attavar: 2023A7TS0659P
# Karthik Tenneti: 2023A7PS0650P
# Neeharika Maram: 2023A7PS0644P
# Kunal Verma: 2023A7PS0565P
# Samhita Ganduri: 2023A7PS0046P
# Aashritha Lakshmi Mallampati: 2023A7PS0583P

stage1exe: build/driver.o build/lexer.o build/parser.o build/utils.o
	gcc build/driver.o build/lexer.o build/parser.o build/utils.o -o stage1exe

build/driver.o: driver.c | build
	gcc -c driver.c -o build/driver.o
build/lexer.o: lexer.c | build
	gcc -c lexer.c -o build/lexer.o
build/parser.o: parser.c | build
	gcc -c parser.c -o build/parser.o
build/utils.o: utils.c | build
	gcc -c utils.c -o build/utils.o

build:
	mkdir -p build

clean:
	rm -r build
