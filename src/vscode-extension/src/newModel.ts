import * as fs from 'fs';
import * as path from 'path';
import * as vscode from 'vscode';
import { getDirs } from './config';

export async function newModel(): Promise<void> {
  const name = await vscode.window.showInputBox({
    prompt: 'Model name',
    validateInput: (value) => (/^[A-Za-z_]\w*$/.test(value) ? undefined : 'Use a valid MicroModelica identifier (letters, digits, _).'),
  });
  if (!name) return;

  const dirs = getDirs();
  fs.mkdirSync(dirs.models, { recursive: true });
  const filePath = path.join(dirs.models, `${name}.mo`);
  if (fs.existsSync(filePath)) {
    vscode.window.showErrorMessage(`QSS Solver: ${filePath} already exists.`);
    return;
  }

  const skeleton = `model ${name}\nequation\n\nend ${name};\n`;
  fs.writeFileSync(filePath, skeleton);
  const doc = await vscode.workspace.openTextDocument(filePath);
  await vscode.window.showTextDocument(doc);
}
