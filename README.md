# Lệnh UNIX ls - Phiên Bản Đơn Giản Hóa

Lập trình lệnh `ls(1)` của UNIX từ đầu như một bài tập giữa kỳ. Dự án này giúp hiểu về các thao tác hệ thống file UNIX và các khái niệm lập trình ở mức hệ thống.

## Thông Tin Tác Giả

- **Sinh viên:** Nguyễn Kim Thạch
- **MSSV:** 24IT239
- **Môn học:** Lập trình hệ thống UNIX
- **GitHub:** https://github.com/minighost212/nguyenkimthach_24IT239_midterm

## Yêu Cầu Hệ Thống
- **Hệ điều hành**: NetBSD, Linux, hoặc các hệ UNIX-like khác
- **Compiler**: GCC hoặc Clang hỗ trợ C99
- **Build tool**: make hoặc gmake
- **Git**: Để clone repository

## 🚀 Cài Đặt và Sử Dụng

```bash
git clone https://github.com/minighost212/nguyenkimthach_24IT239_midterm.git
cd nguyenkimthach_24IT239_midterm
make                                    # biên dịch chương trình (hoặc: gmake)
./ls                                    # liệt kê thư mục hiện tại
./ls -l                                 # xem danh sách chi tiết
./ls -laF                               # all files + long format + indicators
```



## Cấu Trúc Dự Án

```
.
├── README.md           # Tài liệu dự án
├── Makefile           # Cấu hình build
├── .gitignore         # Quy tắc ignore Git
├── include/           # Các file header
│   ├── ls.h          # Cấu trúc chính và khai báo hàm
│   ├── format.h      # Hàm định dạng và hiển thị
│   └── utils.h       # Hàm tiện ích
└── src/              # Các file nguồn
    ├── main.c        # Điểm vào và phân tích đối số
    ├── ls.c          # Logic liệt kê thư mục cốt lõi
    ├── format.c      # Implementation định dạng output
    └── utils.c       # Hàm helper và tiện ích
```

## Các Tính Năng Đã Cài Đặt

Bản cài đặt này hỗ trợ các tùy chọn sau theo trang manual được cung cấp:

| Option | Chức năng |
|--------|-----------|
| `-A` | Liệt kê mọi entry trừ `.` và `..` |
| `-a` | Bao gồm cả entry bắt đầu bằng `.` |
| `-c` | Dùng thời gian đổi trạng thái (ctime) để sắp xếp (`-t`) hoặc in (`-l`) |
| `-d` | Thư mục được liệt kê như file thường; symlink & operand không bị đi xuyên qua |
| `-F` | Thêm ký hiệu: `/` thư mục, `*` thực thi, `@` symlink, `%` whiteout, `=` socket, `|` FIFO |
| `-f` | Không sắp xếp |
| `-h` | Với `-s` / `-l`: kích thước dạng dễ đọc (K, M, G...). Ghi đè `-k` |
| `-i` | In số inode |
| `-k` | Kích thước tính bằng kilobyte |
| `-l` | Định dạng dài (quyền, link, owner, group, size, time, name) |
| `-n` | Như `-l` nhưng hiển thị UID/GID dạng số |
| `-q` | In `?` cho ký tự không in được |
| `-R` | Liệt kê đệ quy thư mục con |
| `-r` | Đảo ngược thứ tự sắp xếp |
| `-S` | Sắp xếp theo kích thước file |
| `-s` | Hiển thị số block được dùng |
| `-t` | Sắp xếp theo thời gian sửa đổi (mới nhất trước) |
| `-u` | Dùng thời gian truy cập (atime) thay vì thời gian sửa đổi (mtime) |
| `-w` | In ký tự không in được nguyên dạng |

## Biên Dịch Dự Án

### Yêu Cầu

- Trình biên dịch GCC (hoặc Clang trên BSD)
- Tiện ích Make (GNU Make khuyến nghị cho NetBSD)
- Hệ điều hành giống UNIX (NetBSD, Linux, macOS, BSD)

