# Lệnh UNIX ls - Phiên Bản Đơn Giản Hóa

Lập trình lệnh `ls(1)` của UNIX từ đầu như một bài tập giữa kỳ. Dự án này giúp hiểu về các thao tác hệ thống file UNIX và các khái niệm lập trình ở mức hệ thống.

## Thông Tin Tác Giả

- **Tên dự án**: Lệnh ls Đơn Giản Hóa
- **Môn học**: Lập Trình Hệ Thống UNIX
- **Loại**: Bài Tập Giữa Kỳ

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

### Tùy Chọn Hiển Thị
- **Mặc định**: Liệt kê file mỗi file một dòng
- **`-l`**: Định dạng liệt kê dài (quyền, liên kết, chủ sở hữu, nhóm, kích thước, thời gian, tên)
- **`-n`**: Giống `-l`, nhưng hiển thị UID và GID dạng số

### Lọc File
- **`-a`**: Bao gồm các entry bắt đầu với `.` (file ẩn)
- **`-A`**: Liệt kê tất cả entry trừ `.` và `..`
- **`-d`**: Liệt kê thư mục như file thông thường (không liệt kê nội dung)

### Tùy Chọn Sắp Xếp
- **Mặc định**: Thứ tự từ điển (lexicographical)
- **`-t`**: Sắp xếp theo thời gian sửa đổi (mới nhất trước)
- **`-S`**: Sắp xếp theo kích thước file (lớn nhất trước)
- **`-r`**: Đảo ngược thứ tự sắp xếp
- **`-f`**: Không sắp xếp (kéo theo `-a`)

### Tùy Chọn Thời Gian
- **`-c`**: Sử dụng thời gian thay đổi trạng thái cuối cùng (với `-t` hoặc `-l`)
- **`-u`**: Sử dụng thời gian truy cập cuối cùng (với `-t` hoặc `-l`)

### Cải Tiến Hiển Thị
- **`-F`**: Thêm ký hiệu vào entry (`/` cho thư mục, `*` cho file thực thi, `@` cho symlink, v.v.)
- **`-i`**: In số inode trước mỗi entry
- **`-s`**: In số lượng block trước mỗi entry
- **`-h`**: Kích thước file dễ đọc (với `-l` hoặc `-s`)
- **`-k`**: Báo cáo kích thước tính bằng kilobyte

### Tùy Chọn Khác
- **`-R`**: Liệt kê đệ quy các thư mục con
- **`-q`**: In `?` cho các ký tự không in được (mặc định khi output là terminal)
- **`-w`**: In các ký tự không in được ở dạng thô

## Biên Dịch Dự Án

### Yêu Cầu

- Trình biên dịch GCC (hoặc Clang trên BSD)
- Tiện ích Make (GNU Make khuyến nghị cho NetBSD)
- Hệ điều hành giống UNIX (NetBSD, Linux, macOS, BSD)

### Biên Dịch

```bash
# Build dự án
make

# Hoặc một cách rõ ràng
make all
```

Lệnh này sẽ tạo file thực thi tên `ls` trong thư mục hiện tại.

### Các Target Make Khác

```bash
# Xóa các artifact build
make clean

# Deep clean (bao gồm cả file backup)
make distclean

# Rebuild từ đầu
make rebuild

# Chạy test cơ bản
make test

# Hiển thị tất cả target có sẵn
make help
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

```
./ls [TÙY_CHỌN]... [FILE]...
```

### Sử Dụng Cơ Bản

#### 1. Liệt kê thư mục hiện tại
```bash
./ls
```

#### 2. Liệt kê file hoặc thư mục cụ thể
```bash
./ls /etc
./ls /tmp
./ls ~/Documents
```

#### 3. Liệt kê một file cụ thể
```bash
./ls README.md
./ls /bin/ls
```

#### 4. Liệt kê nhiều đường dẫn
```bash
./ls Makefile README.md src include
./ls file1.txt /tmp /etc file2.txt
```
**Lưu ý**: Files sẽ hiển thị trước, sau đó đến directories (theo spec NetBSD)

### Các Tùy Chọn Phổ Biến

#### Định Dạng Hiển Thị

```bash
# Định dạng dài (long format) - hiển thị chi tiết
./ls -l

