#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/ls.h"
#include "../include/format.h"

/**
 * Lấy ký tự loại file cho định dạng dài
 */
void format_file_type(mode_t mode, char *type) {
    if (S_ISDIR(mode))       *type = 'd';
    else if (S_ISLNK(mode))  *type = 'l';
    else if (S_ISBLK(mode))  *type = 'b';
    else if (S_ISCHR(mode))  *type = 'c';
    else if (S_ISFIFO(mode)) *type = 'p';
    else if (S_ISSOCK(mode)) *type = 's';
    else                     *type = '-';
}

/**
 * Định dạng chuỗi quyền truy cập file (ví dụ: "rwxr-xr-x")
 */
void format_permissions(mode_t mode, char *buf) {
    /* Quyền của chủ sở hữu */
    buf[0] = (mode & S_IRUSR) ? 'r' : '-';
    buf[1] = (mode & S_IWUSR) ? 'w' : '-';
    if (mode & S_ISUID) {
        buf[2] = (mode & S_IXUSR) ? 's' : 'S';
    } else {
        buf[2] = (mode & S_IXUSR) ? 'x' : '-';
    }
    
    /* Quyền của nhóm */
    buf[3] = (mode & S_IRGRP) ? 'r' : '-';
    buf[4] = (mode & S_IWGRP) ? 'w' : '-';
    if (mode & S_ISGID) {
        buf[5] = (mode & S_IXGRP) ? 's' : 'S';
    } else {
        buf[5] = (mode & S_IXGRP) ? 'x' : '-';
    }
    
    /* Quyền của người khác */
    buf[6] = (mode & S_IROTH) ? 'r' : '-';
    buf[7] = (mode & S_IWOTH) ? 'w' : '-';
    if (mode & S_ISVTX) {
        buf[8] = (mode & S_IXOTH) ? 't' : 'T';
    } else {
        buf[8] = (mode & S_IXOTH) ? 'x' : '-';
    }
    
    buf[9] = '\0';
}

/**
 * Lấy ký tự chỉ báo file cho tùy chọn -F
 */
char get_file_indicator(mode_t mode) {
    if (S_ISDIR(mode))       return '/';
    if (S_ISLNK(mode))       return '@';
    if (S_ISFIFO(mode))      return '|';
    if (S_ISSOCK(mode))      return '=';
    if (mode & S_IXUSR)      return '*';  /* File thực thi */
    return '\0';
}

/**
 * Định dạng kích thước ở dạng dễ đọc (K, M, G, v.v.)
 */
void format_size_human(off_t size, char *buf, size_t bufsize) {
    const char *units[] = {"B", "K", "M", "G", "T", "P"};
    int unit_index = 0;
    double display_size = (double)size;
    
    while (display_size >= 1024.0 && unit_index < 5) {
        display_size /= 1024.0;
        unit_index++;
    }
    
    if (unit_index == 0) {
        snprintf(buf, bufsize, "%lld", (long long)size);
    } else if (display_size < 10.0) {
        snprintf(buf, bufsize, "%.1f%s", display_size, units[unit_index]);
    } else {
        snprintf(buf, bufsize, "%.0f%s", display_size, units[unit_index]);
    }
}

/**
 * Định dạng số lượng block cho tùy chọn -s
 */
void format_size_blocks(blkcnt_t blocks, int kilobytes, char *buf, size_t bufsize) {
    if (kilobytes) {
        snprintf(buf, bufsize, "%lld", (long long)(blocks / 2));
    } else {
        snprintf(buf, bufsize, "%lld", (long long)blocks);
    }
}

/**
 * Định dạng thời gian cho định dạng dài
 * Hiển thị "MMM DD HH:MM" cho file gần đây, "MMM DD  YYYY" cho file cũ
 */
void format_time(time_t file_time, char *buf, size_t bufsize) {
    time_t now = time(NULL);
    struct tm *tm_info;
    time_t six_months = 6 * 30 * 24 * 60 * 60;
    
    tm_info = localtime(&file_time);
    if (!tm_info) {
        snprintf(buf, bufsize, "??? ?? ??:??");
        return;
    }
    
    /* Nếu file gần đây (trong vòng 6 tháng) hoặc tương lai, hiển thị giờ */
    if (file_time > now - six_months && file_time <= now + six_months) {
        strftime(buf, bufsize, "%b %e %H:%M", tm_info);
    } else {
        /* File cũ hiển thị năm thay vì giờ */
        strftime(buf, bufsize, "%b %e  %Y", tm_info);
    }
}

/**
 * Lấy tên người dùng từ UID
 */
void get_user_name(uid_t uid, char *buf, size_t bufsize) {
    struct passwd *pw = getpwuid(uid);
    if (pw) {
        snprintf(buf, bufsize, "%s", pw->pw_name);
    } else {
        snprintf(buf, bufsize, "%u", uid);
    }
}

/**
 * Lấy tên nhóm từ GID
 */
void get_group_name(gid_t gid, char *buf, size_t bufsize) {
    struct group *gr = getgrgid(gid);
    if (gr) {
        snprintf(buf, bufsize, "%s", gr->gr_name);
    } else {
        snprintf(buf, bufsize, "%u", gid);
    }
}

