# MP3-Tag-Reader-and-Editor

A C-based command-line application for viewing and editing MP3 ID3v2 metadata tags.  
This project demonstrates binary file parsing, dynamic memory management, and low-level file handling.

## Features

- View MP3 metadata tags
- Edit ID3v2 tags
- Binary file parsing
- Dynamic memory allocation
- Command line based tool

## Technologies Used

- C Programming
- File Handling
- Binary Data Processing
- ID3v2 Tag Structure

## How to Compile

gcc MP3.c view_edit.c -o mp3tag

## How to Run

./mp3tag <mp3_file>

Example:
./mp3tag song.mp3

## Project Structure

MP3.c        → Main program file  
view_edit.c  → Functions for viewing and editing ID3v2 tags  
README.md    → Project documentation
