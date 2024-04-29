#ifndef NNUE_NETWORK_H
#define NNUE_NETWORK_H

#include "linear.h"

template<int hidden1, int hidden2, int hidden3>
struct Network
{
    Linear<1024, hidden1> layer1;
    Linear<hidden1, hidden2> layer2;
    Linear<hidden2, hidden3> layer3;
    Linear<hidden3, 1> output;

    Network(std::vector<std::vector<std::vector<float>>> weights,
            std::vector<std::vector<float>> biases)
        : layer1(weights[0], biases[0]),
          layer2(weights[1], biases[1]),
          layer3(weights[2], biases[2]),
          output(weights[3], biases[3])
    {}

    float forward(std::array<float, 1024> x)
    {
        auto h1 = layer1.forward(x);
        return forwardAfterInitial(h1);
    }

    float forwardAfterInitial(std::array<float, hidden1> x)
    {
        auto h2 = layer2.forward(layer1.relu(x));
        auto h3 = layer3.forward(layer2.relu(h2));
        return output.forward(layer3.relu(h3))[0];
    }
};

#endif //NNUE_NETWORK_H
