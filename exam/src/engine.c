//TEGNER TIL SKJERM

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h> // the actual game engine
#include <unistd.h>  // for the usleep () function

#include "engine.h"
#include "player.h"
#include "structs.h"
#include "client_handler.h"


/** This SIMPLE formula determines a time (in microseconds) the game
 *  must wait between each screen refresh. It is based on the current
 *  game level, so the higher its value, the lowest is the delay and
 *  faster the game will appear to be. */

#define   REFRESH_DELAY     (50000 + ((9 - game.level) * 10000))


static char PLAYER_HEAD_CHAR = '@'; ///< The 'image' of the snake head
static char PLAYER_CHAR	     = 'o'; ///< The 'image' of the snake body
static char BORDER_CHAR	     = '#'; ///< The 'image' of the border in normal mode
static char MENU_BORDER_CHAR = '*'; ///< The 'image' of the border in the main menu


/*Simple enum to make the colors easier to read: FOREGROUND_BACKGROUND */
enum Colors { BLACK_WHITE = 1, CYAN_BLACK, BLUE_BLACK,
              WHITE_BLACK, GREEN_BLACK, RED_BLACK,  };

struct screen_t screen;
struct game_t game;
//For the username to be shown in the game
char username[8];
//For the error message
char msg[300];
//For printing an message in game
int print_message = 0;
int is_multi;

/**	Just erases everything to black
 */
void draw_background ()
{
	clear();
}
/* Draws the window border */
void draw_borders ()
{
	int i;

	attron (COLOR_PAIR (WHITE_BLACK));
	for (i = 0; i <= (screen.width-1); i++)	
	  {
	    mvaddch (1, i, BORDER_CHAR);
	    mvaddch ((screen.height-1), i, BORDER_CHAR);
	  }
	for (i = 1; i <= (screen.height-1); i++)	
	  {
	    mvaddch (i, 0, BORDER_CHAR);
	    mvaddch (i, (screen.width-1), BORDER_CHAR);
	  }

}


/* Draws the snake - from the head to the whole body */
void draw_player ()
{
	attron (COLOR_PAIR (GREEN_BLACK));
	mvaddch (tron.body[0].y, tron.body[0].x, PLAYER_HEAD_CHAR);

	int i;
	for (i = 1; i < tron.size; i++)
		mvaddch (tron.body[i].y, tron.body[i].x, PLAYER_CHAR);
}


/* Prints the current score */
void draw_score ()
{
	attron (COLOR_PAIR (WHITE_BLACK));
	mvprintw (0, 0,  "----------------Level (speed): %2d", game.level);
	mvprintw (0, 36, " ------------Score: %4d ------------------", tron.score);
}
void print_in_game(char* print_this) {
	strcpy(msg, print_this);
	print_message = 1;
}

/* Exits and dealocates the memory required by ncurses */
void engine_exit ()
{
	clear ();
	refresh ();
	// Effectively ends ncurses mode
	endwin();
}


/**	Get the user input during game and make the right decisions
 */
void engine_get_game_input (int multi)
{
	// The input variable MUST be int to accept non-ascii characters
	int input = getch ();
	
	switch (input)
	{

	case ERR:
		// If we get no input
		break;
	/*
	Sends an update package with the new direction of the player
	to the multiplayer server, if multiplayer playing is activated
	*/
	case KEY_UP:    
		player_change_direction (UP);
		if (is_multi_choosen == 1) {
			declare_update_package(playerID, UP);
			send_package(portnr, ip_adress, 5);
		}
		break;

	case KEY_LEFT:  
		player_change_direction (LEFT);
		if (is_multi_choosen == 1) {
			declare_update_package(playerID, LEFT);
			send_package(portnr, ip_adress, 5);
		}
		break;

	case KEY_DOWN:  
		player_change_direction (DOWN);
		if (is_multi_choosen == 1) {
			declare_update_package(playerID, DOWN);
			send_package(portnr, ip_adress, 5);
		}
		break;

	case KEY_RIGHT: 
		player_change_direction (RIGHT);
		if (is_multi_choosen == 1) {
			declare_update_package(playerID, RIGHT);
			send_package(portnr, ip_adress, 5);
		}
		break;

	case 'q':	
	case 'Q':
	/*
	Sends an end package to the multiplayer server to indicate that a 
	player has quit the game, if multiplayer is activated
	*/
		if (is_multi_choosen == 1) {
			declare_end_package(playerID);
			send_package(portnr, ip_adress, 6);
		}
		engine_exit ();
		ifitron_exit ();
		break;


	default:
		break;
	}
}

