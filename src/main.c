#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "../include/ls.h"
#include "../include/utils.h"

/**
 * Khởi tạo cấu trúc options với các giá trị mặc định
 */
void init_options(Options *opts) {
    memset(opts, 0, sizeof(Options));
    /* Đặt các giá trị mặc định cho enums */
    opts->long_format = MODE_NONE;
    opts->time_mode = TIME_MTIME;
    opts->dir_mode = DIR_LIST;
    opts->size_mode = SIZE_BLOCKS;
    opts->flag_R = 0;  /* Không đệ quy theo mặc định */
    /* PRINT_DEFAULT = tự động dựa trên isatty() khi in */
    opts->print_mode = PRINT_DEFAULT;
}

/**
 * Phân tích các tùy chọn dòng lệnh
 * Trả về chỉ số của đối số đầu tiên không phải là option
 */
int parse_options(int argc, char **argv, Options *opts) {
    int opt;
    
    while ((opt = getopt(argc, argv, "AacdFfhiklnqRrSstuw")) != -1) {
        switch (opt) {
            case 'A': opts->flag_A = 1; break;
            case 'a': opts->flag_a = 1; break;
            case 'c': opts->time_mode = TIME_CTIME; break;
            case 'd': 
                opts->dir_mode = DIR_ENTRY;
                opts->flag_R = 0;  /* -d tắt -R */
                break;
            case 'F': opts->flag_F = 1; break;
            case 'f': opts->flag_f = 1; break;
            case 'h': opts->size_mode = SIZE_HUMAN; break;
            case 'i': opts->flag_i = 1; break;
            case 'k': opts->size_mode = SIZE_KB; break;
            case 'l': opts->long_format = MODE_L; break;
            case 'n': opts->long_format = MODE_N; break;
            case 'q': opts->print_mode = PRINT_QUESTION; break;
            case 'R': 
                opts->flag_R = 1;
                opts->dir_mode = DIR_LIST;  /* -R tắt -d */
                break;
            case 'r': opts->flag_r = 1; break;
            case 'S': opts->flag_S = 1; break;
            case 's': opts->flag_s = 1; break;
            case 't': opts->flag_t = 1; break;
            case 'u': opts->time_mode = TIME_ATIME; break;
            case 'w': opts->print_mode = PRINT_RAW; break;
            default:
                print_usage(argv[0]);
                return -1;
        }
    }
    
    /* -f kéo theo -a và tắt sắp xếp */
    if (opts->flag_f) {
        opts->flag_a = 1;
    }
    
    return optind;  /* Trả về chỉ số của đối số đầu tiên không phải option */
}

/**
 * Hàm so sánh để sắp xếp đường dẫn (thư mục vs không phải thư mục)
 * Chỉ phân nhóm khi KHÔNG có -d
 */
int compare_paths(const void *a, const void *b) {
    const char *path_a = *(const char **)a;
    const char *path_b = *(const char **)b;
    
    /* Luôn sort lexicographically */
    return strcmp(path_a, path_b);
}

/**
 * Xử lý nhiều đối số đường dẫn
 */
int process_paths(char **paths, int count, Options *opts) {
    int status = 0;
    int i;
    int has_dirs = 0;
    int has_files = 0;
    
    /* Với -d, tất cả operands được coi là files, sort chung */
    if (opts->dir_mode == DIR_ENTRY) {
        /* Sort tất cả theo lexicographical order */
        qsort(paths, count, sizeof(char *), compare_paths);
        
        /* Hiển thị tất cả như files */
        for (i = 0; i < count; i++) {
            if (list_file(paths[i], opts) != 0) {
                status = 1;
            }
        }
        return status;
    }
    
    /* Không có -d: phân biệt files và directories */
    /* Đếm và phân loại */
    char **files = malloc(count * sizeof(char *));
    char **dirs = malloc(count * sizeof(char *));
    int file_count = 0;
    int dir_count = 0;
    
    if (!files || !dirs) {
        free(files);
        free(dirs);
        return 1;
    }
    
    for (i = 0; i < count; i++) {
        if (is_directory(paths[i])) {
            dirs[dir_count++] = paths[i];
            has_dirs++;
        } else {
            files[file_count++] = paths[i];
            has_files++;
        }
    }
    
    /* Sort files và dirs riêng theo lexicographical order */
    if (file_count > 1) {
        qsort(files, file_count, sizeof(char *), compare_paths);
    }
    if (dir_count > 1) {
        qsort(dirs, dir_count, sizeof(char *), compare_paths);
    }
    
    /* Hiển thị files trước */
    for (i = 0; i < file_count; i++) {
        if (list_file(files[i], opts) != 0) {
            status = 1;
        }
    }
    
    /* Sau đó hiển thị directories */
    for (i = 0; i < dir_count; i++) {
        /* In tên thư mục nếu có nhiều toán hạng hoặc trộn file và thư mục */
        if (count > 1 || (has_files > 0 && has_dirs > 0)) {
            if (i > 0 || has_files > 0) {
                printf("\n");
            }
            printf("%s:\n", dirs[i]);
        }
        
        if (do_ls(dirs[i], opts) != 0) {
            status = 1;
        }
    }
    
    free(files);
    free(dirs);
    return status;
}

/**
 * Hàm main
 */
int main(int argc, char **argv) {
    Options opts;
    int first_arg_index;
    int status = 0;
    
    /* Khởi tạo options với giá trị mặc định */
    init_options(&opts);
    
    /* Phân tích các tùy chọn dòng lệnh */
    first_arg_index = parse_options(argc, argv, &opts);
    if (first_arg_index < 0) {
        return 1;  /* Lỗi trong phân tích option */
    }
    
    /* Nếu không có đối số nào, liệt kê thư mục hiện tại */
    if (first_arg_index >= argc) {
        status = do_ls(".", &opts);
    } else {
        /* Xử lý các đường dẫn được cung cấp */
        status = process_paths(&argv[first_arg_index], 
                              argc - first_arg_index, 
                              &opts);
    }
    
    return status;
}
