# 🗂️ Mini-UnionFS (FUSE-Based Union File System)

## 📌 Overview

Mini-UnionFS is a simplified implementation of a **Union File System** built using **FUSE (Filesystem in Userspace)**.

It merges two directories:

* **Lower Directory (`lower/`)** → Read-only base layer
* **Upper Directory (`upper/`)** → Read-write layer

into a single **virtual mount point (`mnt/`)**, providing a unified filesystem view.

---

## 🧠 Key Features

### 🔁 Copy-on-Write (CoW)

* Modifying a file from `lower/` creates a copy in `upper/`
* Changes are applied only to the copied file
* Original data remains untouched

---

### ❌ Whiteout Mechanism

* Deleting a file from `lower/` creates `.wh.<filename>` in `upper/`
* This hides the file from the merged view without deleting it

---

### 📂 Layer Merging

* Files in `upper/` override files in `lower/`
* Files only in `lower/` are still visible in `mnt/`

---

## 🏗️ Project Structure

```text
mini-unionfs/
│
├── main.c                 # Core filesystem logic (FUSE operations)
├── unionfs.h              # Data structures and helper functions
├── Makefile               # Build configuration
├── test_unionfs.sh        # Automated test script
├── README.md              # Documentation
├── Mini-UnionFS.pdf       # Design Document (2–3 pages)
│
├── lower/                 # Read-only base layer (input)
├── upper/                 # Writable layer
├── mnt/                   # Mount point (merged output)
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

### Step 1: Setup directories

```bash
mkdir lower upper mnt
echo "hello from lower" > lower/file.txt
```

---

### Step 2: Run filesystem

```bash
./mini_unionfs lower upper mnt
```

---

### Step 3: Open a new terminal and interact

```bash
ls mnt
cat mnt/file.txt
```

---

## 🧪 Testing Functionality

### 🔁 Copy-on-Write

```bash
echo "edit" >> mnt/file.txt
```

Verify:

```bash
cat upper/file.txt   # Modified version
cat lower/file.txt   # Original unchanged
```

---

### ❌ Whiteout (Deletion)

```bash
rm mnt/file.txt
ls upper
```

Expected:

```text
.wh.file.txt
```

---

### ➕ File Creation

```bash
touch mnt/new.txt
ls upper
```

---

### 📁 Directory Creation

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

* FUSE intercepts filesystem calls from the OS
* `resolve_path` determines file location (upper/lower/hidden)
* `readdir` merges directory contents
* `open` handles Copy-on-Write
* `unlink` implements whiteout logic

---

## 📄 Design Document

The detailed design, architecture, and edge-case handling are documented in:

👉 **Mini-UnionFS.pdf**

---

## 🎯 Applications

* Docker container filesystems
* OverlayFS in Linux
* Layered storage systems
---
