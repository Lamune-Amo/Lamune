#include "arch/VGA.h"
#include "lamune/soft.h"
#include "lamune/unistd.h"
#include "lamune/string.h"
#include "lamune/printk.h"

/* whether to use pruning */
#define USE_PRUNING

#define PLY_DEPTH 5

/* board size */
#define BOARD_ROW 5
#define BOARD_COLUMN 5
#define BOARD_DAFAULT (char *[]) { \
	"WWWWW", \
	".....", \
	".....", \
	".....", \
	"BBBBB"  \
}

/* pieces tpye */
#define PAWN_WHITE 0
#define PAWN_BLACK 1

/* current turn */
#define TURN_PLAYER 0
#define TURN_AI 1

/* game */
#define WIN_NONE 1
#define WIN_DRAW 0
#define WIN_WHITE -1
#define WIN_BLACK -2

/* conv */
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) > (b) ? (b) : (a))

#define COMPARE_POSITION(pos, a, b) (pos.row == a && pos.column == b)
#define SET_POSITION(pos, a, b) \
	do \
	{ \
		pos.row = a; \
		pos.column = b; \
	} \
	while (0)
#define COPY_POSITION(pos, new) \
	do \
	{ \
		pos.row = new.row; \
		pos.column = new.column; \
	} \
	while (0)

/*
 * struct
 */
typedef struct coordinate coordinate_t;
struct coordinate
{
    int row;
    int column;
};

/*
 * prototype
 */
int hexapawn_alpha (int depth, int alpha, int beta);
int hexapawn_beta (int depth, int alpha, int beta);

/*
 * global
 */
/* current state */
char state[BOARD_ROW + 2][BOARD_COLUMN + 2];
/* AI-saved state */
char next_state[BOARD_ROW + 2][BOARD_COLUMN + 2];
/* pawn color */
int player_pawn;
/* player */
int turn;

/* trace */
coordinate_t origin_position;
coordinate_t moved_position;

coordinate_t selected_pawn = { -1, -1 };

/* debug */
/* function call count */
static int debug_alpha = 0;
static int debug_beta = 0;

#define DEBUG_CALL_INIT() (debug_alpha = debug_beta = 0)
#define DEBUG_ALPHA_CALL() (debug_alpha++);
#define DEBUG_BETA_CALL() (debug_beta++);

/*
 * AI-algorithm
 */

/*
 * winner () - determine if the game is over
 *   returns: WIN_NONE, WIN_DRAW, WIN_WHITE or WIN_BLACK
 *
 */
int winner (void)
{
	int white_win, black_win;
	int white, black;
	int i, j;

	/* if opponents piece has reached the end */
	white_win = 0;
	black_win = 0;
	for (i = 1; i < BOARD_COLUMN + 1; i++)
	{
		if (state[1][i] == 'B')
		{
			black_win = 1;
		}
	}
	for (i = 1; i < BOARD_COLUMN + 1; i++)
	{
		if (state[BOARD_ROW][i] == 'W')
		{
			white_win = 1;
		}
	}
	/* win condition */
	if (white_win && black_win)
		/* this condition can be only satisfied in the initial state */
		return WIN_DRAW;
	else if (white_win)
		return WIN_WHITE;
	else if (black_win)
		return WIN_BLACK;

	/* piece count */
	white = 0;
	black = 0;
	for (i = 1; i < BOARD_ROW + 1; i++)
	{
		for (j = 1; j < BOARD_COLUMN + 1; j++)
		{
			if (state[i][j] == 'W')
				white++;
			if (state[i][j] == 'B')
				black++;
		}
	}
	/* if there is no pieces */
	/* this condition can be satisfied only in initial state */
	if (white == 0 && black == 0)
		return WIN_DRAW;
	/* black win */
	if (white == 0)
		return WIN_BLACK;
	/* white win */
	if (black == 0)
		return WIN_WHITE;

	return WIN_NONE;
}

/*
 * evaluate_state () - evaluate the state
 *   returns: return value during the game is -27 to 27, and -100 or 100
 *			  when the game is over
 */
