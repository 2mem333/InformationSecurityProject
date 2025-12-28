#pragma once
// Real DES (ECB), PKCS#7 padding, Base64
#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstring>

namespace simple_des {

    // ========================= Base64 =========================
    static const char* kB64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    inline std::string Base64EncodeBytes(const uint8_t* data, size_t len) {
        std::string out;
        out.reserve(((len + 2) / 3) * 4);

        size_t i = 0;
        while (i + 3 <= len) {
            uint32_t v = (uint32_t(data[i]) << 16) | (uint32_t(data[i + 1]) << 8) | uint32_t(data[i + 2]);
            out.push_back(kB64[(v >> 18) & 63]);
            out.push_back(kB64[(v >> 12) & 63]);
            out.push_back(kB64[(v >> 6) & 63]);
            out.push_back(kB64[v & 63]);
            i += 3;
        }

        const size_t rem = len - i;
        if (rem == 1) {
            uint32_t v = (uint32_t(data[i]) << 16);
            out.push_back(kB64[(v >> 18) & 63]);
            out.push_back(kB64[(v >> 12) & 63]);
            out.push_back('=');
            out.push_back('=');
        }
        else if (rem == 2) {
            uint32_t v = (uint32_t(data[i]) << 16) | (uint32_t(data[i + 1]) << 8);
            out.push_back(kB64[(v >> 18) & 63]);
            out.push_back(kB64[(v >> 12) & 63]);
            out.push_back(kB64[(v >> 6) & 63]);
            out.push_back('=');
        }
        return out;
    }

    inline int Base64Val(char c) {
        if (c >= 'A' && c <= 'Z') return c - 'A';
        if (c >= 'a' && c <= 'z') return c - 'a' + 26;
        if (c >= '0' && c <= '9') return c - '0' + 52;
        if (c == '+') return 62;
        if (c == '/') return 63;
        if (c == '=') return -2;
        return -1;
    }

    inline std::vector<uint8_t> Base64DecodeToBytes(const std::string& s) {
        std::vector<uint8_t> out;
        out.reserve((s.size() / 4) * 3);

        int q[4];
        int qi = 0;

        for (char c : s) {
            if (c == '\r' || c == '\n' || c == ' ' || c == '\t') continue;

            int v = Base64Val(c);
            if (v == -1) throw std::runtime_error("Base64Decode: invalid char");
            q[qi++] = v;

            if (qi == 4) {
                if (q[0] < 0 || q[1] < 0) throw std::runtime_error("Base64Decode: invalid padding");

                uint32_t x = (uint32_t(q[0]) << 18) | (uint32_t(q[1]) << 12);

                if (q[2] == -2 && q[3] == -2) {
                    out.push_back(uint8_t((x >> 16) & 0xFF));
                }
                else if (q[2] >= 0 && q[3] == -2) {
                    x |= (uint32_t(q[2]) << 6);
                    out.push_back(uint8_t((x >> 16) & 0xFF));
                    out.push_back(uint8_t((x >> 8) & 0xFF));
                }
                else if (q[2] >= 0 && q[3] >= 0) {
                    x |= (uint32_t(q[2]) << 6) | uint32_t(q[3]);
                    out.push_back(uint8_t((x >> 16) & 0xFF));
                    out.push_back(uint8_t((x >> 8) & 0xFF));
                    out.push_back(uint8_t(x & 0xFF));
                }
                else {
                    throw std::runtime_error("Base64Decode: invalid padding");
                }
                qi = 0;
            }
        }
        if (qi != 0) throw std::runtime_error("Base64Decode: truncated");
        return out;
    }

    // ========================= DES Tables =========================
    static const int IP[64] = {
      58,50,42,34,26,18,10, 2,
      60,52,44,36,28,20,12, 4,
      62,54,46,38,30,22,14, 6,
      64,56,48,40,32,24,16, 8,
      57,49,41,33,25,17, 9, 1,
      59,51,43,35,27,19,11, 3,
      61,53,45,37,29,21,13, 5,
      63,55,47,39,31,23,15, 7
    };

    static const int FP[64] = {
      40, 8,48,16,56,24,64,32,
      39, 7,47,15,55,23,63,31,
      38, 6,46,14,54,22,62,30,
      37, 5,45,13,53,21,61,29,
      36, 4,44,12,52,20,60,28,
      35, 3,43,11,51,19,59,27,
      34, 2,42,10,50,18,58,26,
      33, 1,41, 9,49,17,57,25
    };

