import logging
import os
from pathlib import Path

# Configure logging
logging.basicConfig(level=logging.ERROR, format='%(levelname)s - %(message)s')

CONFIG = {
    'MMOC_MODELS': {
        'path': os.environ['MMOC_MODELS'],
        'ext': ".mo"
    },
    'MMOC_LOG': {
        'path': os.environ['MMOC_OUTPUT'],
        'ext': ".log"
    },
    'MMOC_BUILD': {
        'path': os.environ['MMOC_BUILD'],
        'ext': ".ini"
    }
}

def has_extension(file_name):
    """
    Check if the given file name has an extension.

    Parameters:
    - file_name: The file name to check.

    Returns:
    - True if the file name has an extension, False otherwise.
    """
    # Check if the file name has an extension
    return os.path.splitext(file_name)[1] != ''

def get_base_name(file_path):
    """
    Get the base path of the given file.

    Parameters:
    - file_path: Path to the file.

    Returns:
    - The base path (directory) of the file.
    """
    # Use os.path.splitext to split the file name and extension
    file_name, _ = os.path.splitext(os.path.basename(file_path))
    return file_name


def find_model_file(rel_path, config_entry):
    """Looks for a .mo file relative to MMOC_MODELS directory first.
    
    Args:
        rel_path (str): The relative path to the model file.
    
    Returns:
        Path | None: The path to the model file if found, otherwise None.
    """
    logging.debug(f"Searching for model file with relative path: {rel_path}")
    
    # Create a Path object from the relative path
    path = Path(rel_path)

    # Get the base file name without extension
    base_file_name = path.stem
    logging.debug(f"Base file name without extension: {path.parent}")
    
    if path.parent == Path('.'):
        model_path = base_file_name
    else:
        model_path = (path.parent).relative_to('')
    
    # Construct the model directory path
    model_directory = Path(CONFIG[config_entry]['path']) / model_path / f"{base_file_name}{CONFIG[config_entry]['ext']}"
    logging.debug(f"Constructed model directory path: {model_directory}")

    # Check if the file exists in the models directory
    if model_directory.is_file():
        logging.debug(f"Model file found: {model_directory}")
        return model_directory

    # Construct the model directory path
    model_directory = Path(CONFIG[config_entry]['path']) / f"{base_file_name}{CONFIG[config_entry]['ext']}"
    logging.debug(f"Constructed model directory path: {model_directory}")

    # Check if the file exists in the models directory
    if model_directory.is_file():
        logging.debug(f"Model file found: {model_directory}")
        return model_directory

    # If the file is not found, return None
    return None

def is_relative_path(file_name):
    """
    Check if the given file name has a relative path.

    Parameters:
    - file_name: The file name to check.

    Returns:
    - True if the file name is a relative path, False otherwise.
    """
    # Check if the file name is relative
    return not os.path.isabs(file_name)

import os

def get_full_path(file_path, config_entry='MMOC_MODELS'):
    """
    Get the full path of the given file.

    Parameters:
    - file_path: Path to the file.

    Returns:
    - The full path of the file.
    """
    
    if is_relative_path(file_path):
        file_path = find_model_file(file_path, config_entry)
    
    if file_path == None:
        return ""

    return os.path.abspath(file_path)

def get_base_path(file_path, config_entry='MMOC_MODELS'):
    """
    Get the base path of the given file.

    Parameters:
    - file_path: Path to the file.

    Returns:
    - The base path (directory) of the file.
    """
    return os.path.dirname(get_full_path(file_path, config_entry))

def get_file_name(file_path, config_entry='MMOC_MODELS'):
    """
    Get the file name without the extension.

    Parameters:
    - file_path: Path to the file.

    Returns:
    - The file name without the extension.
    """
    if is_relative_path(file_path):
        file_path = find_model_file(file_path, config_entry)
    
    if file_path == None:
        return ""
    return get_base_name(file_path)