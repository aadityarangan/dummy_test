#ifndef _AES_H_
#define _AES_H_

#define AES_BLOCK_SIZE 16U

void Get_Random_Iv(unsigned char *iv_buffer, unsigned short rand_seed);
void AES_CBC_decrypt (unsigned char *buffer, unsigned short size, unsigned char *Key, unsigned char *iv);
void AES_CBC_encrypt (unsigned char *buffer, unsigned short size, unsigned char *Key, unsigned char *iv);

#endif 