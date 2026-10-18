import matplotlib.pyplot as plt
import numpy as np

flowfield = np.loadtxt('build/Debug/flowfield.csv', delimiter=',')
residual = np.loadtxt('build/Debug/residual_jacobi.txt')

fig, axes = plt.subplots(1, 2, figsize=(11, 4.5))

# 溫度場
im = axes[0].imshow(flowfield, cmap='hot', origin='lower', extent=[0, 1, 0, 1])
fig.colorbar(im, ax=axes[0], label='T')
axes[0].set_xlabel('x')
axes[0].set_ylabel('y')
axes[0].set_title('Temperature')

# 殘差（相對值，對數座標）
it = np.arange(1, len(residual) + 1)
axes[1].semilogy(it, residual / residual[0], label='Jacobi')
axes[1].axhline(1e-6, color='gray', linestyle='--', linewidth=1, label='tolerance')
axes[1].set_xlabel('Iteration')
axes[1].set_ylabel('Relative residual')
axes[1].set_title('Convergence history')
axes[1].grid(True, which='both', alpha=0.3)
axes[1].legend()

fig.tight_layout()
plt.show()