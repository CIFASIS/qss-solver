import * as path from 'path';
import * as vscode from 'vscode';

export interface AppDirs {
  models: string;
  output: string;
  build: string;
  engine: string;
  libs: string;
  bin: string;
  source: string;
  packages: string;
  libraries: string;
  include: string;
}

export interface AppCommands {
  compile: string;
  build: string;
  simulate: string;
  log: string;
  sbml: string;
}

const DEBUG_FLAG_KEYS: { key: string; flag: string }[] = [
  { key: 'all', flag: 'SD_DBG_All' },
  { key: 'initValues', flag: 'SD_DBG_InitValues' },
  { key: 'externalEvent', flag: 'SD_DBG_ExternalEvent' },
  { key: 'memory', flag: 'SD_DBG_Memory' },
  { key: 'synchronize', flag: 'SD_DBG_Synchronize' },
  { key: 'stepInfo', flag: 'SD_DBG_StepInfo' },
  { key: 'varChanges', flag: 'SD_DBG_VarChanges' },
  { key: 'waitFor', flag: 'SD_DBG_WaitFor' },
  { key: 'weights', flag: 'SD_DBG_Weights' },
  { key: 'dt', flag: 'SD_DBG_Dt' },
];

function workspaceRoot(resource?: vscode.Uri): string {
  const folder = resource ? vscode.workspace.getWorkspaceFolder(resource) : vscode.workspace.workspaceFolders?.[0];
  if (folder) {
    return folder.uri.fsPath;
  }
  if (vscode.workspace.workspaceFolders?.length) {
    return vscode.workspace.workspaceFolders[0].uri.fsPath;
  }
  throw new Error('QSS Solver: open a folder/workspace before using this command.');
}

export function resolveDir(resource: vscode.Uri | undefined, relative: string): string {
  return path.isAbsolute(relative) ? relative : path.join(workspaceRoot(resource), relative);
}

export function getDirs(resource?: vscode.Uri): AppDirs {
  const cfg = vscode.workspace.getConfiguration('qssSolver.dirs', resource);
  const resolve = (key: string, fallback: string) => resolveDir(resource, cfg.get<string>(key, fallback));
  return {
    models: resolve('models', 'models'),
    output: resolve('output', 'output'),
    build: resolve('build', 'build'),
    engine: resolve('engine', 'src/engine'),
    libs: resolve('libs', 'lib'),
    bin: resolve('bin', 'bin'),
    source: resolve('source', 'src'),
    packages: resolve('packages', 'packages'),
    libraries: resolve('libraries', 'lib'),
    include: resolve('include', 'src/usr/include'),
  };
}

export function getCommands(resource?: vscode.Uri): AppCommands {
  const cfg = vscode.workspace.getConfiguration('qssSolver.commands', resource);
  return {
    compile: cfg.get('compile', 'mmoc.sh'),
    build: cfg.get('build', 'build.sh'),
    simulate: cfg.get('simulate', 'simulate.sh'),
    log: cfg.get('log', 'createLog.sh'),
    sbml: cfg.get('sbml', 'sbml.sh'),
  };
}

export function getPlotConfig(resource?: vscode.Uri): { script: string; pythonPath: string; markers: boolean } {
  const cfg = vscode.workspace.getConfiguration('qssSolver.plot', resource);
  return {
    script: cfg.get('script', 'plot_data.py'),
    pythonPath: cfg.get('pythonPath', 'python3'),
    markers: cfg.get('markers', false),
  };
}

export function getExtraFlags(resource?: vscode.Uri): string {
  return vscode.workspace.getConfiguration('qssSolver.compiler', resource).get('extraFlags', '');
}

/** Builds the -d <flags> value the same way the desktop GUI's debug flag checkboxes do. */
export function getDebugFlags(resource?: vscode.Uri): string | undefined {
  const cfg = vscode.workspace.getConfiguration('qssSolver.debug', resource);
  if (!cfg.get<boolean>('enabled', false)) {
    return undefined;
  }
  const active = DEBUG_FLAG_KEYS.filter((f) => cfg.get<boolean>(f.key, false)).map((f) => f.flag);
  return active.length ? active.join('|') : undefined;
}

/** Builds the MMOC_* environment variables shared by mmoc.sh/build.sh/simulate.sh, mirroring mmomegui.cpp's compile()/run(). */
export function buildEnv(dirs: AppDirs): NodeJS.ProcessEnv {
  return {
    ...process.env,
    MMOC_BUILD: dirs.build,
    MMOC_BIN: dirs.bin,
    MMOC_ENGINE: dirs.engine,
    MMOC_LIBS: dirs.libs,
    MMOC_MODELS: dirs.models,
    MMOC_SRC: dirs.source,
    MMOC_PACKAGES: dirs.packages,
    MMOC_LIBRARIES: dirs.libraries,
    MMOC_INCLUDE: dirs.include,
    MMOC_OUTPUT: dirs.output,
    LD_LIBRARY_PATH: [path.join(dirs.bin, 'lib'), process.env.LD_LIBRARY_PATH].filter(Boolean).join(':'),
  };
}
