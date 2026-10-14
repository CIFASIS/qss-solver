import * as vscode from 'vscode';
import { ExperimentSettings, findModelName, readAnnotations, writeAnnotations, EMPTY_SETTINGS } from './annotations';
import { compileBuildRun } from './pipeline';
import { output } from './outputChannel';

const SOLVER_FAMILIES: Record<string, string[]> = {
  qss1: ['QSS1', 'EQSS1', 'CHEQSS1', 'LIQSS1'],
  qss2: ['QSS2', 'EQSS2', 'CHEQSS2', 'LIQSS2', 'LIQSS_BDF'],
  qss3: ['QSS3', 'EQSS3', 'CHEQSS3', 'LIQSS3'],
  qss4: ['QSS4'],
  classic: ['DASSL', 'DOPRI', 'CVODE_BDF', 'CVODE_AM', 'IDA'],
  experimental: ['mLIQSS', 'mLIQSS2', 'CQSS1', 'CQSS2', 'CQSS3'],
};

function familyForSolver(solver: string): string {
  for (const [family, solvers] of Object.entries(SOLVER_FAMILIES)) {
    if (solvers.includes(solver)) return family;
  }
  return 'qss2';
}

interface RunFormResult {
  settings: ExperimentSettings;
  semiStaticPartitioning: boolean;
  debugEnabled: boolean;
}

/** Webview form giving full field parity with the desktop GUI's RunDlg (src/gui/runform.hpp/cpp, ui/run.ui). */
export class RunSettingsPanel {
  private static current: RunSettingsPanel | undefined;

  private readonly panel: vscode.WebviewPanel;
  private readonly document: vscode.TextDocument;

  static async show(document: vscode.TextDocument): Promise<void> {
    if (RunSettingsPanel.current) {
      RunSettingsPanel.current.panel.dispose();
    }
    RunSettingsPanel.current = new RunSettingsPanel(document);
  }

  private constructor(document: vscode.TextDocument) {
    this.document = document;
    this.panel = vscode.window.createWebviewPanel('qssSolverRun', `Run: ${document.fileName.split('/').pop()}`, vscode.ViewColumn.Beside, {
      enableScripts: true,
      retainContextWhenHidden: true,
    });

    const modelName = findModelName(document.getText()) ?? 'Model';
    const { values } = readAnnotations(document.getText());
    const settings: ExperimentSettings = { ...EMPTY_SETTINGS, ...values };

    this.panel.webview.html = renderHtml(this.panel.webview, modelName, settings);
    this.panel.webview.onDidReceiveMessage((message) => this.handleMessage(message));
    this.panel.onDidDispose(() => {
      if (RunSettingsPanel.current === this) {
        RunSettingsPanel.current = undefined;
      }
    });
  }

  private async handleMessage(message: { type: 'save' | 'run'; payload: RunFormResult }): Promise<void> {
    const modelName = findModelName(this.document.getText());
    if (!modelName) {
      vscode.window.showErrorMessage('QSS Solver: could not find "model <Name>" in this file.');
      return;
    }
    const { fixed } = readAnnotations(this.document.getText());
    const newText = writeAnnotations(this.document.getText(), modelName, message.payload.settings, fixed);

    const edit = new vscode.WorkspaceEdit();
    const fullRange = new vscode.Range(this.document.positionAt(0), this.document.positionAt(this.document.getText().length));
    edit.replace(this.document.uri, fullRange, newText);
    await vscode.workspace.applyEdit(edit);
    await this.document.save();

    if (message.type === 'run') {
      output.show(true);
      await compileBuildRun(this.document.fileName, message.payload.settings, message.payload.semiStaticPartitioning, message.payload.debugEnabled);
    } else {
      vscode.window.showInformationMessage('QSS Solver: simulation settings saved.');
    }
  }
}

function opts(values: string[], selected: string): string {
  return values.map((v) => `<option value="${v}" ${v === selected ? 'selected' : ''}>${v}</option>`).join('');
}

function nonce(): string {
  let text = '';
  const chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
  for (let i = 0; i < 32; i++) {
    text += chars.charAt(Math.floor(Math.random() * chars.length));
  }
  return text;
}

