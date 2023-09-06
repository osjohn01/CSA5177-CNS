#include <stdio.h>
#include <string.h>

void vigenere_cipher(char *plaintext, char *key, char *ciphertext) {
  int i, j;

  // Encrypt the plaintext using the key.
  for (i = 0; plaintext[i] != '\0'; i++) {
    ciphertext[i] = (plaintext[i] + key[i % strlen(key)]) % 26;
  }
}

int main() {
  char plaintext[] = "THIS IS A PLAINTEXT MESSAGE";
  char key[] = "KEYWORD";
  char ciphertext[100];

  // Encrypt the plaintext.
  vigenere_cipher(plaintext, key, ciphertext);

  // Print the ciphertext.
  printf("Ciphertext: %s\n", ciphertext);

  return 0;
}
