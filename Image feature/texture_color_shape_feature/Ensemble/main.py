# -*- coding: utf-8 -*-
"""
NJUST
Created on Tue Dec  1 18:33:16 2020
GLCM: Grey-Level Co-occurrence Matrix
Modify on Wed Dec 16 14:22:21 2020
COLOR histogram
@author: lzq 120106010718
"""
import numpy as np
from classifier import ANN
from extract_features import get_data


data = get_data('train', 150)
t_data = get_data('test')
accumulate = np.zeros((351, 5))


for i in range(10):
    circle_data = np.random.choice(data.shape[0], int(data.shape[0] * 0.8), replace=True)
    prediction, train_acc, test_acc = ANN(data, \
                          t_data, hidden_list=[64, 32], \
                              lr_rate = 0.01, \
                              epochs = 500, \
                                  dropout_rate = 0.5, \
                                      lr_decay_rate = 1e-2, \
                                          verbose = True)
    print ('%dth train acc: %.3f test acc: %.3f' % (i+1, train_acc, test_acc))
    # prediction = prediction > 0
    accumulate = accumulate + prediction

p = np.argmax((accumulate), axis=1)
test_acc = np.mean(t_data[:,-1]==p)
print ('Ensemble Acc: %.3f' % (test_acc))