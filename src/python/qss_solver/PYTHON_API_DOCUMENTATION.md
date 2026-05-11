# QSS Solver Python API Documentation

## Overview

The QSS Solver provides a Python API for autonomously executing simulations from a Python console. The API is located in `src/python/qss_solver/` and consists of four main modules that provide functionality for model manipulation, simulation execution, result handling, and file management.

## Module Structure

```
qss_solver/
├── __init__.py          # Module initialization
├── simulate.py          # Simulation execution functions
├── model.py             # Model manipulation functions
├── results.py           # Result handling functions
└── file_handlers.py     # File utility functions
```

## Modules

### 1. `simulate.py` - Simulation Execution

This module provides functions for compiling and running Modelica models.

#### Functions

##### `compile_model(model_file, flags='')`
Compiles a Modelica model into an executable.

**Parameters:**
- `model_file` (str): Path to the Modelica model file
- `flags` (str, optional): Additional compilation flags

**Returns:**
- `bool`: `True` if compilation successful, `False` otherwise

**Example:**
```python
from qss_solver import compile_model
success = compile_model("my_model.mo", "-O3")
```

##### `execute_model(model_file)`
Executes a compiled model.

**Parameters:**
- `model_file` (str): Path to the model file

**Returns:**
- `bool`: `True` if execution successful, `False` otherwise

**Example:**
```python
from qss_solver import execute_model
success = execute_model("my_model.mo")
```

##### `run(model_file, flags='')`
Convenience function that compiles and runs a model in sequence.

**Parameters:**
- `model_file` (str): Path to the Modelica model file
- `flags` (str, optional): Additional compilation flags

**Example:**
```python
from qss_solver import run
run("my_model.mo", "-O3")
```

### 2. `model.py` - Model Manipulation

This module provides comprehensive functions for reading and modifying Modelica models.

#### Annotation Functions

##### `annotations(model)`
Extracts experiment annotations from a Modelica model.

**Parameters:**
- `model` (str): Path to the Modelica file

**Returns:**
- `dict`: Dictionary containing annotation key-value pairs, or `None` if no annotations found

**Example:**
```python
from qss_solver import annotations
ann = annotations("my_model.mo")
print(ann)  # {'startTime': 0.0, 'stopTime': 10.0, 'tolerance': 1e-6}
```

##### `set_annotations(model, annotations)`
Replaces or adds annotations to a Modelica model.

**Parameters:**
- `model` (str): Path to the Modelica file
- `annotations` (dict): Dictionary containing annotation data

**Example:**
```python
from qss_solver import set_annotations
set_annotations("my_model.mo", {
    'startTime': 0.0,
    'stopTime': 20.0,
    'tolerance': 1e-8
})
```

#### Constant and Parameter Functions

##### `constants(model)`
Reads all `constant Integer` variables from a Modelica model.

**Parameters:**
- `model` (str): Path to the Modelica file

**Returns:**
- `dict`: Dictionary with variable names as keys and integer values

##### `parameters(model)`
Reads all `parameter Real` variables from a Modelica model.

**Parameters:**
- `model` (str): Path to the Modelica file

**Returns:**
- `dict`: Dictionary with variable names as keys and float values

##### `set_constant(model, variable_name, new_value)`
Changes the value of a `constant Integer` variable.

**Parameters:**
- `model` (str): Path to the Modelica file
- `variable_name` (str): Name of the constant to change
- `new_value` (int): New value for the constant

##### `set_parameter(model, variable_name, new_value)`
Changes the value of a `parameter Real` variable.

**Parameters:**
- `model` (str): Path to the Modelica file
- `variable_name` (str): Name of the parameter to change
- `new_value` (float): New value for the parameter

##### `set_constants(model, constants)`
Changes multiple constants from a dictionary.

**Parameters:**
- `model` (str): Path to the Modelica file
- `constants` (dict): Dictionary of constant name-value pairs

##### `set_parameters(model, params)`
Changes multiple parameters from a dictionary.

**Parameters:**
- `model` (str): Path to the Modelica file
- `params` (dict): Dictionary of parameter name-value pairs