int evaluate_state (void)
{
	int w, b;
    int value;
    int i, j;
    
	w = 0;
	b = 0;
    value = 0;
    for (i = 1; i < BOARD_ROW + 1; i++)
    {
        for (j = 1; j < BOARD_COLUMN + 1; j++)
        {
            /* white */
            if (state[i][j] == 'W')
            {
				w++;
                /* win */
                if (i == BOARD_ROW)
                    return 100;
                /* point */
                else
                    value += i * i;
            }
            /* black */
            else if (state[i][j] == 'B')
            {
				b++;
                /* win */
                if (i == 1)
                    return -100;
                /* point */
                else
                    value -= (4 - i) * (4 - i);
            }
        }
    }
	/* exceptional condition */
	if (w == 0 && b == 0)
		return 0;
	else if (w == 0)
		return -100;
	else if (b == 0)
		return 100;

    return value;
}

/* alpha beta pruning */
/* search the posibilities in order to move pawn or remove the oppenets pawn */
/* this is because there is no evaluation logic that considers 'depth' */

/*
 * hexapawn_alpha () - alpha tree of minimax algorithm
 *   returns: maximum evaluation value
 *   depth (in): ply-depth
 *   alpha (in): alpha of alpha-beta pruning
 *   beta (in): beta of alpha-beta pruning
 *
 * NOTE: this function will envelop the logic ordering by decreasing evaluate value
 */
int hexapawn_alpha (int depth, int alpha, int beta)
{
    coordinate_t move[4] = { { 1, 0 }, { 0, 1 }, { 0, -1 }, { -1, 0 } };
    coordinate_t remove[2] = { { 1, -1 }, { 1, 1 } };
    int evaluation_beta;
    int evaluation = -10000;
    int value;
    int row, column;
    int i, j;
    int p;

	DEBUG_ALPHA_CALL ();

    value = evaluate_state ();
    /* the evaluation value is returned below situation */
    /* 1. depth is zero */
    /* 2. game is end in this state */
    if (!depth || value == -100 || value == 100)
        return value;

    /* search all possibilities for W to move */
    for (i = 1; i < BOARD_ROW + 1; i++)
    {
        for (j = 1; j < BOARD_COLUMN + 1; j++)
        {
            if (state[i][j] == 'W')
            {
				/* posibilities of movement */
                for (p = 0; p < 4; p++)
                {
                    row = i + move[p].row;
                    column = j + move[p].column;
                    
					/* can move to those coordinates */
                    if (state[row][column] == '.')
                    {
                        state[i][j] = '.';
                        state[row][column] = 'W';

                        evaluation_beta = hexapawn_beta (depth - 1, alpha, beta);
                        if (evaluation_beta > evaluation)
                        {
                            /* save current state as next state */
							if (depth == PLY_DEPTH)
							{
								SET_POSITION (origin_position, i, j);
								SET_POSITION (moved_position, row, column);
								memcpy (next_state, state, (BOARD_ROW + 2) * (BOARD_COLUMN + 2));
							}
                            evaluation = evaluation_beta;
                        }

                        state[i][j] = 'W';
                        state[row][column] = '.';

#ifdef USE_PRUNING
						/* pruning */
						if (evaluation >= beta)
							return evaluation;
						alpha = max (alpha, evaluation);
#endif
                    }
                } 
            }
        }
    }

    /* search all possibilities for W to catch the enemy pawn */
    /* this action make evaluation value better than just moving the pawn */
    for (i = 1; i < BOARD_ROW + 1; i++)
    {
        for (j = 1; j < BOARD_COLUMN + 1; j++)
        {
            if (state[i][j] == 'W')
            {
				/* posibilities of removal */
                for (p = 0; p < 2; p++)
                {
                    row = i + remove[p].row;
                    column = j + remove[p].column;

					/* if there is opponents pawn */
                    if (state[row][column] == 'B')
                    {
                        state[i][j] = '.';
                        state[row][column] = 'W';

                        evaluation_beta = hexapawn_beta (depth - 1, alpha, beta);
                        if (evaluation_beta > evaluation)
                        {
                            /* save current state as next state */
							if (depth == PLY_DEPTH)
							{
								SET_POSITION (origin_position, i, j);
								SET_POSITION (moved_position, row, column);
								memcpy (next_state, state, (BOARD_ROW + 2) * (BOARD_COLUMN + 2));
							}
                            evaluation = evaluation_beta;
                        }

                        state[i][j] = 'W';
                        state[row][column] = 'B';
#ifdef USE_PRUNING
						/* pruning */
						if (evaluation >= beta)
							return evaluation;
						alpha = max (alpha, evaluation);
#endif
                    }
                }
            }
        }
    }

    return evaluation;
}

