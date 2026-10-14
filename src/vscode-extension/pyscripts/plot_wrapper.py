"""
Wraps bin/plot_data.py so the QSS Solver VSCode extension can plot results both
locally (native interactive matplotlib window, same as the desktop GUI) and over
a headless remote connection (no DISPLAY/WAYLAND_DISPLAY, e.g. VSCode Remote-SSH
without X11 forwarding), where an interactive window can never appear.

When no display is available, the Agg backend is selected before plot_data is
imported (matplotlib's backend must be picked before pyplot loads), plot_data's
own plt.show() call becomes a harmless no-op, and this wrapper saves the still-open
figure to a PNG next to the plotted .dat files instead, printing "SAVED:<path>" on
its own stdout line so the extension can open that file in an editor tab.

Usage: plot_wrapper.py <plot_script_path> <file_paths...> <title> <xlabel> <ylabel> [--plot_markers]
"""

import argparse
import importlib.util
import os
import sys


def has_display() -> bool:
    if sys.platform.startswith('linux'):
        return bool(os.environ.get('DISPLAY') or os.environ.get('WAYLAND_DISPLAY'))
    return True


def safe_filename(text: str) -> str:
    cleaned = ''.join(c if (c.isalnum() or c in ('-', '_')) else '_' for c in text.strip())
    return cleaned or 'plot'


def load_plot_module(script_path: str):
    spec = importlib.util.spec_from_file_location('qss_solver_plot_script', script_path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def main() -> None:
    if len(sys.argv) < 2:
        print('Usage: plot_wrapper.py <plot_script_path> <file_paths...> <title> <xlabel> <ylabel> [--plot_markers]', file=sys.stderr)
        sys.exit(1)

    script_path = sys.argv[1]
    save_mode = not has_display()

    if save_mode:
        import matplotlib

        matplotlib.use('Agg')

    plot_data = load_plot_module(script_path)

    parser = argparse.ArgumentParser()
    parser.add_argument('file_paths', type=str, nargs='+')
    parser.add_argument('title', type=str)
    parser.add_argument('xlabel', type=str)
    parser.add_argument('ylabel', type=str)
    parser.add_argument('--plot_markers', action=argparse.BooleanOptionalAction)
    args = parser.parse_args(sys.argv[2:])

    plot_data.plot_data(args.file_paths, args.title, args.xlabel, args.ylabel, args.plot_markers)

    if save_mode:
        import matplotlib.pyplot as plt

        out_dir = os.path.dirname(os.path.abspath(args.file_paths[0]))
        out_path = os.path.join(out_dir, f'{safe_filename(args.title)}.png')
        plt.savefig(out_path, dpi=150, bbox_inches='tight')
        plt.close('all')
        print(f'SAVED:{out_path}')


if __name__ == '__main__':
    main()
