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
    /* Đặt chế độ output mặc định dựa trên việc output có phải là terminal không */
    opts->flag_q = isatty(STDOUT_FILENO);
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
            case 'c': opts->flag_c = 1; break;
            case 'd': opts->flag_d = 1; break;
            case 'F': opts->flag_F = 1; break;
            case 'f': opts->flag_f = 1; break;
            case 'h': 
                opts->flag_h = 1;
                opts->flag_k = 0;  /* -h ghi đè -k */
                break;
            case 'i': opts->flag_i = 1; break;
            case 'k': 
                if (!opts->flag_h) {  /* Chỉ đặt nếu -h chưa được set */
                    opts->flag_k = 1;
                }
                break;
            case 'l': opts->flag_l = 1; break;
            case 'n': 
                opts->flag_n = 1;
                opts->flag_l = 1;  /* -n kéo theo -l */
                break;
            case 'q': 
                opts->flag_q = 1;
                opts->flag_w = 0;  /* -q ghi đè -w */
                break;
            case 'R': opts->flag_R = 1; break;
            case 'r': opts->flag_r = 1; break;
            case 'S': opts->flag_S = 1; break;
            case 's': opts->flag_s = 1; break;
            case 't': opts->flag_t = 1; break;
            case 'u': opts->flag_u = 1; break;
            case 'w': 
                opts->flag_w = 1;
                opts->flag_q = 0;  /* -w ghi đè -q */
                break;
            default:
                print_usage(argv[0]);
                return -1;
        }
    }
    
    /* Xử lý các option ghi đè nhau theo manual */
    if (opts->flag_c && opts->flag_u) {
        /* Option cuối cùng sẽ thắng - đã xử lý trong quá trình parsing */
    }
    
    if (opts->flag_R && opts->flag_d) {
        /* Option cuối cùng sẽ thắng - đã xử lý trong quá trình parsing */
    }
    
    if (opts->flag_f) {
        /* -f kéo theo -a và tắt sắp xếp */
        opts->flag_a = 1;
    }
    
    return optind;  /* Trả về chỉ số của đối số đầu tiên không phải option */
}

/**
 * Hàm so sánh để sắp xếp đường dẫn (thư mục vs không phải thư mục)
 */
int compare_paths(const void *a, const void *b) {
    const char *path_a = *(const char **)a;
    const char *path_b = *(const char **)b;
    
    int is_dir_a = is_directory(path_a);
    int is_dir_b = is_directory(path_b);
    
    /* Không phải thư mục đứng trước thư mục */
    if (!is_dir_a && is_dir_b) return -1;
    if (is_dir_a && !is_dir_b) return 1;
    
    /* Trong cùng loại, sắp xếp theo thứ tự từ điển */
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
    
    /* Đếm số thư mục và file */
    for (i = 0; i < count; i++) {
        if (is_directory(paths[i]) && !opts->flag_d) {
            has_dirs++;
        } else {
            has_files++;
        }
    }
    
    /* Sắp xếp đường dẫn: không phải thư mục trước, sau đó đến thư mục */
    qsort(paths, count, sizeof(char *), compare_paths);
    
    /* Trước tiên, hiển thị tất cả các toán hạng không phải thư mục */
    for (i = 0; i < count; i++) {
        if (!is_directory(paths[i]) || opts->flag_d) {
            if (list_file(paths[i], opts) != 0) {
                status = 1;
            }
        }
    }
    
    /* Sau đó hiển thị các thư mục */
    for (i = 0; i < count; i++) {
        if (is_directory(paths[i]) && !opts->flag_d) {
            /* In tên thư mục nếu có nhiều toán hạng hoặc trộn file và thư mục */
            if (count > 1 || (has_files > 0 && has_dirs > 0)) {
                if (i > 0 || has_files > 0) {
                    printf("\n");
                }
                printf("%s:\n", paths[i]);
            }
            
            if (do_ls(paths[i], opts) != 0) {
                status = 1;
            }
        }
    }
    
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
