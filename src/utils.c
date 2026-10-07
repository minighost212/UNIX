#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include "../include/ls.h"
#include "../include/utils.h"

/**
 * So sánh các entry theo thứ tự từ điển (lexicographically) dựa trên tên
 */
int compare_lexical(const void *a, const void *b) {
    FileEntry *entry_a = *(FileEntry **)a;
    FileEntry *entry_b = *(FileEntry **)b;
    return strcmp(entry_a->name, entry_b->name);
}

/**
 * So sánh các entry theo thời gian sửa đổi (mới nhất trước)
 * Sử dụng thời gian được chỉ định bởi time_mode
 */
TimeMode global_time_mode = TIME_MTIME;

int compare_time(const void *a, const void *b) {
    FileEntry *entry_a = *(FileEntry **)a;
    FileEntry *entry_b = *(FileEntry **)b;
    time_t time_a, time_b;
    
    /* Chọn trường thời gian dựa trên time_mode */
    if (global_time_mode == TIME_CTIME) {
        time_a = entry_a->ctime;
        time_b = entry_b->ctime;
    } else if (global_time_mode == TIME_ATIME) {
        time_a = entry_a->atime;
        time_b = entry_b->atime;
    } else {
        time_a = entry_a->mtime;
        time_b = entry_b->mtime;
    }
    
    /* So sánh thời gian - file mới hơn đứng trước */
    if (time_a > time_b) return -1;
    if (time_a < time_b) return 1;
    
    /* Nếu thời gian bằng nhau, so sánh theo tên */
    return strcmp(entry_a->name, entry_b->name);
}

/**
 * So sánh các entry theo kích thước (lớn nhất trước)
 */
int compare_size(const void *a, const void *b) {
    FileEntry *entry_a = *(FileEntry **)a;
    FileEntry *entry_b = *(FileEntry **)b;
    
    /* So sánh kích thước - file lớn hơn đứng trước */
    if (entry_a->size > entry_b->size) return -1;
    if (entry_a->size < entry_b->size) return 1;
    
    /* Nếu kích thước bằng nhau, so sánh theo tên */
    return strcmp(entry_a->name, entry_b->name);
}

/**
 * Wrapper cho so sánh đảo ngược
 */
static int (*base_compare)(const void *, const void *);
static int reverse_compare(const void *a, const void *b) {
    return -base_compare(a, b);
}

/**
 * Sắp xếp các entry dựa trên options
 */
void sort_entries(FileEntry **entries, int count, Options *opts) {
    int (*compare_func)(const void *, const void *);
    
    if (count <= 1) return;
    
    /* Chọn hàm so sánh dựa trên options */
    if (opts->flag_t) {
        /* Sắp xếp theo thời gian - cập nhật time_mode toàn cục */
        global_time_mode = opts->time_mode;
        compare_func = compare_time;
    } else if (opts->flag_S) {
        compare_func = compare_size;
    } else {
        compare_func = compare_lexical;
    }
    
    /* Áp dụng sắp xếp đảo ngược nếu -r được chỉ định */
    if (opts->flag_r) {
        base_compare = compare_func;
        qsort(entries, count, sizeof(FileEntry *), reverse_compare);
    } else {
        qsort(entries, count, sizeof(FileEntry *), compare_func);
    }
}

/**
 * Nối đường dẫn thư mục và tên file
 */
char *join_path(const char *dir, const char *name) {
    size_t dir_len = strlen(dir);
    size_t name_len = strlen(name);
    char *path;
    
    /* Cấp phát không gian cho dir + '/' + name + '\0' */
    path = malloc(dir_len + name_len + 2);
    if (!path) {
        return NULL;
    }
    
    strcpy(path, dir);
    
    /* Thêm dấu phân cách nếu cần */
    if (dir_len > 0 && dir[dir_len - 1] != '/') {
        strcat(path, "/");
    }
    
    strcat(path, name);
    return path;
}

/**
 * Kiểm tra xem đường dẫn có phải là thư mục không
 * Sử dụng stat() để follow symlink (hành vi mặc định của ls)
 */
int is_directory(const char *path) {
    struct stat st;
    
    /* stat() follow symlink, phù hợp với hành vi mặc định */
    if (stat(path, &st) < 0) {
        return 0;
    }
    
    return S_ISDIR(st.st_mode);
}

/**
 * Kiểm tra xem file có tồn tại không
 */
int file_exists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0);
}

/**
 * In các ký tự không in được dựa trên chế độ
 */
