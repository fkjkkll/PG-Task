import matplotlib.pyplot as plt
import numpy as np


# ============ Func =============
def softmax(x):
    if x.ndim == 2:
        x = x.T
        x = x - np.max(x, axis=1).reshape(-1, 1)
        y = np.exp(x) / np.sum(np.exp(x), axis=1).reshape(-1, 1)
        return y.T
    x = x - np.max(x)  # 溢出对策
    return np.exp(x) / np.sum(np.exp(x))


# ========== Get Data ===========
def read_data(file_name):
    data = []
    for line in open(file_name):
        item = line.split(' ')
        item = [float(i) for i in item]
        data.append(item)
    data = np.array(data).T
    return data


train_x = read_data('train_x.txt')
train_y = read_data('train_y.txt')
test_x = read_data('test_x.txt')
test_y = read_data('test_y.txt')
class_num = len(set(train_y.flatten()))
batch_size = train_x.shape[-1]
# ============ normalization =======================
train_mean = np.mean(train_x, 1).reshape(-1, 1)
train_min = np.min(train_x, 1).reshape(-1, 1)
train_max = np.max(train_x, 1).reshape(-1, 1)
train_x = (train_x - train_min) / (train_max - train_min)
test_x = (test_x - train_min) / (train_max - train_min)

# ============ Initialize Parameters ==============
theta = 0.1 * np.random.randn(train_x.shape[0], class_num)
b = np.zeros(class_num).reshape(-1, 1)

# ============ For Show =========================
x1 = [[i / 100] * (100) for i in range(100)]
x1 = np.array(x1)
x1 = x1.flatten()
x2 = [i / 100 for i in range(100)] * (100)
x2 = np.array(x2)
x0 = np.ones(len(x1))
xt = np.array([x1, x2])

# ============ train =============
iteration = 1000
lr = 0.2

for i in range(iteration):
    y_hat = softmax(np.dot(theta.T, train_x) + b)
    error = y_hat.T
    error[np.arange(train_x.shape[-1]), train_y.astype(int)] -= 1

    d_theta = np.dot(train_x, error)
    d_b = np.sum(error, 0).reshape(-1, 1)
    theta = theta - lr * d_theta
    b = b - lr * d_b

    if i % (iteration // 20) == 0:
        plt.cla()
        r_data = softmax(np.dot(theta.T, xt) + b)
        t_data = np.argmax(r_data, axis=0)
        mask0 = [i for i in range(len(t_data)) if t_data[i] == 0]
        mask1 = [i for i in range(len(t_data)) if t_data[i] == 1]
        mask2 = [i for i in range(len(t_data)) if t_data[i] == 2]

        plt.scatter(xt[0, mask0], xt[1, mask0], color='blue')
        plt.scatter(xt[0, mask1], xt[1, mask1], color='red')
        plt.scatter(xt[0, mask2], xt[1, mask2], color='green')

        plt.scatter(train_x[0, :40], train_x[1, :40], color='darkblue')
        plt.scatter(train_x[0, 40:80], train_x[1, 40:80], color='darkred')
        plt.scatter(train_x[0, 80:], train_x[1, 80:], color='darkgreen')
        
        # plt.scatter(test_x[0, :10], test_x[1, :10], color='darkblue')
        # plt.scatter(test_x[0, 10:20], test_x[1, 10:20], color='darkred')
        # plt.scatter(test_x[0, 20:], test_x[1, 20:], color='darkgreen')

        plt.title(str(i))
        plt.pause(0.1)
        pass

    pass

print(theta)



