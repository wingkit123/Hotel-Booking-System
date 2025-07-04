# Rasa Ria Room Booking System

![C Programming](https://img.shields.io/badge/Language-C-blue) ![File Storage](https://img.shields.io/badge/Data%20Storage-File%20System-green)

A comprehensive hotel room booking management system written in C, featuring room assignment, date validation, and persistent data storage.

## Features

- **Room Management**
  - Add new bookings with auto room assignment
  - Cancel existing bookings
  - Modify booking details
  - Search by room number or guest name

- **Data Validation**
  - Contact number validation (8-12 digits)
  - Date validation with leap year handling
  - Future date checking (up to 1 year)

- **Data Persistence**
  - Automatic loading of previous bookings
  - Save all data to file on exit

## How to Run

1. Compile the program:
   ```bash
   gcc sem1project.c -o hotel_booking