/*
 * hexapawn_beta () - beta tree of minimax algorithm
 *   returns: minimum evaluation value
 *   depth (in): ply-depth
 *   alpha (in): alpha of alpha-beta pruning
 *   beta (in): beta of alpha-beta pruning
 *
 * NOTE: this function will envelop the logic ordering by decreasing evaluate value
 */
int hexapawn_beta (int depth, int alpha, int beta)
{
    coordinate_t move[4] = { { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 0 } };
    coordinate_t remove[2] = { { -1, -1 }, { -1, 1 } };
    int evaluation_alpha;
    int evaluation = 10000;
    int value;
    int row, column;
    int i, j;
    int p;

	DEBUG_BETA_CALL ();

    value = evaluate_state ();
    /* the evaluation value is returned below situation */
    /* 1. depth is zero */
    /* 2. game is end in this state */
    if (!depth || value == -100 || value == 100)
        return value;

	/* search all possibilities for W to move */
    for (i = 1; i < BOARD_ROW + 1; i++)
    {
        for (j = 1; j < BOARD_COLUMN + 1; j++)
        {
            if (state[i][j] == 'B')
            {
                for (p = 0; p < 4; p++)
                {
                    row = i + move[p].row;
                    column = j + move[p].column;
                    
                    if (state[row][column] == '.')
                    {
                        state[i][j] = '.';
                        state[row][column] = 'B';

                        evaluation_alpha = hexapawn_alpha (depth - 1, alpha, beta);
                        if (evaluation_alpha < evaluation)
                        {
                            /* save current state as next state */
							if (depth == PLY_DEPTH)
							{
								SET_POSITION (origin_position, i, j);
								SET_POSITION (moved_position, row, column);
								memcpy (next_state, state, (BOARD_ROW + 2) * (BOARD_COLUMN + 2));
							}
                            evaluation = evaluation_alpha;
                        }

                        state[i][j] = 'B';
                        state[row][column] = '.';

#ifdef USE_PRUNING
						/* pruning */
						if (evaluation <= alpha)
							return evaluation;
						beta = min (beta, evaluation);
#endif
                    }
                } 
            }
        }
    }

    /* search all possibilities for W to catch the enemy pawn */
    /* this action make evaluation value better than just moving the pawn */
    for (i = 1; i < BOARD_ROW + 1; i++)
    {
        for (j = 1; j < BOARD_COLUMN + 1; j++)
        {
            if (state[i][j] == 'B')
            {
                for (p = 0; p < 2; p++)
                {
                    row = i + remove[p].row;
                    column = j + remove[p].column;

                    if (state[row][column] == 'W')
                    {
                        state[i][j] = '.';
                        state[row][column] = 'B';

                        evaluation_alpha = hexapawn_alpha (depth - 1, alpha, beta);
                        if (evaluation_alpha < evaluation)
                        {
                            /* save current state as next state */
							if (depth == PLY_DEPTH)
							{
								SET_POSITION (origin_position, i, j);
								SET_POSITION (moved_position, row, column);
								memcpy (next_state, state, (BOARD_ROW + 2) * (BOARD_COLUMN + 2));
							}
                            evaluation = evaluation_alpha;
                        }

                        state[i][j] = 'B';
                        state[row][column] = 'W';

#ifdef USE_PRUNING
						/* pruning */
						if (evaluation <= alpha)
							return evaluation;
						beta = min (beta, evaluation);
#endif
                    }
                }
            }
        }
    }
 
    return evaluation;
}