export function renderHtml(webview: vscode.Webview, modelName: string, s: ExperimentSettings): string {
  const nonceValue = nonce();
  const csp = `default-src 'none'; style-src ${webview.cspSource} 'unsafe-inline'; script-src 'nonce-${nonceValue}';`;
  const family = familyForSolver(s.solver || 'QSS2');
  const familyOptions = opts(
    ['qss1', 'qss2', 'qss3', 'qss4', 'classic', 'experimental'].map((k) => k),
    family
  )
    .replace('value="qss1"', 'value="qss1" data-label="QSS order 1"')
    .replace('value="qss2"', 'value="qss2" data-label="QSS order 2"')
    .replace('value="qss3"', 'value="qss3" data-label="QSS order 3"')
    .replace('value="qss4"', 'value="qss4" data-label="QSS order 4"')
    .replace('value="classic"', 'value="classic" data-label="Classic"')
    .replace('value="experimental"', 'value="experimental" data-label="Experimental"');

  return /* html */ `<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8" />
<meta http-equiv="Content-Security-Policy" content="${csp}" />
<style>
  body { font-family: var(--vscode-font-family); color: var(--vscode-foreground); padding: 12px 16px 32px; }
  h2 { margin-top: 0; }
  fieldset { border: 1px solid var(--vscode-panel-border); border-radius: 4px; margin-bottom: 12px; }
  legend { padding: 0 6px; font-weight: 600; }
  .row { display: flex; align-items: center; gap: 8px; margin: 6px 0; flex-wrap: wrap; }
  .row label { min-width: 150px; display: inline-block; }
  input[type="text"], input[type="number"], select, textarea {
    background: var(--vscode-input-background); color: var(--vscode-input-foreground);
    border: 1px solid var(--vscode-input-border, transparent); border-radius: 2px; padding: 2px 6px;
  }
  textarea { width: 100%; min-height: 40px; }
  #advanced.hidden { display: none; }
  .actions { position: sticky; bottom: 0; background: var(--vscode-editor-background); padding: 10px 0; display: flex; gap: 8px; }
  button { background: var(--vscode-button-background); color: var(--vscode-button-foreground); border: none; padding: 6px 14px; border-radius: 2px; cursor: pointer; }
  button.secondary { background: var(--vscode-button-secondaryBackground); color: var(--vscode-button-secondaryForeground); }
</style>
</head>
<body>
<h2>Run "${modelName}"</h2>

<fieldset>
  <legend>General</legend>
  <div class="row"><label for="startTime">Start time</label><input type="text" id="startTime" value="${s.startTime}"></div>
  <div class="row"><label for="stopTime">Stop time</label><input type="text" id="stopTime" value="${s.stopTime}"></div>
  <div class="row"><label for="tolerance">Tolerance</label><input type="text" id="tolerance" value="${s.tolerance}"></div>
  <div class="row"><label for="absTolerance">Absolute tolerance</label><input type="text" id="absTolerance" value="${s.absTolerance}"></div>
  <div class="row">
    <label for="solverFamily">Solver family</label>
    <select id="solverFamily">${familyOptions}</select>
    <select id="solver"></select>
  </div>
  <div class="row"><label for="scheduler">Scheduler</label>
    <select id="scheduler">${opts(['ST_Binary', 'ST_Random', 'ST_Linear'], s.scheduler)}</select>
  </div>
  <div class="row"><label for="outputType">Output type</label>
    <select id="outputType">${opts(['CI_Step', 'CI_Sampled', 'CI_Dense'], s.outputType)}</select>
  </div>
  <div class="row"><label for="period">Period</label><input type="text" id="period" value="${s.period}"></div>
  <div class="row"><label for="output">Output</label><input type="text" id="output" value="${s.output}" placeholder="var1,var2,..."></div>
  <div class="row"><label for="symDiff">Symbolic differentiation</label>
    <select id="symDiff">${opts(['false', 'true'], s.symDiff)}</select>
  </div>
  <div class="row" style="align-items:flex-start"><label for="description">Description</label><textarea id="description">${s.description}</textarea></div>
  <div class="row"><label><input type="checkbox" id="showAll" checked> Show advanced/parallel settings</label></div>
</fieldset>

<div id="advanced">
  <fieldset>
    <legend>Numeric solver settings</legend>
    <div class="row"><label for="minStep">Min step</label><input type="text" id="minStep" value="${s.minStep}"></div>
    <div class="row"><label for="zcHyst">Zero-crossing hysteresis</label><input type="text" id="zcHyst" value="${s.zcHyst}"></div>
    <div class="row"><label for="derDelta">Derivative delta</label><input type="text" id="derDelta" value="${s.derDelta}"></div>
    <div class="row"><label for="jacobian">Jacobian</label><select id="jacobian">${opts(['Dense', 'Sparse'], s.jacobian)}</select></div>
    <div class="row"><label for="dtSynch">Dt synchronization</label>
      <select id="dtSynch">${opts(['SD_DT_Fixed', 'SD_DT_Asynchronous'], s.dtSynch)}</select>
      <label id="dtLabel" for="dtMin">Dt tolerance</label><input type="text" id="dtMin" value="${s.dtMin}">
    </div>
  </fieldset>

  <fieldset>
    <legend>Parallel simulation</legend>
    <div class="row"><label for="parallel">Parallel</label><select id="parallel">${opts(['false', 'true'], s.parallel)}</select></div>
    <div class="row"><label for="lps">LPS</label><input type="number" id="lps" min="1" value="${s.lps || 1}"></div>
    <div class="row"><label for="partitionMethod">Partition method</label>
      <select id="partitionMethod">${opts(['Metis', 'HMetis', 'Scotch', 'Patoh', 'Manual'], s.partitionMethod)}</select>
    </div>
    <div class="row"><label for="patohSettings">Patoh settings</label><input type="text" id="patohSettings" value="${s.patohSettings}"></div>
    <div class="row"><label for="scotchSettings">Scotch settings</label><input type="text" id="scotchSettings" value="${s.scotchSettings}"></div>
    <div class="row"><label for="metisSettings">Metis settings</label><input type="text" id="metisSettings" value="${s.metisSettings}"></div>
    <div class="row"><label for="imbalance">Imbalance</label><input type="text" id="imbalance" value="${s.imbalance}"></div>
    <div class="row"><label><input type="checkbox" id="semiStaticPartitioning"> Semi-static partitioning (weights)</label></div>
    <div class="row"><label for="reorderPartition">Reorder partition</label><select id="reorderPartition">${opts(['false', 'true'], s.reorderPartition)}</select></div>
  </fieldset>

  <fieldset>
    <legend>BDF partitioning</legend>
    <div class="row"><label for="bdfPart">BDF partition</label><input type="text" id="bdfPart" value="${s.bdfPart}"></div>
    <div class="row"><label for="bdfPartitionDepth">Partition depth</label><input type="number" id="bdfPartitionDepth" min="0" value="${s.bdfPartitionDepth || 0}"></div>
    <div class="row"><label for="bdfMaxStep">Max step</label><input type="text" id="bdfMaxStep" value="${s.bdfMaxStep}"></div>
  </fieldset>

  <fieldset>
    <legend>Debug</legend>
    <div class="row"><label><input type="checkbox" id="debugEnabled"> Compile with debug flags (Settings &gt; qssSolver.debug.*)</label></div>
  </fieldset>
</div>

<div class="actions">
  <button id="runBtn">Run</button>
  <button class="secondary" id="saveBtn">Save</button>
</div>

<script nonce="${nonceValue}">
const vscode = acquireVsCodeApi();
const SOLVER_FAMILIES = ${JSON.stringify(SOLVER_FAMILIES)};
const initialSolver = ${JSON.stringify(s.solver || 'QSS2')};

function populateSolvers() {
  const family = document.getElementById('solverFamily').value;
  const solverSelect = document.getElementById('solver');
  const list = SOLVER_FAMILIES[family];
  solverSelect.innerHTML = list.map(v => '<option value="' + v + '">' + v + '</option>').join('');
  if (list.includes(initialSolver)) {
    solverSelect.value = initialSolver;
  }
}
document.getElementById('solverFamily').addEventListener('change', populateSolvers);
populateSolvers();

document.getElementById('showAll').addEventListener('change', (e) => {
  document.getElementById('advanced').classList.toggle('hidden', !e.target.checked);
});

document.getElementById('outputType').addEventListener('change', (e) => {
  document.getElementById('period').disabled = (e.target.value === 'CI_Step');
});
document.getElementById('period').disabled = (document.getElementById('outputType').value === 'CI_Step');

document.getElementById('dtSynch').addEventListener('change', (e) => {
  document.getElementById('dtLabel').textContent = (e.target.value === 'SD_DT_Fixed') ? 'Dt tolerance' : 'Dt value';
});

function setParallelEnabled(enabled) {
  ['lps','partitionMethod','patohSettings','scotchSettings','metisSettings','imbalance',
   'semiStaticPartitioning','reorderPartition','dtMin','dtSynch']
    .forEach(id => { document.getElementById(id).disabled = !enabled; });
}
document.getElementById('parallel').addEventListener('change', (e) => setParallelEnabled(e.target.value === 'true'));
setParallelEnabled(document.getElementById('parallel').value === 'true');

function collect() {
  const val = (id) => document.getElementById(id).value;
  const checked = (id) => document.getElementById(id).checked;
  return {
    settings: {
      description: val('description'),
      solver: val('solver'),
      startTime: val('startTime'),
      stopTime: val('stopTime'),
      tolerance: val('tolerance'),
      absTolerance: val('absTolerance'),
      minStep: val('minStep'),
      zcHyst: val('zcHyst'),
      derDelta: val('derDelta'),
      symDiff: val('symDiff'),
      period: val('period'),
      parallel: val('parallel'),
      partitionMethod: val('partitionMethod'),
      lps: val('lps'),
      dtSynch: val('dtSynch'),
      dtSynchPeriod: '',
      dtStepLog: '',
      dtMin: val('dtMin'),
      output: val('output'),
      outputType: val('outputType'),
      scheduler: val('scheduler'),
      patohSettings: val('patohSettings'),
      scotchSettings: val('scotchSettings'),
      metisSettings: val('metisSettings'),
      jacobian: val('jacobian'),
      imbalance: val('imbalance'),
      reorderPartition: val('reorderPartition'),
      bdfPart: val('bdfPart'),
      bdfPartitionDepth: val('bdfPartitionDepth'),
      bdfMaxStep: val('bdfMaxStep'),
    },
    semiStaticPartitioning: checked('semiStaticPartitioning'),
    debugEnabled: checked('debugEnabled'),
  };
}

document.getElementById('saveBtn').addEventListener('click', () => {
  vscode.postMessage({ type: 'save', payload: collect() });
});
document.getElementById('runBtn').addEventListener('click', () => {
  vscode.postMessage({ type: 'run', payload: collect() });
});
</script>
</body>
</html>`;
}
