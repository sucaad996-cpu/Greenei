TinyFlow Gd is a lightweight, high-performance Neural Network library written entirely in GDScript. It is designed to allow game developers to implement "intelligence" into their games—such as AI drivers, smart enemies, or complex decision-making systems—without the need for external plugins or complex C++ compilation.
✨ Key Features
🚀 Zero Dependencies: Works out of the box with any Godot 4.x project.
🔌 Autoload Ready: Designed to be used as a Singleton (Autoload), making the "Brain" accessible from any script in your game.
💾 Model Persistence: Built-in save_model() and load_model() functions to save your AI's training progress to disk.
⚡ Optimized Performance: Uses PackedFloat32Array to ensure mathematical operations are as fast as possible within the GDScript engine.
🧠 Feed-Forward Architecture: Implements a classic multilayer perceptron with Sigmoid activation and Backpropagation.
🚀 Quick Start
1. Installation
Download the TinyFlow.gd file.
Drop it into your Godot project folder.
Go to Project -> Project Settings -> Autoload.
Select TinyFlow.gd, give it the name TinyFlow, and click Add.
2. The "Golden Rule" (Normalization)
For the neural network to learn, all inputs and all targets must be normalized between 0.0 and 1.0.
Example: If your sensor detects distance from 0 to 100, pass distance / 100.0 into the network.
3. Basic Usage
Initializing the Brain
You can initialize the brain parameters via the Autoload in your _ready() function or by creating a local instance.
code
Gdscript
# Using the Autoload (Global)
# TinyFlow.init(inputs, hidden_neurons, outputs, learning_rate)
TinyFlow._init(3, 10, 2, 0.05)
Making a Decision (Inference)
code
Gdscript
var sensor_data = [0.1, 0.5, 0.9] # Normalized inputs
var hidden_buffer = []            # Temporary storage for math
hidden_buffer.resize(10)          # Match your hidden_size

var decisions = TinyFlow.forward(sensor_data, hidden_buffer)

var steering = decisions[0]
var gas = decisions[1]
Teaching the AI (Training)
code
Gdscript
# When the AI makes a mistake (e.g., hits a wall):
var inputs = [0.1, 0.5, 0.9]
var correct_behavior = [0.0, 0.0] # The "Target" response

TinyFlow.train(inputs, correct_behavior)
Saving & Loading
code
Gdscript
# Save your trained model
TinyFlow.save_model("user://my_ai_brain.res")

# Load your trained model
if TinyFlow.load_model("user://my_ai_brain.res"):
    print("AI intelligence restored!")
🛠 Technical Specifications
Architecture: Feed-Forward Neural Network.
Activation Function: Sigmoid.
Optimization: Stochastic Gradient Descent (SGD) via Backpropagation.
Data Storage: Binary serialization using Godot's FileAccess and Dictionary systems.
📝 License
This library is released under the MIT License. Feel free to use it in commercial and non-commercial projects!