/*
 * interaction
 */

/*
 * coordinate_validation () - verify that the new coordinates is valid
 *   returns: 1 (valid) or 0 (invalid) 
 *   pawn (in): original coordinates
 *   new (in): target coordinates
 */
int coordinate_validation (coordinate_t *pawn, coordinate_t *new)
{
	/* removal offset */
	coordinate_t white_remove[2] = { { 1, -1 }, { 1, 1 } };
	coordinate_t black_remove[2] = { { -1, -1 }, { -1, 1 } };
	coordinate_t /* depending on a pawn color */ (*remove)[2];
	/* movement offset */
    coordinate_t move[4] = { { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 0 } };
	int row, column;
	int p;

	/* can move to those coordinates ? */
	for (p = 0; p < 4; p++)
	{
		row = pawn->row + move[p].row;
		column = pawn->column + move[p].column;
		
		if (row == new->row && column == new->column && state[row][column] == '.')
			return 1;
	}
	/* does the opponents pawn exist at the new coordinates */
	remove = (player_pawn == PAWN_WHITE ? &white_remove : &black_remove);
	for (p = 0; p < 2; p++)
	{
		row = pawn->row + (*remove)[p].row;
		column = pawn->column + (*remove)[p].column;
		
		if (row == new->row && column == new->column &&
				state[row][column] == (player_pawn == PAWN_WHITE ? 'B' : 'W'))
			return 1;
	}

	/* invalid coordinates */
	return 0;
}

/*
 * print_state () - print the board
 */
void print_state (void)
{
	char *color;
    int i, j;

    for (i = 1; i < BOARD_ROW + 1; i++)
    {
        printk ("  ");
        for (j = 1; j < BOARD_COLUMN + 1; j++)
        {
			vga_set_color (WHITE, BLACK);
			/* position in use */
			if (COMPARE_POSITION (selected_pawn, i, j))
				vga_set_color (RED, BLACK);
			/* previous moved positions */
			if (COMPARE_POSITION (moved_position, i, j) || COMPARE_POSITION (origin_position, i, j))
				vga_set_color (WHITE, LIGHT_BLUE);
			printk ("%c", state[i][j] ? state[i][j] : '*');
			vga_set_color (WHITE, BLACK);
        }
        printk ("\n");
    }
}

/*
 * input_setting () - enter game setting
 */
void input_setting (void)
{
    char answer[128] = { 0, };
	unsigned int size;

    /* input a color which player uses */
    do
    {
        printk ("\n");
        if (answer[0])
            printk ("wrong input. ");
        printk ("select your color. (white or black)\n$> ");
		size = read (0, answer, 128);
		answer[size - 1] = 0;
    }
    while (strcmp (answer, "white") && strcmp (answer, "black"));
    player_pawn = strcmp (answer, "white") ? PAWN_BLACK : PAWN_WHITE;

    /* input first */
    answer[0] = 0;
    do
    {
        printk ("\n");
        if (answer[0])
            printk ("wrong input. ");
        printk ("who plays first? (player or AI)\n(%s) $> ", player_pawn == PAWN_WHITE ? "white" : "black");
		size = read (0, answer, 128);
		answer[size - 1] = 0;
    }
    while (strcmp (answer, "player") && strcmp (answer, "AI"));
    turn = strcmp (answer, "player") ? TURN_AI : TURN_PLAYER;
}

/*
 * print_evalbar () - draw the color bar
 *   grid (in): white bar size and black bar size
 */
void print_evalbar (int *grid)
{
	int i;

	vga_set_color (WHITE, BROWN);
	for (i = 0; i < grid[0]; i++)
		printk ("%c", i == 0 ? 'W' : ' ');
	vga_set_color (BROWN, WHITE);
	for (i = 0; i < grid[1]; i++)
		printk ("%c", i == grid[1] - 1 ? 'B' : ' ');
	printk ("\n");
	vga_set_color (WHITE, BLACK);
}

