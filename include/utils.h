#ifndef UTILS_H
#define UTILS_H

#include "ls.h"

/* Sorting functions */
void sort_entries(FileEntry **entries, int count, Options *opts);
int compare_lexical(const void *a, const void *b);
int compare_time(const void *a, const void *b);
int compare_size(const void *a, const void *b);

/* Path utilities */
char *join_path(const char *dir, const char *name);
int is_directory(const char *path);
int file_exists(const char *path);

/* String utilities */
void print_non_printable(const char *str, int quote_mode);
char *escape_string(const char *str);

/* Error handling */
void print_error(const char *path, const char *message);
void print_usage(const char *program_name);

/* Memory utilities */
FileEntry *create_file_entry(const char *name, const char *path, struct stat *st);
void free_file_entry(FileEntry *entry);

#endif /* UTILS_H */
