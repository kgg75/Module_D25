#pragma once
#include "Constants.h"


#define one_block_size_bytes 64 // ���������� ���� � �����
#define one_block_size_uints 16 // ���������� 4��������  � �����
#define block_expend_size_uints 80 // ���������� 4�������� � ����������� �����

#define SHA1_HASH_LENGTH_BYTES 20
#define SHA1_HASH_LENGTH_UINTS 5

typedef uint* Block;
typedef uint ExpendBlock[block_expend_size_uints];

const uint H[SHA1_HASH_LENGTH_UINTS] = {
    0x67452301,
    0xEFCDAB89,
    0x98BADCFE,
    0x10325476,
    0xC3D2E1F0
}; // ���������, �������� ���������


struct SHA1PwdArray {
	private:
		uint data[SHA1_HASH_LENGTH_UINTS];	// ������ � ����� ������
	public:
		SHA1PwdArray();
		SHA1PwdArray(const SHA1PwdArray& otherSHA1PwdArray);
		SHA1PwdArray& operator = (const SHA1PwdArray& otherSHA1PwdArray);
		bool operator == (const SHA1PwdArray& otherSHA1PwdArray) const;
		bool operator != (const SHA1PwdArray& otherSHA1PwdArray) const;
		uint& operator[](const uint index);
};


uint cycle_shift_left(uint val, uint bit_count);
uint bring_to_human_view(uint val);
void CalculateSHA1(SHA1PwdArray& shaPwdArray, const char* message/*, size_t msize_bytes*/);

