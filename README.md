A lightweight, header-only, C++11 Neural Network library for rapid prototyping and embedded AI.
![Image](https://img.shields.io/badge/C%2B%2B-11%20%7C%2014%20%7C%2017-blue.svg)
![Image](https://img.shields.io/badge/License-MIT-green.svg)
TinyFlow is designed for developers who need a "Brain" without the overhead of massive frameworks like TensorFlow or PyTorch. It is perfect for game AI, simple robotics, and learning the fundamentals of backpropagation.
✨ Features
🚀 Header-Only: No complex installation or linking. Just #include "TinyFlow.h".
💾 Persistence: Built-in save() and load() functions to store trained weights.
⚡ Lightweight: Minimal memory footprint, ideal for mobile (Cxxdroid) and embedded systems.
📈 Standard Math: Implements Feed-Forward architecture with Sigmoid activation and Gradient Descent.
🛠 Installation
Simply download TinyFlow.h and place it in your project directory.
code
C++
#include "TinyFlow.h"
📖 Usage Guide
⚠️ The Golden Rule: Normalization
For the neural network to function correctly, all inputs and all targets must be normalized between 0.0 and 1.0.
Wrong: brain.train({100, 50}, {1}); ❌
Right: brain.train({1.0, 0.5}, {1.0}); ✅
1. Initialization
Define your architecture: TinyFlow(inputs, hidden_neurons, outputs, learning_rate);
code
C++
// A brain with 3 sensors, 10 hidden neurons, and 2 outputs
TinyFlow brain(3, 10, 2, 0.01f);
2. Inference (Thinking)
To get a decision from the brain, use the forward method.
code
C++
std::vector<float> inputs = {0.1f, 0.8f, 0.4f}; // Normalized sensor data
std::vector<float> hidden_buffer(10);           // Temporary storage for math

// Get the output
std::vector<float> decisions = brain.forward(inputs, hidden_buffer);

float action_1 = decisions[0]; // e.g., Steering
float action_2 = decisions[1]; // e.g., Acceleration
3. Training (Learning)
To teach the brain, provide the inputs it saw and the "correct" target output.
code
C++
std::vector<float> inputs = {0.1f, 0.8f, 0.4f};
std::vector<float> targets = {1.0f, 0.0f}; // The "Correct" answer

brain.train(inputs, targets);
4. Save & Load (Persistence)
Don't lose your training progress!
code
C++
// Save the trained brain
brain.save("my_model.tf");

// Load a brain later
TinyFlow new_brain(3, 10, 2);
if(new_brain.load("my_model.tf")) {
    std::cout << "Brain loaded successfully!" << std::endl;
}
🚀 Quick Example (XOR Gate)
A complete minimal example to test your installation:
code
C++
#include "TinyFlow.h"
#include <iostream>

int main() {
    TinyFlow brain(2, 4, 1);
    
    // Training data
    std::vector<std::vector<float>> inputs = {{0,0}, {0,1}, {1,0}, {1,1}};
    std::vector<std::vector<float>> targets = {{0}, {1}, {1}, {0}};

    // Train 10,000 times
    for(int i=0; i<10000; ++i) {
        brain.train(inputs[i%4], targets[i%4]);
    }

    // Test
    std::vector<float> buffer(4);
    for(auto& in : inputs) {
        auto out = brain.forward(in, buffer);
        std::cout << "In: " << in[0] << "," << in[1] << " Out: " << out[0] << std::endl;
    }
    return 0;
}
📜 License
This project is licensed under the MIT License - see the LICENSE file for details.
Developed with ❤️ for the C++ Community.
