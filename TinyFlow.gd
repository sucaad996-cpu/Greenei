# TinyFlow.gd
# A lightweight, header-only, C++ converted Neural Network library for Godot
# Optimized for Godot 4.x (Compatible with 4.0, 4.1, 4.2, 4.3+)

extends Node # Extending Node allows this to be used as an Autoload (Singleton)

# --- Variables (The Brain Structure) ---
var input_size: int          # Number of input sensors
var hidden_size: int         # Number of hidden neurons
var output_size: int         # Number of outputs
var learning_rate: float     # How fast the AI learns

var w1: PackedFloat32Array    # Weights: Input -> Hidden
var w2: PackedFloat32Array    # Weights: Hidden -> Output
var b1: PackedFloat32Array    # Biases for Hidden layer
var b2: PackedFloat32Array    # Biases for Output layer

# --- Initialization ---

# This function replaces the C++ Constructor
func _init(ins: int = 3, hid: int = 6, outs: int = 2, lr: float = 0.1):
	input_size = ins
	hidden_size = hid
	output_size = outs
	learning_rate = lr
	
	# Initialize arrays with the correct sizes
	w1.resize(input_size * hidden_size)
	w2.resize(hidden_size * output_size)
	b1.resize(hidden_size)
	b2.resize(output_size)
	
	# Fill weights with random values between -1.0 and 1.0
	for i in range(w1.size()):
		w1[i] = randf_range(-1.0, 1.0)
	for i in range(w2.size()):
		w2[i] = randf_range(-1.0, 1.0)
	for i in range(b1.size()):
		b1[i] = 0.0
	for i in range(b2.size()):
		b2[i] = 0.0

# --- Math Utilities ---

func _sigmoid(x: float) -> float:
	return 1.0 / (1.0 + exp(-x)) 

func _sigmoid_derivative(x: float) -> float:
	return x * (1.0 - x)        

# --- The Core Engine ---

# Forward Pass: The "Thinking" part
func forward(inputs: Array, hidden_out: Array) -> Array:
	var outputs = [] 
	
	# 1. Calculate Hidden Layer
	for j in range(hidden_size):
		var sum = 0.0
		for i in range(input_size):
			sum += inputs[i] * w1[i * hidden_size + j]
		sum += b1[j]
		hidden_out[j] = _sigmoid(sum)
		
	# 2. Calculate Output Layer
	for j in range(output_size):
		var sum = 0.0
		for i in range(hidden_size):
			sum += hidden_out[i] * w2[i * output_size + j]
		sum += b2[j]
		outputs.append(_sigmoid(sum))
		
	return outputs

# Training: The "Learning" part
func train(inputs: Array, targets: Array):
	var hidden_out = []
	hidden_out.resize(hidden_size)
	
	# 1. Forward Pass
	var outputs = forward(inputs, hidden_out)
	
	# 2. Calculate Output Errors
	var output_errors = []
	for i in range(output_size):
		output_errors.append(targets[i] - outputs[i])
		
	# 3. Backpropagate to Output Layer
	for i in range(hidden_size):
		for j in range(output_size):
			var gradient = output_errors[j] * _sigmoid_derivative(outputs[j])
			var current_w = w2[i * output_size + j]
			w2[i * output_size + j] = current_w + (learning_rate * gradient * hidden_out[i])
			
	# 4. Backpropagate to Hidden Layer
	for i in range(input_size):
		for j in range(hidden_size):
			var error_hidden = 0.0
			for k in range(output_size):
				error_hidden += output_errors[k] * w2[j * output_size + k]
			
			var gradient = error_hidden * _sigmoid_derivative(hidden_out[j])
			var current_w = w1[i * hidden_size + j]
			w1[i * hidden_size + j] = current_w + (learning_rate * gradient * inputs[i])

# --- Save / Load System (Fixed version) ---

func save_model(path: String):
	var file = FileAccess.open(path, FileAccess.WRITE)
	if file:
		# We store everything in a Dictionary. 
		# Godot can save PackedArrays directly without to_array()!
		var data = {
			"input_size": input_size,
			"hidden_size": hidden_size,
			"output_size": output_size,
			"learning_rate": learning_rate,
			"w1": w1,
			"w2": w2,
			"b1": b1,
			"b2": b2
		}
		file.store_var(data) 
		file.close()
		print("Model successfully saved to: ", path)
	else:
		print("Error: Could not open file for saving: ", path)

func load_model(path: String) -> bool:
	if not FileAccess.file_exists(path):
		print("Error: File does not exist: ", path)
		return false
		
	var file = FileAccess.open(path, FileAccess.READ)
	if file:
		var data = file.get_var() # This automatically reconstructs the dictionary
		
		# Verify architecture
		if data["input_size"] != input_size:
			print("Error: Model architecture mismatch!")
			return false
			
		# Restore everything
		learning_rate = data["learning_rate"]
		w1 = data["w1"]
		w2 = data["w2"]
		b1 = data["b1"]
		b2 = data["b2"]
		
		file.close()
		print("Model successfully loaded from: ", path)
		return true
	return false
