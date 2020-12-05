# -*- coding: utf-8 -*-
"""
Created on Sat Dec  5 17:05:47 2020

@author: lzq
"""
import numpy as np
import os
from createdata import get_data

def save_model(W, b):
    for i in range(len(b)):
        np.save(r'weights\model_W_' + str(i) + '.npy', W[i])
        np.save(r'weights\model_b_' + str(i) + '.npy', b[i])
        
        
def load_model():
    l = os.listdir('weights')
    l = [item for item in l if item.find('model')>=0]
    W = []
    b = []
    for i in range(len(l)//2):
        W.append(np.load(r'weights\model_W_' + str(i) + '.npy'))
        b.append(np.load(r'weights\model_b_' + str(i) + '.npy'))
        pass
    return W, b


def inference():
    t_data = get_data('test', form='normal', unity=False, unity_f=4)
    t_X = t_data[:, :t_data.shape[-1]-1]
    t_y = t_data[:, -1].astype(int)
    W, b = load_model()
    _hidden_layer = np.maximum(0, np.dot(t_X, W[0]) + b[0])
    for i in range(1, len(W) - 1):
        _hidden_layer = np.maximum(0, np.dot(_hidden_layer, W[i]) + b[i])
    scores = np.dot(_hidden_layer, W[-1]) + b[-1]
    predicted_class = np.argmax(scores, axis=1)
    test_acc = (np.mean(predicted_class == t_y))
    
    print ('test acc: %.3f' % (test_acc))
    
    
    
if __name__ == '__main__':
    inference()
    