import matplotlib.pyplot as plt
import numpy as np

# ================ Get Data ===================
with open('data.txt', 'r') as f:
    data = f.read()
data = data.split(',')
for i in range(len(data)):
    data[i] = [int(c) for c in data[i]]
    pass
data_x = np.array(data)
data_y = np.zeros((len(data_x), len(data_x))).T
for i in range(len(data_y)):
    data_y[i][i] = 1


# ================= initial weights ==================
net_struct = [9, 16, 8]
w = []
b = []
for i in range(len(net_struct) - 1):
    w.append(np.random.rand(net_struct[i], net_struct[i + 1]))
    b.append(np.expand_dims(np.random.rand(net_struct[i + 1]), 0))


# ================= some func ==================
def sigmoid(x):
    return 1 / (1 + np.exp(-x))


def accuracy(x, t):
    """
    :param x: data x
    :param t: label y
    :return: accuracy
    """
    bn = sigmoid(np.dot(x, w[0]) + b[0])
    y = sigmoid(np.dot(bn, w[1]) + b[1])
    y = np.argmax(y, axis=1)
    t = np.argmax(t, axis=1)
    accu = np.sum(y == t) / float(x.shape[0])
    return accu


# ================= train ==================
times = 5000
batch_size = 30
lr = 1e-2

fig = plt.figure()
ax1 = fig.add_subplot(1, 2, 1)
ax2 = fig.add_subplot(1, 2, 2)
loss_list = []
accuracy_list = []
for i in range(times):
    # stochastic gradient descent
    mask = np.random.choice(len(data_x), batch_size)
    batch_x = data_x[mask]
    batch_y = data_y[mask]

    # forward propagation
    Bn = sigmoid(np.dot(batch_x, w[0]) + b[0])
    Y = sigmoid(np.dot(Bn, w[1]) + b[1])

    # back propagation
    er_o = (Y - batch_y) * Y * (1 - Y)
    dw1 = np.dot(Bn.T, er_o)
    db1 = np.average(er_o, 0)
    er_h = np.dot(er_o, w[1].T) * Bn * (1 - Bn)
    dw0 = np.dot(batch_x.T, er_h)
    db0 = np.average(er_h, 0)

    # for drawing
    if i % 100 == 0:
        loss = 1 / 2 * (Y - batch_y) ** 2
        loss_list.append(np.sum(loss))
        # accu = accuracy(data_x, data_y)
        accu = accuracy(batch_x, batch_y)
        accuracy_list.append(accu)
        ax1.cla()
        ax2.cla()
        ax1.plot(range(len(loss_list)), loss_list)
        ax1.set_title('loss: ' + str(round(np.sum(loss), 4)))
        ax2.plot(range(len(accuracy_list)), accuracy_list)
        ax2.set_title('accuracy: ' + str(accu))
        plt.pause(0.01)

    # update weights
    for j in range(len(net_struct) - 1):
        exec('w[j] = w[j] - lr * dw{}'.format(j))
        exec('b[j] = b[j] - lr * db{}'.format(j))
