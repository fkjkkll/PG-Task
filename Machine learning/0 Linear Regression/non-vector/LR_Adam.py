# -*- coding: utf-8 -*-
"""
Created on Fri Aug 27 15:12:14 2021

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

lr = 1e-1 # learning rate
beta_1 = 0.9# Adam
beta_2 = 0.999# Adam

vdw = np.array([0, 0, 0]) 
vdb = 0 
sdw = np.array([0, 0, 0]) 
sdb = 0





theta = 1e-1 # L2 regularization parameters

_y = 0 # prediction for temporary model
loss_collection = [] 


# for show
rx = 5 * np.arange(101)/100.0;



iterator = 10000
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

    
    # Adam
    vdb = beta_1*vdb + (1-beta_1)* db
    vdw = beta_1*vdw + (1-beta_1)* dw
    
    sdb = beta_2*sdb + (1-beta_2)* db**2
    sdw = beta_2*sdw + (1-beta_2)* dw**2
    
    vcdw = vdw/(1 - beta_1**(i+1))
    vcdb = vdb/(1 - beta_1**(i+1))
    
    scdw = sdw/(1 - beta_2**(i+1))
    scdb = sdb/(1 - beta_2**(i+1))
    
    
    b = b - lr * (vcdb/(np.sqrt(scdb) + 1e-9))
    w = w - lr * (vcdw/(np.sqrt(scdw) + 1e-9))

    
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




