#include <openssl/evp.h>
#include <openssl/sha.h>
#include <string>
#include <iomanip>
#include <sstream>
#include "crypto.h"

HashGenerator& HashGenerator::get()
{
    static HashGenerator instance;
    return instance;
}

std::string HashGenerator::sha256(const std::string& password)
{
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    const EVP_MD* md = EVP_sha256();
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len;
    
    EVP_DigestInit_ex(mdctx, md, nullptr);
    EVP_DigestUpdate(mdctx, password.c_str(), password.size());
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_free(mdctx);

    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (unsigned int i = 0; i < md_len; ++i) {
        ss << std::setw(2) << static_cast<int>(md_value[i]);
    }

    return ss.str();
}
