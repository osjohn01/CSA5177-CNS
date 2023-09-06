#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 8
#define KEY_SIZE 32

void blowfish_encrypt(unsigned char *plaintext, unsigned char *key, unsigned char *ciphertext) {
  // Initialize the round keys.
  unsigned char round_keys[16][4];
  blowfish_generate_round_keys(key, round_keys);

  // Encrypt the plaintext.
  blowfish_encrypt_block(plaintext, round_keys, ciphertext);
}

void blowfish_decrypt(unsigned char *ciphertext, unsigned char *key, unsigned char *plaintext) {
  // Initialize the round keys.
  unsigned char round_keys[16][4];
  blowfish_generate_round_keys(key, round_keys);

  // Decrypt the ciphertext.
  blowfish_decrypt_block(ciphertext, round_keys, plaintext);
}

void blowfish_generate_round_keys(unsigned char *key, unsigned char *round_keys) {
  // Convert the key to binary.
  unsigned char binary_key[KEY_SIZE * 8];
  for (int i = 0; i < KEY_SIZE; i++) {
    binary_key[i * 8] = (key[i] >> 7) & 1;
    binary_key[i * 8 + 1] = (key[i] >> 6) & 1;
    binary_key[i * 8 + 2] = (key[i] >> 5) & 1;
    binary_key[i * 8 + 3] = (key[i] >> 4) & 1;
    binary_key[i * 8 + 4] = (key[i] >> 3) & 1;
    binary_key[i * 8 + 5] = (key[i] >> 2) & 1;
    binary_key[i * 8 + 6] = (key[i] >> 1) & 1;
    binary_key[i * 8 + 7] = key[i] & 1;
  }

  // Initialize the round keys.
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 4; j++) {
      round_keys[i][j] = binary_key[PBOX[i * 4 + j]];
    }
  }
}

void blowfish_encrypt_block(unsigned char *plaintext, unsigned char *round_keys, unsigned char *ciphertext) {
  // Initialize the registers.
  unsigned int L = plaintext[0] | (plaintext[1] << 8) | (plaintext[2] << 16) | (plaintext[3] << 24);
  unsigned int R = plaintext[4] | (plaintext[5] << 8) | (plaintext[6] << 16) | (plaintext[7] << 24);

  // Iterate through the rounds.
  for (int i = 0; i < 16; i++) {
    // F function.
    unsigned int temp = L ^ round_keys[i][0];
    temp = (temp + SBOX[0][temp >> 24]) ^ round_keys[i][1];
    temp = (temp + SBOX[1][temp >> 16]) ^ round_keys[i][2];
    temp = (temp + SBOX[2][temp >> 8]) ^ round_keys[i][3];
    temp = (temp + SBOX[3][temp]) ^ round_keys[i][0];

    // Swap L and R.
    L = R;
    R = temp;
  }

  // XOR the output.
  ciphertext[0] = R & 0xff;
  ciphertext[1] = (R >> 8) & 0xff;
  ciphertext[2] = (R >> 16) & 0xff;
  ciphertext[3] = (R >> 24) & 0xff;
  ciphertext[4] = L & 0xff;
  ciphertext[5] = (L >> 8) & 0xff;
  ciphertext[
