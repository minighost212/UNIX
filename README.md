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

### Cách Sử Dụng Cơ Bản

```bash
# Liệt kê thư mục hiện tại
./ls

# Liệt kê file hoặc thư mục cụ thể
./ls /đường/dẫn/đến/thư_mục

# Liệt kê nhiều đường dẫn
./ls file1.txt /đường/dẫn/đến/dir file2.txt
```

### Ví Dụ Phổ Biến

```bash
# Định dạng dài với kích thước dễ đọc
./ls -lh

# Liệt kê tất cả file bao gồm file ẩn
./ls -la

# Sắp xếp theo thời gian sửa đổi, mới nhất trước
./ls -lt

# Sắp xếp đảo ngược theo kích thước
./ls -lSr

# Liệt kê đệ quy
./ls -R

# Hiển thị số inode và số lượng block
./ls -lis

# Liệt kê chính thư mục, không phải nội dung
./ls -ld /đường/dẫn/đến/thư_mục

# UID/GID dạng số trong định dạng dài
./ls -n
```

### Kết Hợp Tùy Chọn

Các tùy chọn có thể được kết hợp:
```bash
./ls -lah      # Định dạng dài, tất cả file, dễ đọc
./ls -ltR      # Định dạng dài, sắp xếp theo thời gian, đệ quy
./ls -iSr      # Hiển thị inode, sắp xếp theo kích thước (đảo ngược)
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

### Kiểm Thử Thủ Công

Kiểm tra các tình huống khác nhau:

```bash
# Test liệt kê cơ bản
./ls

# Test với đường dẫn không tồn tại
./ls /không_tồn_tại

# Test từ chối quyền
./ls /root

# Test với symbolic link
./ls -l /bin/sh

# Test liệt kê đệ quy
./ls -R /etc/systemd

# Test sắp xếp
./ls -lt /tmp
./ls -lS /var/log
```

### Các Trường Hợp Đặc Biệt

Bản cài đặt xử lý:
- Thư mục rỗng
- Symbolic link
- Device file
- File có ký tự đặc biệt trong tên
- Thư mục lớn
- Các loại file hỗn hợp

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
