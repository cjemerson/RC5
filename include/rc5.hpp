// rc5.hpp
// Author: Charles Emerson
//
// Created: 17 November 2020
// Updated: 7 December 2020
//
// RC5 implementation based on:
//   https://people.csail.mit.edu/rivest/Rivest-rc5rev.pdf 
//
// RC5 header for RC5 Project for CS 665 Fall 2020.


#ifndef FILE_RC5_HPP_INCLUDED
#define FILE_RC5_HPP_INCLUDED

#include <string>
#include <cstdint>
#include <vector>
#include <algorithm>


/*** HELPER FUNCTION DECLARATIONS ***/

template <class WORD>
void rc5_encrypt_worker(const std::vector<WORD> & EXPANDED_KEY_TABLE, std::size_t NUM_ROUNDS, const std::vector<WORD> & plain_text, std::vector<WORD> & cipher_text);

template <class WORD>
void rc5_decrypt_worker(const std::vector<WORD> & EXPANDED_KEY_TABLE, std::size_t NUM_ROUNDS, const std::vector<WORD> & cipher_text, std::vector<WORD> & plain_text);

template <class WORD>
std::vector<WORD> rc5_setup(std::size_t num_rounds, const std::string & secret_key);


/*** PUBLIC INTERFACE ***/

template <class WORD>
void rc5_decrypt(std::size_t num_rounds, const std::string & secret_key, const std::vector<WORD> & cipher_text, std::vector<WORD> & plain_text)
{
    const auto EXPANDED_KEY_TABLE = rc5_setup<WORD>(num_rounds, secret_key);
    rc5_decrypt_worker<WORD>(EXPANDED_KEY_TABLE, num_rounds, cipher_text, plain_text);
}


template <class WORD>
void rc5_encrypt(std::size_t num_rounds, const std::string & secret_key, const std::vector<WORD> & plain_text, std::vector<WORD> & cipher_text)
{
    const auto EXPANDED_KEY_TABLE = rc5_setup<WORD>(num_rounds, secret_key);
    rc5_encrypt_worker<WORD>(EXPANDED_KEY_TABLE, num_rounds, plain_text, cipher_text);    
}


/*** HELPER FUNCTION DEFINITIONS ***/

/* Rotation operators. x must be unsigned, to get logical right shift */
template <class WORD>
WORD ROTL(WORD x, WORD y)
{
    WORD WORD_LEN_BITS = sizeof(WORD)*8;
    return (WORD) (((x)<<(y&(WORD_LEN_BITS-1))) | ((x)>>(WORD_LEN_BITS-(y&(WORD_LEN_BITS-1)))));
}

template <class WORD>
WORD ROTR(WORD x, WORD y)
{
    WORD WORD_LEN_BITS = sizeof(WORD)*8;
    return (WORD) (((x)>>(y&(WORD_LEN_BITS-1))) | ((x)<<(WORD_LEN_BITS-(y&(WORD_LEN_BITS-1)))));
}


template<class WORD>
WORD magicP()
{
    const std::size_t WORD_LEN_BITS = sizeof(WORD)*8;

    switch (WORD_LEN_BITS)
    {
        case 16:
            return (WORD) 0xB7E1;
        case 32:
            return (WORD) 0xB7E15163;
        case 64:
            return (WORD) 0xB7E151628AED2A6B;
        default:
            return 0;
    }
}


template<class WORD>
WORD magicQ()
{
    const std::size_t WORD_LEN_BITS =  sizeof(WORD)*8;

    switch (WORD_LEN_BITS)
    {
        case 16:
            return (WORD) 0x9E37;
        case 32:
            return (WORD) 0x9E3779B9;
        case 64:
            return (WORD) 0x9E3779B97F4A7C15;
        default:
            return 0;
    }
}


