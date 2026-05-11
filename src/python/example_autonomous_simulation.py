#!/usr/bin/env python3
"""
Example script demonstrating autonomous QSS simulation execution from Python.

This script shows how to:
1. Configure environment variables
2. Set up model parameters and annotations
3. Compile and run simulations
4. Process results

Author: QSS Solver Python API
"""

import os
import sys
import logging
from pathlib import Path

# Add the qss_solver module to the Python path
# Adjust this path according to your installation
sys.path.insert(0, '/opt/CIFASIS-CONICET/qss-solver/src/python')

try:
    from qss_solver import (
        # Simulation functions
        compile_model, execute_model, run,
        # Model manipulation functions
        annotations, set_annotations,
        constants, parameters, set_constant, set_parameter, set_constants, set_parameters,
        config, set_config,
        # Result functions
        simulation_log, output_files
    )
except ImportError as e:
    print(f"Error importing qss_solver modules: {e}")
    print("Make sure the qss_solver package is in your Python path")
    sys.exit(1)

def setup_environment():
    """
    Configure required environment variables for QSS Solver.
    
    Adjust these paths according to your QSS Solver installation.
    """
    # Base installation directory
    qss_base = "/home/joaquin/work/qss-solver"
    
    # Required environment variables
    env_vars = {
        'MMOC_BIN': os.path.join(qss_base, 'bin'),
        'MMOC_MODELS': os.path.join(qss_base, 'models'),
        'MMOC_OUTPUT': os.path.join(qss_base, 'output'),
        'MMOC_BUILD': os.path.join(qss_base, 'build')
    }
    
    # Set environment variables
    for var, value in env_vars.items():
        os.environ[var] = value
        print(f"Set {var} = {value}")
    
    # Create directories if they don't exist
    for var in ['MMOC_OUTPUT', 'MMOC_BUILD']:
        path = os.environ[var]
        Path(path).mkdir(parents=True, exist_ok=True)
        print(f"Ensured directory exists: {path}")
    
    return env_vars

def configure_logging():
    """Configure logging for detailed output."""
    logging.basicConfig(
        level=logging.INFO,
        format='%(asctime)s - %(levelname)s - %(message)s',
        handlers=[
            logging.StreamHandler(sys.stdout),
            logging.FileHandler('simulation_example.log')
        ]
    )

def example_basic_simulation(model_name):
    """
    Basic example: compile and run a model with default settings.
    
    Args:
        model_name (str): Name of the Modelica model file (without .mo extension)
    """
    print(f"\n=== Basic Simulation Example: {model_name} ===")
    
    model_file = f"{model_name}.mo"
    
    # Step 1: Check current model annotations
    print("Current model annotations:")
    current_ann = annotations(model_file)
    if current_ann:
        for key, value in current_ann.items():
            print(f"  {key}: {value}")
    else:
        print("  No annotations found")
    
    # Step 2: Compile the model
    print(f"\nCompiling model: {model_file}")
    if compile_model(model_file):
        print("  Compilation successful")
    else:
        print("  Compilation failed")
        return False
    
    # Step 3: Run the simulation
    print(f"\nRunning simulation: {model_file}")
    if execute_model(model_file):
        print("  Simulation completed successfully")
    else:
        print("  Simulation failed")
        return False
    
    # Step 4: Get simulation results
    print("\nSimulation results:")
    log_data = simulation_log(model_name)
    if log_data:
        for key, value in log_data.items():
            print(f"  {key}: {value} ms")
    
    output_files_list = output_files(model_name)
    print(f"\nOutput files ({len(output_files_list)}):")
    for file_path in output_files_list:
        print(f"  {file_path}")
    
    return True