/*
 * print_header () - print the header
 *   bturn (in): whether to print the turn message
 */
void print_header (int bturn)
{
	/* 0: white, 1: black */
	int grid[2];
	int eval;

	/* -27 ~ 27 */
	/* -100, 100 */
	eval = evaluate_state ();

	/* draw eval */
	if (eval == 100)
	{
		grid[0] = 28 * 2;
		grid[1] = 0;
	}
	else if (eval == -100)
	{
		grid[0] = 0;
		grid[1] = 28 * 2;
	}
	else
	{
		grid[0] = eval + 28;
		grid[1] = 28 - eval;
	}
	printk ("EVALUATION\n");
	print_evalbar (grid);

	if (bturn)
		printk ("%s TURN\n\n", turn == TURN_PLAYER ? "PLAYER" : "AI");
}

int cordindate_parser (int *row, int *column)
{
	char *token[10];
    char answer[128];
	char *str;
	unsigned int size;
	int index;

	size = read (0, answer, 128);
	answer[size - 1] = 0;

	str = answer;
	
	index = 0;
	while (*str)
	{
		while (*str && (*str == ' ' || *str == '\t'))
			str++;
		token[index++] = str;
		while (*str && !(*str == ' ' || *str == '\t'))
			str++;
		if (*str == '\0')
			break;
		*(str++) = '\0';
	}
	if (index != 2)
		return 0;

	*row = atoi (token[0]);
	*column = atoi (token[1]);
	return 1;
}

/*
 * TURN handling
 */

/*
 * handle_player () - handle the user turn 
 */
void handle_player (void)
{
	coordinate_t pawn;
	coordinate_t new;
	int wrong;

	/* pawn selection */
	wrong = 0;
	do
    {
        printk ("\n");
        if (wrong)
            printk ("wrong input. ");
        printk ("select the pawn you want to move. (row column)\n(%s) $> ",
				player_pawn == PAWN_WHITE ? "white" : "black");
		if (!cordindate_parser (&pawn.row, &pawn.column))
			pawn.row = -1;

		wrong = 1;
    }
    while (!((pawn.row >= 1 && pawn.row <= BOARD_ROW) &&
			(pawn.column >= 1 && pawn.column <= BOARD_COLUMN) &&
			state[pawn.row][pawn.column] == (player_pawn == PAWN_WHITE ? 'W' : 'B')));
	
	/* mark the selected pawn */
	COPY_POSITION (selected_pawn, pawn);
	
	vga_clear ();
	print_header (1);
	print_state ();

	/* input coordinates to move */
	wrong = 0;
	do
    {
        printk ("\n");
        if (wrong)
            printk ("wrong input. ");
        printk ("enter the coordinates where you want to move the pawn. (row column)\n(%s) $> ",
				player_pawn == PAWN_WHITE ? "white" : "black");
		if (!cordindate_parser (&new.row, &new.column))
			new.row = -1;
		wrong = 1;
    }
    while (!((new.row >= 1 && new.row <= BOARD_ROW) &&
			(new.column >= 1 && new.column <= BOARD_COLUMN) &&
			coordinate_validation (&pawn, &new)));

	/* clear */
	SET_POSITION (selected_pawn, -1, -1);
	/* mark the movement */
	COPY_POSITION (origin_position, pawn);
	COPY_POSITION (moved_position, new);

	/* move */
	printk ("(%s) $> move (%d, %d) -> (%d, %d)\n", player_pawn == PAWN_WHITE ? "white" : "black",
			pawn.row, pawn.column, new.row, new.column);

	/* actual movement */
	state[pawn.row][pawn.column] = '.';
	state[new.row][new.column] = (player_pawn == PAWN_WHITE ? 'W' : 'B');
}

/*
 * handle_AI () - handle the ai turn 
 */
