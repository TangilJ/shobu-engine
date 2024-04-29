#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "network.h"
#include "linear.h"

#ifndef NNUE_PARSE_H
#define NNUE_PARSE_H

// Tensor string is delimited by commas
std::vector<float> read1DTensor(const std::string &str)
{
    std::vector<float> tensor;
    std::stringstream stream(str);
    std::string element;

    while (std::getline(stream, element, ','))
        tensor.push_back(std::stof(element));

    return tensor;
}

// Tensor string is delimited by commas and semicolons
std::vector<std::vector<float>> read2DTensor(const std::string &str)
{
    std::vector<std::vector<float>> tensor;
    std::stringstream stream(str);
    std::string row;

    while (std::getline(stream, row, ';'))
        tensor.push_back(read1DTensor(row));

    return tensor;
}

template<int h1, int h2, int h3>
Network<h1, h2, h3> parseNetwork(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::invalid_argument("File not found");

    std::string line;
    int lineNum = 0;

    std::vector<std::vector<std::vector<float>>> weights;
    std::vector<std::vector<float>> biases;

    while (std::getline(file, line))
    {
        if (lineNum % 2 == 0)
            weights.push_back(read2DTensor(line));
        else
            biases.push_back(read1DTensor(line));
        lineNum++;
    }
    file.close();

    if (weights[0].size() != h1 || weights[1].size() != h2 || weights[2].size() != h3)
        throw std::invalid_argument("Invalid network dimensions");

    Network<h1, h2, h3> network(weights, biases);
    return network;
}

#endif //NNUE_PARSE_H
