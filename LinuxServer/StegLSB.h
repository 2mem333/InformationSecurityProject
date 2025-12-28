#pragma once
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::string fotograftanSifreyiCikar(std::string filePath)
{
    // 1) Resmi decode et (RGBA)
    int w = 0, h = 0, comp = 0;
    unsigned char* data = stbi_load(filePath.c_str(), &w, &h, &comp, 4); // 4 = RGBA
    if (!data) {
        std::cout << "HATA: Resim okunamadi: " << filePath << "\n";
        std::cout << "stb: " << stbi_failure_reason() << "\n";
        return "";
    }

    const size_t totalBytes = (size_t)w * (size_t)h * 4;

    // 2) 16 byte (128 bit) oku (alpha kanalý atlanýr)
    const int passLen = 16;
    unsigned char out[passLen];
    std::memset(out, 0, sizeof(out));

    int bitIndex = 0;     // 0..127
    int byteIndex = 0;    // 0..15
    int bitPos = 7;       // MSB -> LSB

    for (size_t i = 0; i < totalBytes && bitIndex < passLen * 8; i++) {

        // alpha byte'ý atla (RGBA'da her 4. byte)
        if ((i % 4) == 3) continue;

        int bit = data[i] & 1;

        out[byteIndex] |= (unsigned char)(bit << bitPos);

        bitPos--;
        bitIndex++;

        if (bitPos < 0) {
            bitPos = 7;
            byteIndex++;
        }
    }

    stbi_image_free(data);

    if (bitIndex < passLen * 8) {
        std::cerr << "HATA: Resim icinde yeterli veri yok (128 bit okunamadi).\n";
        return "";
    }

    // 3) Çýkan 16 byte'ý string'e çevir (0 byte'da kes)
    std::string sifre;
    sifre.reserve(passLen);
    for (int i = 0; i < passLen; i++) {
        if (out[i] == '\0') break;
        sifre.push_back((char)out[i]);
    }

    return sifre;
}