void handle_AI (int type)
{
	int evaluation;

	DEBUG_CALL_INIT ();

	/* AI */
	/* search the next move */
	if (player_pawn == PAWN_WHITE)
		evaluation = hexapawn_beta (PLY_DEPTH, -10000, 10000);
	else
		evaluation = hexapawn_alpha (PLY_DEPTH, -10000, 10000);

	if (type)
	{
		vga_clear ();
		print_header (1);
		print_state ();
		printk ("\n");
	}

	printk ("(%s) $> move (%d, %d) -> (%d, %d)\n\n", player_pawn == PAWN_WHITE ? "black" : "white",
			origin_position.row, origin_position.column, moved_position.row, moved_position.column);
	printk ("alpha: %d beta: %d total: %d\n",
			debug_alpha, debug_beta, debug_alpha + debug_beta);

	/* reflect search results */
	memcpy (state, next_state, (BOARD_ROW + 2) * (BOARD_COLUMN + 2));
}

/*
 * entry
 */
void hexapawn (void)
{
	int win;
	int i;

	/* initial state */
    memset (state, 0, BOARD_ROW * BOARD_COLUMN);
	for (i = 0; i < BOARD_ROW; i++)
		strcpy (&state[i + 1][1], BOARD_DAFAULT[i]);
	/* clear */
	SET_POSITION (selected_pawn, -1, -1);
	/* mark the movement */
	SET_POSITION (origin_position, -1, -1);
	SET_POSITION (moved_position, -1, -1);

	vga_clear ();
	print_header (0);
	printk ("\n");
    /* display the initial state */
    print_state ();
    /* get a information about player */
    input_setting ();

	vga_clear ();
    /* in game */
    while ((win = winner ()) == WIN_NONE)
    {
		print_header (1);
		print_state ();

		/* player */
		if (turn == TURN_PLAYER)
		{
			handle_player ();
			turn = TURN_AI;

			vga_clear ();
		}
		/* AI */
		else
		{
			printk ("\n");
			handle_AI (0);
			turn = TURN_PLAYER;
			printk ("\n\n");
		}
    }
	
	/* game is over */
	print_header (0);
	if (win == WIN_DRAW)
		printk ("DRAW! ");
	else
		printk ("%s (%s) WIN!\n\n",
				(win == WIN_WHITE ?
				 (player_pawn == PAWN_WHITE ? "player" : "AI") :
				 (player_pawn == PAWN_WHITE ? "AI" : "player")),
				win == WIN_WHITE ? "white" : "black");
	print_state ();
	printk ("\n");
}

void hexapawn_auto (void)
{
	int win;
	int i;

	/* initial state */
    memset (state, 0, BOARD_ROW * BOARD_COLUMN);
	for (i = 0; i < BOARD_ROW; i++)
		strcpy (&state[i + 1][1], BOARD_DAFAULT[i]);
	/* clear */
	SET_POSITION (selected_pawn, -1, -1);
	/* mark the movement */
	SET_POSITION (origin_position, -1, -1);
	SET_POSITION (moved_position, -1, -1);

	turn = TURN_AI;
	vga_clear ();

    /* in game */
    while ((win = winner ()) == WIN_NONE)
    {
		print_header (1);
		print_state ();

		/* player */
		if (turn == TURN_PLAYER)
		{
			player_pawn = PAWN_WHITE;

			handle_AI (1);
			turn = TURN_AI;
			printk ("\n\n");
		}
		/* AI */
		else
		{
			player_pawn = PAWN_BLACK;

			handle_AI (1);
			turn = TURN_PLAYER;
			printk ("\n\n");
		}

		sleep (3);
		vga_clear ();
    }
	
	/* game is over */
	print_header (0);
	if (win == WIN_DRAW)
		printk ("DRAW! ");
	else
		printk ("%s (%s) WIN!\n\n",
				(win == WIN_WHITE ?
				 (player_pawn == PAWN_WHITE ? "player" : "AI") :
				 (player_pawn == PAWN_WHITE ? "AI" : "player")),
				win == WIN_WHITE ? "white" : "black");
	print_state ();
	printk ("\n");
}
