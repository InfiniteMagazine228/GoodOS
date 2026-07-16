ENTRY(_start)

SECTIONS
{
    /* GRUB khuyên khích nạp Kernel tại địa chỉ 1 MiB trở lên */
    . = 1M;

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot) /* Đặt multiboot header lên đầu tiên */
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
}
