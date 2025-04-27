[BITS 64]

;; THIS IS HEAVILY BASED OFF OF UNMAPPEDSTACK/TACOS
;; Check them out!
;; https://github.com/UnmappedStack/TacOS/blob/beee6216b58ee07ad39ac5e5350384cfb39d1aaa/src/mem/paging.c
;; This is under a different license from this project.

global reload_gdt

section .text

;; Reloads the GDT
reload_gdt:
  push 0x08
  push .reload_CS 
  retfq

.reload_CS:
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov ss, ax
  xor ax, ax
  mov fs, ax
  mov gs, ax
  ret
