#ifndef LS_H
#define LS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>

/* Maximum path length */
#define MAX_PATH 4096

/* Command line options structure */
typedef struct {
    int flag_A;      /* List all except . and .. */
    int flag_a;      /* Include entries starting with . */
    int flag_c;      /* Use time of last status change */
    int flag_d;      /* List directories as plain files */
    int flag_F;      /* Append indicator (/, *, @, etc.) */
    int flag_f;      /* Output is not sorted */
    int flag_h;      /* Human readable sizes */
    int flag_i;      /* Print inode number */
    int flag_k;      /* Sizes in kilobytes */
    int flag_l;      /* Long format */
    int flag_n;      /* Numeric UIDs and GIDs */
    int flag_q;      /* Force printing of non-printable as ? */
    int flag_R;      /* Recursively list subdirectories */
    int flag_r;      /* Reverse sort order */
    int flag_S;      /* Sort by size */
    int flag_s;      /* Display block count */
    int flag_t;      /* Sort by modification time */
    int flag_u;      /* Use access time instead of modification */
    int flag_w;      /* Raw printing of non-printable */
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
