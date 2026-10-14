import * as vscode from 'vscode';

interface ActionEntry {
  label: string;
  command: string;
  icon: string;
  tooltip: string;
}

const ACTIONS: ActionEntry[] = [
  { label: 'Run Model...', command: 'qssSolver.run', icon: 'play', tooltip: 'Open the run/experiment settings and simulate the active model' },
  { label: 'Settings...', command: 'qssSolver.openSettings', icon: 'gear', tooltip: 'Open QSS Solver settings' },
  { label: 'Python Console', command: 'qssSolver.openPythonConsole', icon: 'terminal', tooltip: 'Open a Python console with qss_solver pre-imported' },
];

class ActionTreeItem extends vscode.TreeItem {
  constructor(entry: ActionEntry) {
    super(entry.label, vscode.TreeItemCollapsibleState.None);
    this.iconPath = new vscode.ThemeIcon(entry.icon);
    this.tooltip = entry.tooltip;
    this.command = { command: entry.command, title: entry.label };
  }
}

/** Static list of shortcut actions shown above the Simulation Results view in the QSS Solver activity bar panel. */
export class ActionsTreeProvider implements vscode.TreeDataProvider<ActionTreeItem> {
  getTreeItem(element: ActionTreeItem): vscode.TreeItem {
    return element;
  }

  getChildren(): ActionTreeItem[] {
    return ACTIONS.map((entry) => new ActionTreeItem(entry));
  }
}
