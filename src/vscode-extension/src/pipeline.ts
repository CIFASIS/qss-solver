import { ChildProcess, spawn } from 'child_process';
import * as fs from 'fs';
import * as path from 'path';
import * as vscode from 'vscode';
import { buildEnv, getCommands, getDebugFlags, getDirs, getExtraFlags } from './config';
import { hideProgress, output, reportSimulationProgress, showProgress } from './outputChannel';
import { ExperimentSettings } from './annotations';

let currentChild: ChildProcess | undefined;

/** Fires with the model file path after a simulation run completes successfully, so views can refresh. */
export const onSimulationFinished = new vscode.EventEmitter<string>();

export function baseName(modelFile: string): string {
  return path.basename(modelFile, path.extname(modelFile));
}

function run(cmd: string, args: string[], cwd: string, env: NodeJS.ProcessEnv, onStderr?: (chunk: string) => void): Promise<number> {
  return new Promise((resolve, reject) => {
    output.appendLine(`\n$ ${cmd} ${args.join(' ')}`);
    let child: ChildProcess;
    try {
      child = spawn(cmd, args, { cwd, env, detached: true });
    } catch (err) {
      reject(err);
      return;
    }
    currentChild = child;
    child.stdout?.on('data', (d) => output.append(d.toString()));
    child.stderr?.on('data', (d) => {
      const text = d.toString();
      output.append(text);
      onStderr?.(text);
    });
    child.on('error', (err) => {
      currentChild = undefined;
      reject(err);
    });
    child.on('close', (code) => {
      currentChild = undefined;
      resolve(code ?? -1);
    });
  });
}

export function isRunning(): boolean {
  return !!currentChild;
}

/** Kills the whole process group of the currently running child, mirroring the desktop GUI's Stop action. */
export function stop(): void {
  if (!currentChild?.pid) {
    output.appendLine('\nNo running QSS Solver process to stop.');
    return;
  }
  try {
    process.kill(-currentChild.pid, 'SIGKILL');
  } catch {
    currentChild.kill('SIGKILL');
  }
  output.appendLine('\nSimulation stopped by user.');
  hideProgress();
}

function cleanBuildDir(buildDir: string, name: string): void {
  for (const ext of ['.makefile', '.c', '']) {
    const file = path.join(buildDir, ext ? `${name}${ext}` : name);
    if (fs.existsSync(file)) {
      fs.rmSync(file, { force: true });
    }
  }
}

export interface CompileOptions {
  settingsOnly?: boolean;
  debug?: boolean;
}

export async function compileModel(modelFile: string, opts: CompileOptions = {}): Promise<boolean> {
  const dirs = getDirs(vscode.Uri.file(modelFile));
  const commands = getCommands(vscode.Uri.file(modelFile));
  const name = baseName(modelFile);
  const modelBuildDir = path.join(dirs.build, name);
  fs.mkdirSync(modelBuildDir, { recursive: true });

  const modelDir = path.dirname(modelFile);
  const partFile = path.join(modelDir, `${name}.part`);
  if (fs.existsSync(partFile)) {
    fs.copyFileSync(partFile, path.join(modelBuildDir, `${name}.part`));
  }
  if (!opts.settingsOnly) {
    cleanBuildDir(modelBuildDir, name);
  }

  const flagParts: string[] = [];
  if (opts.settingsOnly) flagParts.push('-s');
  const extra = getExtraFlags(vscode.Uri.file(modelFile));
  if (extra) flagParts.push(extra);
  if (opts.debug) {
    const dbgFlags = getDebugFlags(vscode.Uri.file(modelFile));
    if (dbgFlags) {
      flagParts.push(`-d ${dbgFlags}`);
    } else {
      vscode.window.showWarningMessage('Debug mode enabled but no debug flags are set. Configure qssSolver.debug.* settings.');
    }
  }
  flagParts.push(`-o ${path.join(modelBuildDir, name)}`);

  const env = buildEnv(dirs);
  output.appendLine(`\nCompiling model: ${modelFile}`);
  const code = await run(path.join(dirs.bin, commands.compile), [modelFile, flagParts.join(' ')], dirs.bin, env);
  if (code === 0) {
    output.appendLine('MicroModelica compilation completed successfully.');
    return true;
  }
  output.appendLine(`Compilation failed. Exit code: ${code}`);
  return false;
}

export async function buildModel(modelFile: string): Promise<boolean> {
  const dirs = getDirs(vscode.Uri.file(modelFile));
  const commands = getCommands(vscode.Uri.file(modelFile));
  const name = baseName(modelFile);
  const env = buildEnv(dirs);
  output.appendLine(`\nBuilding model: ${name}`);
  const code = await run(path.join(dirs.bin, commands.build), [name], dirs.bin, env);
  if (code === 0) {
    output.appendLine('Build completed successfully.');
    return true;
  }
  output.appendLine(`Build failed. Exit code: ${code}`);
  return false;
}

