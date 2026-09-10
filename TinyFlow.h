#ifndef TINYFLOW_H
#define TINYFLOW_H

/**
 * ============================================================================
 * TINYFLOW LIBRARY - VERSION 2.0 (Header-Only)
 * ============================================================================
 * A lightweight, header-only Neural Network library for C++.
 * 
 * --- USAGE DOCUMENTATION ---
 * 
 * 1. INITIALIZATION:
 *    TinyFlow brain(input_nodes, hidden_nodes, output_nodes, learning_rate);
 * 
 * 2. CONNECTING INPUT SENSORS:
 *    Your sensors (distance, speed, etc.) must be normalized between 0.0 and 1.0.
 *    Example: std::vector<float> inputs = { sensor_left / max_dist, sensor_right / max_dist };
 *    brain.forward(inputs, hidden_buffer);
 * 
 * 3. GETTING DATA FROM OUTPUT LAYER:
 *    The forward() function returns a std::vector<float>.
 *    If you have 2 outputs (Steering, Gas):
 *    float steering = output[0]; // Maps to 0.0 - 1.0
 *    float gas      = output[1]; // Maps to 0.0 - 1.0
 * 
 * 4. SAVING AND LOADING:
 *    To save your progress after hours of training:
 *    brain.save("model_checkpoint.tf");
 * 
 *    To resume training next time:
 *    TinyFlow brain(in, hid, out); // Create empty brain first
 *    brain.load("model_checkpoint.tf"); // Load the weights
 * 
 * 5. TRAINING:
 *    brain.train(inputs, targets); // targets should be the "correct" answer
 * ============================================================================
 */

#include <iostream>            // For file error reporting
#include <vector>              // For data storage
#include <cmath>               // For math functions
#include <algorithm>           // For data manipulation
#include <ctime>               // For random seeding
#include <fstream>             // For Save/Load file systems
#include <string>              // For file paths
#include <iomanip>             // For high-precision saving

// --- Internal Matrix Engine ---
struct Matrix {
    int rows, cols;
    std::vector<float> data;

    Matrix(int r, int c) : rows(r), cols(c), data(r * c, 0.0f) {}

    float get(int r, int c) const { return data[r * cols + c]; }
    void set(int r, int c, float val) { data[r * cols + c] = val; }

    // Helper to write matrix to a file stream
    void serialize(std::ostream& os) const {
        os << rows << " " << cols << " "; // Write dimensions
        for (float val : data) {
            os << std::setprecision(8) << val << " "; // Write data with precision
        }
        os << "\n"; // New line after matrix
    }

    // Helper to read matrix from a file stream
    void deserialize(std::istream& is) {
        is >> rows >> cols; // Read dimensions
        data.resize(rows * cols); // Resize vector to fit
        for (int i = 0; i < rows * cols; ++i) {
            is >> data[i]; // Read each value
        }
    }
};

// --- TinyFlow Neural Network ---
class TinyFlow {
public:
    int inputSize, hiddenSize, outputSize;
    float learningRate;

    Matrix W1, W2, b1, b2; // Weights and Biases

    // Constructor
    TinyFlow(int in, int hid, int out, float lr = 0.1f) 
        : inputSize(in), hiddenSize(hid), outputSize(out), learningRate(lr), 
          W1(in, hid), W2(hid, out), b1(1, hid), b2(1, out) {
        
        std::srand(std::time(0));
        // Randomize weights
        for (float &val : W1.data) val = ((float)rand() / RAND_MAX) * 2 - 1;
        for (float &val : W2.data) val = ((float)rand() / RAND_MAX) * 2 - 1;
    }

    // Math Utilities
    float sigmoid(float x) { return 1.0f / (1.0f + std::exp(-x)); }
    float sigmoidDeriv(float x) { return x * (1.0f - x); }

    // Inference (The "Think" step)
    std::vector<float> forward(const std::vector<float>& inputs, std::vector<float>& hiddenOut) {
        std::vector<float> outputs(outputSize, 0.0f);
        hiddenOut.assign(hiddenSize, 0.0f); // Clear previous hidden state

        // Input -> Hidden
        for (int j = 0; j < hiddenSize; j++) {
            float sum = 0.0f;
            for (int i = 0; i < inputSize; i++) sum += inputs[i] * W1.get(i, j);
            sum += b1.get(0, j);
            hiddenOut[j] = sigmoid(sum);
        }

        // Hidden -> Output
        for (int j = 0; j < outputSize; j++) {
            float sum = 0.0f;
            for (int i = 0; i < hiddenSize; i++) sum += hiddenOut[i] * W2.get(i, j);
            sum += b2.get(0, j);
            outputs[j] = sigmoid(sum);
        }
        return outputs;
    }

    // Training (The "Learn" step)
    void train(const std::vector<float>& inputs, const std::vector<float>& targets) {
        std::vector<float> hiddenOut(hiddenSize);
        std::vector<float> outputs = forward(inputs, hiddenOut);

        // Error calculation
        std::vector<float> outputErrors(outputSize);
        for (int i = 0; i < outputSize; i++) outputErrors[i] = targets[i] - outputs[i];

        // Update W2 and b2
        for (int i = 0; i < hiddenSize; i++) {
            for (int j = 0; j < outputSize; j++) {
                float gradient = outputErrors[j] * sigmoidDeriv(outputs[j]);
                W2.set(i, j, W2.get(i, j) + learningRate * gradient * hiddenOut[i]);
            }
        }

        // Update W1 and b1
        for (int i = 0; i < inputSize; i++) {
            for (int j = 0; j < hiddenSize; j++) {
                float errorHidden = 0.0f;
                for (int k = 0; k < outputSize; k++) errorHidden += outputErrors[k] * W2.get(j, k);
                float gradient = errorHidden * sigmoidDeriv(hiddenOut[j]);
                W1.set(i, j, W1.get(i, j) + learningRate * gradient * inputs[i]);
            }
        }
    }

    // --- SAVE SYSTEM ---
    void save(const std::string& filename) {
        std::ofstream file(filename); // Open file for writing
        if (!file.is_open()) {
            std::cerr << "Error: Could not save model to " << filename << std::endl;
            return;
        }

        // Write Header info
        file << inputSize << " " << hiddenSize << " " << outputSize << " " << learningRate << "\n";

        // Write Matrices
        W1.serialize(file);
        W2.serialize(file);
        b1.serialize(file);
        b2.serialize(file);

        file.close(); // Close file
        std::cout << "Model successfully saved to: " << filename << std::endl;
    }

    // --- LOAD SYSTEM ---
    bool load(const std::string& filename) {
        std::ifstream file(filename); // Open file for reading
        if (!file.is_open()) {
            std::cerr << "Error: Could not load model from " << filename << std::endl;
            return false;
        }

        // Read Header info
        int in, hid, out;
        float lr;
        file >> in >> hid >> out >> lr;

        // Check if the file architecture matches our brain
        if (in != inputSize || hid != hiddenSize || out != outputSize) {
            std::cerr << "Error: Model file architecture mismatch!" << std::endl;
            return false;
        }

        learningRate = lr;

        // Read Matrices
        W1.deserialize(file);
        W2.deserialize(file);
        b1.deserialize(file);
        b2.deserialize(file);

        file.close(); // Close file
        std::cout << "Model successfully loaded from: " << filename << std::endl;
        return true;
    }
};

#endif // TINYFLOW_H