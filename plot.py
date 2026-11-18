"""
Usage:
    python plot_results.py                 # files in the current folder
    python plot_results.py path/to/results # files in another folder
    python plot_results.py path -o fig.png # also choose the output name
"""

import argparse
import re
from pathlib import Path

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.tri as mtri


MESHES = [41, 81]
PECLETS = [1, 10, 100]
LEVELS = np.linspace(0.0, 1.0, 21)


def read_field(path):
    """Return x, y, T from a CSV with a header line."""
    data = np.genfromtxt(path, delimiter=",", names=True)
    names = data.dtype.names
    return data[names[0]], data[names[1]], data[names[2]]


def read_convergence(path):
    """Return a dict with the numbers in converge_*.txt, or None if missing."""
    if not path.exists():
        return None

    info = {}
    for line in path.read_text().splitlines():
        if ":" not in line:
            continue
        key, value = (s.strip() for s in line.split(":", 1))
        info[key] = value
    return info


def convergence_text(info):
    lines = []
    if "nIterations" in info:
        lines.append(f"iterations: {int(float(info['nIterations']))}")
    if "initial residual" in info:
        lines.append(f"initial res: {float(info['initial residual']):.2e}")
    if "final residual" in info:
        lines.append(f"final res: {float(info['final residual']):.2e}")
    if "converged" in info:
        ok = info["converged"].strip().lower() in ("1", "true", "yes")
        lines.append("converged" if ok else "NOT converged")
    if "outer iterations" in info:
        lines.append(f"outer iter: {float(info['outer iterations']):.2e}")
    return "\n".join(lines)


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("folder", nargs="?", default=".",
                        help="folder that contains the CSV files")
    parser.add_argument("-o", "--output", default="T_fields.png",
                        help="output image name")
    args = parser.parse_args()
    folder = Path(args.folder)

    fig, axes = plt.subplots(len(MESHES), len(PECLETS),
                             figsize=(4.2 * len(PECLETS), 4.0 * len(MESHES)),
                             sharex=True, sharey=True,
                             constrained_layout=True)

    contour_set = None

    for r, mesh in enumerate(MESHES):
        for c, pe in enumerate(PECLETS):
            ax = axes[r, c]
            csv_path = folder / f"T_{mesh}_{pe}.csv"

            ax.set_aspect("equal")
            ax.set_xlim(0, 1)
            ax.set_ylim(0, 1)
            ax.set_title(f"mesh {mesh}$\\times${mesh},  Pe = {pe}")

            if not csv_path.exists():
                ax.text(0.5, 0.5, f"missing\n{csv_path.name}",
                        ha="center", va="center", transform=ax.transAxes)
                continue

            x, y, T = read_field(csv_path)
            tri = mtri.Triangulation(x, y)

            contour_set = ax.tricontourf(tri, T, levels=LEVELS,
                                         cmap="viridis", extend="both")
            ax.tricontour(tri, T, levels=[0.5], colors="white", linewidths=0.8)

            info = read_convergence(folder / f"converge_{mesh}_{pe}.txt")
            if info:
                ax.text(0.97, 0.03, convergence_text(info),
                        transform=ax.transAxes, ha="right", va="bottom",
                        fontsize=8, family="monospace",
                        bbox=dict(boxstyle="round,pad=0.3",
                                  facecolor="white", alpha=0.85, edgecolor="0.6"))

            if r == len(MESHES) - 1:
                ax.set_xlabel("x")
            if c == 0:
                ax.set_ylabel("y")

    if contour_set is not None:
        fig.colorbar(contour_set, ax=axes, shrink=0.9, label="T",
                     ticks=np.linspace(0, 1, 6))

    out = folder / args.output
    fig.savefig(out, dpi=200)
    print(f"Saved {out}")
    plt.show()


if __name__ == "__main__":
    main()