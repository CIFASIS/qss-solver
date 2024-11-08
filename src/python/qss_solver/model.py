import configparser
import json
import re

import file_handlers as fh

def annotations(model):
    """
    Extract annotations from a Modelica model file.

    :param model: The name or path of the Modelica file to read.
    :return: A dictionary containing the key-value pairs of the experiment annotations, or None if no annotations are found.
    """
    model_path = fh.get_full_path(model)

    # Read the Modelica code from the specified file
    with open(model_path, 'r') as file:
        modelica_model = file.read()

    # Regular expression to find the annotation section
    pattern = r'annotation\s*\(\s*experiment\s*\(\s*(.*?)\s*\)\s*\);'
    
    # Search for the pattern in the provided Modelica code
    match = re.search(pattern, modelica_model, re.DOTALL)
    
    if not match:
        return None
    
    # Extract the content inside the experiment(...)
    experiment_content = match.group(1)
    
    # Split the content into key-value pairs
    parameters = re.findall(r'(\w+)\s*=\s*([^,]+)', experiment_content)
    
    # Create a dictionary from the key-value pairs
    result = {key.strip(): value.strip() for key, value in parameters}
    
    return result

def set_annotations(model, annotations):
    """
    Replace Modelica annotations in a file with the provided annotations dictionary.

    :param model: Path to the Modelica file.
    :param annotations: Dictionary containing the Modelica annotations.
    """
    model_path = fh.get_full_path(model)

    # Read the existing content of the Modelica file
    with open(model_path, 'r') as file:
        modelica_code = file.read()

    # Build the new annotation code
    new_annotation_code = "annotation(\n    experiment(\n"
    for key, value in annotations.items():
        new_annotation_code += f"        {key} = {value},\n"
    new_annotation_code = new_annotation_code.rstrip(",\n") + "\n    )\n);\n"

    # Regular expression to find the existing annotation section
    pattern = r'annotation\s*\(\s*experiment\s*\(\s*(.*?)\s*\)\s*\);'
    
    # Replace the existing annotation with the new one
    modified_code = re.sub(pattern, new_annotation_code, modelica_code, flags=re.DOTALL)

    # Write the modified content back to the file
    with open(model_path, 'w') as file:
        file.write(modified_code)

def set_constant(model, variable_name, new_value):
    """
    Change the value of a constant Integer variable in a Modelica model.

    :param model: Path to the Modelica file to modify.
    :param variable_name: The name of the constant Integer variable to change.
    :param new_value: The new value to set for the constant Integer variable.
    """
    
    model_path = fh.get_full_path(model)

    # Read the existing content of the Modelica file
    with open(model_path, 'r') as file:
        modelica_code = file.read()

    # Regular expression to find the constant Integer variable
    pattern = rf'constant\s+Integer\s+{variable_name}\s*=\s*\d+;'

    # Create the new declaration with the new value
    new_declaration = f'constant Integer {variable_name} = {new_value};'

    # Replace the old declaration with the new one
    modified_code = re.sub(pattern, new_declaration, modelica_code)

    # Write the modified content back to the file
    with open(model_path, 'w') as file:
        file.write(modified_code)

def set_constants(model, constants):
    """
    Change the value of a constant Integer variable from the given dictionary.

    :param model: Path to the Modelica file to modify.
    :param constants: Dictionary containing the Modelica constants.
    """
    for key, value in constants.items():
        set_constant(model, key, value)


def constants(model):
    """
    Read all constant Integer variables from a Modelica model.

    :param model: Path to the Modelica file to read.
    :return: A dictionary with variable names as keys and their values as integers.
    """
    model_path = fh.get_full_path(model)

    # Initialize an empty dictionary to store the constant Integer variables
    constant_integers = {}

    # Read the existing content of the Modelica file
    with open(model_path, 'r') as file:
        modelica_code = file.read()

    # Regular expression to find constant Integer declarations
    pattern = r'constant\s+Integer\s+(\w+)\s*=\s*(\d+);'

    # Find all matches in the Modelica code
    matches = re.findall(pattern, modelica_code)

    # Populate the dictionary with variable names and their integer values
    for variable_name, value in matches:
        constant_integers[variable_name] = int(value)

    return constant_integers

def parameters(model):
    """
    Read a JSON file containing a list of parameter records with values that are either 
    a double or a list of doubles.

    :param model: Path to the JSON file to read.
    :return: A list of dictionaries with the parsed data.
    :raises ValueError: If any value is not a double or a list of doubles.
    """
    
    model_path = fh.get_full_path(model)

    # Initialize an empty list to store the records
    records = []

    # Read the JSON file
    with open(model_path, 'r') as file:
        data = json.load(file)

    # Validate and parse the records
    for record in data:
        parsed_record = {}
        for key, value in record.items():
            if isinstance(value, (float, int)):  # Check if value is a double (float or int)
                parsed_record[key] = float(value)
            elif isinstance(value, list):  # Check if value is a list
                if all(isinstance(item, (float, int)) for item in value):  # Ensure all items are doubles
                    parsed_record[key] = [float(item) for item in value]
                else:
                    raise ValueError(f"Invalid value in list for key '{key}': {value}")
            else:
                raise ValueError(f"Invalid value for key '{key}': {value}")
        
        records.append(parsed_record)

    return records

def set_parameters(model, parameters):
    """
    Generate a JSON parameters file with values that are either a double or a list of doubles.

    :param model: Path to the JSON file to write.
    :param parameters: A list of dictionaries containing the records to write.
    :raises ValueError: If any value in the records is not a double or a list of doubles.
    """

    for record in parameters:
        for key, value in record.items():
            if not (isinstance(value, (float, int)) or
                    (isinstance(value, list) and all(isinstance(item, (float, int)) for item in value))):
                raise ValueError(f"Invalid value for key '{key}': {value}")

    model_path = fh.get_full_path(model)

    # Write the records to the JSON file
    with open(model_path, 'w') as file:
        json.dump(parameters, file, indent=4)  # Use indent for pretty printing


def config(model):
    """
    Reads the model configuration from a given INI file and returns its contents as a dictionary.

    :param model: The name of the INI file to read.
    :return: A dictionary containing the configuration key-value pairs.
    """
    config = configparser.ConfigParser()

    config_path = fh.get_full_path(model, 'MMOC_BUILD')

    # Read the INI file
    with open(config_path, 'r') as file:
        first_line = file.readline().strip()
        # Check if the first line is empty or does not start with a section header
        if not first_line.startswith('['):
            # If no section header, prepend a default section header
            file.seek(0)  # Reset file pointer to the beginning
            content = '[QSS_SOLVER]\n' + file.read()  # Add the section header
            with open(config_path, 'w') as write_file:
                write_file.write(content)  # Write back the modified content

    # Read the INI file
    config.read(config_path)

    # Convert the ConfigParser object to a dictionary
    ini_dict = {key: value for key, value in config['QSS_SOLVER'].items()}

    # Remove trailing semicolons from the values
    ini_dict = {key: value.rstrip(';') for key, value in ini_dict.items()}

    return ini_dict

def set_config(model, data_dict):
    """
    Writes a dictionary with ini file info to an INI file.

    :param model: The name or path of the INI file to write.
    :param data_dict: A dictionary containing the configuration data to write.
    """
    config = configparser.ConfigParser()

    # Add a section named 'QSS_SOLVER' and populate it with the dictionary items
    config['QSS_SOLVER'] = {key: f"{value};" for key, value in data_dict.items()}

    config_path = fh.get_full_path(model, 'MMOC_BUILD')

    # Write the configuration to the specified file
    with open(config_path, 'w') as configfile:
        config.write(configfile)