#### JSON Parameter Functions

##### `json_parameters(model)`
Reads parameter records from a JSON file.

**Parameters:**
- `model` (str): Path to the JSON file

**Returns:**
- `list`: List of dictionaries with parsed parameter data

**Example:**
```python
from qss_solver import json_parameters
params = json_parameters("params.json")
print(params)  # [{'k': 1.5, 'initial_values': [0.0, 1.0]}, ...]
```

##### `set_json_parameters(model, parameters)`
Writes parameter records to a JSON file.

**Parameters:**
- `model` (str): Path to the JSON file to write
- `parameters` (list): List of dictionaries containing parameter records

#### Configuration Functions

##### `config(model)`
Reads model configuration from an INI file.

**Parameters:**
- `model` (str): Path to the INI file

**Returns:**
- `dict`: Dictionary containing configuration data

##### `set_config(model, data_dict)`
Writes configuration data to an INI file.

**Parameters:**
- `model` (str): Path to the INI file to write
- `data_dict` (dict): Dictionary containing configuration data

### 3. `results.py` - Result Handling

This module provides functions for accessing simulation results and logs.

#### Functions

##### `simulation_log(model)`
Reads simulation log data for a model.

**Parameters:**
- `model` (str): Name of the model

**Returns:**
- `dict`: Dictionary containing log key-value pairs (excluding 'Simulation output')

**Example:**
```python
from qss_solver import simulation_log
log_data = simulation_log("my_model")
print(log_data)  # {'Compilation time': 15.2, 'Simulation time': 123.4, ...}
```

##### `output_files(model)`
Lists all output data files for a model.

**Parameters:**
- `model` (str): Name of the model

**Returns:**
- `list`: List of absolute paths to `.dat` files in the model's output directory

**Example:**
```python
from qss_solver import output_files
files = output_files("my_model")
print(files)  # ['/path/to/output/my_model_state1.dat', '/path/to/output/my_model_state2.dat', ...]
```

### 4. `file_handlers.py` - File Utilities

This module provides utility functions for file path handling and resolution. It's primarily used internally by other modules but contains some useful public functions.

#### Configuration

The module uses environment variables for path resolution:
- `MMOC_MODELS`: Base directory for model files
- `MMOC_OUTPUT`: Base directory for output files
- `MMOC_BUILD`: Base directory for build files

#### Key Functions

##### `get_full_path(file_path, config_entry='MMOC_MODELS')`
Resolves a relative file path to an absolute path.

**Parameters:**
- `file_path` (str): Relative or absolute file path
- `config_entry` (str): Configuration entry for path resolution ('MMOC_MODELS', 'MMOC_LOG', 'MMOC_BUILD')

**Returns:**
- `str`: Absolute path to the file

##### `get_file_name(file_path, config_entry='MMOC_MODELS')`
Extracts the base filename without extension.

**Parameters:**
- `file_path` (str): Path to the file
- `config_entry` (str): Configuration entry for path resolution

**Returns:**
- `str`: Base filename without extension

## Environment Variables

The Python API requires the following environment variables to be set:

- `MMOC_BIN`: Directory containing compilation and simulation scripts
- `MMOC_MODELS`: Base directory for Modelica model files
- `MMOC_OUTPUT`: Base directory for simulation output files
- `MMOC_BUILD`: Base directory for build configuration files

## Dependencies

The Python API requires the following external libraries:
- `libconf`: For INI file parsing
- Standard Python libraries: `os`, `subprocess`, `logging`, `json`, `re`, `pathlib`

## Error Handling

Most functions return boolean success indicators or raise exceptions for critical errors. The module uses Python's logging system for error reporting, configured at ERROR level by default.

## Usage Pattern

The typical workflow for autonomous simulation execution is:

1. **Configure the model**: Use `model.py` functions to set parameters, constants, and annotations
2. **Compile the model**: Use `compile_model()` to generate the executable
3. **Run the simulation**: Use `execute_model()` to run the compiled model
4. **Process results**: Use `results.py` functions to access logs and output files

This API enables complete automation of the QSS simulation workflow from Python scripts.
