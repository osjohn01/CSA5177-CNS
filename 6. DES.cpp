#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 64
#define KEY_SIZE 8

void des_encrypt(unsigned char *plaintext, unsigned char *key, unsigned char *ciphertext) {
  // Initialize the round keys.
  unsigned char round_keys[16][8];
  des_generate_round_keys(key, round_keys);

  // Encrypt the plaintext.
  des_encrypt_block(plaintext, round_keys, ciphertext);
}

void des_decrypt(unsigned char *ciphertext, unsigned char *key, unsigned char *plaintext) {
  // Initialize the round keys.
  unsigned char round_keys[16][8];
  des_generate_round_keys(key, round_keys);

  // Decrypt the ciphertext.
  des_decrypt_block(ciphertext, round_keys, plaintext);
}

void des_generate_round_keys(unsigned char *key, unsigned char *round_keys) {
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

  // Generate the round keys.
  for (int i = 0; i < 16; i++) {
    // Apply the initial permutation to the binary key.
    unsigned char permuted_key[8];
    for (int j = 0; j < 8; j++) {
      permuted_key[j] = binary_key[IP[j]];
    }

    // Encrypt the permuted key using the Feistel function.
    des_feistel_function(permuted_key, round_keys[i]);
  }
}

void des_feistel_function(unsigned char *block, unsigned char *key) {
  // Expand the block to 48 bits.
  unsigned char expanded_block[48];
  for (int i = 0; i < 48; i++) {
    expanded_block[i] = block[ETABLE[i]];
  }

  // XOR the expanded block with the key.
  for (int i = 0; i < 48; i++) {
    expanded_block[i] ^= key[i];
  }

  // Apply the S-boxes.
  unsigned char output[32];
  for (int i = 0; i < 8; i++) {
    int row = expanded_block[i * 6] * 2 + expanded_block[i * 6 + 5];
    int column = expanded_block[i * 6 + 1] * 8 + expanded_block[i * 6 + 2] * 4 + expanded_block[i * 6 + 3] * 2 + expanded_block[i * 6 + 4];
    output[i] = SBOX[i][row * 16 + column];
  }

  // Permute the output.
  for (int i = 0; i < 32; i++) {
    block[i] = output[PTABLE[i]];
  }
}

void des_encrypt_block(unsigned char *plaintext, unsigned char *round_keys, unsigned char *ciphertext) {
