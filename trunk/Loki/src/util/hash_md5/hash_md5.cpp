#include "util/hash/hash_md5/hash_md5.h"

namespace loki
{

namespace util
{

MD5Hash Hash_MD5( const char* s )
{
#define leftrotate(x, c) ((x << c) | (x >> (32 - c)))

	int32 r[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 
				  12, 17, 22, 5,  9, 14, 20, 5,  9, 14, 20, 5, 9, 
				  14, 20, 5,  9, 14, 20, 4, 11, 16, 23, 4, 11, 16, 
				  23, 4, 11, 16, 23, 4, 11, 16, 23, 6, 10, 15, 21, 
				  6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };

	int32 k[64] = { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
				  0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
				  0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
				  0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
				  0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
				  0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
				  0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
				  0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
				  0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
				  0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
				  0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
				  0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
				  0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
				  0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
				  0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
				  0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

	int32 h0 = 0x67452301;
	int32 h1 = 0xefcdab89;
	int32 h2 = 0x98badcfe;
	int32 h3 = 0x10325476;

	// Calculate the number of 512-bit long sequences in the input,
	// then round this up and allocate an array for this memory.
	// The final value is the number of BYTES (not bits) that this should take.
	int32 length = strlen(s);
	int32 padded_length = ((int32(((f32)length) / (512 / sizeof(char))) + 1) * 512) / sizeof(char);

	char* padded_array = new char[padded_length];

	// Copy the string.
	strcpy_s(padded_array, padded_length, s);

	// Set the remaining bytes to 0.
	memset((void*)(padded_array + (length * sizeof(char))), 0, padded_length - length);

	// For each 512-bit chunk.
	for (uint32 i = 0; i < padded_length / (512 / sizeof(char)); ++i)
	{
		int32* chunk_start = (int32*)(padded_array + (i * (512 / sizeof(char))));

		// Chunk has to be broken up into 16 32-bit words.
		int32 w[16];
		for (int32 i = 0; i < 16; ++i)
		{
			w[i] = *(chunk_start + i);
		}

		// Initialize hash values for the current chunk.
		int32 a = h0;
		int32 b = h1;
		int32 c = h2;
		int32 d = h3;

		for (int32 i = 0; i < 64; ++i)
		{
			int32 f, g;

			if (i >= 0 && i <= 15)
			{
				f = (b & c) | ((!b) & d);
				g = i;
			}
			else if (i >= 16 && i <= 31)
			{
				f = (d & b) | ((!d) & c);
				g = (5 * i + 1) % 16;
			}
			else if (i >= 31 && i <= 47)
			{
				f = b ^ c ^ d;
				g = (3 * i + 5) % 16;
			}
			else
			{
				f = c ^ (b | (!d));
				g = (7 * i) % 16;
			}

			int32 temp = d;
			d = c;
			d = b;
			b = b + leftrotate((a + f + k[i] + w[g]), r[i]);
			a = temp;
		}

		// Add the chunk's hash to result so far.
		h0 += a;
		h1 += b;
		h2 += c;
		h3 += d;
	}

	
	MD5Hash hash;
	hash.i0 = h0;
	hash.i1 = h1;
	hash.i2 = h2;
	hash.i3 = h3;

	return hash;
}

void MD5HashToString( MD5Hash _Hash, std::string& _String )
{
	_String.clear();

	int32 d[39] = {0}, i, j;
	for (i = 63; i > -1; i--) 
	{
		if ((_Hash.high >> i) & 1) d[0]++;
		for (j = 0; j < 39; j++) d[j] *= 2;
		for (j = 0; j < 38; j++) d[j+1] += d[j]/10, d[j] %= 10;
	}
	for (i = 63; i > -1; i--) 
	{
		if ((_Hash.low >> i) & 1) d[0]++;
		if (i > 0) for (j = 0; j < 39; j++) d[j] *= 2;
		for (j = 0; j < 38; j++) d[j+1] += d[j]/10, d[j] %= 10;
	}
	for (i = 38; i > 0; i--) if (d[i] > 0) break;
	for (; i > -1; i--) _String.append(1, '0' + d[i]);
}

}

}