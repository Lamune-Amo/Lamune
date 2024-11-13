#ifndef _LAMUNE_SH_
#define _LAMUNE_SH_

/* sh */
void shell (void);

/* built-in */
void builtin_help (int argc, char *argv[]);
void builtin_ps (int argc, char *argv[]);
void builtin_mm (int argc, char *argv[]);
void builtin_time (int argc, char *argv[]);
void builtin_kill (int argc, char *argv[]);
void builtin_clear (int argc, char *argv[]);
void builtin_colorful (int argc, char *argv[]);
void builtin_hexapawn (int argc, char *argv[]);
void builtin_hexapawn_auto (int argc, char *argv[]);
void builtin_malloc (int argc, char *argv[]);
void builtin_free (int argc, char *argv[]);
void builtin_sp (int argc, char *argv[]);

#endif