# Định dạng dài với UID/GID dạng số
./ls -n

# Hiển thị file ẩn (bắt đầu với .)
./ls -a

# Hiển thị tất cả trừ . và ..
./ls -A

# Thêm ký hiệu loại file (/ * @ = |)
./ls -F
```

#### Sắp Xếp

```bash
# Sắp xếp theo thời gian (mới nhất trước)
./ls -t

# Sắp xếp theo kích thước (lớn nhất trước)
./ls -S

# Đảo ngược thứ tự sắp xếp
./ls -r

# Không sắp xếp (theo thứ tự trong directory)
./ls -f
```

#### Kích Thước File

```bash
# Kích thước dễ đọc (K, M, G)
./ls -lh

# Kích thước tính bằng kilobytes
./ls -lk

# Hiển thị số block
./ls -s
```

#### Thông Tin Bổ Sung

```bash
# Hiển thị số inode
./ls -i

# Hiển thị block count
./ls -s

# Kết hợp inode + blocks + long format
./ls -lis
```

#### Liệt Kê Đặc Biệt

```bash
# Liệt kê đệ quy (bao gồm thư mục con)
./ls -R

# Liệt kê thư mục như file (không vào trong)
./ls -ld /etc
./ls -ld */
```

#### Thời Gian

```bash
# Sử dụng access time (atime)
./ls -lu

# Sử dụng change time (ctime)
./ls -lc

# Sắp xếp theo atime
./ls -ltu

# Sắp xếp theo ctime
./ls -ltc
```

### Ví Dụ Kết Hợp

```bash
# Long format + all files + human readable
./ls -lah

# Long format + sort by time + reverse
./ls -ltr

# Long format + sort by size + human readable
./ls -lSh

# All files + recursive + with indicators
./ls -aRF

# Inode + long format + sort by size
./ls -ilS

# All options combined (stress test)
./ls -ailshRFnrtScuw
```

### Ví Dụ Thực Tế

#### Xem file log mới nhất
```bash
./ls -lt /var/log | head
```

#### Tìm file lớn nhất
```bash
./ls -lSh /tmp | head
```

#### Xem tất cả file kể cả ẩn
```bash
./ls -la
```

#### Xem cấu trúc thư mục đầy đủ
```bash
./ls -R
```

#### So sánh thời gian giữa các file
```bash
./ls -lt *.c
```

#### Kiểm tra quyền truy cập chi tiết
```bash
./ls -l /etc/passwd
```

#### Xem symlink trỏ đến đâu
```bash
./ls -l /bin/sh
```

### Ghi Chú Quan Trọng

#### Phân Biệt Với ls Hệ Thống

```bash
./ls         # Chạy chương trình của BẠN
ls           # Chạy /bin/ls (hệ thống)
/bin/ls      # Chạy ls hệ thống (rõ ràng)
```

#### Options Override (cái cuối thắng)

```bash
./ls -ln     # -n thắng: hiển thị numeric UID/GID
./ls -nl     # -l thắng: hiển thị user/group names

./ls -cu     # -u thắng: dùng atime
./ls -uc     # -c thắng: dùng ctime

./ls -kh     # -h thắng: human readable
./ls -hk     # -k thắng: kilobytes

./ls -Rd .   # -d thắng: list . như file
./ls -dR .   # -R thắng: recursive
```

#### Xử Lý Lỗi

```bash
# File không tồn tại
./ls /nonexistent
# Output: ls: /nonexistent: No such file or directory
# Exit code: > 0

# Permission denied
./ls /root
# Output: ls: /root: Permission denied
# Exit code: > 0

# Success
./ls README.md
# Exit code: 0
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

### Kiểm Thử Thủ Công

#### Test Cơ Bản

```bash
# 1. Test không tham số
./ls

# 2. Test với file cụ thể
./ls README.md

# 3. Test với directory
./ls /etc

# 4. Test với multiple paths
./ls README.md src Makefile
```

#### Test Options

