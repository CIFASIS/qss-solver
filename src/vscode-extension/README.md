# QSS Solver for VSCode

A VSCode extension that replicates the core workflow of the QSS Solver desktop
GUI (`src/gui`) for editing, compiling, running and plotting MicroModelica
(`.mo`) models, without leaving the editor.

It is a thin orchestrator around the same command-line tools the desktop GUI
uses (`bin/mmoc.sh`, `bin/build.sh`, `bin/simulate.sh`, `bin/createLog.sh`,
`bin/sbml.sh`, `bin/plot_data.py`) — it does not reimplement the compiler or
solver engine.

## Features

- Syntax highlighting for `.mo` MicroModelica files and `.log` simulation logs.
- **Actions** view (activity bar) — one-click **Run Model...**, **Settings...**
  and **Python Console**.
- **QSS Solver: New Model** — create a new `.mo` file.
- **QSS Solver: Run...** — a settings form with full parity to the desktop
  GUI's Run dialog (solver, tolerances, scheduler, output, parallel
  partitioning, Jacobian, BDF partitioning, debug flags). Saves the
  `annotation(experiment(...))` block into the model file, then compiles,
  builds and runs it.
- **QSS Solver: Compile / Compile (Settings Only) / Build / Stop** — drive the
  pipeline manually.
- **Simulation Results** view (activity bar) — check the output variables
  (`.dat` files) you want to plot.
- **QSS Solver: Plot Results** — plots the checked variables with
  `bin/plot_data.py` (matplotlib), automatically creating/updating a
  `bin/.qss-solver-venv` virtual environment with its dependencies the first
  time it's used (same as the desktop GUI's `bin/python_console_setup.sh`).
  Falls back to saving and opening a PNG when no display is available, e.g.
  over a headless VSCode Remote-SSH connection.
- **QSS Solver: Python Console** — opens a terminal with `qss_solver`,
  `qss_solver.results`, `qss_solver.model` and `qss_solver.simulate`
  pre-imported.
- **QSS Solver: Show Log** — merges per-LPS logs and opens them.
- **QSS Solver: Import SBML Model...** — translate an SBML file into a `.mo`
  model.

## Installation

This extension isn't published to the Marketplace — package and install it
from source:

```sh
cd src/vscode-extension
npm install
npx @vscode/vsce package --allow-missing-repository
```

This produces `qss-solver-<version>.vsix`. Install it with either:

- The command line: `code --install-extension qss-solver-<version>.vsix`
  (use whichever `code` matches your setup — e.g. the
  `~/.vscode-server/.../bin/remote-cli/code` shim when working over
  Remote-SSH, so the extension lands in the remote host's extension list).
- The Extensions view in VSCode: `...` menu (top-right) →
  **Install from VSIX...** → select the generated file.

Reload the window afterwards (**Developer: Reload Window**) so the new
language, views and commands register.

> `@vscode/vsce` requires Node.js 20+. If your default `node` is older, run
> the `npm install`/`vsce package` commands with a Node 20+ binary on `PATH`
> (e.g. a local download from nodejs.org) — the packaged extension itself has
> no such requirement once installed.

Plotting also needs a `python3` on `PATH` able to create a virtual
environment (`python3 -m venv`); the extension bootstraps everything else
(matplotlib, pandas, ...) into `bin/.qss-solver-venv` automatically.

## Configuration

All settings live under the `qssSolver.*` prefix (see Settings UI), mirroring
the desktop GUI's Settings dialog: directories (`qssSolver.dirs.*`), script
names (`qssSolver.commands.*`), the plotting script/interpreter
(`qssSolver.plot.*`), extra compiler flags and debug flags
(`qssSolver.debug.*`). Directory settings are resolved relative to the
workspace root and default to this repository's own `bin/`, `models/`,
`build/`, `output/`, etc.

## Development

```sh
npm install
npm run compile   # or: npm run watch
```

Press F5 (or use the "Run Extension" launch configuration) to open an
Extension Development Host with the extension loaded.
