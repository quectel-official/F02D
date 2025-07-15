/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
#ifndef QL_AES_H
#define  QL_AES_H


extern INT32  ql_aes_string_encryption( char *szin, INT32 inlen,  UINT8 *key,  UINT8 *ciphertext);

extern unsigned int ql_aes_encrypt(void *inbuf, size_t inlen,void **outbuf,size_t *outlen);

extern unsigned int ql_aes_decrypt(void *inbuf, size_t inlen,void **outbuf,size_t *outlen);

#endif


