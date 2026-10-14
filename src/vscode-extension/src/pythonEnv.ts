import { spawn } from 'child_process';
import * as fs from 'fs';
import * as path from 'path';
import * as vscode from 'vscode';
import { output } from './outputChannel';

const VENV_DIR = '.qss-solver-venv';

let cachedVenvPython: string | undefined;

function venvPythonPath(binDir: string): string {
  return path.join(binDir, VENV_DIR, 'bin', 'python3');
}

function run(cmd: string, args: string[], cwd: string): Promise<number> {
  return new Promise((resolve, reject) => {
    output.appendLine(`\n$ ${cmd} ${args.join(' ')}`);
    const child = spawn(cmd, args, { cwd });
    child.stdout?.on('data', (d) => output.append(d.toString()));
    child.stderr?.on('data', (d) => output.append(d.toString()));
    child.on('error', reject);
    child.on('close', (code) => resolve(code ?? -1));
  });
}

/**
 * Mirrors bin/python_console_setup.sh (which mmomegui.cpp's INIT_PYTHON_VENV sends to its
 * embedded terminals before every plot): creates bin/.qss-solver-venv if it doesn't exist yet
 * and installs bin/requirements.txt into it, so plot_data.py's matplotlib/pandas/etc.
 * dependencies are available without needing them in the system Python. The resolved
 * interpreter is cached for the rest of the session so this only runs once.
 */
export async function ensurePlotPython(binDir: string, bootstrapPython: string): Promise<string> {
  if (cachedVenvPython) {
    return cachedVenvPython;
  }
  const venvPython = venvPythonPath(binDir);
  const activateScript = path.join(binDir, VENV_DIR, 'bin', 'activate');

  await vscode.window.withProgress(
    { location: vscode.ProgressLocation.Notification, title: 'QSS Solver: preparing Python plotting environment', cancellable: false },
    async (progress) => {
      if (!fs.existsSync(activateScript)) {
        progress.report({ message: `Creating ${VENV_DIR}...` });
        const code = await run(bootstrapPython, ['-m', 'venv', VENV_DIR], binDir);
        if (code !== 0) {
          throw new Error(`Failed to create the Python virtual environment (exit code ${code}). See the "QSS Solver" output channel.`);
        }
      }
      const requirements = path.join(binDir, 'requirements.txt');
      if (fs.existsSync(requirements)) {
        progress.report({ message: 'Installing plotting dependencies (matplotlib, pandas, ...)...' });
        const code = await run(venvPython, ['-m', 'pip', 'install', '-r', requirements], binDir);
        if (code !== 0) {
          throw new Error(`Failed to install Python plotting dependencies (exit code ${code}). See the "QSS Solver" output channel.`);
        }
      }
    }
  );

  cachedVenvPython = venvPython;
  return venvPython;
}