### Các Target Make Khác

```bash
make clean                              # xóa các artifact build
make distclean                          # deep clean (bao gồm cả file backup)
make rebuild                            # rebuild từ đầu
make test                               # chạy test cơ bản
make help                               # hiển thị tất cả target có sẵn
```

## Cách Sử Dụng

### Khởi Động Nhanh

Sau khi build thành công bằng `make`, bạn có file thực thi `ls` trong thư mục dự án.

```bash
# Chạy chương trình (PHẢI có ./ ở đầu để phân biệt với /bin/ls hệ thống)
./ls

# So sánh với ls hệ thống
/bin/ls      # ls của NetBSD
./ls         # ls của bạn
```

### Cú Pháp Cơ Bản

### Sử Dụng Cơ Bản

```bash
./ls                                    # liệt kê thư mục hiện tại
./ls /etc                               # liệt kê thư mục cụ thể
./ls /tmp                               # liệt kê thư mục /tmp
./ls ~/Documents                        # liệt kê thư mục trong home
./ls README.md                          # liệt kê một file cụ thể
./ls /bin/ls                            # liệt kê file hệ thống
./ls Makefile README.md src include     # liệt kê nhiều đường dẫn
./ls file1.txt /tmp /etc file2.txt      # mix files và directories (files hiển thị trước)
# Một số tùy chọn hay dùng với -l
./ls -la                                # xem chi tiết, hiện cả file ẩn (gồm . và ..)
./ls -lA                                # xem chi tiết, hiện file ẩn nhưng bỏ . và ..
./ls -ls                                # xem chi tiết, thêm cột số block đĩa
./ls -lS                                # xem chi tiết, sắp xếp theo kích thước
./ls -lt                                # xem chi tiết, file sửa đổi mới nhất lên đầu
```

### Ví Dụ Sử Dụng

```bash
./ls                        # thư mục hiện tại
./ls -l                     # định dạng dài
./ls -n                     # định dạng dài với UID/GID dạng số
./ls -a                     # hiển thị file ẩn (bắt đầu với .)
./ls -A                     # hiển thị tất cả trừ . và ..
./ls -F                     # thêm ký hiệu loại file (/ * @ = |)
./ls -t                     # sắp xếp theo thời gian (mới nhất trước)
./ls -S                     # sắp xếp theo kích thước (lớn nhất trước)
./ls -r                     # đảo ngược thứ tự sắp xếp
./ls -f                     # không sắp xếp (theo thứ tự trong directory)
./ls -lh                    # kích thước dễ đọc (K, M, G)
./ls -lk                    # kích thước tính bằng kilobytes
./ls -s                     # hiển thị số block
./ls -i                     # hiển thị số inode
./ls -lis                   # kết hợp inode + blocks + long format
./ls -R                     # liệt kê đệ quy (bao gồm thư mục con)
./ls -ld /etc               # liệt kê thư mục như file (không vào trong)
./ls -ld */                 # liệt kê tất cả thư mục như file
./ls -lu                    # sử dụng access time (atime)
./ls -lc                    # sử dụng change time (ctime)
./ls -ltu                   # sắp xếp theo atime
./ls -ltc                   # sắp xếp theo ctime
./ls -lah                   # long format + all files + human readable
./ls -ltr                   # long format + sort by time + reverse
./ls -lSh                   # long format + sort by size + human readable
./ls -aRF                   # all files + recursive + with indicators
./ls -ilS                   # inode + long format + sort by size
./ls -lt /var/log | head    # xem file log mới nhất
./ls -lSh /tmp | head       # tìm file lớn nhất
./ls -lt *.c                # so sánh thời gian giữa các file
./ls -l /etc/passwd         # kiểm tra quyền truy cập chi tiết
./ls -l /bin/sh             # xem symlink trỏ đến đâu
./ls Makefile src           # mix files và directories: file in trước
./ls /nonexistent           # báo lỗi, exit code > 0
```

