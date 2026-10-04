#ifndef LS_H
#define LS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>

/* Maximum path length */
#define MAX_PATH 4096

/* Enums for mutually-exclusive option pairs */
typedef enum { MODE_NONE, MODE_L, MODE_N } LongFormatMode;
typedef enum { TIME_MTIME, TIME_CTIME, TIME_ATIME } TimeMode;
typedef enum { DIR_LIST, DIR_ENTRY } DirectoryMode;
typedef enum { SIZE_BLOCKS, SIZE_KB, SIZE_HUMAN } SizeMode;
typedef enum { PRINT_DEFAULT, PRINT_QUESTION, PRINT_RAW } PrintMode;

/* Command line options structure */
typedef struct {
    int flag_A;      /* List all except . and .. */
    int flag_a;      /* Include entries starting with . */
    int flag_F;      /* Append indicator (/, *, @, etc.) */
    int flag_f;      /* Output is not sorted */
    int flag_i;      /* Print inode number */
    int flag_r;      /* Reverse sort order */
    int flag_S;      /* Sort by size */
    int flag_s;      /* Display block count */
    int flag_t;      /* Sort by modification time */
    int flag_R;      /* Recursively list subdirectories */
    
    /* Mutually-exclusive option pairs (last one wins) */
    LongFormatMode long_format;  /* -l/-n */
    TimeMode time_mode;          /* -c/-u (default: mtime) */
    DirectoryMode dir_mode;      /* -R/-d */
    SizeMode size_mode;          /* -k/-h (default: 512-byte blocks) */
    PrintMode print_mode;        /* -q/-w */
} Options;

/* File entry structure for sorting and display */
typedef struct {
    char *name;           /* File name */
    char *path;           /* Full path */
    struct stat st;       /* File stat information */
    ino_t inode;          /* Inode number */
    off_t size;           /* File size */
    time_t mtime;         /* Modification time */
    time_t atime;         /* Access time */
    time_t ctime;         /* Status change time */
    blkcnt_t blocks;      /* Number of blocks */
} FileEntry;

/* Main ls functions */
int do_ls(const char *path, Options *opts);
int list_directory(const char *path, Options *opts);
int list_file(const char *path, Options *opts);

/* Directory reading and filtering */
FileEntry **read_directory(const char *path, Options *opts, int *count);
int should_skip_entry(const char *name, Options *opts);
void free_entries(FileEntry **entries, int count);

#endif /* LS_H */
