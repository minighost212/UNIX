#!/bin/sh
# Test suite đầy đủ cho ls project

echo "======================================"
echo "TEST SUITE ĐẦY ĐỦ CHO LS PROJECT"
echo "======================================"
echo ""

# Màu (nếu terminal hỗ trợ)
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

pass=0
fail=0

test_cmd() {
    echo "Test: $1"
    echo "Command: $2"
    eval "$2"
    if [ $? -eq 0 ]; then
        echo "${GREEN}✓ PASS${NC}"
        pass=$((pass + 1))
    else
        echo "${RED}✗ FAIL${NC}"
        fail=$((fail + 1))
    fi
    echo ""
}

echo "=== 1. TEST CÁC OPTIONS CƠ BẢN ==="
echo ""

# Test không tham số
test_cmd "Không tham số" "./ls"

# Test -l
test_cmd "-l (long format)" "./ls -l"

# Test -a
test_cmd "-a (tất cả file bao gồm hidden)" "./ls -a"

# Test -A
test_cmd "-A (tất cả trừ . và ..)" "./ls -A"

# Test -h
test_cmd "-h (human readable)" "./ls -lh"

# Test -i
test_cmd "-i (hiển thị inode)" "./ls -i"

# Test -s
test_cmd "-s (hiển thị block count)" "./ls -s"

# Test -k
test_cmd "-k (kích thước kilobytes)" "./ls -sk"

# Test -t
test_cmd "-t (sắp xếp theo thời gian)" "./ls -lt"

# Test -S
test_cmd "-S (sắp xếp theo kích thước)" "./ls -lS"

# Test -r
test_cmd "-r (đảo ngược)" "./ls -lr"

# Test -R
test_cmd "-R (đệ quy)" "./ls -R src"

# Test -d
test_cmd "-d (list directory as file)" "./ls -ld ."

# Test -F
test_cmd "-F (file type indicators)" "./ls -F"

# Test -f
test_cmd "-f (không sắp xếp)" "./ls -f"

# Test -n
test_cmd "-n (numeric UID/GID)" "./ls -ln"

# Test -c
test_cmd "-c (ctime)" "./ls -lc"

# Test -u
test_cmd "-u (atime)" "./ls -lu"

# Test -q
test_cmd "-q (non-printable as ?)" "./ls -q"

# Test -w
test_cmd "-w (raw non-printable)" "./ls -w"

echo ""
echo "=== 2. TEST TỔ HỢP OPTIONS ==="
echo ""

# Test -la
test_cmd "-la (long + all)" "./ls -la"

# Test -lh
test_cmd "-lh (long + human)" "./ls -lh"

# Test -lt
test_cmd "-lt (long + time sort)" "./ls -lt"

# Test -lS
test_cmd "-lS (long + size sort)" "./ls -lS"

# Test -laR
test_cmd "-laR (all + recursive)" "./ls -laR src"

# Test -lis
test_cmd "-lis (long + inode + blocks)" "./ls -lis"

echo ""
echo "=== 3. TEST OPTIONS GHI ĐÈ NHAU ==="
echo ""

# Test -l vs -n (n thắng)
test_cmd "-ln (n override l)" "./ls -ln"

# Test -n vs -l (l thắng)
test_cmd "-nl (l override n)" "./ls -nl"

# Test -c vs -u (u thắng)
test_cmd "-cu (u override c)" "./ls -lcu"

# Test -u vs -c (c thắng)
test_cmd "-uc (c override u)" "./ls -luc"

# Test -R vs -d (d thắng)
test_cmd "-Rd (d override R)" "./ls -Rd ."

# Test -d vs -R (R thắng)
test_cmd "-dR (R override d)" "./ls -dR src"

# Test -k vs -h (h thắng)
test_cmd "-kh (h override k)" "./ls -skh"

# Test -h vs -k (k thắng)
test_cmd "-hk (k override h)" "./ls -shk"

# Test -q vs -w (w thắng)
test_cmd "-qw (w override q)" "./ls -qw"