template <class WORD>
void rc5_encrypt_worker(const std::vector<WORD> & EXPANDED_KEY_TABLE, std::size_t NUM_ROUNDS, const std::vector<WORD> & plain_text, std::vector<WORD> & cipher_text)
{
    WORD cbc_a = 0, cbc_b = 0;
    for (std::size_t j = 0; j < cipher_text.size(); j += 2)
    {
        WORD A = (WORD) ((cbc_a ^ plain_text[j]) + EXPANDED_KEY_TABLE[0]);
        WORD B = (WORD) ((cbc_b ^ plain_text[j+1]) + EXPANDED_KEY_TABLE[1]);

        for (WORD i = 1; i <= NUM_ROUNDS; i++)
        {
            A = (WORD) (ROTL<WORD>(A ^ B, B) + EXPANDED_KEY_TABLE[2 * i]);
            B = (WORD) (ROTL<WORD>(B ^ A, A) + EXPANDED_KEY_TABLE[2 * i + 1]);
        }

        cbc_a = cipher_text[j] = A;
        cbc_b = cipher_text[j+1] = B;
    }
}


template <class WORD>
void rc5_decrypt_worker(const std::vector<WORD> & EXPANDED_KEY_TABLE, std::size_t NUM_ROUNDS, const std::vector<WORD> & cipher_text, std::vector<WORD> & plain_text)
{
    WORD cbc_a = 0, cbc_b = 0;
    for (std::size_t j = 0; j < cipher_text.size(); j += 2)
    {
        WORD B = cipher_text[j+1];
        WORD A = cipher_text[j];


        for (WORD i = (WORD) NUM_ROUNDS; i > 0; i--)
        {
            B = ROTR<WORD>((WORD) (B - EXPANDED_KEY_TABLE[2 * i + 1]), A) ^ A;
            A = ROTR<WORD>((WORD) (A - EXPANDED_KEY_TABLE[2 * i]), B) ^ B;
        }

        plain_text[j+1] = cbc_b ^ (WORD) (B - EXPANDED_KEY_TABLE[1]);
        plain_text[j] = cbc_a ^ (WORD) (A - EXPANDED_KEY_TABLE[0]);

        cbc_b = cipher_text[j+1];
        cbc_a = cipher_text[j];
    }
}


template <class WORD>
std::vector<WORD> rc5_setup(std::size_t NUM_ROUNDS, const std::string & secret_key)
{
    const std::size_t TABLE_SIZE_WORDS = 2 * (NUM_ROUNDS + 1);
    const std::size_t WORD_LEN_BYTES = sizeof(WORD);
    const std::size_t KEY_LEN_BYTES = (std::size_t) secret_key.length();
    const std::size_t KEY_LEN_WORDS = KEY_LEN_BYTES / WORD_LEN_BYTES;

    std::vector<WORD> EXPANDED_KEY_TABLE(TABLE_SIZE_WORDS);

    WORD i, j, k;
    WORD A, B;
    std::vector<WORD> L(KEY_LEN_WORDS); /* temp EXPANDED_KEY_TABLE */

    /* Initialize L, then S, then mix key into S */
    for (i = (WORD) (KEY_LEN_BYTES - 1), L[KEY_LEN_WORDS - 1] = 0; i != (WORD) -1; i--)
    {
        L[i / WORD_LEN_BYTES] = (WORD) ((L[i / WORD_LEN_BYTES] << 8) + secret_key[i]);
    }

    for (EXPANDED_KEY_TABLE[0] = magicP<WORD>(), i = 1; i < TABLE_SIZE_WORDS; i++)
    {
        EXPANDED_KEY_TABLE[i] = (WORD) (EXPANDED_KEY_TABLE[i - 1] + magicQ<WORD>());
    }

    for (A = B = i = j = k = 0; k < 3 * TABLE_SIZE_WORDS; k++, i = (WORD) (i+1) % ((WORD) TABLE_SIZE_WORDS), j = ((WORD) (j+1) % ((WORD) KEY_LEN_WORDS)) )
    {
        A = EXPANDED_KEY_TABLE[i] = ROTL<WORD>((WORD) (EXPANDED_KEY_TABLE[i] + (A+B)), 3);
        B = L[j] = ROTL<WORD>((WORD) (L[j] + (WORD)(A+B)), (WORD)(A+B));
    }

    return EXPANDED_KEY_TABLE;
}


#endif // #ifndef FILE_RC5_HPP_INCLUDED
