// Hàm hỗ trợ in chuỗi trực tiếp ra bộ nhớ màn hình VGA text mode
void print_string(const char* str, unsigned char color) {
    volatile char* video_memory = (volatile char*)0xB8000;
    static int position = 0; // Vị trí ký tự trên màn hình
    
    for (int i = 0; str[i] != '\0'; ++i) {
        video_memory[position++] = str[i];
        video_memory[position++] = color; // Byte thuộc tính màu sắc
    }
}

// Hàm kiểm tra chuỗi thô cơ bản (thay thế cho strcmp)
bool string_contains(const char* source, const char* target) {
    if (!source || !target) return false;
    for (int i = 0; source[i] != '\0'; ++i) {
        int j = 0;
        while (target[j] != '\0' && source[i + j] == target[j]) {
            j++;
        }
        if (target[j] == '\0') return true;
    }
    return false;
}

// Cấu trúc nhận gói tin từ GRUB truyền sang
struct multiboot_info {
    unsigned long flags;
    unsigned long mem_lower;
    unsigned long mem_upper;
    unsigned long boot_device;
    unsigned long cmdline; // Con trỏ 32-bit trỏ đến chuỗi lệnh boot từ GRUB
};

// Sử dụng extern "C" để trình biên dịch C++ giữ nguyên tên hàm cho file ASM gọi
extern "C" void kernel_main(unsigned long magic, multiboot_info* info) {
    // Xóa toàn bộ màn hình văn bản (In các khoảng trắng màu xám)
    volatile char* video_memory = (volatile char*)0xB8000;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i+1] = 0x07;
    }

    // Kiểm tra tính hợp lệ của trình nạp GRUB
    if (magic != 0x2BADB002) {
        print_string("[ERROR] Invalid Multiboot Magic Number!", 0x0C); // Chữ đỏ
        return;
    }

    const char* boot_args = (const char*)info->cmdline;

    // Phân tích cú pháp chuỗi cmdline để kích hoạt chế độ tương ứng
    if (string_contains(boot_args, "mode=live")) {
        print_string("**************************************************\n", 0x0E);
        print_string("  Welcome to GoodOS [LIVE MODE] - Running on RAM  \n", 0x0A); // Chữ xanh lá
        print_string("**************************************************\n", 0x0E);
        // Tiến hành gọi module nạp RAM Disk / Live Init của bạn tại đây
    } else {
        print_string("==================================================\n", 0x0B);
        print_string("  GoodOS Standard Bootable - Initializing System  \n", 0x0B); // Chữ xanh lam
        print_string("==================================================\n", 0x0B);
        // Tiến hành nạp Driver từ ổ đĩa chính tại đây
    }
}