## Chi Tiết Cài Đặt

### Thiết Kế Modular

Dự án được tổ chức thành bốn module chính:

1. **main.c**: Xử lý phân tích đối số dòng lệnh và điều khiển luồng chương trình
2. **ls.c**: Logic đọc thư mục cốt lõi và xử lý entry
3. **format.c**: Định dạng output cho cả định dạng ngắn và dài
4. **utils.c**: Hàm tiện ích cho sắp xếp, xử lý đường dẫn và quản lý bộ nhớ

### Các Thuật Toán Chính

- **Sắp xếp**: Sử dụng `qsort()` với các hàm so sánh tùy chỉnh cho sắp xếp theo từ điển, thời gian và kích thước
- **Xử lý đường dẫn**: Nối đúng tên thư mục và file
- **Định dạng quyền**: Chuyển đổi mode bit sang định dạng dễ đọc (ví dụ: `rwxr-xr-x`)
- **Kích thước dễ đọc**: Chuyển đổi byte sang đơn vị K, M, G, T, P
- **Định dạng thời gian**: Hiển thị `MMM DD HH:MM` cho file gần đây, `MMM DD YYYY` cho file cũ

### Xử Lý Lỗi

Chương trình xử lý các điều kiện lỗi khác nhau:
- File hoặc thư mục không tồn tại
- Lỗi từ chối quyền truy cập
- Tùy chọn dòng lệnh không hợp lệ
- Lỗi cấp phát bộ nhớ

Lỗi được báo cáo ra stderr với thông báo mô tả.

## Kiểm Thử

### Chạy Test Suite Tự Động

Dự án có sẵn script test tự động:

```bash
# Chạy test suite đầy đủ
chmod +x test_full.sh
./test_full.sh
```

Test suite sẽ kiểm tra:
- ✅ Tất cả options đơn (-l, -a, -A, -h, -i, -s, -k, -t, -S, -r, -R, -d, -F, -f, -n, -c, -u, -q, -w)
- ✅ Tổ hợp options (-la, -lh, -lt, -lS, -laR, -lis)
- ✅ Options ghi đè nhau (-ln/-nl, -cu/-uc, -Rd/-dR, -kh/-hk, -qw/-wq)
- ✅ Edge cases (file không tồn tại, permission denied, symlink, file đặc biệt)
- ✅ Không segfault với input lớn
- ✅ So sánh với ls hệ thống



## Hạn Chế

Đây là bản cài đặt đơn giản. Một số tính năng từ lệnh `ls` đầy đủ không được cài đặt:

- Output màu sắc
- Định dạng output nhiều cột
- Tuân thủ POSIX đầy đủ cho tất cả trường hợp đặc biệt
- Tất cả tùy chọn nâng cao (ví dụ: `--color`, `--time-style`)

## Ghi Chú

- Chương trình tuân theo đặc tả từ trang manual NetBSD được cung cấp
- Code tuân theo các phương pháp coding sạch với comment có ý nghĩa
- Bộ nhớ được cấp phát và giải phóng đúng cách để ngăn rò rỉ
- Chương trình robust chống lỗi segmentation fault

## Build Trên Các Hệ Thống Khác Nhau

### NetBSD (Khuyến nghị - manual page từ NetBSD)
```bash
# Cài đặt GNU make
pkgin install gmake

# Build project
gmake

# Hoặc thử với BSD make (có thể hoạt động)
make
```

**Lưu ý**: Dự án dựa trên manual page NetBSD `ls(1)`. Makefile tương thích với cả GNU make và BSD make, nhưng `gmake` được khuyến nghị.

## Giấy Phép

Đây là dự án giáo dục cho mục đích học thuật.

## Tài Liệu Tham Khảo

- Trang manual `ls(1)` 
- Tài liệu môn học Lập Trình Hệ Thống UNIX
- Đặc tả POSIX.1-2008

---

**Cập nhật lần cuối**: Tháng 10 năm 2026 