    static const int E[48] = {
      32, 1, 2, 3, 4, 5,
       4, 5, 6, 7, 8, 9,
       8, 9,10,11,12,13,
      12,13,14,15,16,17,
      16,17,18,19,20,21,
      20,21,22,23,24,25,
      24,25,26,27,28,29,
      28,29,30,31,32, 1
    };

    static const int P[32] = {
      16, 7,20,21,
      29,12,28,17,
       1,15,23,26,
       5,18,31,10,
       2, 8,24,14,
      32,27, 3, 9,
      19,13,30, 6,
      22,11, 4,25
    };

    static const int PC1[56] = {
      57,49,41,33,25,17, 9,
       1,58,50,42,34,26,18,
      10, 2,59,51,43,35,27,
      19,11, 3,60,52,44,36,
      63,55,47,39,31,23,15,
       7,62,54,46,38,30,22,
      14, 6,61,53,45,37,29,
      21,13, 5,28,20,12, 4
    };

    static const int PC2[48] = {
      14,17,11,24, 1, 5,
       3,28,15, 6,21,10,
      23,19,12, 4,26, 8,
      16, 7,27,20,13, 2,
      41,52,31,37,47,55,
      30,40,51,45,33,48,
      44,49,39,56,34,53,
      46,42,50,36,29,32
    };

    static const int SHIFTS[16] = {
      1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
    };

    static const int SBOX[8][64] = {
    {
      14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
      0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
      4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
      15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
    },
    {
      15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
      3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
      0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
      13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
    },
    {
      10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
      13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
      13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
      1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
    },
    {
      7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
      13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
      10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
      3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
    },
    {
      2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
      14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
      4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
      11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
    },
    {
      12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
      10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
      9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
      4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
    },
    {
      4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
      13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
      1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
      6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
    },
    {
      13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
      1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
      7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
      2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
    }
    };

    // ========================= DES Core Helpers =========================
    inline uint64_t ReadBE64(const uint8_t* p) {
        return (uint64_t(p[0]) << 56) | (uint64_t(p[1]) << 48) | (uint64_t(p[2]) << 40) | (uint64_t(p[3]) << 32) |
            (uint64_t(p[4]) << 24) | (uint64_t(p[5]) << 16) | (uint64_t(p[6]) << 8) | (uint64_t(p[7]) << 0);
    }
    inline void WriteBE64(uint8_t* p, uint64_t v) {
        p[0] = uint8_t(v >> 56); p[1] = uint8_t(v >> 48); p[2] = uint8_t(v >> 40); p[3] = uint8_t(v >> 32);
        p[4] = uint8_t(v >> 24); p[5] = uint8_t(v >> 16); p[6] = uint8_t(v >> 8); p[7] = uint8_t(v >> 0);
    }

    // Permute bits: table uses 1-based positions. inBits = 64 or 56 or 32.
    inline uint64_t Permute(uint64_t in, const int* table, int n, int inBits) {
        uint64_t out = 0;
        for (int i = 0; i < n; ++i) {
            out <<= 1;
            int srcPos = table[i];                 // 1..inBits
            uint64_t bit = (in >> (inBits - srcPos)) & 1ULL;
            out |= bit;
        }
        return out;
    }

    inline uint32_t RotL28(uint32_t v, int s) {
        v &= 0x0FFFFFFF;
        return ((v << s) | (v >> (28 - s))) & 0x0FFFFFFF;
    }

    inline void KeySchedule(const uint8_t key8[8], uint64_t subkeys[16]) {
        uint64_t k64 = ReadBE64(key8);
        uint64_t k56 = Permute(k64, PC1, 56, 64);       // 56-bit
        uint32_t C = (uint32_t)((k56 >> 28) & 0x0FFFFFFF);
        uint32_t D = (uint32_t)(k56 & 0x0FFFFFFF);

        for (int r = 0; r < 16; ++r) {
            C = RotL28(C, SHIFTS[r]);
            D = RotL28(D, SHIFTS[r]);
            uint64_t CD = (uint64_t(C) << 28) | uint64_t(D);
            subkeys[r] = Permute(CD, PC2, 48, 56);      // 48-bit
        }
    }

