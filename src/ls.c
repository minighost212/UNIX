#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include "../include/ls.h"
#include "../include/utils.h"
#include "../include/format.h"

/**
 * Kiểm tra xem một entry có nên bị bỏ qua dựa trên options
 */
int should_skip_entry(const char *name, Options *opts) {
    /* Bỏ qua . và .. nếu -A được đặt (nhưng không phải -a) */
    if (opts->flag_A && !opts->flag_a) {
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
            return 1;
        }
    }
    
    /* Bỏ qua file ẩn (bắt đầu với .) trừ khi có -a hoặc -A hoặc -f */
    if (!opts->flag_a && !opts->flag_A && !opts->flag_f) {
        if (name[0] == '.') {
            return 1;
        }
    }
    
    return 0;
}

/**
 * Đọc thư mục và trả về mảng các con trỏ FileEntry
 */
FileEntry **read_directory(const char *path, Options *opts, int *count) {
    DIR *dir;
    struct dirent *entry;
    FileEntry **entries = NULL;
    int capacity = 32;
    int size = 0;
    
    *count = 0;
    
    /* Mở thư mục */
    dir = opendir(path);
    if (!dir) {
        print_error(path, strerror(errno));
        return NULL;
    }
    
    /* Cấp phát mảng ban đầu */
    entries = malloc(capacity * sizeof(FileEntry *));
    if (!entries) {
        closedir(dir);
        return NULL;
    }
    
    /* Đọc tất cả các entry */
    while ((entry = readdir(dir)) != NULL) {
        struct stat st;
        char *full_path;
        FileEntry *file_entry;
        
        /* Kiểm tra xem có nên bỏ qua entry này không */
        if (should_skip_entry(entry->d_name, opts)) {
            continue;
        }
        
        /* Xây dựng đường dẫn đầy đủ */
        full_path = join_path(path, entry->d_name);
        if (!full_path) {
            continue;
        }
        
        /* Lấy thông tin thống kê file */
        if (lstat(full_path, &st) < 0) {
            print_error(full_path, strerror(errno));
            free(full_path);
            continue;
        }
        
        /* Tạo file entry */
        file_entry = create_file_entry(entry->d_name, full_path, &st);
        free(full_path);
        
        if (!file_entry) {
            continue;
        }
        
        /* Mở rộng mảng nếu cần */
        if (size >= capacity) {
            capacity *= 2;
            FileEntry **new_entries = realloc(entries, capacity * sizeof(FileEntry *));
            if (!new_entries) {
                free_file_entry(file_entry);
                break;
            }
            entries = new_entries;
        }
        
        entries[size++] = file_entry;
    }
    
    closedir(dir);
    *count = size;
    return entries;
}

/**
 * Giải phóng mảng các file entry
 */
void free_entries(FileEntry **entries, int count) {
    int i;
    if (!entries) return;
    
    for (i = 0; i < count; i++) {
        free_file_entry(entries[i]);
    }
    free(entries);
}

/**
 * Hiển thị một file đơn lẻ
 */
int list_file(const char *path, Options *opts) {
    struct stat st;
    FileEntry *entry;
    
    /* Lấy thông tin thống kê file */
    if (lstat(path, &st) < 0) {
        print_error(path, strerror(errno));
        return 1;
    }
    
    /* Tạo file entry */
    entry = create_file_entry(path, path, &st);
    if (!entry) {
        return 1;
    }
    
    /* In dựa trên định dạng */
    if (opts->long_format != MODE_NONE) {
        print_long_format(entry, opts);
    } else {
        print_short_format(entry, opts);
    }
    
    free_file_entry(entry);
    return 0;
}

/**
 * Hiển thị nội dung thư mục
 */
int list_directory(const char *path, Options *opts) {
    FileEntry **entries;
    int count;
    int i;
    blkcnt_t total_blocks = 0;
    
    /* Đọc các entry trong thư mục */
    entries = read_directory(path, opts, &count);
    if (!entries) {
        return 1;
    }
    
    /* Sắp xếp các entry trừ khi có -f */
    if (!opts->flag_f) {
        sort_entries(entries, count, opts);
    }
    
    /* Tính tổng số block cho định dạng dài */
    if ((opts->long_format != MODE_NONE || opts->flag_s) && count > 0) {
        total_blocks = calculate_total_blocks(entries, count);
        
        if (opts->size_mode == SIZE_KB) {
            printf("total %lld\n", (long long)(total_blocks / 2));
        } else {
            printf("total %lld\n", (long long)total_blocks);
        }
    }
    
    /* Hiển thị các entry */
    for (i = 0; i < count; i++) {
        if (opts->long_format != MODE_NONE) {
            print_long_format(entries[i], opts);
        } else {
            print_short_format(entries[i], opts);
        }
    }
    
    /* Xử lý liệt kê đệ quy */
    if (opts->flag_R) {
        for (i = 0; i < count; i++) {
            /* Bỏ qua nếu không phải thư mục */
            if (!S_ISDIR(entries[i]->st.st_mode)) {
                continue;
            }
            
            /* Bỏ qua . và .. */
            if (strcmp(entries[i]->name, ".") == 0 || 
                strcmp(entries[i]->name, "..") == 0) {
                continue;
            }
            
            /* In thư mục con */
            printf("\n%s:\n", entries[i]->path);
            list_directory(entries[i]->path, opts);
        }
    }
    
    free_entries(entries, count);
    return 0;
}

/**
 * Hàm ls chính - xác định liệt kê file hay thư mục
 */
int do_ls(const char *path, Options *opts) {
    struct stat st;
    
    /* Kiểm tra đường dẫn có tồn tại không */
    if (lstat(path, &st) < 0) {
        print_error(path, strerror(errno));
        return 1;
    }
    
    /* Nếu -d được chỉ định hoặc path không phải thư mục, liệt kê như file */
    if (opts->dir_mode == DIR_ENTRY || !S_ISDIR(st.st_mode)) {
        return list_file(path, opts);
    }
    
    /* Ngược lại, liệt kê nội dung thư mục */
    return list_directory(path, opts);
}