```bash
# 5. Test long format
./ls -l

# 6. Test hidden files
./ls -a
./ls -A

# 7. Test sorting
./ls -t    # by time
./ls -S    # by size
./ls -r    # reverse

# 8. Test recursive
./ls -R src

# 9. Test indicators
./ls -F

# 10. Test với -d
./ls -ld /etc
```

#### Test Edge Cases

```bash
# 11. File không tồn tại (phải trả về lỗi)
./ls /nonexistent
echo $?  # Phải > 0

# 12. Permission denied
./ls /root
echo $?  # Phải > 0

# 13. Symbolic link
./ls -l /bin/sh
# Phải hiển thị -> target

# 14. Empty directory
mkdir /tmp/empty_test
./ls /tmp/empty_test
rmdir /tmp/empty_test

# 15. File với spaces
touch "/tmp/test file.txt"
./ls "/tmp/test file.txt"
rm "/tmp/test file.txt"

# 16. Large directory (không crash)
./ls -la /usr/bin

# 17. Deep recursion (không crash)
./ls -R /usr
```

#### Test Mix File + Directory

```bash
# 18. Mix files và directories
# Files phải hiển thị trước, directories sau
./ls file1.txt dir1 file2.txt dir2

# Expected output:
# file1.txt
# file2.txt
# 
# dir1:
# [contents]
# 
# dir2:
# [contents]
```

#### So Sánh Với ls Hệ Thống

```bash
# So sánh output
echo "=== System ls ==="
/bin/ls -l

echo "=== Your ls ==="
./ls -l

# So sánh với diff
/bin/ls -l > /tmp/sys.txt
./ls -l > /tmp/mine.txt
diff /tmp/sys.txt /tmp/mine.txt
```

### Các Trường Hợp Đã Test

✅ **Đã xử lý:**
- Thư mục rỗng
- Symbolic link (cả đúng và broken)
- Device files
- File có ký tự đặc biệt trong tên
- File có spaces trong tên
- Thư mục lớn (>1000 files)
- Deep recursion
- Permission denied
- File/directory không tồn tại
- Mix files và directories
- Tất cả options và tổ hợp

### Expected Exit Codes

```bash
# Success (exit 0)
./ls README.md
echo $?  # 0

# Error (exit > 0)
./ls /nonexistent
echo $?  # 1 hoặc 2

./ls /root  # nếu không có quyền
echo $?  # 1 hoặc 2
```

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

### Linux
```bash
make
```

### macOS
```bash
make
```

### NetBSD (Khuyến nghị - manual page từ NetBSD)
```bash
# Cài đặt GNU make
pkgin install gmake

# Build project
gmake

# Hoặc thử với BSD make (có thể hoạt động)
make
```

### Hệ Thống BSD Khác
```bash
# FreeBSD
pkg install gmake
gmake

# OpenBSD
pkg_add gmake
gmake
```

**Lưu ý**: Dự án dựa trên manual page NetBSD `ls(1)`. Makefile tương thích với cả GNU make và BSD make, nhưng `gmake` được khuyến nghị.

## Git Repository

Dự án này nên được host trong repository GitHub có tên:
```
Tên_MãSinhViên_midterm
```

Repository bao gồm:
- Tất cả source code (file `.c` và `.h`)
- Makefile
- README.md này
- `.gitignore` (được cấu hình để loại trừ binary và object file)

## Flag Biên Dịch

Makefile sử dụng các flag trình biên dịch sau:
- `-Wall`: Bật tất cả cảnh báo thông thường
- `-Wextra`: Bật cảnh báo bổ sung
- `-Werror`: Coi cảnh báo như lỗi
- `-std=c99`: Sử dụng chuẩn C99

## Giấy Phép

Đây là dự án giáo dục cho mục đích học thuật.

## Tài Liệu Tham Khảo

- Trang manual `ls(1)` của NetBSD (được cung cấp trong `a.txt`)
- Tài liệu môn học Lập Trình Hệ Thống UNIX
- Đặc tả POSIX.1-2008

---

**Cập nhật lần cuối**: Tháng 10 năm 2026
"# UNIX" 
