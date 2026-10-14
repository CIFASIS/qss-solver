import { spawn } from 'child_process';
import * as path from 'path';
import * as vscode from 'vscode';
import { getDirs, getPlotConfig } from './config';
import { output } from './outputChannel';
import { ensurePlotPython } from './pythonEnv';
import { ResultsTreeProvider } from './resultsTreeProvider';

/**
 * Plots the checked .dat files with bin/plot_data.py (the same matplotlib script the
 * desktop GUI's "Python plot" backend shells out to, mmomegui.cpp:502-520,977-1011),
 * via a small bundled wrapper (pyscripts/plot_wrapper.py) that falls back to saving a
 * PNG when no display is available -- e.g. a headless VSCode Remote-SSH host, where the
 * native interactive matplotlib window plot_data.py normally opens can never appear.
 */
export async function plotResults(provider: ResultsTreeProvider, modelName: string | undefined, extensionPath: string): Promise<void> {
  const files = provider.getCheckedFiles();
  if (!files.length) {
    vscode.window.showInformationMessage('QSS Solver: check one or more variables in the Simulation Results view first.');
    return;
  }

  const title = modelName ?? 'Simulation Results';
  const xlabel = 'Time';
  const ylabel = 'Value';

  const dirs = getDirs();
  const plot = getPlotConfig();

  let pythonPath: string;
  try {
    pythonPath = await ensurePlotPython(dirs.bin, plot.pythonPath);
  } catch (err) {
    vscode.window.showErrorMessage(`QSS Solver: ${(err as Error).message}`);
    return;
  }

  const scriptPath = path.join(dirs.bin, plot.script);
  const wrapperPath = path.join(extensionPath, 'pyscripts', 'plot_wrapper.py');
  const args = [wrapperPath, scriptPath, ...files, title, xlabel, ylabel];
  if (plot.markers) {
    args.push('--plot_markers');
  }

  output.appendLine(`\n$ ${pythonPath} ${args.join(' ')}`);
  const child = spawn(pythonPath, args, { cwd: dirs.bin, detached: true, stdio: ['ignore', 'pipe', 'pipe'] });
  let stdout = '';
  child.stdout?.on('data', (d) => {
    const text = d.toString();
    stdout += text;
    output.append(text);
  });
  child.stderr?.on('data', (d) => output.append(d.toString()));
  child.on('error', (err) => vscode.window.showErrorMessage(`QSS Solver: failed to launch plot script (${err.message}).`));
  child.on('close', (code) => {
    const saved = /^SAVED:(.+)$/m.exec(stdout);
    if (saved) {
      const savedPath = saved[1].trim();
      vscode.window
        .showInformationMessage(`QSS Solver: no display available here, so the plot was saved and opened as an image (${savedPath}).`);
      vscode.commands.executeCommand('vscode.open', vscode.Uri.file(savedPath));
    } else if (code !== 0) {
      vscode.window.showErrorMessage('QSS Solver: plotting failed. See the "QSS Solver" output channel for details.');
    }
  });
  child.unref();
}
