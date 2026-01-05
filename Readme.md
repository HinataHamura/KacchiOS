# 🍚 kacchiOS

A minimal, educational **bare-metal operating system** developed for learning and implementing core Operating System concepts.

> This project is based on the original kacchiOS framework and extended as part of  
> **CSE 3202 – Operating Systems Sessional (RUET)**.

---

## 📖 Overview

**kacchiOS** is a simple x86 bare-metal operating system built from scratch for educational purposes.  
It provides a clean and understandable foundation to study how an OS works internally by implementing essential components manually.

This version of kacchiOS extends the base system with **memory management, process management, scheduling, and inter-process communication (IPC)**.

---

## ✅ Implemented Features

### 🔹 Boot & Kernel
- Multiboot-compliant bootloader (GRUB compatible)
- Boots and runs on QEMU
- Serial console I/O (COM1)
- Interactive null process shell

### 🔹 Memory Manager
- Heap allocation & deallocation (`kmalloc`, `kfree`)
- Heap placed safely after kernel using linker symbol
- Optimized allocation (block splitting + coalescing)
- Fixed-size stack allocation per process
- Stack reuse after deallocation

### 🔹 Process Manager
- Process Control Block (PCB) table
- Process creation & termination
- Process states:
  - `NEW`
  - `READY`
  - `CURRENT`
  - `WAITING`
  - `TERMINATED`
- Utility functions to query process information

### 🔹 Scheduler
- Round-Robin scheduling policy
- Software-level context switching
- Configurable time quantum
- Aging mechanism to prevent starvation

### 🔹 Inter-Process Communication (IPC)
- Per-process message queues
- FIFO message passing
- Sender / Receiver processes

---

## 🚀 Quick Start

### Prerequisites

#### Ubuntu / Debian
```bash
sudo apt-get install build-essential qemu-system-x86 gcc-multilib
Arch Linux
bash
Copy code
sudo pacman -S base-devel qemu gcc-multilib
macOS
bash
Copy code
brew install qemu i686-elf-gcc
Build & Run
bash
Copy code
# Clone repository
git clone https://github.com/farhan-shakib/kacchiOS.git
cd kacchiOS

# Build
make clean
make

# Run in QEMU
make run
🖥️ Expected Output (Sample)
sql
Copy code
Process created successfully
Creating multiple test processes...
Running scheduler (schedule())...
Hello from test process!
IPC tests complete
Running memory + stack tests...
Memory allocation successful

========================================
    kacchiOS - Minimal Baremetal OS
========================================
Hello from kacchiOS!
Running null process...

kacchiOS>
Type any input and press Enter — it will echo back.

📁 Project Structure
php
Copy code
kacchiOS/
├── boot.S          # Bootloader entry (Assembly)
├── kernel.c        # Kernel + tests + null process
├── memory.c        # Heap & stack memory manager
├── memory.h
├── process.c       # Process manager
├── process.h
├── scheduler.c     # Round Robin scheduler + aging
├── scheduler.h
├── ipc.c           # Inter-process communication
├── ipc.h
├── serial.c        # Serial port driver (COM1)
├── serial.h
├── string.c        # String utilities
├── string.h
├── types.h         # Basic type definitions
├── io.h            # I/O port helpers
├── link.ld         # Linker script
├── Makefile        # Build system
└── README.md       # This file
🛠️ Build System
Makefile Targets
Command	Description
make	Build kernel.elf
make run	Run in QEMU (serial only)
make run-vga	Run in QEMU with VGA
make debug	Run with GDB support
make clean	Remove build artifacts

🎓 Academic Context
Course: CSE 3202 – Operating Systems Sessional

Institution: Rajshahi University of Engineering & Technology (RUET)

Purpose: Educational OS development and experimentation

📚 Learning Resources
XINU OS

OSDev Wiki

The Little OS Book

Operating Systems: Three Easy Pieces (OSTEP)
