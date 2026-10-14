import * as vscode from 'vscode';
import { importSbml } from './pipeline';
import { output } from './outputChannel';

export async function importSbmlCommand(): Promise<void> {
  const picked = await vscode.window.showOpenDialog({
    canSelectMany: false,
    filters: { 'SBML models': ['xml'] },
    openLabel: 'Import SBML Model',
  });
  if (!picked || !picked.length) return;

  output.show(true);
  output.appendLine(`\nImporting SBML model: ${picked[0].fsPath}`);
  const generated = await importSbml(picked[0].fsPath);
  if (!generated) {
    vscode.window.showErrorMessage('QSS Solver: SBML import failed. See the QSS Solver output channel for details.');
    return;
  }
  const doc = await vscode.workspace.openTextDocument(generated);
  await vscode.window.showTextDocument(doc);
}
