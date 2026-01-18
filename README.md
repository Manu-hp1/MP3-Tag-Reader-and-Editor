# 🎵 MP3 Tag Reader & Tag Editor using C

A command-line based **MP3 Tag Reader and Tag Editor** application developed in C, designed to read, display, and modify ID3 metadata from MP3 files.
This project demonstrates strong fundamentals in C programming, binary file handling, and structured data processing.

---

## 🚀 Project Description

The MP3 Tag Reader & Tag Editor application enables users to extract and update metadata information such as **title, artist, album, year, and genre** from MP3 files.
It works by parsing ID3 tags directly from MP3 files using low-level file operations.

This project was developed as part of the **C Programming module** in the **Embedded Systems Professional Course at Emertxe Information Technologies**.

---

## 🎯 Objective

- To understand the ID3 tag structure in MP3 files
- To apply binary file handling techniques in C
- To read and modify metadata without corrupting MP3 content
- To improve low-level programming and file parsing skills

---

## ✨ Features

- Read and display MP3 ID3 tag information
- Edit metadata fields such as title, artist, album, year, and genre
- Supports ID3 tag-based MP3 files
- Command-line based interface
- Input validation and error handling
- Preserves original audio data while editing tags
---

## 🛠️ Technologies & Concepts Used

- **C Programming**
- **Binary File Handling**
- **File I/O (fopen, fread, fwrite, fseek)**
- **Structures**
- **Pointers and Arrays**
- **String Manipulation**
- **Modular Programming**

---

## 🧠 How the Application Works

- MP3 files are opened in binary mode
- ID3 header and frames are parsed using file offsets
- Metadata values are extracted and displayed to the user
- For editing, selected tag frames are updated safely
- Audio data remains untouched during metadata modification
- Separate modules handle reading and editing operations

---

## 📂 Project Structure

```text
AddressBook/
├── main.c
├── contact.c
├── contact.h
├── file.c
├── file.h
└── contacts.csv
```

---

## ▶️ Compilation and Execution

### Compile and run:
```bash
gcc *.c -o address_book
./address_book
```

## Learning Outcome and Impact

This project strengthened my understanding of core C programming concepts such as structures, pointers, and file handling. It improved my problem-solving approach, debugging skills, and ability to design clean, modular applications. Working on this project increased my confidence in translating theoretical knowledge into a practical, working solution.

---

## Acknowledgment

I would like to sincerely thank Anand Lokhande Sir for his guidance and clear explanation of C programming concepts, which helped me build this project with a strong logical foundation.

---

## GitHub Repository

[https://github.com/Manu-hp1/Address-Book]

---

## About Me

I am an Embedded Systems Trainee at Emertxe Information Technologies, building strong fundamentals in C programming, Linux, and system-level concepts. I am passionate about writing clean code and continuously improving my technical skills.

---

## License

This project is developed for learning and educational purposes.

