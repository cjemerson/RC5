// convert_vector.hpp
// Author: Charles Emerson
//
// Created: 18 November 2020
// Updated: 7 December 2020
//
// Shuffle data between vectors of two different types.
// Works if one type's byte size is a multiple of another
//
// Supporting code for RC5 Project for CS 665 Fall 2020.



#ifndef FILE_CONVERT_VECTOR_HPP_INCLUDED
#define FILE_CONVERT_VECTOR_HPP_INCLUDED

#include <vector>

// Fills the output vector of OutputType with the contents of the input
// Expanding or breaking up data accordingly
template<class InputType, class OutputType>
void convertVector(const std::vector<InputType> & input, std::vector<OutputType> & output)
{
    if (sizeof(InputType) < sizeof(OutputType))
    {
        if (sizeof(OutputType) % sizeof(InputType) != 0)
        {
            std::cerr << "OutputType is not a multiple of InputType (" << sizeof(OutputType) << "%" << sizeof(InputType) << " != 0)" << std::endl;
            exit(1);
        }

        for (auto iit = input.begin(), oit = output.begin(); iit != input.end() && oit != output.end(); ++oit)
        {
            *oit = 0;
            for (std::size_t j = 0; j < sizeof(OutputType); j += sizeof(InputType))
            {
                // *oit |= *(iit++) << (j * 8);
                // Suppresses implicit conversion warning
                auto x = ((OutputType) *(iit++)) << (j * 8);
                *oit |= (OutputType) x;
            }
        }
    }
    else if (sizeof(OutputType) < sizeof(InputType))
    {
        if (sizeof(InputType) % sizeof(OutputType) != 0)
        {
            std::cerr << "InputType is not a multiple of OutputType (" << sizeof(InputType) << "%" << sizeof(OutputType) << " != 0)" << std::endl;
            exit(1);
        }

        InputType OUTPUT_TYPE_MASK = (InputType) (-1) >> 8 * (sizeof(InputType) - sizeof(OutputType));
        for (auto iit = input.begin(), oit = output.begin(); iit != input.end() && oit != output.end(); ++iit)
        {
            for (std::size_t j = 0; j < sizeof(InputType); j += sizeof(OutputType))
            {
                *(oit++) = (OutputType) ((*iit >> (j * 8)) & OUTPUT_TYPE_MASK);
            }
        }
    }
    else
    {
        auto iit = input.begin(), oit = output.end();
        while (iit != input.end() && oit != output.end())
        {
            *(oit++) = *(iit++);
        }
    }
}

#endif // #ifndef FILE_CONVERT_VECTOR_HPP_INCLUDED
