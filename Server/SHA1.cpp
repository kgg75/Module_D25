#include "StdAfx.h"

#include "SHA1.h"


// ������� ��������� SHA1PwdArray
SHA1PwdArray::SHA1PwdArray() {
    memset(data, 0, SHA1_HASH_LENGTH_BYTES);
}


SHA1PwdArray::SHA1PwdArray(const SHA1PwdArray& otherSHA1PwdArray) {
    memcpy(data, otherSHA1PwdArray.data, SHA1_HASH_LENGTH_BYTES);	// �������� ��� ������
}


SHA1PwdArray& SHA1PwdArray::operator = (const SHA1PwdArray& otherSHA1PwdArray) {
    memcpy(data, otherSHA1PwdArray.data, SHA1_HASH_LENGTH_BYTES);	// �������� ��� ������
    return *this;
}


bool SHA1PwdArray::operator == (const SHA1PwdArray& otherSHA1PwdArray) const {
    return memcmp(data, otherSHA1PwdArray.data, SHA1_HASH_LENGTH_BYTES) == 0;
}


bool SHA1PwdArray::operator != (const SHA1PwdArray& otherSHA1PwdArray) const {
    return memcmp(data, otherSHA1PwdArray.data, SHA1_HASH_LENGTH_BYTES) != 0;
}


uint& SHA1PwdArray::operator[](const uint index) {
    return data[index];
}
// ����� ����� ������� ��������� SHA1PwdArray


// ����������� ����� �����
uint cycle_shift_left(uint val, uint bit_count) {
    return (val << bit_count | val >> (32 - bit_count));
}


// ����������� ������������� ����� � ������ � ������������ ���
uint bring_to_human_view(uint val) {
    return  ((val & 0x000000FF) << 24) |
        ((val & 0x0000FF00) << 8) |
        ((val & 0x00FF0000) >> 8) |
        ((val & 0xFF000000) >> 24);
}


void CalculateSHA1(SHA1PwdArray& shaPwdArray, const char* message/*, size_t msize_bytes*/) {
    //�������������
    uint A = H[0];
    uint B = H[1];
    uint C = H[2];
    uint D = H[3];
    uint E = H[4];

    // ������� ������ ����� ������
    size_t msize_bytes = strlen(message);
    size_t totalBlockCount = msize_bytes / one_block_size_bytes;

    // �������, ������� ���� �����, ����� ��������� ��������� ����
    size_t needAdditionalBytes = one_block_size_bytes - (msize_bytes - totalBlockCount * one_block_size_bytes);

    if (needAdditionalBytes < 8) {
        totalBlockCount += 2;
        needAdditionalBytes += one_block_size_bytes;
    }
    else {
        totalBlockCount += 1;
    }

    // ������ ������������ �� ���� �������� ���������
    size_t extendedMessageSize = msize_bytes + needAdditionalBytes;

    // �������� ����� ����� � �������� � ���� ��������
    uchar* newMessage = new uchar[extendedMessageSize];
    memcpy(newMessage, message, msize_bytes);

    // ������ ��� ������ '1', ��������� ��������
    newMessage[msize_bytes] = 0x80;
    memset(newMessage + msize_bytes + 1, 0, needAdditionalBytes - 1);

    // ������ ����� ��������� ��������� � �����
    size_t* ptr_to_size = (size_t*)(newMessage + extendedMessageSize - 4);
    *ptr_to_size = bring_to_human_view(msize_bytes * 8);

    ExpendBlock exp_block;
    //������ �������
    for (int i = 0; i < totalBlockCount; i++) {
        // ����� ������� ���� � ��������� ���
        uchar* cur_p = newMessage + one_block_size_bytes * i;
        Block block = (Block)cur_p;

        // ������ 16 4�������� �����
        for (int j = 0; j < one_block_size_uints; j++) {
            exp_block[j] = bring_to_human_view(block[j]);
        }
        // ��������� 64...
        for (int j = one_block_size_uints; j < block_expend_size_uints; j++) {
            exp_block[j] =
                exp_block[j - 3] ^
                exp_block[j - 8] ^
                exp_block[j - 14] ^
                exp_block[j - 16];
            exp_block[j] = cycle_shift_left(exp_block[j], 1);
        }

        // ������������� 
        uint a = H[0];
        uint b = H[1];
        uint c = H[2];
        uint d = H[3];
        uint e = H[4];

        // �������������
        for (int j = 0; j < block_expend_size_uints; j++) {
            uint f;
            uint k;
            // � ����������� �� ������ ������� ��-�������
            if (j < 20) {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            }
            else if (j < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            }
            else if (j < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            }
            else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }

            // �������������
            uint temp = cycle_shift_left(a, 5) + f + e + k + exp_block[j];
            e = d;
            d = c;
            c = cycle_shift_left(b, 30);
            b = a;
            a = temp;
        }

        // �������������
        shaPwdArray[0] = A + a;
        shaPwdArray[1] = B + b;
        shaPwdArray[2] = C + c;
        shaPwdArray[3] = D + d;
        shaPwdArray[4] = E + e;
    }
}