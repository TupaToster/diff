import matplotlib.pyplot as plt
import numpy as np
x = np.linspace (2.000000 - 10, 2.000000 + 10, 1000)
y = np.sin(2*x**(2)+14)
plt.plot (x, y, 'r', label = 'Функция')
x = np.linspace (2.000000 - 10, 2.000000 + 10, 1000)
y = np.cos(2*x**(2)+14)*2*2*x
plt.plot (x, y, 'g', label = 'Первая производная')
x = np.linspace (2.000000 - 10, 2.000000 + 10, 1000)
y = (0-np.cos(2*x**(2)+14))*2*2*x*2*2*x+np.cos(2*x**(2)+14)*4
plt.plot (x, y, 'b', label = 'Вторая производная')
plt.grid ()
plt.xlabel ('x')
plt.ylabel ('y')
plt.legend ()
plt.savefig ("graph.png", dpi = 500)
