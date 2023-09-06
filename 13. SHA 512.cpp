#include <stdio.h>
#include <string.h>

#define SHA512_BLOCK_SIZE 1024
#define SHA512_DIGEST_SIZE 64

void sha512_init(unsigned char *digest) {
  // Initialize the digest.
  for (int i = 0; i < SHA512_DIGEST_SIZE; i++) {
    digest[i] = 0;
  }
}

void sha512_update(unsigned char *digest, unsigned char *message, size_t message_len) {
  // Process the message in blocks of 1024 bytes.
  for (int i = 0; i < message_len; i += SHA512_BLOCK_SIZE) {
    // Update the digest.
    sha512_process_block(digest, message + i);
  }
}

void sha512_final(unsigned char *digest) {
  // Append the padding.
  unsigned char padding[SHA512_BLOCK_SIZE - 8] = {0x80};
  size_t padding_len = SHA512_BLOCK_SIZE - (digest[0] + digest[1] + digest[2] + digest[3]) % SHA512_BLOCK_SIZE;
  sha512_update(digest, padding, padding_len);

  // Append the length of the message.
  unsigned char message_len_bytes[8];
  memcpy(message_len_bytes, &message_len, 8);
  sha512_update(digest, message_len_bytes, 8);
}

void sha512_process_block(unsigned char *digest, unsigned char *message) {
  // Initialize the registers.
  unsigned long long a = digest[0];
  unsigned long long b = digest[1];
  unsigned long long c = digest[2];
  unsigned long long d = digest[3];
  unsigned long long e = digest[4];
  unsigned long long f = digest[5];
  unsigned long long g = digest[6];
  unsigned long long h = digest[7];

  // Iterate through the rounds.
  for (int i = 0; i < 80; i++) {
    // Sigma1(e)
    unsigned long long temp1 = (e >> 6) ^ (e >> 11) ^ (e >> 25);
    // Sigma0(a)
    unsigned long long temp2 = (a >> 2) ^ (a >> 13) ^ (a >> 22);
    // Ch(b, c, d)
    unsigned long long temp3 = (b & c) ^ (~b & d);
    // Maj(a, b, c)
    unsigned long long temp4 = (a & b) ^ (a & c) ^ (b & c);

    // Update the registers.
    h = g;
    g = f;
    f = e;
    e = (d + temp1 + temp3) % 0xFFFFFFFFFFFFFFFF;
    d = c;
    c = b;
    b = a;
    a = (temp2 + temp4 + message[i * 8 + 0] + K[i]) % 0xFFFFFFFFFFFFFFFF;
  }

  // Update the digest.
  digest[0] += a;
  digest[1] += b;
  digest[2] += c;
  digest[3] += d;
  digest[4] += e;
  digest[5] += f;
  digest[6] += g;
  digest[7] += h;
}

int main() {
  // Initialize the digest.
  unsigned char digest[SHA512_DIGEST_SIZE];
  sha512_init(digest);

  // Update the digest with the message.
  char message[] = "Hello, world!";
  size_t message_len = strlen(message);
  sha512_update(digest, message, message_len);

  // Finalize the digest.
  sha512_final(digest);

  // Print the digest.
  for (int i = 0; i < SHA
