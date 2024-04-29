#ifndef NNUE_LINEAR_H
#define NNUE_LINEAR_H

#include <array>
#include <vector>

template<int inputs, int outputs>
struct Linear
{
    float **weights = new float *[inputs];
    float *biases = new float[outputs];

    Linear(std::vector<std::vector<float>> weights, std::vector<float> biases)
    {
        for (int i = 0; i < inputs; i++)
            this->weights[i] = new float[outputs];

        for (int o = 0; o < outputs; o++)
        {
            for (int i = 0; i < inputs; i++)
                // Input encoding is in row-major order
                // this->weights should be in column-major order
                this->weights[i][o] = weights[o][i];
            this->biases[o] = biases[o];
        }
    }

    std::array<float, outputs> forward(std::array<float, inputs> input)
    {
        std::array<float, outputs> output;

        for (int o = 0; o < outputs; o++)
        {
            output[o] = biases[o];
            for (int i = 0; i < inputs; i++)
                output[o] += weights[i][o] * input[i];
        }

        return output;
    }

    std::array<float, outputs> relu(std::array<float, outputs> input)
    {
        std::array<float, outputs> output;

        for (int o = 0; o < outputs; o++)
            output[o] = std::max(0.0f, std::min(1.0f, input[o]));

        return output;
    }
};

#endif //NNUE_LINEAR_H