def example_advanced_configuration(model_name):
    """
    Advanced example: modify model parameters and annotations before simulation.
    
    Args:
        model_name (str): Name of the Modelica model file (without .mo extension)
    """
    print(f"\n=== Advanced Configuration Example: {model_name} ===")
    
    model_file = f"{model_name}.mo"
    
    # Step 1: Read current model parameters and constants
    print("Current model parameters:")
    try:
        current_params = parameters(model_file)
        if current_params:
            for key, value in current_params.items():
                print(f"  {key}: {value}")
        else:
            print("  No parameters found")
    except Exception as e:
        print(f"  Error reading parameters: {e}")
    
    print("\nCurrent model constants:")
    try:
        current_constants = constants(model_file)
        if current_constants:
            for key, value in current_constants.items():
                print(f"  {key}: {value}")
        else:
            print("  No constants found")
    except Exception as e:
        print(f"  Error reading constants: {e}")
    
    # Step 2: Modify model annotations
    new_annotations = {
        'startTime': 0.0,
        'stopTime': 10.0,
        'tolerance': 1e-8,
        'stepSize': 1e-4
    }
    
    print(f"\nSetting new annotations:")
    for key, value in new_annotations.items():
        print(f"  {key}: {value}")
    
    set_annotations(model_file, new_annotations)
    
    # Step 3: Modify model parameters (example values)
    new_parameters = {
        'k': 2.5,
        'omega': 1.0,
        'damping': 0.1
    }
    
    print(f"\nSetting new parameters:")
    for key, value in new_parameters.items():
        print(f"  {key}: {value}")
    
    try:
        set_parameters(model_file, new_parameters)
    except Exception as e:
        print(f"  Warning: Could not set parameters: {e}")
    
    # Step 4: Run the complete simulation
    print(f"\nRunning complete simulation: {model_file}")
    success = run(model_file, "-O3")  # With optimization flags
    
    if success:
        print("  Simulation completed successfully")
        
        # Step 5: Process results
        log_data = simulation_log(model_name)
        if log_data:
            print("\nPerformance metrics:")
            for key, value in log_data.items():
                print(f"  {key}: {value} ms")
    else:
        print("  Simulation failed")
    
    return success

def example_batch_simulation(model_name, parameter_sets):
    """
    Batch simulation example: run multiple simulations with different parameter sets.
    
    Args:
        model_name (str): Name of the Modelica model file
        parameter_sets (list): List of dictionaries containing parameter sets
    """
    print(f"\n=== Batch Simulation Example: {model_name} ===")
    
    model_file = f"{model_name}.mo"
    results = []
    
    for i, params in enumerate(parameter_sets):
        print(f"\n--- Simulation {i+1}/{len(parameter_sets)} ---")
        print(f"Parameters: {params}")
        
        # Set parameters for this run
        try:
            set_parameters(model_file, params)
        except Exception as e:
            print(f"  Warning: Could not set parameters: {e}")
            continue
        
        # Run simulation
        success = run(model_file)
        
        if success:
            # Collect results
            log_data = simulation_log(model_name)
            result = {
                'run': i+1,
                'parameters': params,
                'simulation_time': log_data.get('Simulation time', 0) if log_data else 0,
                'compilation_time': log_data.get('Compilation time', 0) if log_data else 0
            }
            results.append(result)
            print(f"  Success: Sim time = {result['simulation_time']} ms")
        else:
            print(f"  Failed")
    
    # Summary
    print(f"\n=== Batch Simulation Summary ===")
    print(f"Total runs: {len(parameter_sets)}")
    print(f"Successful runs: {len(results)}")
    
    if results:
        avg_sim_time = sum(r['simulation_time'] for r in results) / len(results)
        avg_comp_time = sum(r['compilation_time'] for r in results) / len(results)
        print(f"Average simulation time: {avg_sim_time:.2f} ms")
        print(f"Average compilation time: {avg_comp_time:.2f} ms")
    
    return results

def main():
    """Main function demonstrating various simulation scenarios."""
    print("QSS Solver Python API - Autonomous Simulation Example")
    print("=" * 60)
    
    # Setup
    configure_logging()
    env_vars = setup_environment()
    
    # Example model names (adjust according to your available models)
    # example_models = ["bouncing_ball", "pendulum", "van_der_pol"]
    
    # Choose a model that exists in your models directory
    model_name = "bball_downstairs"  # Change this to your model
    
    # Check if model file exists
    model_path = os.path.join(env_vars['MMOC_MODELS'], f"{model_name}.mo")
    if not os.path.exists(model_path):
        print(f"Error: Model file not found: {model_path}")
        print("Please update the model_name variable to point to an existing model")
        return
    
    try:
        # Example 1: Basic simulation
        example_basic_simulation(model_name)
        
        # Example 2: Advanced configuration
        #example_advanced_configuration(model_name)
        
        # Example 3: Batch simulation with different parameters
        #parameter_sets = [
        #    {'k': 1.0, 'damping': 0.1},
        #    {'k': 2.0, 'damping': 0.2},
        #    {'k': 3.0, 'damping': 0.3}
        #]
        #example_batch_simulation(model_name, parameter_sets)
        
    except Exception as e:
        print(f"Error during simulation: {e}")
        logging.error(f"Simulation error: {e}", exc_info=True)
    
    print("\n" + "=" * 60)
    print("Example completed successfully!")
    print(f"Check the log file: simulation_example.log")

if __name__ == "__main__":
    main()
