// rc5_encrypt_main.cpp
// Author: Charles Emerson
//
// Created: 7 December 2020
// Updated: 7 December 2020
//
// RC5 Encrypt main program for RC5 Project for CS 665 Fall 2020.


#include <iostream>
#include <fstream>
#include <cstdint> // uint16_t, uint32_t, uint64_t
#include <vector>
#include <string> // std::to_string

#include "cmdparser.hpp"
#include "rc5.hpp"
#include "convert_vector.hpp"


// Helper functions
template<class WORD>
void call_rc5_encrypt(std::size_t num_rounds, const std::string & secret_key, std::vector<unsigned char> & buffer, const std::string & output_file_name);

void configure_parser(cli::Parser & parser);


int main(int argc, char const *argv[])
{
    cli::Parser parser(argc, argv);
    configure_parser(parser);
    parser.run_and_exit_if_error();

    auto input_file_name    = parser.get<std::string>("i");
    auto output_file_name   = parser.get<std::string>("o");
    auto word_bit_length    = parser.get<std::size_t>("w");
    auto num_rounds         = parser.get<std::size_t>("r");
    auto secret_key_length  = parser.get<std::size_t>("b");
    auto secret_key         = parser.get<std::string>("k");
    auto verbose            = parser.get<bool>("v");

    // Attempt to use input file name for all params
    if (output_file_name.empty())
    {
        auto pos = input_file_name.rfind("/");
        if (pos == std::string::npos)
        {
            output_file_name = input_file_name;
        }
        else
        {
            output_file_name = input_file_name.substr(pos + 1);
        }

        output_file_name += + ".rc5_" + std::to_string(word_bit_length) + "-" + std::to_string(num_rounds) + "-" + std::to_string(secret_key_length);
    }

    if (verbose)
    {
        std::cout << "Input file: \"" << input_file_name << "\"" << std::endl;
        std::cout << "Output file: \"" << output_file_name << "\"" << std::endl;
        std::cout << "Word bit length: " << word_bit_length << std::endl;
        std::cout << "Number of rounds: " << num_rounds << std::endl;
        std::cout << "Secret key length: " << secret_key_length << std::endl;
        std::cout << "Secret key: \"" << secret_key << "\"" << std::endl;
    }

    // More Error checking
    bool error = false;
    if (word_bit_length != 16 && word_bit_length != 32 && word_bit_length != 64)
    {
        std::cerr << "Invalid word bit length. Must be 16, 32, or 64." << std::endl;
        error = true;
    }
    if (num_rounds < 1 || num_rounds > 255)
    {
        std::cerr << "Invalid number of rounds. Must be between 1 and 255." << std::endl;
        error = true;
    }
    if (secret_key_length < 1 || secret_key_length > 255)
    {
        std::cerr << "Invalid secret key length. Must be between 1 and 255." << std::endl;
        error = true;
    }
    if (error) exit(1);

    secret_key.resize(secret_key_length);

    // Open input file
    std::fstream input_file(input_file_name, std::ios::in | std::ios::binary);
    if (!input_file)
    {
        std::cerr << "Unable to open \'" << input_file_name << "\'" << std::endl;
        exit(1);
    }

    // Copy input file contents into a buffer
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input_file), {});

    // Decrypt using the appropriate word type
    switch (word_bit_length)
    {
        default:
            std::cerr << "Word bit length must be 16, 32, or 64" << std::endl;
            exit(1);
        case 16:
            {
                typedef uint16_t WORD;
                call_rc5_encrypt<WORD>(num_rounds, secret_key, buffer, output_file_name);
            }
            break;
        case 32:
            {
                typedef uint32_t WORD;
                call_rc5_encrypt<WORD>(num_rounds, secret_key, buffer, output_file_name);
            }
            break;
        case 64:
            {
                typedef uint64_t WORD;
                call_rc5_encrypt<WORD>(num_rounds, secret_key, buffer, output_file_name);
            }
            break;
    }

    return 0;
}


// Template function is necessary to reduce repeated code in main for each possible WORD size
template<class WORD>
void call_rc5_encrypt(std::size_t num_rounds, const std::string & secret_key, std::vector<unsigned char> & buffer, const std::string & output_file_name)
{
    auto file_length = buffer.size();
    std::vector<WORD> plain_text(file_length / sizeof(WORD), 0);
    std::vector<WORD> cipher_text(file_length / sizeof(WORD));

    // Convert vector<char> buffer -> vector<WORD> plain_text
    convertVector(buffer, plain_text);

    // Encrypt plain_text into cipher_text
    rc5_encrypt<WORD>(num_rounds, secret_key, plain_text, cipher_text);

    {
        // Convert vector<WORD> cipher_text -> vector<char> buffer
        convertVector(cipher_text, buffer);

        // Write buffer to output file
        std::fstream output(std::string(output_file_name), std::ios::out | std::ios::binary);
        std::copy(buffer.begin(), buffer.end(), std::ostreambuf_iterator<char>(output));
    }
}


void configure_parser(cli::Parser & parser) {
    parser.set_required<std::string>("i", "input_file",            "Input file name");
    parser.set_optional<std::string>("o", "output_file",     "",   "Output file name");
    parser.set_optional<std::size_t>("w", "word_bit_length", 32,   "Word length in bits [16, 32, 64]");
    parser.set_optional<std::size_t>("r", "num_rounds",      12,   "Number of rounds [1 < r < 255]");
    parser.set_optional<std::size_t>("b", "key_length",      16,   "Secret key length [1 < b < 255]");
    parser.set_optional<std::string>("k", "secret_key",      "",   "Secret key");
    parser.set_optional<bool>(       "v", "verbose",         false, "Verbose Command line output");
}
