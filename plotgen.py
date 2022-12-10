import matplotlib.pyplot as plt
x = [1, 1.01, 1.02, 1.03, 1.04, 1.05, 1.06, 1.07, 1.08, 1.09, 1.1, 1.11, 1.12, 1.13, 1.14, 1.15, 1.16, 1.17, 1.18, 1.19, 1.2, 1.21, 1.22, 1.23, 1.24, 1.25, 1.26, 1.27, 1.28, 1.29, 1.3, 1.31, 1.32, 1.33, 1.34, 1.35, 1.36, 1.37, 1.38, 1.39, 1.4, 1.41, 1.42, 1.43, 1.44, 1.45, 1.46, 1.47, 1.48, 1.49, 1.5, 1.51, 1.52, 1.53, 1.54, 1.55, 1.56, 1.57, 1.58, 1.59, 1.6, 1.61, 1.62, 1.63, 1.64, 1.65, 1.66, 1.67, 1.68, 1.69, 1.7, 1.71, 1.72, 1.73, 1.74, 1.75, 1.76, 1.77, 1.78, 1.79, 1.8, 1.81, 1.82, 1.83, 1.84, 1.85, 1.86, 1.87, 1.88, 1.89, 1.9, 1.91, 1.92, 1.93, 1.94, 1.95, 1.96, 1.97, 1.98, 1.99, 2, 2.01, 2.02, 2.03, 2.04, 2.05, 2.06, 2.07, 2.08, 2.09, 2.1, 2.11, 2.12, 2.13, 2.14, 2.15, 2.16, 2.17, 2.18, 2.19, 2.2, 2.21, 2.22, 2.23, 2.24, 2.25, 2.26, 2.27, 2.28, 2.29, 2.3, 2.31, 2.32, 2.33, 2.34, 2.35, 2.36, 2.37, 2.38, 2.39, 2.4, 2.41, 2.42, 2.43, 2.44, 2.45, 2.46, 2.47, 2.48, 2.49, 2.5, 2.51, 2.52, 2.53, 2.54, 2.55, 2.56, 2.57, 2.58, 2.59, 2.6, 2.61, 2.62, 2.63, 2.64, 2.65, 2.66, 2.67, 2.68, 2.69, 2.7, 2.71, 2.72, 2.73, 2.74, 2.75, 2.76, 2.77, 2.78, 2.79, 2.8, 2.81, 2.82, 2.83, 2.84, 2.85, 2.86, 2.87, 2.88, 2.89, 2.9, 2.91, 2.92, 2.93, 2.94, 2.95, 2.96, 2.97, 2.98, 2.99, 3, 3]
f = [0, 0.122408, 0.24974, 0.382158, 0.519833, 0.662938, 0.81165, 0.966156, 1.12664, 1.29331, 1.46635, 1.64598, 1.83241, 2.02586, 2.22655, 2.43471, 2.65059, 2.87443, 3.10649, 3.34702, 3.5963, 3.85459, 4.1222, 4.3994, 4.68649, 4.9838, 5.29164, 5.61033, 5.94022, 6.28166, 6.63499, 7.00059, 7.37884, 7.77013, 8.17486, 8.59345, 9.02631, 9.47388, 9.93662, 10.415, 10.9095, 11.4205, 11.9487, 12.4944, 13.0584, 13.641, 14.2429, 14.8646, 15.5068, 16.17, 16.8549, 17.5621, 18.2923, 19.0463, 19.8246, 20.628, 21.4573, 22.3133, 23.1967, 24.1083, 25.049, 26.0197, 27.0212, 28.0544, 29.1203, 30.2198, 31.354, 32.5238, 33.7302, 34.9745, 36.2576, 37.5807, 38.9449, 40.3516, 41.8018, 43.2969, 44.8382, 46.4271, 48.0648, 49.7529, 51.4927, 53.2859, 55.1338, 57.0382, 59.0006, 61.0227, 63.1063, 65.253, 67.4648, 69.7434, 72.0909, 74.5092, 77.0003, 79.5662, 82.2093, 84.9315, 87.7353, 90.623, 93.5968, 96.6594, 99.8132, 103.061, 106.405, 109.848, 113.394, 117.044, 120.802, 124.672, 128.655, 132.756, 136.977, 141.323, 145.796, 150.4, 155.139, 160.017, 165.038, 170.205, 175.523, 180.996, 186.628, 192.424, 198.389, 204.526, 210.842, 217.341, 224.028, 230.908, 237.987, 245.27, 252.764, 260.473, 268.404, 276.564, 284.958, 293.593, 302.475, 311.613, 321.012, 330.68, 340.624, 350.853, 361.374, 372.195, 383.324, 394.771, 406.543, 418.65, 431.101, 443.906, 457.074, 470.615, 484.541, 498.861, 513.585, 528.727, 544.296, 560.305, 576.766, 593.691, 611.093, 628.985, 647.381, 666.294, 685.739, 705.73, 726.283, 747.412, 769.134, 791.464, 814.421, 838.019, 862.278, 887.215, 912.849, 939.199, 966.284, 994.125, 1022.74, 1052.16, 1082.39, 1113.47, 1145.41, 1178.23, 1211.98, 1246.65, 1282.29, 1318.92, 1356.57, 1395.26, 1435.02, 1475.88, 1517.87, 1561.03, 1605.38, 1650.95, 1697.78, 1745.91, 1795.36, 1846.18, 1898.4, 1898.4]
f1 = [12, 12.4843, 12.9847, 13.5018, 14.036, 14.5878, 15.1578, 15.7465, 16.3544, 16.9821, 17.6302, 18.2993, 18.99, 19.7029, 20.4388, 21.1982, 21.9819, 22.7906, 23.625, 24.4859, 25.374, 26.2901, 27.2351, 28.2099, 29.2151, 30.2519, 31.321, 32.4235, 33.5602, 34.7322, 35.9406, 37.1863, 38.4704, 39.7942, 41.1586, 42.5649, 44.0143, 45.508, 47.0474, 48.6337, 50.2683, 51.9525, 53.6879, 55.4758, 57.3178, 59.2155, 61.1704, 63.1841, 65.2584, 67.395, 69.5956, 71.8621, 74.1964, 76.6004, 79.0761, 81.6255, 84.2508, 86.9541, 89.7375, 92.6034, 95.5542, 98.5922, 101.72, 104.94, 108.255, 111.667, 115.179, 118.795, 122.517, 126.348, 130.29, 134.349, 138.525, 142.824, 147.247, 151.8, 156.485, 161.306, 166.266, 171.371, 176.624, 182.028, 187.589, 193.311, 199.198, 205.254, 211.485, 217.895, 224.49, 231.274, 238.253, 245.432, 252.817, 260.414, 268.228, 276.265, 284.532, 293.035, 301.781, 310.775, 320.026, 329.541, 339.326, 349.389, 359.738, 370.38, 381.324, 392.579, 404.152, 416.053, 428.29, 440.873, 453.811, 467.115, 480.794, 494.858, 509.318, 524.186, 539.472, 555.187, 571.344, 587.955, 605.032, 622.588, 640.636, 659.189, 678.262, 697.868, 718.023, 738.741, 760.038, 781.929, 804.431, 827.56, 851.334, 875.77, 900.887, 926.702, 953.236, 980.506, 1008.53, 1037.34, 1066.95, 1097.38, 1128.65, 1160.78, 1193.81, 1227.75, 1262.64, 1298.48, 1335.32, 1373.17, 1412.07, 1452.05, 1493.13, 1535.34, 1578.71, 1623.28, 1669.08, 1716.13, 1764.49, 1814.17, 1865.22, 1917.67, 1971.57, 2026.94, 2083.83, 2142.29, 2202.35, 2264.05, 2327.45, 2392.58, 2459.49, 2528.24, 2598.87, 2671.43, 2745.98, 2822.56, 2901.23, 2982.06, 3065.09, 3150.38, 3238.01, 3328.02, 3420.49, 3515.47, 3613.05, 3713.28, 3816.24, 3922, 4030.64, 4142.24, 4256.87, 4374.61, 4495.55, 4619.78, 4747.38, 4878.44, 5013.06, 5151.33, 5293.35, 5293.35]
f2 = [47.6378, 49.2293, 50.8679, 52.5552, 54.2924, 56.0811, 57.9226, 59.8187, 61.7707, 63.7804, 65.8494, 67.9794, 70.1721, 72.4294, 74.7531, 77.1451, 79.6073, 82.1418, 84.7506, 87.4359, 90.1997, 93.0444, 95.9723, 98.9856, 102.087, 105.278, 108.563, 111.943, 115.422, 119.001, 122.684, 126.475, 130.375, 134.388, 138.517, 142.765, 147.136, 151.634, 156.261, 161.021, 165.918, 170.957, 176.14, 181.472, 186.957, 192.599, 198.403, 204.373, 210.514, 216.83, 223.327, 230.009, 236.883, 243.951, 251.222, 258.699, 266.389, 274.298, 282.431, 290.795, 299.396, 308.241, 317.337, 326.69, 336.308, 346.197, 356.366, 366.823, 377.574, 388.628, 399.994, 411.68, 423.695, 436.048, 448.748, 461.805, 475.228, 489.028, 503.215, 517.8, 532.794, 548.207, 564.051, 580.338, 597.081, 614.291, 631.982, 650.166, 668.857, 688.07, 707.817, 728.115, 748.977, 770.419, 792.458, 815.109, 838.389, 862.315, 886.905, 912.177, 938.149, 964.841, 992.272, 1020.46, 1049.43, 1079.2, 1109.8, 1141.24, 1173.54, 1206.74, 1240.86, 1275.91, 1311.93, 1348.95, 1386.98, 1426.06, 1466.22, 1507.48, 1549.87, 1593.43, 1638.19, 1684.17, 1731.42, 1779.96, 1829.84, 1881.08, 1933.72, 1987.81, 2043.38, 2100.47, 2159.12, 2219.38, 2281.28, 2344.88, 2410.21, 2477.33, 2546.28, 2617.11, 2689.87, 2764.62, 2841.41, 2920.29, 3001.31, 3084.55, 3170.04, 3257.87, 3348.09, 3440.75, 3535.94, 3633.72, 3734.15, 3837.31, 3943.26, 4052.1, 4163.89, 4278.71, 4396.64, 4517.77, 4642.19, 4769.97, 4901.21, 5036.01, 5174.46, 5316.65, 5462.69, 5612.67, 5766.72, 5924.92, 6087.4, 6254.27, 6425.64, 6601.64, 6782.39, 6968.02, 7158.65, 7354.42, 7555.47, 7761.94, 7973.97, 8191.72, 8415.32, 8644.95, 8880.76, 9122.91, 9371.57, 9626.92, 9889.14, 10158.4, 10434.9, 10718.8, 11010.4, 11309.8, 11617.2, 11932.9, 12257, 12589.9, 12931.6, 13282.6, 13642.9, 14012.9, 14392.8, 14392.8]
plt.grid ()
plt.plot (x, f, 'r', label='func')
plt.plot (x, f1, 'g', label='first derivative')
plt.plot (x, f2, 'b', label='second derivative')
plt.xlabel ('x')
plt.ylabel ('y')
plt.legend ()
plt.savefig ("graph.png")
