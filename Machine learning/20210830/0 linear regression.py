import matplotlib.pyplot as plt
import numpy as np

#%% 数据生成
data_size = 100
x = np.random.randn(data_size)
y = x * 5 + 1.5 + np.random.randn(data_size) * 0.3
x = np.column_stack((np.ones(data_size), x))
y = y.reshape((-1, 1))

#%% 参数设定
theta = np.random.randn(2, 1)

#%% 超参数设定
epoch = 100
batch_size = 8
lr = 1e-2

#%% 假说函数
def calHypo(x, theta):
    return np.matmul(x, theta)

#%% 损失函数
def calLoss(x, y, theta):
    h = calHypo(x, theta)
    return np.mean(1/2 * (h - y)**2)

#%% 显示
fig = plt.figure()
ax1 = fig.add_subplot(1, 2, 1)
ax2 = fig.add_subplot(1, 2, 2)
loss_list = []

#%% 训练
for i in range(1, epoch+1):
    mask = np.random.choice(data_size, batch_size)
    train_x, train_y = x[mask], y[mask]
    h = calHypo(train_x, theta)
    grad = np.matmul(train_x.T, (h - train_y))
    theta = theta - lr*grad
    if i%(epoch//20) == 0:
        loss = calLoss(x, y, theta)
        loss_list.append(loss)
        
        ax1.cla()
        ax1.scatter(x[:, 1], y)
        ax1.plot(x[:, 1], calHypo(x, theta))
        ax1.set_title(i)
        
        ax2.cla()
        ax2.plot(range(len(loss_list)), loss_list)
        ax2.set_title('loss:' + str(loss))
        plt.pause(0.1)