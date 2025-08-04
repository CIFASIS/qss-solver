venv_path=".qss-solver-venv" # Example path, change this
if [ -f "$venv_path/bin/activate" ]; then
    echo "Virtual env installed at $venv_path"
else
    echo "Installing virtual env at $venv_path"
    python3 -m venv .qss-solver-venv     
fi

source .qss-solver-venv/bin/activate

python3 -m pip install -r requirements.txt