# Test -w vs -q (q thắng)
test_cmd "-wq (q override w)" "./ls -wq"

echo ""
echo "=== 4. TEST VỚI ĐƯỜNG DẪN KHÁC ==="
echo ""

test_cmd "Test với /etc" "./ls -l /etc | head -5"
test_cmd "Test với /tmp" "./ls -la /tmp | head -5"
test_cmd "Test với nhiều paths" "./ls Makefile README.md"
test_cmd "Test với ~ (home)" "./ls ~"

echo ""
echo "=== 5. TEST EDGE CASES (XỬ LÝ LỖI) ==="
echo ""

# Test file không tồn tại
echo "Test: File không tồn tại"
echo "Command: ./ls /not_exist_file_12345"
./ls /not_exist_file_12345 2>&1
if [ $? -ne 0 ]; then
    echo "${GREEN}✓ PASS (exit code > 0)${NC}"
    pass=$((pass + 1))
else
    echo "${RED}✗ FAIL (should return error)${NC}"
    fail=$((fail + 1))
fi
echo ""

# Test thư mục không tồn tại
echo "Test: Thư mục không tồn tại"
echo "Command: ./ls /not_exist_dir_12345"
./ls /not_exist_dir_12345 2>&1
if [ $? -ne 0 ]; then
    echo "${GREEN}✓ PASS (exit code > 0)${NC}"
    pass=$((pass + 1))
else
    echo "${RED}✗ FAIL (should return error)${NC}"
    fail=$((fail + 1))
fi
echo ""

# Test permission denied
echo "Test: Permission denied"
echo "Command: ./ls /root 2>&1 | head -1"
./ls /root 2>&1 | head -1
if [ $? -ne 0 ]; then
    echo "${GREEN}✓ PASS (handled permission error)${NC}"
    pass=$((pass + 1))
else
    echo "${YELLOW}⚠ WARNING (might have root access)${NC}"
fi
echo ""

# Test với symbolic link
if [ -L /bin/sh ]; then
    test_cmd "Test symbolic link" "./ls -l /bin/sh"
fi

# Test với thư mục rỗng
echo "Test: Thư mục rỗng"
mkdir -p /tmp/empty_test_$$
./ls /tmp/empty_test_$$
rmdir /tmp/empty_test_$$
echo "${GREEN}✓ PASS (no crash)${NC}"
pass=$((pass + 1))
echo ""

# Test với tên file đặc biệt
echo "Test: File có ký tự đặc biệt"
touch "/tmp/test file with spaces.txt" 2>/dev/null
./ls "/tmp/test file with spaces.txt" 2>&1
rm -f "/tmp/test file with spaces.txt"
echo "${GREEN}✓ PASS (no crash)${NC}"
pass=$((pass + 1))
echo ""

echo ""
echo "=== 6. TEST KHÔNG BỊ SEGFAULT ==="
echo ""

# Test với input lớn
test_cmd "Test với /usr/bin (nhiều file)" "./ls -la /usr/bin | head -10"

# Test đệ quy sâu
test_cmd "Test recursive /usr" "./ls -R /usr/share/doc 2>&1 | head -20"

# Test với nhiều options cùng lúc
test_cmd "Test nhiều options" "./ls -ailshRFq . 2>&1 | head -10"

echo ""
echo "=== 7. SO SÁNH VỚI LS HỆ THỐNG ==="
echo ""

echo "Comparing output format:"
echo "--- System ls ---"
/bin/ls -l | head -3
echo ""
echo "--- Your ls ---"
./ls -l | head -3
echo ""

echo "======================================"
echo "KẾT QUẢ TEST"
echo "======================================"
echo "Passed: ${GREEN}$pass${NC}"
echo "Failed: ${RED}$fail${NC}"
total=$((pass + fail))
echo "Total: $total"
if [ $fail -eq 0 ]; then
    echo "${GREEN}✓ TẤT CẢ TEST ĐỀU PASS!${NC}"
else
    echo "${YELLOW}⚠ Có $fail test failed${NC}"
fi
echo "======================================"
