CC=gcc
CFLAGS=-Wall
SRC=src/
BIN=bin/
OBJ=obj/

#Prints info
info: 
	@echo "make server		- Compiles the code for ifitron"
	@echo "make client		- Compiles the code for highscoreserver"
	@echo "make runclient		- Runs ifitron"
	@echo "make runserver		- Runs highscoreserver"
	@echo "make runmulticlient" - Runs ifitron on multiplayer setup
	@echo "make runmultiserver" - Runs the multiplayer server
	@echo "make valgrindtron	- Runs ifitron with valgrind"
	@echo "make valgrindserver	- Runs highscoreserver with valgrind"
	@echo "make makedirs		- Makes directories and moves files to them for categorizing"
	@echo "make resetdirs		- Reset directories and files to standard paths"

#Links the .o-files
server: highscoreserver.o netcomserver.o bit_handling.o server_handler.o memory.o moveobjs
	$(CC) $(CFLAGS) -DSERVER $(OBJ)netcom.o $(OBJ)bit_handling.o $(OBJ)server_handler.o $(OBJ)memory.o $(OBJ)highscoreserver.o -o $(BIN)highscoreserver

client: engine.o player.o bit_handling.o ifitron.o netcomclient.o client_handler.o memory.o moveobjs
	$(CC) $(CFLAGS) $(OBJ)engine.o $(OBJ)player.o $(OBJ)bit_handling.o $(OBJ)ifitron.o $(OBJ)netcom.o $(OBJ)client_handler.o $(OBJ)memory.o -o $(BIN)ifitron -lncurses	

#For making the .o-files
highscoreserver.o:
	$(CC) $(CFLAGS) -c $(SRC)highscoreserver.c
netcomclient.o: 
	$(CC) $(CFLAGS) -c -DCLIENT $(SRC)netcom.c
netcomserver.o: 
	$(CC) $(CFLAGS) -c -DSERVER $(SRC)netcom.c
server_handler.o: 
	$(CC) $(CFLAGS) -c $(SRC)server_handler.c
memory.o: 
	$(CC) $(CFLAGS) -c $(SRC)memory.c
engine.o: 
	$(CC) $(CFLAGS) -c $(SRC)engine.c
player.o: 
	$(CC) $(CFLAGS) -c $(SRC)player.c
bit_handling.o: 
	$(CC) $(CFLAGS) -c $(SRC)bit_handling.c
ifitron.o: 
	$(CC) $(CFLAGS) -c $(SRC)ifitron.c	
client_handler.o: 
	$(CC) $(CFLAGS) -c $(SRC)client_handler.c	

#Run with valgrind for debugging purposes
valgrindtron:
	@echo "Running client with Valgrind..........."
	valgrind -v $(BIN)./ifitron --hs 127.0.0.1 --hp 2730
valgrindserver:
	@echo "Running server with Valgrind..........."
	valgrind -v $(BIN)./highscoreserver --p 2730 --load highscore.sav
#Moves the .o-files to their destined directory
moveobjs:
	mv *.o $(OBJ)

#Categorizes filetypes in folders
makedirs:
	@echo "Making directories for first time use"
	mkdir $(SRC)
	mkdir $(BIN)
	mkdir $(OBJ)
	mv netcom.c $(SRC)
	mv server_handler.c $(SRC)
	mv memory.c $(SRC)
	mv engine.c $(SRC)
	mv player.c $(SRC)
	mv bit_handling.c $(SRC)
	mv ifitron.c $(SRC)
	mv client_handler.c $(SRC)
	mv highscoreserver.c $(SRC)
	mv netcom.h $(SRC)
	mv server_handler.h $(SRC)
	mv memory.h $(SRC)
	mv engine.h $(SRC)
	mv player.h $(SRC)
	mv bit_handling.h $(SRC)
	mv client_handler.h $(SRC)
	mv highscoreserver.h $(SRC)
	mv structs.h $(SRC)
	mv ipportchecks.h $(SRC)
	mv highscore.txt $(SRC)
#For me during testing
runmultiserver:
	$(BIN)./highscoreserver --p 2730 --load highscore.sav -m
runserver:
	$(BIN)./highscoreserver --p 2730 --load highscore.sav
runclient:
	$(BIN)./ifitron --hs 127.0.0.1 --hp 2730
runmulticlient:
	$(BIN)./ifitron --ms 127.0.0.1 --mp 2730
#Resets the files to their original paths when downloadet from 1060-page
clear:
	@echo "Deletes the .o-files"
	rm $(OBJ)*.o
compress:
	@echo "Compresses all files to a .tar.gzip-archive"
	tar -cvzf 360.tgz *

