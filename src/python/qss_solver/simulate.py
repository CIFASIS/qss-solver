import logging
import os
import subprocess

from . import file_handlers as fh

def compile_model(model_file, flags=''):
    """
    Compile the specified Modelica model.

    Parameters:
    - model_file: Path to the model file.
    - flags: Additional flags for compilation.
    """
    compile_cmd = os.path.join(os.environ['MMOC_BIN'], 'compile.sh')

    model_name = fh.get_file_name(model_file)
    model_path = fh.get_base_path(model_file)

    logging.info(f'Compiling model: {model_name}')
    try:
        subprocess.check_call([compile_cmd, model_name, model_path, flags])
        logging.info('Compilation done')
    except subprocess.CalledProcessError as e:
        logging.error(f"Compilation failed for {model_name}: {e}")
        return False
    return True

def compile_mmoc_model(model_file, flags=''):
    """
    Compile the specified Modelica model.

    Parameters:
    - model_file: Path to the model file.
    - flags: Additional flags for compilation.
    """
    mmoc_cmd = os.path.join(os.environ['MMOC_BIN'], 'mmoc.sh')

    model_name = fh.get_file_name(model_file)
    #model_path = fh.get_base_path(model_file)

    logging.info(f'Compiling model: {model_name}')
    try:
        if flags == '':
            comp_flags = '-o ' + flags
            subprocess.check_call([mmoc_cmd, comp_flags, model_file])
        else:
            subprocess.check_call([mmoc_cmd, model_file])
        logging.info('Compilation done')
    except subprocess.CalledProcessError as e:
        logging.error(f"Compilation failed for {model_name}: {e}")
        return False
    return True

def compile_c_model(model_file, flags=''):
    """
    Compile the specified Modelica model.

    Parameters:
    - model_file: Path to the C model file.
    - flags: Additional flags for compilation.
    """
    build_cmd = os.path.join(os.environ['MMOC_BIN'], 'build.sh')

    model_name = fh.get_file_name(model_file)
    model_path = fh.get_base_path(model_file)

    logging.info(f'Compiling model: {model_name}')
    try:
        subprocess.check_call([build_cmd, model_name, model_path, flags])
        logging.info('Compilation done')
    except subprocess.CalledProcessError as e:
        logging.error(f"Compilation failed for {model_name}: {e}")
        return False
    return True

def execute_model(model_file):
    """
    Run the executable model given.

    Parameters:
    - model_file: Path to the model file.
    """

    model_name = fh.get_file_name(model_file)
    simulation_cmd = os.path.join(os.environ['MMOC_BIN'], 'simulate.sh')

    logging.info(f'Running executable model: {model_name}')
    
    try:
        subprocess.check_call([simulation_cmd, model_name, 'false' , 'false'])
        logging.info('Simulation done')
    except subprocess.CalledProcessError as e:
        logging.error(f"Simulation failed for {model_name}: {e}")
        return False
    
    return True

def run(model_file, flags=''):
    """
    Compile and run the executable model.

    Parameters:
    - model_file: Path to the model file.
    - flags: Additional compilation flags.
    """
    # Compile the model
    if compile_model(model_file, flags):
        # Execute the model if compilation was successful
        execute_model(model_file)
