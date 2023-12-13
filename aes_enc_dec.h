/*
--   ----------------------------------------------------------------------
--   Project:          Custom dll for piotec  
--
--   Author:           Hamza Qureshi
--
--   File name:        aes_enc_dec_class.h
--
--   Rev.:             1.0
--   Creation date:    NOV 2023
--
--   ---------------------------------------------------------------------
Purpose of this module:


Comments:

WARININGS

---------------------------------------------------------------------------
*/
// RDU_1_0Dlg.cpp : implementation file
//


#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <sstream>

// for installation use below
// vcpkg.exe install openssl:x86-windows


/**
 * @brief An instance of AESEncDec is tied to a key, with which
 * files can be encrypted or decrypted as desired.
 */

class AESEncDec 

{

	static const int KEYLEN = 16;
	static const int BLOCKSIZE = 512;
	static const int ITER_COUNT = 1;
	unsigned char key[KEYLEN];
	unsigned char iv[BLOCKSIZE];
public:
	AESEncDec(unsigned char* keybase);

	int encrypt_file(const char* path,
		const char* out);

	int decrypt_file(const char* path,
		const char* out);


	//modifications made by Hamza Qureshi
	//int encrypt_string(const char* path,
	//	const char* out);


	std::string encrypt_string(const std::string& plaintext);
	std::string decrypt_string(const std::string& encrypted_text);

	// protected:
	//	void set_AES_key(unsigned char* keybase);
	//	const unsigned char * get_AES_key();
	//
	//private:
	//unsigned char* key = (unsigned char*)"00000000000000000000000000000000";  // Key to be used in enc/dec
};


