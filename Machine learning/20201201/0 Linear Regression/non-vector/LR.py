# -*- coding: utf-8 -*-
"""
Created on Fri Aug 27 14:53:09 2021

@author: lee
"""
import numpy as np
import matplotlib.pyplot as plt

n = 100 # count of training data
x = np.random.rand(n,) # random Gaussion distribution (0, 1)
noise = (np.random.rand(len(x),) - 0.5) * 0.2

y = 2 * x + 1 + noise # function for regression

plt.subplot(1,2,1)
plt.scatter(x,y)

w = 1 # learnable parameters
b = 0

dw = 0 # grad of parameters
db = 0

lr = 1e-1 # learning rate
batch = 30

_y = 0 # prediction for temporary model
loss_collection = [] 

iterator = 100
for i in range(iterator):
    dw = 0
    db = 0
    loss = 0
    for j in np.random.choice(n, batch):  # choose randomly data
        _y = w * x[j] + b
        loss = loss + np.power((y[j] - _y), 2);
        dw = dw + 2 * (y[j] - _y) * (- x[j])
        db = db + 2 * (y[j] - _y) * (- 1)
        pass
    dw /= batch
    db /= batch
    b = b - lr * db
    w = w - lr * dw
    loss_collection.append(loss)
    pass

rx = np.array([0, 1])
ry = rx * w + b
plt.subplot(1,2,1)
plt.plot(rx, ry)
plt.title('regression')

plt.subplot(1,2,2)
plt.plot(loss_collection)
plt.title('loss func')
