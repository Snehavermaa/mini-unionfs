# 🗂️ Mini-UnionFS (FUSE-Based Union File System)

## 📌 Overview

Mini-UnionFS is a simplified implementation of a **Union File System** built using **FUSE (Filesystem in Userspace)**.

It merges two directories:

* **Lower Directory (`lower/`)** → Read-only base layer
* **Upper Directory (`upper/`)** → Read-write layer

into a single **virtual mount point (`mnt/`)** that presents a unified view.

---

## 🧠 Key Concepts

### 🔁 Copy-on-Write (CoW)

When a file from the lower directory is modified:

* A copy is created in the upper directory
* Modifications are applied only to the copy
* Original file remains unchanged

---

### ❌ Whiteout Mechanism

When a file from the lower directory is deleted:

* A hidden file `.wh.<filename>` is created in the upper directory
* This hides the file from the merged view without deleting it from lower

---

### 📂 Layer Merging

* Files in `upper/` override files in `lower/`
* If a file exists only in `lower/`, it is still visible in `mnt/`

---

## 🏗️ Project Structure

```
mini-unionfs/
│
├── main.c              # Core FUSE filesystem implementation
├── unionfs.h           # Data structures and helper functions
├── Makefile            # Build configuration
├── test_unionfs.sh     # Test script
├── README.md           # Project documentation
│
├── lower/              # Read-only base layer (input)
├── upper/              # Writable layer
├── mnt/                # Mount point (merged output)
```

---

## ⚙️ Installation

### 1. Install dependencies

```bash
sudo apt update
sudo apt install fuse3 libfuse3-dev pkg-config
```

---

### 2. Build the project

```bash
make
```

---

## 🚀 Usage

### Step 1: Create directories

```bash
mkdir lower upper mnt
echo "hello from lower" > lower/file.txt
```

---

### Step 2: Run the filesystem

```bash
./mini_unionfs lower upper mnt
```

---

### Step 3: Open a new terminal and test

```bash
ls mnt
cat mnt/file.txt
```

---

## 🧪 Testing Features

### 🔁 Copy-on-Write

```bash
echo "edit" >> mnt/file.txt
```

Check:

```bash
cat upper/file.txt   # modified
cat lower/file.txt   # unchanged
```

---

### ❌ Whiteout (Delete)

```bash
rm mnt/file.txt
ls upper
```

Expected:

```
.wh.file.txt
```

---

### ➕ Create File

```bash
touch mnt/new.txt
ls upper
```

---

### 📁 Create Directory

```bash
mkdir mnt/testdir
ls upper
```

---

## 🛑 Unmount Filesystem

```bash
fusermount -u mnt
```

---

## 🧠 How It Works

The filesystem intercepts file operations using FUSE and:

* Resolves paths between `upper/` and `lower/`
* Applies Copy-on-Write when needed
* Uses whiteout files to handle deletions
* Merges directory listings dynamically

---

## 🎯 Applications

This project demonstrates concepts used in:

* 🐳 Docker & Container Filesystems
* 📦 OverlayFS
* 🖥️ Virtual Filesystems in Linux

---

## 📌 Notes

* Do not modify files directly in `mnt/` without mounting
* Always unmount before deleting directories
* Requires Linux environment with FUSE support

---

## 🚀 Status

✅ Fully functional
✅ Supports CoW and Whiteout
✅ Ready for academic submission

---
