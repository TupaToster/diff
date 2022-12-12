import matplotlib.pyplot as plt
import numpy as np
def ln(x):
	return np.log(x)
x = np.linspace (2.000000 - 10, 2.000000 + 10, 1000)
y = ln(x)*12**x
plt.plot (x, y, 'r', label = 'Функция')
x = np.linspace (2.000000 - 10, 2.000000 + 10, 1000)
y = x**(-1)*12**x+ln(x)*ln(12)*12**x
plt.plot (x, y, 'g', label = 'Первая производная')
x = np.linspace (2.000000 - 10, 2.000000 + 10, 1000)
y = -1*x**(-2)*12**x+x**(-1)*ln(12)*12**x+x**(-1)*ln(12)*12**x+ln(x)*ln(12)*ln(12)*12**x
plt.plot (x, y, 'b', label = 'Вторая производная')
plt.grid ()
plt.xlabel ('x')
plt.ylabel ('y')
plt.legend ()
plt.savefig ("graph.png")