void print_non_printable(const char *str, int quote_mode) {
    if (quote_mode) {
        /* Thay thế ký tự không in được bằng '?' */
        while (*str) {
            if (*str >= 32 && *str <= 126) {
                putchar(*str);
            } else {
                putchar('?');
            }
            str++;
        }
    } else {
        /* In thô */
        printf("%s", str);
    }
}

/**
 * Escape các ký tự đặc biệt trong chuỗi (để sử dụng sau này)
 */
char *escape_string(const char *str) {
    size_t len = strlen(str);
    char *escaped = malloc(len * 2 + 1);  /* Trường hợp xấu nhất: mỗi ký tự được escape */
    size_t i, j;
    
    if (!escaped) return NULL;
    
    for (i = 0, j = 0; i < len; i++) {
        if (str[i] < 32 || str[i] > 126) {
            escaped[j++] = '?';
        } else {
            escaped[j++] = str[i];
        }
    }
    escaped[j] = '\0';
    
    return escaped;
}

/**
 * In thông báo lỗi với đường dẫn
 */
void print_error(const char *path, const char *message) {
    fprintf(stderr, "ls: %s: %s\n", path, message);
}

/**
 * In thông tin cách sử dụng
 */
void print_usage(const char *program_name) {
    fprintf(stderr, "Cách dùng: %s [TÙY_CHỌN]... [FILE]...\n", program_name);
    fprintf(stderr, "Liệt kê thông tin về các FILE (thư mục hiện tại theo mặc định).\n");
    fprintf(stderr, "\nTùy chọn:\n");
    fprintf(stderr, "  -A        liệt kê tất cả trừ . và ..\n");
    fprintf(stderr, "  -a        bao gồm các entry bắt đầu với .\n");
    fprintf(stderr, "  -c        sử dụng thời gian thay đổi trạng thái cuối cùng\n");
    fprintf(stderr, "  -d        liệt kê chính thư mục, không phải nội dung của chúng\n");
    fprintf(stderr, "  -F        thêm ký hiệu (một trong */=>@|) vào các entry\n");
    fprintf(stderr, "  -f        không sắp xếp, kích hoạt -a\n");
    fprintf(stderr, "  -h        in kích thước ở định dạng dễ đọc\n");
    fprintf(stderr, "  -i        in số inode của mỗi file\n");
    fprintf(stderr, "  -k        in kích thước tính bằng kilobyte\n");
    fprintf(stderr, "  -l        sử dụng định dạng liệt kê dài\n");
    fprintf(stderr, "  -n        giống -l, nhưng liệt kê UID và GID dạng số\n");
    fprintf(stderr, "  -q        in ? thay vì các ký tự không in được\n");
    fprintf(stderr, "  -R        liệt kê thư mục con đệ quy\n");
    fprintf(stderr, "  -r        đảo ngược thứ tự khi sắp xếp\n");
    fprintf(stderr, "  -S        sắp xếp theo kích thước file, lớn nhất trước\n");
    fprintf(stderr, "  -s        in kích thước đã cấp phát của mỗi file, tính theo block\n");
    fprintf(stderr, "  -t        sắp xếp theo thời gian sửa đổi, mới nhất trước\n");
    fprintf(stderr, "  -u        sử dụng thời gian truy cập thay vì thời gian sửa đổi\n");
    fprintf(stderr, "  -w        in các ký tự không in được dưới dạng thô\n");
}

/**
 * Tạo một cấu trúc FileEntry mới
 */
FileEntry *create_file_entry(const char *name, const char *path, struct stat *st) {
    FileEntry *entry;
    
    entry = malloc(sizeof(FileEntry));
    if (!entry) {
        return NULL;
    }
    
    /* Sao chép tên */
    entry->name = strdup(name);
    if (!entry->name) {
        free(entry);
        return NULL;
    }
    
    /* Sao chép đường dẫn */
    entry->path = strdup(path);
    if (!entry->path) {
        free(entry->name);
        free(entry);
        return NULL;
    }
    
    /* Sao chép thông tin stat */
    memcpy(&entry->st, st, sizeof(struct stat));
    
    /* Trích xuất các trường thường dùng */
    entry->inode = st->st_ino;
    entry->size = st->st_size;
    entry->mtime = st->st_mtime;
    entry->atime = st->st_atime;
    entry->ctime = st->st_ctime;
    entry->blocks = st->st_blocks;
    
    return entry;
}

/**
 * Giải phóng một cấu trúc FileEntry
 */
void free_file_entry(FileEntry *entry) {
    if (!entry) return;
    
    free(entry->name);
    free(entry->path);
    free(entry);
}
