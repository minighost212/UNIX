#ifndef FORMAT_H
#define FORMAT_H

#include "ls.h"
#include <sys/stat.h>

/* Format and print functions */
void print_long_format(FileEntry *entry, Options *opts);
void print_short_format(FileEntry *entry, Options *opts);
void print_file_name(FileEntry *entry, Options *opts);

/* Permission and mode formatting */
void format_permissions(mode_t mode, char *buf);
void format_file_type(mode_t mode, char *type);
char get_file_indicator(mode_t mode);

/* Size formatting */
void format_size_human(off_t size, char *buf, size_t bufsize);
void format_size_blocks(blkcnt_t blocks, SizeMode mode, char *buf, size_t bufsize);

/* Time formatting */
void format_time(time_t time, char *buf, size_t bufsize);

/* User/Group name resolution */
void get_user_name(uid_t uid, char *buf, size_t bufsize);
void get_group_name(gid_t gid, char *buf, size_t bufsize);

/* Total blocks calculation */
blkcnt_t calculate_total_blocks(FileEntry **entries, int count);

#endif /* FORMAT_H */
