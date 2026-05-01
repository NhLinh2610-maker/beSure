#pragma once

#ifndef RSACIPHER_H
#define RSACIPHER_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <cmath>
#include <ctime>

using namespace std;

// Cấu trúc lưu trữ bộ khóa RSA
struct RSAKeyPair {
    long long n, e, d;
};

// Lớp hỗ trợ băm dữ liệu (Dùng cho Chữ ký số)
class SHA256 {
public:
    static string hash(string input) {
        unsigned long long h = 5381;
        for (char c : input) h = ((h << 5) + h) + c;
        stringstream ss;
        ss << hex << setw(16) << setfill('0') << h;
        return ss.str();
    }
};

class RSACipher {
private:
    // --- CÁC HÀM TOÁN HỌC HỖ TRỢ ---

    long long gcd(long long a, long long b) {
        while (b) { a %= b; swap(a, b); }
        return a;
    }

    // Tìm nghịch đảo modulo (d) bằng Euclid mở rộng
    long long modInverse(long long e, long long phi) {
        long long m0 = phi, t, q, x0 = 0, x1 = 1;
        if (phi == 1) return 0;
        while (e > 1) {
            q = e / phi; t = phi;
            phi = e % phi; e = t;
            t = x0; x0 = x1 - q * x0; x1 = t;
        }
        return (x1 < 0) ? x1 + m0 : x1;
    }

    bool isPrime(long long n) {
        if (n < 2) return false;
        for (long long i = 2; i <= sqrt(n); i++)
            if (n % i == 0) return false;
        return true;
    }

public:
    // --- 1. TẠO KHÓA NGẪU NHIÊN ---
    RSAKeyPair generateKeyPair() {
        srand(time(0));
        long long p, q;
        do { p = rand() % 400 + 100; } while (!isPrime(p));
        do { q = rand() % 400 + 100; } while (!isPrime(q) || q == p);

        long long n = p * q;
        long long phi = (p - 1) * (q - 1);
        long long e = 3;
        while (gcd(e, phi) != 1) e += 2;
        long long d = modInverse(e, phi);
        return { n, e, d };
    }

    // Nhân modulo an toàn: (a * b) % mod
    long long mulMod(long long a, long long b, long long mod) {
        long long res = 0; a %= mod;
        while (b > 0) {
            if (b % 2 == 1) res = (res + a) % mod;
            a = (a * 2) % mod; b /= 2;
        }
        return res;
    }

    // Lũy thừa nhanh: (base^exp) % mod
    long long power(long long base, long long exp, long long mod) {
        long long res = 1; base %= mod;
        while (exp > 0) {
            if (exp % 2 == 1) res = mulMod(res, base, mod);
            base = mulMod(base, base, mod); exp /= 2;
        }
        return res;
    }

    // --- 2. MÃ HÓA & GIẢI MÃ (TEXT <-> HEX) ---
    string encrypt(string text, long long e, long long n) {
        stringstream ss;
        for (char c : text)
            ss << hex << setw(8) << setfill('0') << power((long long)c, e, n);
        return ss.str();
    }

    string decrypt(string hexCipher, long long d, long long n) {
        string res = "";
        for (size_t i = 0; i < hexCipher.length(); i += 8) {
            string part = hexCipher.substr(i, 8);
            long long c = stoll(part, nullptr, 16);
            res += (char)power(c, d, n);
        }
        return res;
    }

    // --- 3. CHỮ KÝ SỐ (SIGN & VERIFY) ---

    // Ký số: Văn bản -> Hash -> Ký mã Hash bằng Khóa bí mật (d, n)
    string sign(string message, long long d, long long n) {
        string hashed = SHA256::hash(message);
        stringstream ss;
        for (char c : hashed)
            ss << hex << setw(8) << setfill('0') << power((long long)c, d, n);
        return ss.str();
    }

    // Xác thực: Kiểm tra văn bản và chữ ký bằng Khóa công khai (e, n)
    bool verify(string message, string signature, long long e, long long n) {
        string decryptedHash = "";
        for (size_t i = 0; i < signature.length(); i += 8) {
            string part = signature.substr(i, 8);
            long long c = stoll(part, nullptr, 16);
            decryptedHash += (char)power(c, e, n);
        }
        return (decryptedHash == SHA256::hash(message));
    }
};

#endif
