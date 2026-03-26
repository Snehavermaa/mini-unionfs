# Mini-UnionFS 🗂️

A simplified implementation of a Union File System built using FUSE (Filesystem in Userspace). This project demonstrates how modern container systems like Docker manage layered file systems efficiently.

---

## 🚀 Project Overview

Mini-UnionFS merges multiple directories into a single virtual file system. It supports:

- A **read-only lower layer** (base files)
- A **read-write upper layer** (modifiable files)
- A unified view combining both layers

---

## ✨ Features

### 1. Layer Stacking (Directory Union)
- Combines `lower_dir` and `upper_dir` into one mount point
- If a file exists in both, the **upper layer takes precedence**

### 2. Copy-on-Write (CoW)
- Files in `lower_dir` are not modified directly
- When a file is edited:
  - It is copied to `upper_dir`
  - Modifications happen there

### 3. Whiteout (Deletion Handling)
- Deleting a file from `lower_dir` creates a hidden file:
- mini-unionfs/
├── main.c
├── unionfs.h
├── Makefile
├── README.md

---

## ⚙️ Setup Instructions

### 1. Install FUSE
```bash
sudo apt-get install libfuse3-dev fuse3