export async function runSimulation(modelFile: string, settings: ExperimentSettings, semiStaticPartitioning: boolean): Promise<boolean> {
  const dirs = getDirs(vscode.Uri.file(modelFile));
  const commands = getCommands(vscode.Uri.file(modelFile));
  const name = baseName(modelFile);
  const parallel = settings.parallel === 'true';

  fs.mkdirSync(path.join(dirs.output, name), { recursive: true });

  // Arg order mirrors mmomegui.cpp's run(): <name> <parallel> [<modelDir> if parallel] <weights>.
  // simulate.sh reads WEIGHTS from $4, so it only takes effect for parallel runs -- matching the
  // desktop GUI, semi-static weights have no effect on a sequential simulation.
  const args = [name, parallel ? 'true' : 'false'];
  if (parallel) {
    args.push(path.dirname(modelFile));
  }
  args.push(semiStaticPartitioning ? 'true' : 'false');

  const startTime = parseFloat(settings.startTime) || 0;
  const stopTime = parseFloat(settings.stopTime) || 0;
  const env = buildEnv(dirs);
  showProgress('Simulating... 0%');
  output.appendLine(`\nStarting simulation: ${name}`);
  const code = await run(path.join(dirs.bin, commands.simulate), args, dirs.bin, env, (chunk) =>
    reportSimulationProgress(chunk, startTime, stopTime)
  );
  hideProgress();
  if (code === 0) {
    output.appendLine('Simulation ended.');
    onSimulationFinished.fire(modelFile);
    return true;
  }
  output.appendLine(`Simulation failed. Exit code: ${code}`);
  return false;
}

/**
 * Compiles, builds and runs the model, reporting progress with a notification so it's
 * unmistakable that compilation actually happened even if the Output channel isn't in view.
 */
export async function compileBuildRun(
  modelFile: string,
  settings: ExperimentSettings,
  semiStaticPartitioning: boolean,
  debug: boolean
): Promise<boolean> {
  const name = baseName(modelFile);
  return vscode.window.withProgress(
    { location: vscode.ProgressLocation.Notification, title: `QSS Solver: ${name}`, cancellable: false },
    async (progress) => {
      progress.report({ message: 'Compiling...' });
      if (!(await compileModel(modelFile, { debug }))) {
        vscode.window.showErrorMessage(`QSS Solver: compilation of "${name}" failed. See the "QSS Solver" output channel for details.`);
        return false;
      }
      progress.report({ message: 'Building...' });
      if (!(await buildModel(modelFile))) {
        vscode.window.showErrorMessage(`QSS Solver: build of "${name}" failed. See the "QSS Solver" output channel for details.`);
        return false;
      }
      progress.report({ message: 'Running simulation...' });
      const ok = await runSimulation(modelFile, settings, semiStaticPartitioning);
      if (ok) {
        vscode.window.showInformationMessage(`QSS Solver: "${name}" simulation finished successfully.`);
      } else {
        vscode.window.showErrorMessage(`QSS Solver: simulation of "${name}" failed. See the "QSS Solver" output channel for details.`);
      }
      return ok;
    }
  );
}

export async function mergeLog(modelFile: string): Promise<string | undefined> {
  const dirs = getDirs(vscode.Uri.file(modelFile));
  const commands = getCommands(vscode.Uri.file(modelFile));
  const name = baseName(modelFile);
  const env = buildEnv(dirs);
  const code = await run(path.join(dirs.bin, commands.log), [name], dirs.bin, env);
  const logFile = path.join(dirs.output, name, `${name}.log`);
  if (code === 0 && fs.existsSync(logFile)) {
    return logFile;
  }
  return undefined;
}

export async function importSbml(sbmlFile: string): Promise<string | undefined> {
  const dirs = getDirs(vscode.Uri.file(sbmlFile));
  const commands = getCommands(vscode.Uri.file(sbmlFile));
  const name = baseName(sbmlFile);
  const targetDir = path.join(dirs.models, name);
  fs.mkdirSync(targetDir, { recursive: true });
  const targetFile = path.join(targetDir, `${name}.mo`);
  const env = buildEnv(dirs);
  const code = await run(path.join(dirs.bin, commands.sbml), [`-o ${targetFile}`, sbmlFile], dirs.bin, env);
  return code === 0 && fs.existsSync(targetFile) ? targetFile : undefined;
}
