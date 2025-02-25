# Library Management System (LMS) 📚

## Overview
The **Library Management System (LMS)** is a C++ console-based application designed to efficiently manage books, students, and transactions in a library. It provides role-based access to librarians, counter staff, and students, ensuring smooth operation and data security.

## Features
### 🔐 User Roles & Authentication
- **Librarian**: Full control over books and student records.
- **Counter Staff**: Manages book issues and returns.
- **Students**: Can borrow up to 3 books.
- **Security**:
  - Password authentication (default credentials: `lib123` for Librarian, `counter123` for Counter Staff).
  - Locks out after 5 failed login attempts.
  - Logs all login attempts in `login_log.txt`.

### 📑 Data Validation
- **Email**: Must end with `@gmail.com`, `@outlook.com`, or `@lpu.in`.
- **Phone Number**: 10 digits, starts with 6-9.
- **Registration Number**: 8-digit number.
- **ISBN**: 13-digit number.
- **Book Title & Author Name**: Can contain letters, numbers, spaces, and basic punctuation.

### 🛠 Functionalities
#### 👩‍🏫 Librarian
- Add, delete, and update books.
- Manage student records.
- View system logs.

#### 🏷 Counter Staff
- Issue books to students.
- Return books.
- Update book inventory.

#### 📖 Students
- Borrow up to **3 books** at a time.
- View issued books.

### 📂 File Management
- **books.txt**: Stores book details.
- **students.txt**: Stores student records.
- **issued_books.txt**: Tracks issued books.
- **login_log.txt**: Logs all login attempts.
- Data is **loaded on startup** and **saved before exit** to prevent data loss.

### ⚠️ Error Handling
- Invalid input detection.
- Prevents issuing unavailable books.
- Enforces borrowing limit (max 3 books per student).
- Incorrect login attempts are logged and locked after 5 tries.

### ✅ Success Messages
- Displayed after successful operations like adding/updating/deleting books, issuing/returning books, and logging in/out.

## 🏗 How to Run
1. **Compile the Code**
   ```sh
   g++ library_management.cpp -o lms
   ```
2. **Run the Executable**
   ```sh
   ./lms
   ```
3. **Login and Start Managing the Library!**

## 💡 Future Enhancements
- Add a GUI for better user experience.
- Implement a database (MySQL/PostgreSQL) for scalable storage.
- Send email notifications for due dates.
- Implement search and filter features.

## 🏆 Contributions
Feel free to contribute by reporting issues, suggesting improvements, or submitting pull requests!

## 📜 License
This project is open-source. Use it freely, and improve upon it! 😊


