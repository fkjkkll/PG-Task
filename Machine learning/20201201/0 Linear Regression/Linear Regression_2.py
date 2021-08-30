# -*- coding: utf-8 -*-
"""
machine learning task one: Linear Regression gradient descent
Created on Fri Sep 18 15:49:14 2020
@author: LiZhiqiang
"""
import matplotlib.pyplot as plt
import numpy as np

# %% import data
x = [2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008,
     2009, 2010, 2011, 2012, 2013]
y = [2000, 2500, 2900, 3147, 4515, 4903, 5365, 5704,
     6853, 7971, 8561, 10000, 11280, 12900]

# %% data preprocessing
x = np.array(x).astype(np.float32)
x_mean = x.mean()
y = np.array(y).astype(np.float32)
x -= x_mean
y -= y.min()
x = x.reshape((-1, 1))
x = np.insert(x, obj=0, values=np.ones(len(x)), axis=1)
theta = np.random.randn(2)

# %% hyper parameters
lr = 1e-2
epoch = 1000

# %% training
cost_list = []
fig = plt.figure()
ax1 = fig.add_subplot(1, 2, 1)
ax2 = fig.add_subplot(1, 2, 2)
for i in range(epoch):
    error = (np.dot(theta, x.T) - y)
    grad = error.reshape(-1, 1) * x
    theta = theta - lr * np.sum(grad, 0) / len(y)
    cost = int(np.average(error ** 2))
    cost_list.append(cost)

    if i % (epoch//20) == 0:
        ax1.cla()
        ax1.scatter(x[:, 1], y)
        ax1.plot(x[:, 1], np.dot(theta, x.T))
        ax1.set_xlabel('year')
        ax1.set_ylabel('price')
        ax1.set_title(i)

        ax2.cla()
        ax2.plot(range(len(cost_list)), cost_list)
        ax2.set_xlabel('times')
        ax2.set_ylabel('cost')
        ax2.set_title('cost:' + str(cost))

        plt.pause(0.01)

# %% print details
print('#'*50)
print('Gradient descent method: predicted price in 2014 is '
      + str(int(np.dot(theta, np.array([1, 2014 - x_mean])))))
