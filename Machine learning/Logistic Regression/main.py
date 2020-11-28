import matplotlib.pyplot as plt
import numpy as np

# ============ Get Data =============
data_x = []
for line in open(r'ex4Data\ex4x.dat'):
    item = line.split(' ')
    item = [float(i) for i in item if len(i) > 1]
    data_x.append(item)

data_y = []
for line in open(r'ex4Data\ex4y.dat'):
    item = line.split(' ')
    item = [float(i) for i in item if len(i) > 1]
    data_y.append(item)

data_x = np.array(data_x)
data_y = np.array(data_y)

extend_x = np.ones(data_x.shape[0]).reshape(-1, 1)
data_x = np.column_stack((extend_x, data_x)).T
data_y = data_y.T


# =========== normalization(crucial) =========
nor_1 = (data_x[1] - data_x[1].min()) / (data_x[1].max() - data_x[1].min())
nor_2 = (data_x[2] - data_x[2].min()) / (data_x[2].max() - data_x[2].min())
nor_data_x = np.array([data_x[0], nor_1, nor_2])


# ============ func =============
def sigmoid(x):
    return 1.0 / (1.0 + np.exp(-x))


# ============ initialize params ==============
theta = np.array([1, 1, 1]).reshape(-1, 1)

# ============ 111 =============
iteration = 1000
lr = 1e-2

for i in range(iteration):
    y_hat = sigmoid(np.dot(theta.T, nor_data_x))
    grad = np.dot(nor_data_x, (data_y - y_hat).T)
    theta = theta + lr * grad

    if i % (iteration // 20) == 0:
        plt.cla()
        plt.scatter(nor_data_x[1, :40], nor_data_x[2, :40], color='blue')
        plt.scatter(nor_data_x[1, 40:], nor_data_x[2, 40:], color='red')
        x1 = np.array([0, 1])
        plt.plot(x1, -((theta[1][0] / theta[2][0]) * x1 + theta[0][0] / theta[2][0]))
        plt.title(str(i))
        plt.pause(0.1)
    pass
