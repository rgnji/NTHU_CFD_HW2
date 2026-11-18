"""
Plot the temperature profile T(y) along the line x = 0.5.

Input files are CSVs with a header line "x,y,T" (cell centres, optionally
followed by boundary-face centres), as written by volField::writeXY().

File names:
    T_<mesh>_<Pe>.csv             e.g. T_41_100.csv
    T_<scheme>_<mesh>_<Pe>.csv    e.g. T_QUICK_81_10.csv   (optional scheme)

One subplot is drawn for each Peclet number; every (scheme, mesh) pair
found for that Pe becomes one line. The exact solution of the pure
convection problem (a step at y = 0.5) is drawn as a dashed reference.

Usage:
    python plot_profile.py                    # files in the current folder
    python plot_profile.py path/to/results    # files in another folder
    python plot_profile.py path -x 0.5 -o profile.png
    python plot_profile.py path --vertical    # T on the horizontal axis
"""

import argparse
import re
from collections import defaultdict
from pathlib import Path

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.tri as mtri


NAME_PATTERN = re.compile(r"^T_(?:(?P<scheme>.+)_)?(?P<mesh>\d+)_(?P<pe>[0-9.eE+-]+)\.csv$")


def read_field(path):
    data = np.genfromtxt(path, delimiter=",", names=True)
    names = data.dtype.names
    return data[names[0]], data[names[1]], data[names[2]]


def profile(path, x0, ys):
    """Interpolate T along x = x0 at the heights ys."""
    x, y, T = read_field(path)
    interp = mtri.LinearTriInterpolator(mtri.Triangulation(x, y), T)
    values = interp(np.full_like(ys, x0), ys)
    return np.ma.filled(values, np.nan)


def pe_label(pe):
    value = float(pe)
    return f"{value:g}"


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("folder", nargs="?", default=".",
                        help="folder that contains the CSV files")
    parser.add_argument("-x", type=float, default=0.5,
                        help="x position of the profile (default 0.5)")
    parser.add_argument("-o", "--output", default="T_profile.png",
                        help="output image name")
    parser.add_argument("--vertical", action="store_true",
                        help="plot T on the horizontal axis and y on the vertical axis")
    args = parser.parse_args()
    folder = Path(args.folder)

    # Group files by Pe:  {pe: [(scheme, mesh, path), ...]}
    groups = defaultdict(list)
    for path in sorted(folder.glob("T_*.csv")):
        m = NAME_PATTERN.match(path.name)
        if m is None:
            print(f"skip {path.name}: name does not match T_[scheme_]<mesh>_<Pe>.csv")
            continue
        groups[m["pe"]].append((m["scheme"] or "", int(m["mesh"]), path))

    if not groups:
        raise SystemExit(f"No T_*.csv files found in {folder.resolve()}")

    pes = sorted(groups, key=float)
    fig, axes = plt.subplots(1, len(pes), figsize=(4.6 * len(pes), 4.4),
                             sharey=not args.vertical, sharex=args.vertical,
                             squeeze=False, constrained_layout=True)
    axes = axes[0]

    ys = np.linspace(0.0, 1.0, 801)
    exact = np.where(ys > args.x, 1.0, 0.0)   # pure convection: T = 1 above y = x
    linestyles = ["-", "--", "-.", ":"]

    for ax, pe in zip(axes, pes):
        entries = sorted(groups[pe], key=lambda e: (e[0], e[1]))
        meshes = sorted({mesh for _, mesh, _ in entries})

        for scheme, mesh, path in entries:
            T = profile(path, args.x, ys)
            label = f"{scheme} {mesh}x{mesh}".strip()
            style = linestyles[meshes.index(mesh) % len(linestyles)]
            if args.vertical:
                ax.plot(T, ys, style, label=label)
            else:
                ax.plot(ys, T, style, label=label)

        if args.vertical:
            ax.plot(exact, ys, "k:", linewidth=1, label=r"exact ($\Gamma=0$)")
            ax.set_xlabel("T")
            ax.set_ylabel("y")
            ax.set_ylim(0, 1)
        else:
            ax.plot(ys, exact, "k:", linewidth=1, label=r"exact ($\Gamma=0$)")
            ax.set_xlabel("y")
            ax.set_ylabel("T")
            ax.set_xlim(0, 1)

        ax.set_title(f"Pe = {pe_label(pe)},  x = {args.x:g}")
        ax.grid(alpha=0.3)
        ax.legend(fontsize=8)

    out = folder / args.output
    fig.savefig(out, dpi=200)
    print(f"Saved {out}")
    plt.show()


if __name__ == "__main__":
    main()