import matplotlib.pyplot as plt
import numpy as np

#%% 数据生成
data_size = 400
m = 2
cov = [[1, -0.2],[-0.2, 1]]
x = np.random.multivariate_normal([m, 0], cov, data_size//4)
x = np.row_stack((x, np.random.multivariate_normal([0, m], cov, data_size//4)))
x = np.row_stack((x, np.random.multivariate_normal([-m, 0], cov, data_size//4)))
x = np.row_stack((x, np.random.multivariate_normal([0, -m], cov, data_size//4)))
x = np.column_stack((np.ones(data_size), x))
y = np.row_stack((np.ones((data_size//2, 1)), np.zeros((data_size//2, 1))))

#%% 参数设定
theta = np.random.randn(3, 1)

#%% 超参数设定
epoch = 1000
batch_size = 16
lr = 1e-3

#%% 假说函数
def stepFunc(x):
    return (x >= 0).astype(int)

def calHypo(x, theta):
    return stepFunc(np.matmul(x, theta))

#%% 损失函数
def calLoss(x, y, theta):
    h = calHypo(x, theta)
    return np.mean((h - y)*np.matmul(x, theta))

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
        ax1.scatter(x[:data_size//2, 1], x[:data_size//2, 2])
        ax1.scatter(x[data_size//2:data_size, 1], x[data_size//2:data_size, 2])
        ax1.plot([-4, 4], [-(theta[2][0]*e + theta[0][0])/theta[1][0] for e in [-4,4]])
        ax1.set_title(i)
        
        ax2.cla()
        ax2.plot(range(len(loss_list)), loss_list)
        ax2.set_title('loss:' + str(loss))
        plt.pause(0.1)