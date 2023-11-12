#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <cassert>
#include <cstring>

//#include <algorithm>



#include <openssl/evp.h>
#include <openssl/rand.h>

using namespace std;

struct crypto_config
{
	const char * m_crypto_function;
    std::unique_ptr<uint8_t[]> m_key;
	std::unique_ptr<uint8_t[]> m_IV;
    size_t m_key_len;
	size_t m_IV_len;
};

#endif /* _PROGTEST_ */


///////////////////////////////////////////////
void generate_key_and_iv(crypto_config & config) {
    const EVP_CIPHER * cipher = EVP_get_cipherbyname(config.m_crypto_function);
    if (!cipher) {
        return;// false;
    }

    size_t key_len = EVP_CIPHER_key_length(cipher);
    size_t IV_len = EVP_CIPHER_iv_length(cipher);

    if (config.m_key == nullptr || config.m_key_len < key_len) {
        std::unique_ptr<uint8_t[]> key(new uint8_t[key_len]);
        if (RAND_bytes(key.get(), key_len) != 1) {
            return ;//false;
        }
        //cout << "LEN KEY " << key_len << endl;
        config.m_key = std::make_unique<uint8_t[]>(key_len);
        std::memcpy(config.m_key.get(), key.get(), key_len);
        config.m_key_len = key_len;

    }

    if (config.m_IV == nullptr || config.m_IV_len < IV_len) {
        if (IV_len == 0) {
            //std::cout << " not require an IV" << std::endl;
            return;// true;
        }
        std::unique_ptr<uint8_t[]> IV(new uint8_t[IV_len]);
        if (RAND_bytes(IV.get(), IV_len) != 1) {
            return;// false;
        }
        //cout << "!!!!!!!!!! IV " << endl;
        config.m_IV = std::make_unique<uint8_t[]>(IV_len);
        config.m_IV_len = IV_len;
        memcpy(config.m_IV.get(), IV.get(), IV_len);
    }

    //true;
}

