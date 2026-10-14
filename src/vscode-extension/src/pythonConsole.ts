import * as vscode from 'vscode';
import { buildEnv, getDirs } from './config';

let terminal: vscode.Terminal | undefined;

/**
 * Opens an integrated terminal that sets up/activates bin/.qss-solver-venv and starts a
 * Python REPL with the qss_solver package pre-imported, mirroring mmomegui.cpp's
 * INIT_PYTHON_VENV + INIT_PYTHON constants (mmomegui.cpp:43-59) sent to its embedded
 * Python console. Reuses the terminal (just refocusing it) if one is already running.
 */
export function openPythonConsole(): void {
  const dirs = getDirs();
  const isNew = !terminal || terminal.exitStatus !== undefined;
  if (isNew) {
    terminal = vscode.window.createTerminal({
      name: 'QSS Solver Python Console',
      cwd: dirs.bin,
      env: buildEnv(dirs),
    });
  }
  terminal!.show();
  if (!isNew) {
    return;
  }

  terminal!.sendText('./python_console_setup.sh');
  terminal!.sendText('source .qss-solver-venv/bin/activate');
  terminal!.sendText('clear');
  terminal!.sendText('python3');
  terminal!.sendText('import os');
  terminal!.sendText('import sys');
  terminal!.sendText("MMOC_SRC = os.environ['MMOC_SRC']");
  terminal!.sendText("sys.path.append(MMOC_SRC+'/python')");
  terminal!.sendText("sys.path.append(MMOC_SRC+'/python/qss_solver')");
  terminal!.sendText('import qss_solver');
  terminal!.sendText('import qss_solver.results as solver_results');
  terminal!.sendText('import qss_solver.model as solver_model');
  terminal!.sendText('import qss_solver.simulate as solver_sim');
  terminal!.sendText("c = os.system('clear')");
}
