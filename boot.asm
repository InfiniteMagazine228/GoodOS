[org 0x7c00]          ; Điểm nạp mặc định của Bootloader trong RAM

mov ah, 0x0e         ; Lệnh BIOS: Chọn chế độ in ký tự lên màn hình
mov bx, msg          ; Nạp địa chỉ của chuỗi ký tự vào thanh ghi BX

print_loop:
    mov al, [bx]     ; Lấy ký tự tại địa chỉ BX bỏ vào AL
    cmp al, 0        ; Kiểm tra xem đã gặp ký tự kết thúc (0) chưa
    je hang          ; Nếu đúng, nhảy đến nhãn 'hang' (dừng lại)
    int 0x10         ; Gọi ngắt BIOS 0x10 để in ký tự trong AL ra màn hình
    inc bx           ; Tăng BX lên 1 để trỏ tới ký tự tiếp theo
    jmp print_loop   ; Lặp lại cú nhảy

hang:
    jmp hang         ; Vòng lặp vô tận để treo máy, tránh thực thi mã rác

msg:
    db "Hello, World!", 0 ; Chuỗi ký tự muốn in, kết thúc bằng số 0

times 510-($-$$) db 0 ; Điền các byte 0 cho đủ kích thước 510 bytes
dw 0xaa55            ; 2 bytes cuối bắt buộc để BIOS nhận diện đây là Bootloader
