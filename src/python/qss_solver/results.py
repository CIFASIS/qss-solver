import os

from . import file_handlers as fh

def simulation_log(model):
    """
    Reads the simulation log for a given model and returns a dictionary
    of key-value pairs extracted from the log file.

    Parameters:
    model: The name of the model for which to read the log.

    Returns:
    dict: A dictionary containing the log data, excluding the 'Simulation output' key.
    """
    # Initialize an empty dictionary to store the results
    results = {}
    
    model_path = fh.get_full_path(model, 'MMOC_LOG')

    # Read the log data from the file
    with open(model_path, 'r') as file:
        log = file.read()
    
    # Split the log into lines
    lines = log.strip().split('\n')
    
    # Iterate through each line
    for line in lines:
        # Split the line at the colon
        if ':' in line:
            key, value = line.split(':', 1)
            # Strip whitespace, remove 'ms', and convert to float
            cleaned_value = value.strip().replace(' ms', '')
            # Store the value in the dictionary if the key is not 'Simulation output'
            if key.strip() != 'Simulation output':
                results[key.strip()] = float(cleaned_value)
    
    return results

def output_files(model):
    """
    Returns a list of all '.dat' files in the specified output folder 
    for a given model.

    Parameters:
    model: The name of the model for which to list the output files.

    Returns:
    list: A list of absolute paths to all '.dat' files in the model's output directory.
    """
    output_path = os.path.join(os.environ['MMOC_OUTPUT'], model)

    dat_files = []
    
    # Iterate through the files in the given folder
    for file_name in os.listdir(output_path):
        # Check if the file ends with '.dat'
        if file_name.endswith('.dat'):
            dat_files.append(os.path.abspath(os.path.join(output_path, file_name)))
    
    return dat_files