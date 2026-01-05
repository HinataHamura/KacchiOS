# KacchiOS — Repository file index

This README lists and briefly describes every file in the repository HinataHamura/KacchiOS as of 2026-01-05. It is intended to help contributors and users quickly find the important files and understand their purpose.

Notes:
- Compiled object files (.o) and kernel.elf are included in the repository but typically are build artifacts. Prefer building from source using the provided Makefile.
- Links point to the files on GitHub.

Files:

- [LICENSE](https://github.com/HinataHamura/KacchiOS/blob/main/LICENSE)
  - Project license text.

- [Makefile](https://github.com/HinataHamura/KacchiOS/blob/main/Makefile)
  - Build rules and targets for compiling the kernel and creating images.

- [Readme.md](https://github.com/HinataHamura/KacchiOS/blob/main/Readme.md)
  - The repository's original README (kept for reference).

- [boot.S](https://github.com/HinataHamura/KacchiOS/blob/main/boot.S)
  - Assembly bootstrap that prepares the environment before jumping to the kernel.

- [boot.o](https://github.com/HinataHamura/KacchiOS/blob/main/boot.o)
  - Compiled object for the bootstrap assembly.

- [io.h](https://github.com/HinataHamura/KacchiOS/blob/main/io.h)
  - I/O helper declarations and low-level hardware interfaces.

- [ipc.c](https://github.com/HinataHamura/KacchiOS/blob/main/ipc.c)
  - Inter-process communication implementation.

- [ipc.h](https://github.com/HinataHamura/KacchiOS/blob/main/ipc.h)
  - IPC public interfaces and types.

- [ipc.o](https://github.com/HinataHamura/KacchiOS/blob/main/ipc.o)
  - Compiled object for ipc.c.

- [kernel.c](https://github.com/HinataHamura/KacchiOS/blob/main/kernel.c)
  - Main kernel code and entry points.

- [kernel.elf](https://github.com/HinataHamura/KacchiOS/blob/main/kernel.elf)
  - Linked ELF binary image of the kernel (build artifact).

- [kernel.o](https://github.com/HinataHamura/KacchiOS/blob/main/kernel.o)
  - Compiled object for kernel.c.

- [link.ld](https://github.com/HinataHamura/KacchiOS/blob/main/link.ld)
  - Linker script controlling the kernel's memory layout.

- [memory.c](https://github.com/HinataHamura/KacchiOS/blob/main/memory.c)
  - Physical/virtual memory management implementation.

- [memory.h](https://github.com/HinataHamura/KacchiOS/blob/main/memory.h)
  - Memory management interfaces and types.

- [memory.o](https://github.com/HinataHamura/KacchiOS/blob/main/memory.o)
  - Compiled object for memory.c.

- [process.c](https://github.com/HinataHamura/KacchiOS/blob/main/process.c)
  - Process creation, management, and lifecycle.

- [process.h](https://github.com/HinataHamura/KacchiOS/blob/main/process.h)
  - Process-related types and declarations.

- [process.o](https://github.com/HinataHamura/KacchiOS/blob/main/process.o)
  - Compiled object for process.c.

- [scheduler.c](https://github.com/HinataHamura/KacchiOS/blob/main/scheduler.c)
  - Task scheduler implementation.

- [scheduler.h](https://github.com/HinataHamura/KacchiOS/blob/main/scheduler.h)
  - Scheduler public interfaces.

- [scheduler.o](https://github.com/HinataHamura/KacchiOS/blob/main/scheduler.o)
  - Compiled object for scheduler.c.

- [serial.c](https://github.com/HinataHamura/KacchiOS/blob/main/serial.c)
  - Serial port driver and console I/O.

- [serial.h](https://github.com/HinataHamura/KacchiOS/blob/main/serial.h)
  - Serial driver interfaces.

- [serial.o](https://github.com/HinataHamura/KacchiOS/blob/main/serial.o)
  - Compiled object for serial.c.

- [string.c](https://github.com/HinataHamura/KacchiOS/blob/main/string.c)
  - Basic string and memory helper functions used throughout the kernel.

- [string.h](https://github.com/HinataHamura/KacchiOS/blob/main/string.h)
  - Declarations for string helpers.

- [string.o](https://github.com/HinataHamura/KacchiOS/blob/main/string.o)
  - Compiled object for string.c.

- [types.h](https://github.com/HinataHamura/KacchiOS/blob/main/types.h)
  - Common type definitions used across the kernel.

Build and usage

1. Install the required cross-toolchain and tools (see Makefile for targets).
2. Run `make` to build the kernel and generate images.
3. Use QEMU or your preferred emulator to boot kernel. Example: `make run` (if configured in the Makefile).

Notes and suggestions

- Consider removing compiled artifacts (.o, kernel.elf) from the repository and adding them to .gitignore to keep the repo source-only. They can be built reproducibly with the provided Makefile.
- If you want this README to include file contents (not just links and descriptions), tell me and I will update it to embed each source file content or provide a script that generates a full dump.

License

This repository includes a [LICENSE](https://github.com/HinataHamura/KacchiOS/blob/main/LICENSE) file. Please refer to it for licensing information.

Generated on 2026-01-05 by HinataHamura's repository assistant.