    inline uint32_t Feistel(uint32_t R, uint64_t K48) {
        // Expand 32->48
        uint64_t ER = Permute(uint64_t(R) << 32, E, 48, 64); // place R in top bits to use Permute with 64-bit input
        uint64_t x = ER ^ K48;

        // S-boxes: 8 groups of 6 bits -> 32 bits
        uint32_t out = 0;
        for (int i = 0; i < 8; ++i) {
            uint8_t six = (uint8_t)((x >> (42 - 6 * i)) & 0x3F); // take from MSB side
            int row = ((six & 0x20) >> 4) | (six & 0x01);        // b5 and b0
            int col = (six >> 1) & 0x0F;                         // b4..b1
            int sVal = SBOX[i][row * 16 + col];
            out = (out << 4) | (uint32_t)sVal;
        }

        // Permutation P
        uint32_t pOut = (uint32_t)Permute(uint64_t(out) << 32, P, 32, 64);
        return pOut;
    }

    inline uint64_t DES_Block(uint64_t block, const uint64_t subkeys[16], bool decrypt) {
        uint64_t ip = Permute(block, IP, 64, 64);
        uint32_t L = (uint32_t)(ip >> 32);
        uint32_t R = (uint32_t)(ip & 0xFFFFFFFF);

        for (int r = 0; r < 16; ++r) {
            uint64_t K = decrypt ? subkeys[15 - r] : subkeys[r];
            uint32_t f = Feistel(R, K);
            uint32_t newL = R;
            uint32_t newR = L ^ f;
            L = newL;
            R = newR;
        }

        // Swap L/R
        uint64_t preFP = (uint64_t(R) << 32) | uint64_t(L);
        uint64_t fp = Permute(preFP, FP, 64, 64);
        return fp;
    }

    // ========================= Padding =========================
    inline void PKCS7Pad(std::vector<uint8_t>& data, size_t blockSize) {
        size_t pad = blockSize - (data.size() % blockSize);
        if (pad == 0) pad = blockSize;
        for (size_t i = 0; i < pad; ++i) data.push_back((uint8_t)pad);
    }

    inline void PKCS7Unpad(std::vector<uint8_t>& data, size_t blockSize) {
        if (data.empty() || (data.size() % blockSize) != 0) throw std::runtime_error("Unpad: invalid length");
        uint8_t pad = data.back();
        if (pad == 0 || pad > blockSize) throw std::runtime_error("Unpad: invalid pad");
        // check all pad bytes
        for (size_t i = 0; i < pad; ++i) {
            if (data[data.size() - 1 - i] != pad) throw std::runtime_error("Unpad: invalid pad bytes");
        }
        data.resize(data.size() - pad);
    }

    // ========================= Public API =========================
    // key8 MUST be exactly 8 bytes (e.g. "12345678")
    inline std::string DES_EncryptBase64(const std::string& plain, const std::string& key8) {
        if (key8.size() != 8) throw std::runtime_error("DES key must be exactly 8 bytes");

        uint8_t k[8];
        std::memcpy(k, key8.data(), 8);

        uint64_t subkeys[16];
        KeySchedule(k, subkeys);

        std::vector<uint8_t> buf(plain.begin(), plain.end());
        PKCS7Pad(buf, 8);

        for (size_t i = 0; i < buf.size(); i += 8) {
            uint64_t b = ReadBE64(&buf[i]);
            uint64_t c = DES_Block(b, subkeys, false);
            WriteBE64(&buf[i], c);
        }

        return Base64EncodeBytes(buf.data(), buf.size());
    }

    inline std::string DES_DecryptBase64(const std::string& cipherB64, const std::string& key8) {
        if (key8.size() != 8) throw std::runtime_error("DES key must be exactly 8 bytes");

        std::vector<uint8_t> ct = Base64DecodeToBytes(cipherB64);
        if (ct.empty() || (ct.size() % 8) != 0) throw std::runtime_error("Ciphertext length invalid");

        uint8_t k[8];
        std::memcpy(k, key8.data(), 8);

        uint64_t subkeys[16];
        KeySchedule(k, subkeys);

        for (size_t i = 0; i < ct.size(); i += 8) {
            uint64_t b = ReadBE64(&ct[i]);
            uint64_t p = DES_Block(b, subkeys, true);
            WriteBE64(&ct[i], p);
        }

        PKCS7Unpad(ct, 8);
        return std::string(ct.begin(), ct.end());
    }

} // namespace simple_des
