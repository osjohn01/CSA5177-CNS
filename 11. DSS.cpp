#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRIME_NUMBER 11

void generate_keys(unsigned int *private_key, unsigned int *public_key) {
  // Generate a random private key.
  *private_key = rand() % PRIME_NUMBER;

  // Compute the public key.
  *public_key = pow(*private_key, 2) % PRIME_NUMBER;
}

unsigned int sign(unsigned int message, unsigned int private_key) {
  // Generate a random k.
  unsigned int k = rand() % PRIME_NUMBER;

  // Compute the signature.
  unsigned int r = (k * public_key) % PRIME_NUMBER;
  unsigned int s = (k * message + private_key * r) % PRIME_NUMBER;

  return (r << 16) | s;
}

int verify(unsigned int message, unsigned int signature, unsigned int public_key) {
  // Extract the r and s components of the signature.
  unsigned int r = signature >> 16;
  unsigned int s = signature & 0xffff;

  // Compute v.
  unsigned int v = (pow(s, -1) * r) % PRIME_NUMBER;

  // Verify that v is equal to the hash of the message.
  return v == (hash(message) % PRIME_NUMBER);
}

int main() {
  // Generate the keys.
  unsigned int private_key, public_key;
  generate_keys(&private_key, &public_key);

  // Sign a message.
  unsigned int message = 12345;
  unsigned int signature = sign(message, private_key);

  // Verify the signature.
  int is_valid = verify(message, signature, public_key);

  // Print the results.
  if (is_valid) {
    printf("The signature is valid.\n");
  } else {
    printf("The signature is invalid.\n");
  }

  return 0;
}
