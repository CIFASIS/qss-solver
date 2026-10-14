import * as fs from 'fs';
import * as path from 'path';
import * as vscode from 'vscode';

export class ResultVariableItem extends vscode.TreeItem {
  constructor(public readonly datFile: string, label: string) {
    super(label, vscode.TreeItemCollapsibleState.None);
    this.checkboxState = vscode.TreeItemCheckboxState.Unchecked;
    this.contextValue = 'qssSolverVariable';
    this.tooltip = datFile;
  }
}

/**
 * Lists the .dat files produced by a simulation run (output/<model>/*.dat) as
 * checkable items, replacing the desktop GUI's variable-selection tree
 * (src/gui/treemodel.cpp TreeModel::addFiles).
 */
export class ResultsTreeProvider implements vscode.TreeDataProvider<ResultVariableItem> {
  private readonly _onDidChangeTreeData = new vscode.EventEmitter<void>();
  readonly onDidChangeTreeData = this._onDidChangeTreeData.event;

  private modelDir: string | undefined;
  private readonly checked = new Set<string>();

  setModelDir(modelDir: string | undefined): void {
    this.modelDir = modelDir;
    this.checked.clear();
    this.refresh();
  }

  refresh(): void {
    this._onDidChangeTreeData.fire();
  }

  getTreeItem(element: ResultVariableItem): vscode.TreeItem {
    return element;
  }

  getChildren(): ResultVariableItem[] {
    if (!this.modelDir || !fs.existsSync(this.modelDir)) {
      return [];
    }
    return fs
      .readdirSync(this.modelDir)
      .filter((f) => f.endsWith('.dat'))
      .sort()
      .map((f) => {
        const full = path.join(this.modelDir!, f);
        const item = new ResultVariableItem(full, path.basename(f, '.dat'));
        item.checkboxState = this.checked.has(full) ? vscode.TreeItemCheckboxState.Checked : vscode.TreeItemCheckboxState.Unchecked;
        return item;
      });
  }

  handleCheckboxChange(event: vscode.TreeCheckboxChangeEvent<ResultVariableItem>): void {
    for (const [item, state] of event.items) {
      if (state === vscode.TreeItemCheckboxState.Checked) {
        this.checked.add(item.datFile);
      } else {
        this.checked.delete(item.datFile);
      }
    }
  }

  getCheckedFiles(): string[] {
    return [...this.checked];
  }
}
