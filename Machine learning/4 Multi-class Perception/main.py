import matplotlib.pyplot as plt
import numpy as np


# ========== Get Data ===========
def read_data(file_name):
    data = []
    for line in open(file_name):
        item = line.split(' ')
        item = [float(i) for i in item]
        data.append(item)
    data = np.array(data).T
    if file_name.find('_x') >= 0:
        data = np.row_stack((np.ones(data.shape[1]), data))
    return data


train_x = read_data('train_x.txt')
train_y = read_data('train_y.txt')
test_x = read_data('test_x.txt')
test_y = read_data('test_y.txt')
train_x_mean = np.round(np.mean(train_x[1:], 1).reshape(-1, 1), 1)
test_x_mean = np.round(np.mean(test_x[1:], 1).reshape(-1, 1), 1)
# ============ normalization =======================
train_x[1:] = train_x[1:] - train_x_mean
test_x[1:] = test_x[1:] - test_x_mean

# ============ Initialize Parameters ==============
class_num = len(set(train_y.ravel()))
theta = np.random.rand(class_num, train_x.shape[0]) - 0.5
print(theta)

# ============ For Show =========================
x1_min = int(round(train_x[1].min(), 1) * 100)
x1_max = int(round(train_x[1].max(), 1) * 100)
x2_min = int(round(train_x[2].min(), 1) * 100)
x2_max = int(round(train_x[2].max(), 1) * 100)
x1 = [[i / 100] * (x1_max - x1_min + 1) for i in range(x1_min, x1_max + 1)]
x1 = np.array(x1)
x1 = x1.flatten()
x2 = [i / 100 for i in range(x2_min, x2_max + 1)] * (x2_max - x2_min + 1)
x2 = np.array(x2)
x0 = np.ones(len(x1))
xt = np.array([x0, x1, x2])
xt[1:] = xt[1:]

# ============ train =============
iteration = 1000
lr = 1e-3

for i in range(iteration):
    y_hat = np.dot(theta.T, train_x)
    indicate = np.zeros(y_hat.shape)
    indicate[train_y.astype(int), np.arange(y_hat.shape[1])] = 1
    indicate2 = np.zeros(y_hat.shape)
    indicate2[np.argmax(y_hat, axis=0),  np.arange(y_hat.shape[1])] = 1
    grad = np.dot(train_x, (indicate - indicate2).T)
    theta = theta + lr * grad

    if i % (iteration // 20) == 0:
        plt.cla()
        r_data = np.dot(theta.T, xt)
        t_data = np.argmax(r_data, axis=0)
        mask0 = [i for i in range(len(t_data)) if t_data[i] == 0]
        mask1 = [i for i in range(len(t_data)) if t_data[i] == 1]
        mask2 = [i for i in range(len(t_data)) if t_data[i] == 2]

        plt.scatter(xt[1, mask0], xt[2, mask0], color='blue')
        plt.scatter(xt[1, mask1], xt[2, mask1], color='red')
        plt.scatter(xt[1, mask2], xt[2, mask2], color='green')

        plt.scatter(train_x[1, :40], train_x[2, :40], color='darkblue')
        plt.scatter(train_x[1, 40:80], train_x[2, 40:80], color='darkred')
        plt.scatter(train_x[1, 80:], train_x[2, 80:], color='darkgreen')

        plt.title(str(i))
        plt.pause(0.1)
        pass

        # 底下以=0为边界去画则会过于限制边界位置，过于hard，失去soft，应该用上面的方式，抉择出3个中的最大值
        # plt.cla()
        # plt.scatter(train_x[1, :40], train_x[2, :40], color='blue')
        # plt.scatter(train_x[1, 40:80], train_x[2, 40:80], color='red')
        # plt.scatter(train_x[1, 80:], train_x[2, 80:], color='green')
        # x1 = np.array([-1, 1.4])
        # plt.plot(x1, -((theta[1][0] / theta[2][0]) * x1 + theta[0][0] / theta[2][0]), color='blue')
        # # plt.plot(x1, -((theta[1][1] / theta[2][1]) * x1 + theta[0][1] / theta[2][1]), color='red')
        # plt.plot(x1, -((theta[1][2] / theta[2][2]) * x1 + theta[0][2] / theta[2][2]), color='green')
        # plt.title(str(i))
        # plt.pause(0.1)
        # pass

    pass





