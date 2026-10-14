import * as vscode from 'vscode';

export const output = vscode.window.createOutputChannel('QSS Solver');

let progressItem: vscode.StatusBarItem | undefined;

function ensureProgressItem(): vscode.StatusBarItem {
  if (!progressItem) {
    progressItem = vscode.window.createStatusBarItem(vscode.StatusBarAlignment.Left, 100);
  }
  return progressItem;
}

export function showProgress(text: string): void {
  const item = ensureProgressItem();
  item.text = `$(sync~spin) ${text}`;
  item.show();
}

/** Parses stderr for a trailing "*<time>" marker, the same way mmomegui.cpp's simulation_message() does. */
export function reportSimulationProgress(chunk: string, startTime: number, stopTime: number): void {
  const lastMark = chunk.lastIndexOf('*');
  if (lastMark < 0) {
    return;
  }
  const time = parseFloat(chunk.slice(lastMark + 1));
  if (Number.isNaN(time)) {
    return;
  }
  const interval = stopTime - startTime;
  const percent = interval !== 0 ? Math.max(0, Math.min(100, Math.round((time / interval) * 100))) : 0;
  showProgress(`Simulating... ${percent}%`);
}

export function hideProgress(): void {
  progressItem?.hide();
}
