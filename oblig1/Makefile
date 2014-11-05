.PHONY: all clean zip reset test_assignment3 encode_test decode_test sting_run encode_run decode_run assignment3_run

# Kompilerer alle oppgavene
all: sting encode  decode assignment3
# Kjører alle oppgavene (Et eksempel for sting)
run: sting_run encode_run decode_run assignment3_run

encode_test: compress_test.c encode_test.c compress_test.h
	@echo "\n"
	@echo '-----------------------------------------------'
	gcc encode_test.c compress_test.c -O2 -o encode_test -lpthread
	@echo '-----------------------------------------------'

decode_test: compress_test.c decode_test.c compress_test.h
	@echo "\n"
	@echo '-----------------------------------------------'
	gcc $^ -O2 -o $@ -lpthread
	@echo '-----------------------------------------------'

# tests just compiles the test
tests: encode_test decode_test

# while test compiles and runs them
test: tests encode decode 
	@echo "\n"
	@echo '-----------------------------------------------'
	@echo 'Testing encoder:'
	./encode_test ./encode
	@echo '-----------------------------------------------'
	@echo "\n"
	@echo '-----------------------------------------------'
	@echo 'Testing decoder:'
	./decode_test ./decode
	@echo '-----------------------------------------------'

#The test-file for assignment 3 didn't work on my computer (Mac). 
test_assignment3: 
	@echo 'Testing assignment 3:'
	./assignment3_test
	@echo "\n"

# Run the files
encode_run: encode.c encode.h 
	@echo "\n"
	@echo '-----------------------------------------------'
	@echo 'Running encode.c'
	@echo 'Compresses "ghost.txt" to file "ut.compressed"'
	./encode ghost.txt ut.compressed	
	@echo '-----------------------------------------------'
decode_run: decode.c decode.h 
	@echo "\n"
	@echo '-----------------------------------------------'
	@echo 'Running decode.c'
	@echo 'Decompresses "ut.compressed" to terminal'
	./decode ut.compressed -
	@echo '-----------------------------------------------'
sting_run: sting.c header.h
	@echo "\n"
	@echo '-----------------------------------------------'
	@echo 'Running sting.c'
	@echo 'Example: Prints "tresmaa.txt" with all possible vowels to terminal'
	./sting replace tresmaa.txt -
	@echo '-----------------------------------------------'
assignment3_run: sting.c header.h
	@echo "\n"
	@echo '-----------------------------------------------'
	@echo 'Running assignment3.c'
	./assignment3
	@echo '-----------------------------------------------'
	
# Compile the files
encode: encode.c encode.h
	@echo "\n"
	@echo '-----------------------------------------------'
	@echo 'Compiling encode.c'
	gcc -Wall -g encode.c -o encode
	@echo '-----------------------------------------------'
decode: decode.c decode.h
	@echo "\n"
	@echo '-----------------------------------------------'
	@echo 'Compiling decode.c'
	gcc -Wall -g decode.c -o decode
	@echo '-----------------------------------------------'
sting:	sting.c header.h
	@echo "\n"
	@echo '-----------------------------------------------'
	@echo 'Compiling sting.c'
	gcc -Wall -g sting.c -o sting
	@echo '-----------------------------------------------'
assignment3: assignment3.c assignment3.h
	@echo "\n"
	@echo '-----------------------------------------------'
	@echo 'Compiling assignment3.c'
	gcc -Wall -g assignment3.c -o assignment3
	@echo '-----------------------------------------------'
	
# Only in here to give you ideas
make_tar: *.c *.h *.compressed *.txt Makefile
	@echo "\n"
	@echo '-----------------------------------------------'
	@echo "Compresses all .c, .h, .txt, .compressed and the Makefile to"
	@echo "a .tgz-library\n"
	tar -cvzf roarher.tgz *.c *.h *.compressed *.txt Makefile
	@echo '-----------------------------------------------'

# Oh be carefull with this one
reset: zip precode.c
	@echo "\n!!! WARNING !!! string.c, encode.c and decode.c has been replaced"
	@echo "       Backups as string.bak, encode.bak and decode.bak\n"
	@cp sting.c string.bak
	@cp encode.c encode.bak
	@cp decode.c decode.bak
	@cp precode.c string.c
	@cp precode.c encode.c
	@cp precode.c decode.c

clean: 
	@echo 'Deletes all files created by this Makefile!'
	rm encode
	rm decode
	rm sting
	rm -rf *.dSYM
	rm assignment3
	@echo "\n"