/**
 * Tính tổng số block được sử dụng bởi tất cả các entry
 */
blkcnt_t calculate_total_blocks(FileEntry **entries, int count) {
    blkcnt_t total = 0;
    int i;
    
    for (i = 0; i < count; i++) {
        total += entries[i]->blocks;
    }
    
    return total;
}

/**
 * In tên file với xử lý đúng các ký tự đặc biệt
 */
void print_file_name(FileEntry *entry, Options *opts) {
    const char *name = entry->name;
    char indicator = '\0';
    
    /* Lấy ký tự chỉ báo loại file nếu -F được chỉ định */
    if (opts->flag_F) {
        indicator = get_file_indicator(entry->st.st_mode);
    }
    
    /* Xử lý các ký tự không in được */
    if (opts->flag_q) {
        /* Thay thế ký tự không in được bằng '?' */
        while (*name) {
            if (*name >= 32 && *name <= 126) {
                putchar(*name);
            } else {
                putchar('?');
            }
            name++;
        }
    } else {
        /* In thô */
        printf("%s", name);
    }
    
    /* In ký tự chỉ báo nếu có */
    if (indicator) {
        putchar(indicator);
    }
}

/**
 * In ở định dạng dài (tùy chọn -l hoặc -n)
 */
void print_long_format(FileEntry *entry, Options *opts) {
    char type;
    char perms[10];
    char user_buf[32];
    char group_buf[32];
    char size_buf[32];
    char time_buf[64];
    char blocks_buf[32];
    time_t display_time;
    
    /* In inode nếu -i được chỉ định */
    if (opts->flag_i) {
        printf("%7llu ", (unsigned long long)entry->inode);
    }
    
    /* In số lượng block nếu -s được chỉ định */
    if (opts->flag_s) {
        format_size_blocks(entry->blocks, opts->flag_k, blocks_buf, sizeof(blocks_buf));
        printf("%6s ", blocks_buf);
    }
    
    /* Loại file */
    format_file_type(entry->st.st_mode, &type);
    
    /* Quyền truy cập */
    format_permissions(entry->st.st_mode, perms);
    
    /* Số lượng liên kết */
    nlink_t nlinks = entry->st.st_nlink;
    
    /* Chủ sở hữu và nhóm */
    if (opts->flag_n) {
        /* ID dạng số */
        snprintf(user_buf, sizeof(user_buf), "%u", entry->st.st_uid);
        snprintf(group_buf, sizeof(group_buf), "%u", entry->st.st_gid);
    } else {
        /* Tên */
        get_user_name(entry->st.st_uid, user_buf, sizeof(user_buf));
        get_group_name(entry->st.st_gid, group_buf, sizeof(group_buf));
    }
    
    /* Kích thước hoặc số device */
    if (S_ISCHR(entry->st.st_mode) || S_ISBLK(entry->st.st_mode)) {
        snprintf(size_buf, sizeof(size_buf), "%3u, %3u", 
                 major(entry->st.st_rdev), minor(entry->st.st_rdev));
    } else {
        if (opts->flag_h) {
            format_size_human(entry->size, size_buf, sizeof(size_buf));
        } else {
            snprintf(size_buf, sizeof(size_buf), "%lld", (long long)entry->size);
        }
    }
    
    /* Thời gian - sử dụng thời gian phù hợp dựa trên flag -c hoặc -u */
    if (opts->flag_c) {
        display_time = entry->ctime;
    } else if (opts->flag_u) {
        display_time = entry->atime;
    } else {
        display_time = entry->mtime;
    }
    format_time(display_time, time_buf, sizeof(time_buf));
    
    /* In dòng định dạng dài */
    printf("%c%s %3lu %-8s %-8s %8s %s ",
           type, perms, (unsigned long)nlinks, 
           user_buf, group_buf, size_buf, time_buf);
    
    /* In tên file */
    print_file_name(entry, opts);
    
    /* In đích của liên kết nếu là symbolic link */
    if (S_ISLNK(entry->st.st_mode)) {
        char link_buf[MAX_PATH];
        ssize_t len = readlink(entry->path, link_buf, sizeof(link_buf) - 1);
        if (len > 0) {
            link_buf[len] = '\0';
            printf(" -> %s", link_buf);
        }
    }
    
    printf("\n");
}

/**
 * In ở định dạng ngắn (mặc định)
 */
void print_short_format(FileEntry *entry, Options *opts) {
    char blocks_buf[32];
    
    /* In inode nếu -i được chỉ định */
    if (opts->flag_i) {
        printf("%7llu ", (unsigned long long)entry->inode);
    }
    
    /* In số lượng block nếu -s được chỉ định */
    if (opts->flag_s) {
        format_size_blocks(entry->blocks, opts->flag_k, blocks_buf, sizeof(blocks_buf));
        printf("%6s ", blocks_buf);
    }
    
    /* In tên file */
    print_file_name(entry, opts);
    
    printf("\n");
}
