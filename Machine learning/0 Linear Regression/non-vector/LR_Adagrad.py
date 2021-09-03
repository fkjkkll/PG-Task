# -*- coding: utf-8 -*-
"""
Created on Fri Aug 27 15:02:52 2021

@author: lee
"""
import numpy as np
import matplotlib.pyplot as plt

n = 200 # count of training data
x = np.random.rand(n,) * 5 # random Gaussion distribution (0, 1)
noise = (np.random.rand(len(x),) - 0.5) * 0.1

y = -3 * x + 3 * x**2 + -1 * x**3 + 10# + noise # function for regression

w = np.array([1, 1, 1]) # learnable parameters
b = -10

dw = np.array([0, 0, 0]) # grad of parameters
db = 0

lr = 1e-2 # learning rate
beta = 0.9
vdw = 0
vdb = 0 #Adagrad
vdw = np.array([0, 0, 0]) #Adagrad

theta = 1 # L2 regularization parameters

_y = 0 # prediction for temporary model
loss_collection = [] 

# for show
rx = 5 * np.arange(101)/100.0;

iterator = 10000  #100000才有点用
for i in range(iterator):
    dw = np.array([0, 0, 0]) # grad of parameters
    db = 0
    loss = 0
#    for j in np.random.choice(n, 100):  # choose randomly data
    for j in range(n):
        _y = np.dot(w, np.array([x[j], x[j]**2, x[j]**3])) + b
        loss = loss + np.power((y[j] - _y), 2);
        dw = dw + 2 * (y[j] - _y) * np.array([-x[j], -x[j]**2, -x[j]**3])# + theta * w
        db = db + 2 * (y[j] - _y) * (- 1)
        pass

    #Adagrad
    vdb = beta*vdb + (1-beta)* db**2
    vdw = beta*vdw + (1-beta)* dw**2
    
    b = b - lr * (db/np.sqrt(vdb))
    w = w - lr * (dw/np.sqrt(vdw))

    loss_collection.append(loss)
    
    if i%(iterator>>4) == 0:
        plt.clf()
        plt.subplot(1,2,1)
        plt.scatter(x,y)
        ry = w[0]* rx + w[1]* rx**2 + w[2]* rx**3 + b
        plt.subplot(1,2,1)
        plt.plot(rx, ry)
        plt.title('regression')
        
        plt.subplot(1,2,2)
        plt.plot(loss_collection)
        plt.title('loss func')
        plt.pause(0.1)
        
        pass

    pass




