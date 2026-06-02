<p align="center">
  <img src="./icons/beSurelogo.png" alt="beSure Logo" width="150" />
</p>

<h1 align="center">beSure</h1>
<p align="center">
  <strong>Cryptography & Checksum Utility</strong><br />
  <i>A comprehensive tool for data encryption, digital signing, and file integrity verification.</i>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C%2B%2B-blue?style=flat-square&logo=c%2B%2B" alt="C++" />
  <img src="https://img.shields.io/badge/Framework-Qt%20Widgets-41CD52?style=flat-square&logo=qt" alt="Qt" />
  <img src="https://img.shields.io/badge/Platform-Windows-0078D6?style=flat-square&logo=windows" alt="Windows" />
  <img src="https://img.shields.io/github/v/release/phaamlong102/beSure?style=flat-square&color=orange" alt="Release" />
</p>

---

## 📖 Introduction
This utility program was developed as the final project for the **NT101 (An toàn mạng máy tính)** course at the **University of Information Technology (VNUHCM-UIT)**. 

**beSure** demonstrates the practical application of both Classical and Modern Cryptography, combined with essential data integrity tools used in network security.

---

## 🚀 Features & Video Demonstrations

### 1. Classical Cryptography: Playfair Cipher
* Full support for symmetric text encryption and decryption using a structured keyword matrix.
* **Demo:**
  <kbd>
    <video src="https://github.com/user-attachments/assets/fe69d137-1dc3-461d-8c7c-046fb77331df" width="100%" controls></video>
  </kbd>

### 2. Modern Cryptography: RSA Algorithm & Digital Signature
* **Asymmetric Encryption:** Secure message encryption and decryption using public/private key pairs.
* **Digital Signature:** Generates and verifies digital signatures utilizing a built-in SHA-256 hashing mechanism to ensure non-repudiation and authenticity.
* **Demo (Decryption & Signing):**
  <table>
    <tr>
      <td width="50%"><strong>RSA Decryption Test</strong></td>
      <td width="50%"><strong>RSA Signature Test</strong></td>
    </tr>
    <tr>
      <td><video src="https://github.com/user-attachments/assets/ea09a01e-d800-460e-9f3e-6afaef214f50" width="100%" controls></video></td>
      <td><video src="https://github.com/user-attachments/assets/bd12529c-3673-4774-8a93-cf19ea3e8d8b" width="100%" controls></video></td>
    </tr>
  </table>

### 3. File Checksum Checker
A vital utility for verifying file integrity against tampering or transmission errors.
* **Supported Hash Functions:** MD5, SHA-1, SHA-256, and SHA-512.
* **Verification:** Real-time calculation and side-by-side comparison with expected hash values.
* **Demo:**
  <table>
    <tr>
      <td width="50%"><strong>Checksum Test 01</strong></td>
      <td width="50%"><strong>Checksum Test 02</strong></td>
    </tr>
    <tr>
      <td><video src="https://github.com/user-attachments/assets/696fa26a-fd06-4a73-8d98-8b814d57fe09" width="100%" controls></video></td>
      <td><video src="https://github.com/user-attachments/assets/a0e6d33e-5227-4542-9674-167a57643473" width="100%" controls></video></td>
    </tr>
  </table>

<p align="center">
  <img src="./checksum.png" alt="Checksum Preview" width="80%" />
</p>

---

## 🛠️ Built With
* **C++** - Core algorithmic logic.
* **Qt Widgets** - Cross-platform GUI framework design.
* **CMake** - Build automation system.

---

## 📥 Downloads & Installation
You can fetch the pre-compiled, portable executable for Windows directly without setting up the build environment:
1. Navigate to the [beSure Releases Page](https://github.com/phaamlong102/beSure/releases).
2. Download the latest `.zip` archive.
3. Extract and run `beSure.exe`.

---

## 👨‍💻 Authors & Contributions
The project was successfully designed and completed by:

* **Pham Duc Long**  UI/UX Design & Checksum Feature Implementation.
* **Ngo Nhat Linh**  Core Logic for Playfair & RSA Cryptographic Algorithms.

---
<p align="center">🏆 <i>NT101 - University of Information Technology</i></p>