bool encrypt_data ( const std::string & in_filename, const std::string & out_filename, crypto_config & config ) {

    std::ifstream in_file(in_filename, std::ios::binary);
    if (!in_file)
    {
       // std::cerr << "fail to open input file: " << in_filename << std::endl;
        return false;
    }

    std::ofstream out_file(out_filename, std::ios::binary);
    if (!out_file)
    {
        //std::cerr << "0 fail to open output file: " << out_filename << std::endl;
        return false;
    }

    char header[18];
    in_file.read(header, 18);
    if (!in_file)
    {
       // std::cerr << "Failed to read input file: " << in_filename << std::endl;
        return false;
    }
    out_file.write(header, 18);
    if (!out_file)
    {
       // std::cerr << "Failed to write output file: " << out_filename << std::endl;
        return false;
    }

    ////   end header

    const EVP_CIPHER * cipher;
    cipher = EVP_get_cipherbyname(config.m_crypto_function);

    if (!cipher) {
        //printf("Sifra %s neexistuje.\n", cipherName);
        return false;
    }

    uint8_t inbuf[1024], outbuf[1024 + EVP_MAX_BLOCK_LENGTH];
    int outlen ; //, tmplen;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if (!ctx)
        return false;


    //____________


    generate_key_and_iv(config);
    //cout << "KEYY len" << config.m_key_len << endl;


    if (!EVP_EncryptInit_ex(ctx, cipher, NULL, config.m_key.get(), config.m_IV.get())){
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    while (in_file.good())
    {
        in_file.read(reinterpret_cast<char*>(inbuf), sizeof(inbuf));
        if (in_file.gcount() > 0)
        {
            if (!EVP_EncryptUpdate(ctx, outbuf, &outlen, inbuf, in_file.gcount())){
                EVP_CIPHER_CTX_free(ctx);
                return false;
            }

            out_file.write(reinterpret_cast<char*>(outbuf), outlen);
            if (!out_file)
            {
               // std::cerr << "errr to write output file: " << out_filename << std::endl;
                EVP_CIPHER_CTX_free(ctx);
                return false;
            }
        }
    }
    if (!EVP_EncryptFinal_ex(ctx, outbuf, &outlen)){
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    EVP_CIPHER_CTX_free(ctx);

    if (outlen > 0)
    {
        out_file.write(reinterpret_cast<char*>(outbuf), outlen);
        if (!out_file)
        {
            //std::cerr << "errrrro to write output file: " << out_filename << std::endl;
            return false;
        }
    }

/*    std::ifstream out_string_file(out_filename, std::ios::binary);
    if (!out_string_file)
    {
       // std::cerr << "! errrorr to open output file: " << out_filename << std::endl;
        return false;
    }

    std::ostringstream out_string_stream;
    out_string_stream << out_string_file.rdbuf();
    std::string out_string = out_string_stream.str();  */

    //////

    ///

    return true;
}

bool decrypt_data ( const std::string & in_filename, const std::string & out_filename, crypto_config & config )
{
    // Delete contents of out_filename
    std::ofstream ofs(out_filename, std::ios::trunc);
    if (!ofs)
    {
       // std::cerr << "errr to clear output file: " << out_filename << std::endl;
        return false;
    }
    ofs.close();

    std::ifstream in_file(in_filename, std::ios::binary);
    if (!in_file)
    {
        //std::cerr << "errr to open input file: " << in_filename << std::endl;
        return false;
    }

    std::ofstream out_file(out_filename, std::ios::binary);
    if (!out_file)
    {
       // std::cerr << "errr to open output file: " << out_filename << std::endl;
        return false;
    }

    // Copy first 18 bytes to out_filename without decryption
    char header[18];
    in_file.read(header, 18);
    if (!in_file)
    {
       // std::cerr << "errr to read input file: " << in_filename << std::endl;
        return false;
    }
    out_file.write(header, 18);
    if (!out_file)
    {
       // std::cerr << "errr to write output file: " << out_filename << std::endl;
        return false;
    }

    //OpenSSL_add_all_ciphers();

    // Decrypt the rest of the input file and write to output file
    const EVP_CIPHER *cipher;
    cipher = EVP_get_cipherbyname(config.m_crypto_function);

    if (!cipher) {
        //printf("Sifra %s neexistuje.\n", cipherName);
        return false;
    }

    size_t key_len = EVP_CIPHER_key_length(cipher);
    size_t IV_len = EVP_CIPHER_iv_length(cipher);




    if (config.m_key == nullptr || config.m_key_len < key_len) {
        //cout << "LEEEENNNNN key " << config.m_IV_len << endl;
        return false;
    }

    if (config.m_IV == nullptr || config.m_IV_len < IV_len) {
        if (IV_len != 0) {
           // std::cout << "DECR: does not require an IV" << std::endl;
            return false;// true;
        }
        //return false;
    }

    //0
    uint8_t inbuf[1024], outbuf[1024 + EVP_MAX_BLOCK_LENGTH];
    int outlen ;//, tmplen;
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
        return false;


    if (!EVP_DecryptInit_ex(ctx, cipher, NULL, config.m_key.get(), config.m_IV.get())){
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    //
    while (in_file.good())
    {
        in_file.read(reinterpret_cast<char *>(inbuf), sizeof(inbuf));
        if (in_file.gcount() > 0)
        {
            if (!EVP_DecryptUpdate(ctx, outbuf, &outlen, inbuf, in_file.gcount())){
                EVP_CIPHER_CTX_free(ctx);
                return false;
            }
            //if (outlen == 0) continue;
            out_file.write(reinterpret_cast<char *>(outbuf), outlen);
            if (!out_file)
            {
                // std::cerr << "errr to write output file: " << out_filename << std::endl;
                EVP_CIPHER_CTX_free(ctx);
                return false;
            }
        }
    }

   if (!EVP_DecryptFinal_ex(ctx, outbuf, &outlen)){
       EVP_CIPHER_CTX_free(ctx);
       return false;
   }
    EVP_CIPHER_CTX_free(ctx);

    if (outlen > 0)
    {
        out_file.write(reinterpret_cast<char *>(outbuf), outlen);
        if (!out_file)
        {
            // std::cerr << "errr to write output file: " << out_filename << std::endl;
            return false;
        }
    }

    return true;
}



#ifndef __PROGTEST__

bool compare_files ( const char * name1, const char * name2 )
{
    std::ifstream file1(name1, std::ios::binary);
    std::ifstream file2(name2, std::ios::binary);
    char c1, c2;
    int cnt = 0;

    while (file1.get(c1) && file2.get(c2))
    {
        cnt++;
        if (c1 != c2) {
            cout << "CNT " << cnt << endl;
            return false;
        }
    }



    return true;
}



int main ( void )
{
	crypto_config config {nullptr, nullptr, nullptr, 0, 0};

	// ECB mode
    //config.m_crypto_function = "AES-128-CBC";

   // config.m_crypto_function = "AES-128-CBC";

    config.m_crypto_function = "AES-128-ECB";
	config.m_key = std::make_unique<uint8_t[]>(16);
 	memset(config.m_key.get(), 0, 16);
	config.m_key_len = 16;

    //in_5203779.bin


    assert( encrypt_data  ("homer-simpson.TGA", "ECB.TGA", config)  );//&&
            //compare_files ("out_file.TGA", "homer-simpson_enc_ecb.TGA") );


    assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config)  &&
            compare_files ("out_file.TGA", "homer-simpson_enc_ecb.TGA") );




    assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config)  &&
            compare_files ("out_file.TGA", "homer-simpson_enc_ecb.TGA") );

    assert( decrypt_data  ("homer-simpson_enc_ecb.TGA", "out_file.TGA", config)  &&
			compare_files ("out_file.TGA", "homer-simpson.TGA") );

       assert( encrypt_data  ("UCM8.TGA", "out_file.TGA", config) &&
               compare_files ("out_file.TGA", "UCM8_enc_ecb.TGA") );

       assert( decrypt_data  ("UCM8_enc_ecb.TGA", "out_file.TGA", config) &&
               compare_files ("out_file.TGA", "UCM8.TGA") );

       assert( encrypt_data  ("image_1.TGA", "out_file.TGA", config) &&
               compare_files ("out_file.TGA", "ref_1_enc_ecb.TGA") );

       assert( encrypt_data  ("image_2.TGA", "out_file.TGA", config) &&
               compare_files ("out_file.TGA", "ref_2_enc_ecb.TGA") );

       assert( decrypt_data ("image_3_enc_ecb.TGA", "out_file.TGA", config)  &&
               compare_files("out_file.TGA", "ref_3_dec_ecb.TGA") );

       assert( decrypt_data ("image_4_enc_ecb.TGA", "out_file.TGA", config)  &&
               compare_files("out_file.TGA", "ref_4_dec_ecb.TGA") );



       // CBC mode
       config.m_crypto_function = "AES-128-CBC";
       config.m_IV =  std::make_unique<uint8_t[]>(16);
       config.m_IV_len = 16;
       memset(config.m_IV.get(), 0, 16);
    assert( encrypt_data  ("homer-simpson.TGA", "CBC.TGA", config) );//&&
           // compare_files ("out_file.TGA", "homer-simpson_enc_cbc.TGA") );



       assert( encrypt_data  ("UCM8.TGA", "out_file.TGA", config) &&
               compare_files ("out_file.TGA", "UCM8_enc_cbc.TGA") );

       assert( decrypt_data  ("UCM8_enc_cbc.TGA", "out_file.TGA", config) &&
               compare_files ("out_file.TGA", "UCM8.TGA") );

       //assert (config.m_key_len == 0);


       assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config) &&
               compare_files ("out_file.TGA", "homer-simpson_enc_cbc.TGA") );

       assert( decrypt_data  ("homer-simpson_enc_cbc.TGA", "out_file.TGA", config) &&
               compare_files ("out_file.TGA", "homer-simpson.TGA") );

       assert( encrypt_data  ("image_1.TGA", "out_file.TGA", config) &&
               compare_files ("out_file.TGA", "ref_5_enc_cbc.TGA") );

       assert( encrypt_data  ("image_2.TGA", "out_file.TGA", config) &&
               compare_files ("out_file.TGA", "ref_6_enc_cbc.TGA") );

       assert( decrypt_data ("image_7_enc_cbc.TGA", "out_file.TGA", config)  &&
               compare_files("out_file.TGA", "ref_7_dec_cbc.TGA") );

       assert( decrypt_data ("image_8_enc_cbc.TGA", "out_file.TGA", config)  &&
               compare_files("out_file.TGA", "ref_8_dec_cbc.TGA") );



	return 0;
}

#endif /* _PROGTEST_ */
