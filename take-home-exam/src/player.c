#include <stdlib.h>
#include <strings.h>

#include "engine.h"
#include "player.h"

struct player_t	tron;

/* Changes the tron direction based on the input received */
void player_change_direction (int new_direction)
{
	if      ((new_direction == UP) && (tron.direction != DOWN))
		tron.direction = UP;

	else if ((new_direction == LEFT) && (tron.direction != RIGHT))
		tron.direction = LEFT;

	else if ((new_direction == DOWN) && (tron.direction != UP))
		tron.direction = DOWN;

	else if ((new_direction == RIGHT) && (tron.direction != LEFT))
		tron.direction = RIGHT;
}

/* Checks collision between the player and the walls */
int player_hit_borders ()
{
	if ((tron.body[0].x < 1) || (tron.body[0].x > (screen.width-2)) ||
	    (tron.body[0].y < 2) || (tron.body[0].y > (screen.height-2)))
	{
		return TRUE;
	}
	return FALSE;
}


/**	Checks if the tron have collided with itself */
int player_hit_self ()
{
	int i;
	for (i = (tron.size-1); i > 1; i--)
	{
		if ((tron.body[0].x == tron.body[i].x) &&
		    (tron.body[0].y == tron.body[i].y))
		{
			return TRUE;
		}
	}
	return FALSE;
}


/**	Simply increases the score by the value sent as the parameter.
 */
void player_increase_score (int add)
{
	tron.score += add;
}


/**	Increases the tron size.
 *
 * 	Here we have the core function of this file. Each time the tron
 * 	increases its size, we expand the array that represents its body. */

void player_increase_size (int size)
{
	int piece_size = sizeof (struct player_pieces_t);

	tron.size += size;

	tron.body = realloc (tron.body, (tron.size * piece_size));
	if (tron.body == NULL)
		ifitron_abort ("Memory Error!\n");
	
	int i;
	for(i = 0; i < size; ++i)
		tron.body[tron.size-size+i].x = tron.body[tron.size-size+i].y = -1;
}


/**	Starts the player-related variables. */
void player_init ()
{
	tron.is_alive  = TRUE;
	tron.speed     = 1;
	tron.score     = 0;
	tron.direction = RIGHT;
	tron.size      = 1;

	tron.body = malloc (tron.size * sizeof (struct player_pieces_t));
	if (tron.body == NULL) ifitron_abort ("Memory Error!\n");

	int i;
	for (i = 0; i < tron.size; i++)
	{
		tron.body[i].x = screen.width/2;
		tron.body[i].y = screen.height/2;
	}
}

/**	Updates the player position, one piece at a time
  */
void player_update ()
{
// body
	int i;
	for (i = (tron.size-1); i > 0; i--)
	{
		tron.body[i].x = tron.body[i-1].x;
		tron.body[i].y = tron.body[i-1].y;
	}

// head
	if      (tron.direction == UP)    tron.body[0].y -= 1;
	else if (tron.direction == LEFT)  tron.body[0].x -= 1;
	else if (tron.direction == DOWN)  tron.body[0].y += 1;
	else if (tron.direction == RIGHT) tron.body[0].x += 1;
}
