import * as path from 'path';
import * as vscode from 'vscode';
import { getDirs } from './config';
import { output } from './outputChannel';
import * as pipeline from './pipeline';
import { newModel } from './newModel';
import { importSbmlCommand } from './sbmlImport';
import { RunSettingsPanel } from './runSettingsPanel';
import { ResultsTreeProvider } from './resultsTreeProvider';
import { ActionsTreeProvider } from './actionsTreeProvider';
import { plotResults } from './plot';
import { openPythonConsole } from './pythonConsole';

const DOCS = {
  mmoc: 'https://www.cifasis-conicet.gov.ar/jfernandez/mmoc/',
  engine: 'https://www.cifasis-conicet.gov.ar/jfernandez/qss-engine/',
  sbml: 'https://www.cifasis-conicet.gov.ar/jfernandez/sbml/',
};

// Match by extension too, not just languageId: a tab opened before this extension was
// installed/enabled keeps whatever language mode VS Code guessed at the time (often
// Plain Text) until the user reopens it or changes the mode explicitly.
function isModelDocument(doc: vscode.TextDocument | undefined): doc is vscode.TextDocument {
  return !!doc && (doc.languageId === 'micromodelica' || doc.fileName.toLowerCase().endsWith('.mo'));
}

function reportOutcome(ok: boolean, what: string): void {
  if (ok) {
    vscode.window.showInformationMessage(`QSS Solver: ${what} succeeded.`);
  } else {
    vscode.window.showErrorMessage(`QSS Solver: ${what} failed. See the "QSS Solver" output channel for details.`);
  }
}

function activeModelDocument(): vscode.TextDocument | undefined {
  const doc = vscode.window.activeTextEditor?.document;
  if (isModelDocument(doc)) {
    return doc;
  }
  vscode.window.showErrorMessage('QSS Solver: open a MicroModelica (.mo) file first.');
  return undefined;
}

export function activate(context: vscode.ExtensionContext): void {
  const resultsProvider = new ResultsTreeProvider();
  let currentModelName: string | undefined;

  const treeView = vscode.window.createTreeView('qssSolver.results', {
    treeDataProvider: resultsProvider,
  });
  treeView.onDidChangeCheckboxState((e) => resultsProvider.handleCheckboxChange(e));

  context.subscriptions.push(vscode.window.registerTreeDataProvider('qssSolver.actions', new ActionsTreeProvider()));

  function refreshResultsFor(modelFile: string): void {
    const dirs = getDirs(vscode.Uri.file(modelFile));
    currentModelName = pipeline.baseName(modelFile);
    resultsProvider.setModelDir(path.join(dirs.output, currentModelName));
  }

  context.subscriptions.push(
    vscode.window.onDidChangeActiveTextEditor((editor) => {
      if (isModelDocument(editor?.document)) {
        refreshResultsFor(editor!.document.fileName);
      }
    }),
    pipeline.onSimulationFinished.event((modelFile) => refreshResultsFor(modelFile)),
    treeView,
    output
  );

  if (isModelDocument(vscode.window.activeTextEditor?.document)) {
    refreshResultsFor(vscode.window.activeTextEditor!.document.fileName);
  }

  const commands: [string, (...args: any[]) => any][] = [
    ['qssSolver.newModel', newModel],

    ['qssSolver.run', async () => {
      const doc = activeModelDocument();
      if (!doc) return;
      await RunSettingsPanel.show(doc);
    }],

    ['qssSolver.compile', async () => {
      const doc = activeModelDocument();
      if (!doc) return;
      output.show(true);
      const ok = await pipeline.compileModel(doc.fileName);
      reportOutcome(ok, `Compilation of "${pipeline.baseName(doc.fileName)}"`);
    }],

    ['qssSolver.compileSettingsOnly', async () => {
      const doc = activeModelDocument();
      if (!doc) return;
      output.show(true);
      const ok = await pipeline.compileModel(doc.fileName, { settingsOnly: true });
      reportOutcome(ok, `Settings-only compilation of "${pipeline.baseName(doc.fileName)}"`);
    }],

    ['qssSolver.build', async () => {
      const doc = activeModelDocument();
      if (!doc) return;
      output.show(true);
      const ok = await pipeline.buildModel(doc.fileName);
      reportOutcome(ok, `Build of "${pipeline.baseName(doc.fileName)}"`);
    }],

    ['qssSolver.stop', () => pipeline.stop()],

    ['qssSolver.showLog', async () => {
      const doc = activeModelDocument();
      if (!doc) return;
      output.show(true);
      const logFile = await pipeline.mergeLog(doc.fileName);
      if (!logFile) {
        vscode.window.showInformationMessage('QSS Solver: no log files found for this model.');
        return;
      }
      const logDoc = await vscode.workspace.openTextDocument(logFile);
      await vscode.window.showTextDocument(logDoc);
    }],

    ['qssSolver.plot', () => plotResults(resultsProvider, currentModelName, context.extensionPath)],

    ['qssSolver.importSbml', importSbmlCommand],

    ['qssSolver.clearOutput', () => output.clear()],

    ['qssSolver.openSettings', () => vscode.commands.executeCommand('workbench.action.openSettings', 'qssSolver')],

    ['qssSolver.openPythonConsole', () => openPythonConsole()],

    ['qssSolver.refreshResults', () => {
      const doc = vscode.window.activeTextEditor?.document;
      if (isModelDocument(doc)) {
        refreshResultsFor(doc.fileName);
      } else {
        resultsProvider.refresh();
      }
    }],

    ['qssSolver.openMmocDocs', () => vscode.env.openExternal(vscode.Uri.parse(DOCS.mmoc))],
    ['qssSolver.openEngineDocs', () => vscode.env.openExternal(vscode.Uri.parse(DOCS.engine))],
    ['qssSolver.openSbmlDocs', () => vscode.env.openExternal(vscode.Uri.parse(DOCS.sbml))],
  ];

  for (const [id, handler] of commands) {
    context.subscriptions.push(vscode.commands.registerCommand(id, handler));
  }
}

export function deactivate(): void {
  if (pipeline.isRunning()) {
    pipeline.stop();
  }
}
