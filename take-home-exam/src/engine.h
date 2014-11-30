#ifndef ENGINE_DEFINED
#define ENGINE_DEFINED

#define TRUE  1
#define FALSE 0

/** Global definitions related to the game screen */
struct screen_t
{
	/** The fixed width of the game area */
	int width;
	/** The fixed height of the game area */
	int height;
	/** The time (in microsseconds) that the game */
	int delay;
};

/* Global definitions related to the game in general. */
struct game_t
{
	short level;
	short points;
	short result;
	char name[8];
};

char* portnr;
char* ip_adress;
int is_multi_choosen;


/** The global screen structure */
extern struct screen_t screen;

/** The global game structure */
extern struct game_t game;

void draw_background ();
void draw_borders ();
void draw_player ();
void draw_score ();
void set_game_info();

void engine_exit ();
void engine_init (char* player, char* port, char* ip);
void ifitron_game_over ();
int set_highscore_list(int s);
void engine_show_main_menu();
void engine_show_pause ();
void engine_show_screen ();
void print_in_game(char* error);

int  get_main_menu_input (int* current_option);
void engine_get_game_input (int multi);
void get_pause_input ();

void ifitron_abort (char* error_msg);
void ifitron_exit ();
void ifitron_game_over ();
void ifitron_init ();



#endif
