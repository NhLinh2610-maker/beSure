#pragma once

#ifndef PLAYFAIRCIPHER_H
#define PLAYFAIRCIPHER_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class PlayfairCipher {
private:
    int size; // Kích thước ma trận (5 cho 5x5 hoặc 6 cho 6x6)
    char matrix[6][6]; // Mảng 2 chiều lưu trữ bảng mã trận
    char firstSep = 'X', secondSep = 'Y'; // Ký tự đệm khi gặp chữ trùng hoặc lẻ

    // --- HÀM LẤY BẢNG CHỮ CÁI MẪU ---
    string getAlphabet() {
        // Nếu 5x5: dùng 25 chữ cái (bỏ J). Nếu 6x6: dùng 26 chữ cái + 10 số.
        return (size == 5) ? "ABCDEFGHIKLMNOPQRSTUVWXYZ" : "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    }

    // --- HÀM KHỞI TẠO MA TRẬN TỪ KEY ---
    void prepareMatrix(string key) {
        string alphabet = getAlphabet();
        string temp = "";

        // Bước 1: Đưa Key vào chuỗi tạm, viết hoa và loại bỏ ký tự trùng
        for (char c : key) {
            c = toupper(c);
            if (size == 5 && c == 'J') c = 'I'; // Quy tắc 5x5: Coi J là I
            if (alphabet.find(c) != string::npos && temp.find(c) == string::npos)
                temp += c;
        }

        // Bước 2: Điền các ký tự còn thiếu trong bảng chữ cái vào sau Key
        for (char c : alphabet)
            if (temp.find(c) == string::npos) temp += c;

        // Bước 3: Đổ toàn bộ chuỗi temp vào ma trận 2 chiều
        int k = 0;
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++) matrix[i][j] = temp[k++];
    }

    // --- HÀM TÌM VỊ TRÍ (TỌA ĐỘ) CỦA MỘT KÝ TỰ ---
    void findPos(char c, int& r, int& col) {
        if (size == 5 && c == 'J') c = 'I';
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                if (matrix[i][j] == toupper(c)) {
                    r = i; // Trả về dòng
                    col = j; // Trả về cột
                    return;
                }
    }

public:
    // Hàm nhận cấu hình từ Giao diện (UI)
    void setConfig(int s, char s1, char s2) {
        size = s;
        firstSep = toupper(s1);
        secondSep = toupper(s2);
    }

    // --- HÀM XỬ LÝ CHÍNH: MÃ HÓA VÀ GIẢI MÃ ---
    string transform(string text, string key, bool encrypt) {
        prepareMatrix(key); // Luôn tạo lại ma trận dựa trên Key mới nhất
        string input = "", alphabet = getAlphabet();

        // 1. Lọc dữ liệu đầu vào: Chỉ lấy ký tự có trong bảng alphabet đã chọn
        for (char c : text) {
            c = toupper(c);
            if (alphabet.find(c) != string::npos || (size == 5 && c == 'J'))
                input += (size == 5 && c == 'J') ? 'I' : c;
        }

        // 2. XỬ LÝ ĐỆM (Chỉ làm khi MÃ HÓA)
        // 2. XỬ LÝ ĐỆM (Chỉ làm khi MÃ HÓA)
        if (encrypt) {
            string processed = "";
            size_t i = 0;

            while (i < input.length()) {
                processed += input[i]; // Lấy chữ cái đầu tiên của cặp

                // Nếu đây là chữ cái cuối cùng bị lẻ loi -> Chèn đệm vào đuôi rồi nghỉ
                if (i + 1 == input.length()) {
                    processed += (input[i] == firstSep) ? secondSep : firstSep;
                    break;
                }

                // Kiểm tra chữ cái thứ 2 của cặp
                if (input[i] == input[i + 1]) {
                    // Trùng nhau trong CÙNG 1 CẶP -> Chèn đệm, và chỉ nhảy 1 bước để vòng sau xét lại chữ kia
                    processed += (input[i] == firstSep) ? secondSep : firstSep;
                    i += 1;
                } else {
                    // Khác nhau -> Ghép thành 1 cặp bình thường, nhảy 2 bước
                    processed += input[i + 1];
                    i += 2;
                }
            }
            input = processed;
        }

        string result = "";
        int shift = encrypt ? 1 : -1; // Mã hóa thì tiến (+1), Giải mã thì lùi (-1)

        // 3. THỰC HIỆN BIẾN ĐỔI THEO TỪNG CẶP (2 ký tự)
        for (size_t i = 0; i < input.length(); i += 2) {
            int r1, c1, r2, c2;
            findPos(input[i], r1, c1);
            findPos(input[i + 1], r2, c2);

            // QUY TẮC 1: CÙNG HÀNG -> Dịch chuyển cột (Sang phải nếu mã hóa, trái nếu giải mã)
            if (r1 == r2) {
                result += matrix[r1][(c1 + shift + size) % size];
                result += matrix[r2][(c2 + shift + size) % size];
            }
            // QUY TẮC 2: CÙNG CỘT -> Dịch chuyển hàng (Xuống dưới nếu mã hóa, lên trên nếu giải mã)
            else if (c1 == c2) {
                result += matrix[(r1 + shift + size) % size][c1];
                result += matrix[(r2 + shift + size) % size][c2];
            }
            // QUY TẮC 3: TẠO THÀNH HÌNH CHỮ NHẬT -> Đổi cột của nhau
            else {
                result += matrix[r1][c2]; // Chữ 1 lấy cột chữ 2
                result += matrix[r2][c1]; // Chữ 2 lấy cột chữ 1
            }
        }
        return result;
    }
};

#endif