/**	Starts the game engine. Initializes all the stuff related to ncurses.
 *
 *  @note If some engine-specific initialization fails, the game aborts.
 */
void engine_init (char* player, char* port, char* ip)
{
	strcpy(username, player);
	screen.width  = 80;
	screen.height = 24;
	portnr = port;
	ip_adress = ip;

	// Starts the ncurses mode
	initscr ();

	if (has_colors() == FALSE)
		ifitron_abort ("Your terminal does not support colors.\n");

	// Start support for colors ( Name, Foreground, Background )
	start_color ();
	init_pair (GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
	init_pair (CYAN_BLACK,  COLOR_CYAN,  COLOR_BLACK);
	init_pair (WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
	init_pair (RED_BLACK,   COLOR_RED,   COLOR_BLACK);
	init_pair (BLUE_BLACK,  COLOR_BLUE,  COLOR_BLACK);
	init_pair (BLACK_WHITE, COLOR_BLACK, COLOR_WHITE);

	int current_height, current_width;
	// Gets the current width and height of the terminal
	getmaxyx (stdscr, current_height, current_width);

	if ((current_width < screen.width) || (current_height < screen.height))
		ifitron_abort ("Your console screen is smaller than 80x24\n"
		               "Please resize your window and try again\n\n");

	// Character input doesnt require the <enter> key anymore
	raw ();

	// Makes the cursor invisible
	curs_set (0);

	// Support for extra keys (life F1, F2, ... )
	keypad (stdscr, true);

	// Wont print the input received
	noecho ();

	// Wont wait for input - the game will run instantaneously
	nodelay (stdscr, true);

	// Refresh the screen (prints whats in the buffer)
	refresh ();
}


/**	Draws the Game Over screen.
 *
 *	Besides drawing 'Game Over', it highlights where the player died.
 */
void engine_show_game_over ()
{
	attron (COLOR_PAIR (RED_BLACK));
	mvaddch (tron.body[0].y, tron.body[0].x, 'x');

	free(tron.body);
	tron.body = NULL;

	mvprintw (10, 17, "Username: ");
	mvprintw (10, 17+strlen("Username: "), username);
	mvprintw (11, 17, "Score: %4d", tron.score);
	mvprintw (12, 17, "GAME OVER");
	mvprintw (13, 17, "Returning to main menu");
	draw_score ();

	refresh ();
}

/* Displays the main menu and gets the user input from it.
 *
 * This function blocks the game execution and waits for user input,
 * refreshing the main menu screen according to the options selected. */

void engine_show_main_menu ()
{
	int wait = TRUE;

	int speed_option = 1;
	char speed_options[10] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'M'};

	int menu_x_padding = 18;
	int option_x_padding = menu_x_padding + 17;

	clear ();

	while (wait == TRUE)
	{
		// The borders
		attron (COLOR_PAIR (WHITE_BLACK));
		int i;
		for (i = 0; i < screen.width; i++)
		{
			mvaddch (0, i, MENU_BORDER_CHAR);
			mvaddch (screen.height - 1, i, MENU_BORDER_CHAR);
		}
		for (i = 0; i < screen.height; i++)
		{
			mvaddch (i, 0, MENU_BORDER_CHAR);
			mvaddch (i, screen.width - 1, MENU_BORDER_CHAR);
		}

		attron (COLOR_PAIR (GREEN_BLACK));

		mvprintw (1, 12,  "IFI-TRON");

		attron (COLOR_PAIR (GREEN_BLACK));
		int k = 0;
		/*
		Checks if a message is to printed, and does so if it is
		*/
		if (print_message == 1) {
			mvprintw(2, 1, "%s", msg);
		}
		mvprintw(3, 12,   "              HIGHSCORES                ");
		mvprintw(4, 12,   "+----------------------------------------+");
		mvprintw(5, 12, "|Navn \tScore \tGame type\t     |");
		/*
		Iterates through the recieved highscore list, and prints the scores
		*/
		for (k = 0; k < size; k++) {
			points_package pkg;
			memcpy(&pkg, &to_game[(k*sizeof(points_package))], sizeof(points_package));
			mvprintw(k+6, 12, "|%s \t%d \t%d \t             |", pkg.name, pkg.points, pkg.game_type);
		}
		k = k+6;
		mvprintw(k++, 12, "|                                        |");
		mvprintw(k++, 12, "+----------------------------------------+");
		mvprintw (k, menu_x_padding, "Username: ");
		mvprintw (k++, menu_x_padding+strlen("Username: "), username);
		mvprintw (k++, menu_x_padding, "Press <enter> or <space> to start game");
		mvprintw (k++, menu_x_padding, "Press <q> to quit game");
		k++;

		// And here we draw the level numbers
		attron (COLOR_PAIR (BLUE_BLACK));
		mvprintw (k, menu_x_padding, "Starting speed/level:");

		// Tricky, draw the options with the right colors
		int j;
		for (i = 0, j = 0; i < 10; i++)
		{
			if (i == (speed_option-1))
				attron (COLOR_PAIR (WHITE_BLACK));
			else
				attron (COLOR_PAIR (BLUE_BLACK));

			mvprintw (k, option_x_padding+j+6, "%c", speed_options [i]);
			j += 2;
		}

		attron (COLOR_PAIR (WHITE_BLACK));
		mvprintw (screen.height-3, 3, "Use --help for guidelines");

		// Now we wait for orders
		wait = get_main_menu_input(&speed_option);

		// This function is so refreshing...
		refresh ();
	}
	game.level = speed_option;
	/*
	Checks to see if multiplayer if choosen. If so, a start package 
	is sent to the server to declare that a player has startet playing.
	If not, is_multi_choosen is set to 0 so that the game doesn't send
	multiplayer packages (start, update, end, setup-packages)
	*/
	if (game.level == 10) {
		is_multi_choosen = 1;
		if (is_multi_choosen == 1) {
			declare_start_package(playerID);
			send_package(portnr, ip_adress, 4);
		}
	} else {
		is_multi_choosen = 0;
	} 
}

/**	Completely draws the screen during game.
 *
 * 	The usleep() function "stops" the program for 'n' microseconds.
  */
void engine_show_screen ()
{
	draw_background ();
	draw_borders ();
	draw_player ();
	draw_score ();

	usleep (REFRESH_DELAY);

	refresh();
}

/* Gets input for the main menu */
int get_main_menu_input (int* speed_option)
{
	int input = getch();

	switch (input)
	{
	case ERR: // no input
	        break;

	case '\n': case ' ':
		return FALSE;
		break;

	case 'q': case 'Q':
		engine_exit();
		ifitron_exit();
		break;

	case KEY_LEFT:
		if (*speed_option > 1) (*speed_option)--;
		break;

	case KEY_RIGHT:
		if (is_multi == 1) {
			if (*speed_option < 10) (*speed_option)++;
			break;
		} else {
			if (*speed_option < 9) (*speed_option)++;
			break;
		}
		
	//Sets the multiplayer check
	case '1':
		*speed_option = 1;
		is_multi_choosen = 0;
		break;
	case '2':
		*speed_option = 2;
		is_multi_choosen = 0;
		break;
	case '3':
		*speed_option = 3;
		is_multi_choosen = 0;
		break;
	case '4':
		*speed_option = 4;
		is_multi_choosen = 0;
		break;
	case '5':
		*speed_option = 5;
		is_multi_choosen = 0;
		break;
	case '6':
		*speed_option = 6;
		is_multi_choosen = 0;
		break;
	case '7':
		*speed_option = 7;
		is_multi_choosen = 0;
		break;
	case '8':
		*speed_option = 8;
		is_multi_choosen = 0;
		break;
	case '9':
		*speed_option = 9;
		is_multi_choosen = 0;
		break;
	case 'M':
		is_multi_choosen = 1;
		*speed_option = 10;
		break;
	default:
		break;
	}
	return TRUE;
}

/* Aborts the game and displays the error message */
void ifitron_abort (char* error_msg)
{
	engine_exit ();
	printf ("%s", error_msg);
	exit (EXIT_FAILURE);
}

/* Interrupts the game and quits to the terminal. */
void ifitron_exit ()
{
	if (tron.body != NULL)
	{
		free (tron.body);
		tron.body = NULL;
	}
	exit (EXIT_SUCCESS);
}
/*
This function is called from ifitron.c if a game is over.
The game's values needed for a point_package is set
*/
void set_game_info() {
	game.points = tron.score;
	if (player_hit_self()) {
		game.result = 0x02;
	} else if (player_hit_borders()) {
		game.result = 0x01;
	} else {
		game.result = 0x00;
	}
	strcpy(game.name, username);
}

/* Finish the game after haveing lost a life. */
void ifitron_game_over ()
{	
	engine_show_game_over ();
	usleep (4000000); // Wait 4 seconds before returning...
	engine_show_main_menu();
	ifitron_init ();
}

/* Starts all the necessairy stuff */
void ifitron_init ()
{
	player_init ();
	engine_show_screen ();
}