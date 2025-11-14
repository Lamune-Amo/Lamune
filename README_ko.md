# Lamune Kernel

[English](README.md) · [**한국어**](README_ko.md)

Lamune는 [AMO 32-bit RISC SoC](https://github.com/Lamune-Amo/Architecture)를 대상으로 작성한 시분할 시스템 커널로, 4MB 물리 메모리 모델, 자체 부트 스트랩, 라운드 로빈 태스크 스케줄러, VGA/PS2 driver, `read` `write` 인터페이스, 그리고 커널 레벨에서 실행되는 여러 데모 유틸리티를 포함합니다.

| Path       | Notes                                                                                   |
| ---------- | --------------------------------------------------------------------------------------- |
| `arch/`    | Hardware-specific layer (atomics/interrupt control, MMIO defs; glue for VGA/PS2/timer). |
| `boot/`    | Early boot code and linker script (entry sections, ROM/RAM layout).                     |
| `drivers/` | Console and basic device I/O (e.g., VGA text output, keyboard input).                   |
| `fs/`      | Virtual file system scaffolding and interfaces.                                         |
| `init/`    | Kernel bring-up and the **init task**.                                                  |
| `kernel/`  | Core kernel logic: task management, scheduler, context switch, signal.                  |
| `lib/`     | Runtime helpers and software integer arithmetic operation routines.                     |
| `mm/`      | Memory management (allocator and related structures).                                   |
| `tools/`   | `shell` (built-ins) and demo utilities.                                                 |

## Highlights

**Time-sharing**: timer-tick driven scheduling (round-robin/preemptive)

**Interrupts**: IRQ (timer/PS2) and SWI (trap/syscall)

**MMIO**: VGA text console, PS/2 keyboard, timer, interrupt controller

**Toolchain**: works with AMO [Binutils](https://github.com/Lamune-Amo/Binutils) and [GCC](https://github.com/Lamune-Amo/GCC)

## Build and Usage

```
make all
```

```
$> ls
... lamune.coe ...
```

`lamune.coe` is the kernel's binary, so it will be the image to be loaded into RAM.

## Links

[Architecture](https://github.com/Lamune-Amo/Architecture) — AMO 32-bit RISC CPU & SoC

[Binutils](https://github.com/Lamune-Amo/Binutils) — binary toolchain for AMO

[GCC](https://github.com/Lamune-Amo/GCC) — C compiler for AMO

[Firmware](https://github.com/Lamune-Amo/Firmware) — ROM-resident boot stub

[Document](https://github.com/Lamune-Amo/Document) — ISA, microarchitecture, memory map, manuals