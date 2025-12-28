#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <stdexcept>

// stb headers (projene ekle)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

std::vector<unsigned char> lsb_pixels(std::vector<unsigned char> pixelsRGBA, const char* password16)
{
    const int passLen = 16;
    const int totalBits = passLen * 8;

    // RGBA: her piksel 4 byte. Alpha'yý bozmayalým -> sadece R,G,B kullanacaðýz.
    // Bu yüzden kapasite = (toplam byte - alpha byte sayýsý) = w*h*3 gibi düþün.
    // Burada vektör düz olduðu için alpha atlayarak yazacaðýz.
    int writable = 0;
    for (size_t i = 0; i < pixelsRGBA.size(); i++) {
        if ((i % 4) != 3) writable++; // alpha deðilse
    }
    if (writable < totalBits) {
        std::cout << "HATA: Piksel verisi LSB stego icin cok kucuk!\n";
        return pixelsRGBA;
    }

    int bitIndex = 0; // 0..127

    for (int i = 0; i < passLen; i++) {
        unsigned char ch = (unsigned char)password16[i];

        for (int b = 7; b >= 0; b--) {

            int bit = (ch >> b) & 1;

            // piksel indexine ilerle (alpha byte'larýný atla)
            size_t p = 0;
            int seenWritable = 0;

            // bitIndex'in yazýlacaðý gerçek byte indexini bul
            // (çok verimli deðil ama 128 bit için sorun deðil)
            for (p = 0; p < pixelsRGBA.size(); p++) {
                if ((p % 4) == 3) continue; // alpha atla
                if (seenWritable == bitIndex) break;
                seenWritable++;
            }

            // 1) Son bitini sýfýrla
            pixelsRGBA[p] &= 0xFE;

            // 2) Þifreden gelen biti son bite yaz
            pixelsRGBA[p] |= (unsigned char)bit;

            bitIndex++;
        }
    }
    return pixelsRGBA;
}

void fotografaSifreyiGom(std::string filePath, std::string sifre)
{
    // 1) Resmi decode et (BMP/PNG/JPG hepsi olur)
    int w = 0, h = 0, comp = 0;
    unsigned char* data = stbi_load(filePath.c_str(), &w, &h, &comp, 4); // 4 = RGBA
    if (!data) {
        std::cerr << "HATA: Resim okunamadi: " << filePath << "\n";
        std::cerr << "stb: " << stbi_failure_reason() << "\n";
        return;
    }

    const size_t pixelBytes = (size_t)w * (size_t)h * 4;
    std::vector<unsigned char> pixels(data, data + pixelBytes);
    stbi_image_free(data);

    // 2) Þifreyi 16 byte'a sabitle (kýsa ise 0 ile doluyor, uzun ise kesiliyor)
    const int passLen = 16;
    char pass[passLen] = { 0 };
    size_t copyLen = sifre.size();
    if (copyLen > (size_t)passLen) copyLen = (size_t)passLen;
    std::memcpy(pass, sifre.data(), copyLen);

    // 3) Piksel buffer'a LSB ile göm
    std::vector<unsigned char> stegoPixels = lsb_pixels(std::move(pixels), pass);

    // 4) Çýkýþ: PNG yaz (LSB korunur)
    std::string outPathPng = "encrypted.png";
    int stride = w * 4;
    if (!stbi_write_png(outPathPng.c_str(), w, h, 4, stegoPixels.data(), stride)) {
        std::cerr << "HATA: PNG yazilamadi: " << outPathPng << "\n";
        return;
    }

    std::cout << "OK: Sifre gomuldu -> " << outPathPng << "\n";
}

void fotograftanSifreyiCikar(std::string filePath)
{
    // 1) Resmi decode et (RGBA)
    int w = 0, h = 0, comp = 0;
    unsigned char* data = stbi_load(filePath.c_str(), &w, &h, &comp, 4); // 4 = RGBA
    if (!data) {
        std::cerr << "HATA: Resim okunamadi: " << filePath << "\n";
        std::cerr << "stb: " << stbi_failure_reason() << "\n";
        return;
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
        return;
    }

    // 3) Çýkan 16 byte'ý string'e çevir (0 byte'da kes)
    std::string sifre;
    sifre.reserve(passLen);
    for (int i = 0; i < passLen; i++) {
        if (out[i] == '\0') break;
        sifre.push_back((char)out[i]);
    }

    std::cout << "Cikarilan sifre: " << sifre << "\n";
}