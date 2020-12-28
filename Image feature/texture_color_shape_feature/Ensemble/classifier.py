# -*- coding: utf-8 -*-
"""
Created on Wed Dec 23 21:39:39 2020

@author: lzq
"""

import numpy as np

def ANN(data, t_data, hidden_list=[64, 32], lr_rate=0.5, epochs=100, dropout_rate = 0.5, lr_decay_rate = 3e-3, verbose=True):
    #%% ---------------------------- data ----------------------------
    X = data[:, :-1]
    y = data[:, -1].astype(int)
    
    t_X = t_data[:, :-1]
    t_y = t_data[:, -1].astype(int)
    
    
    #%% ---------------------------- for adam ---------------------------- #
    beta1 = 0.9
    beta2 = 0.999
    m = []
    mm = []
    v = []
    vv = []
    

    #%% ---------------------------- hyperparameters ----------------------------
    num_inputs = X.shape[-1] # number of features
    num_outputs = 5 # number of classes
    layers = hidden_list
    layers.insert(0, num_inputs)
    layers.append(num_outputs)
    W = []
    b = []
    
    for i in range(len(layers) - 1):
        _W = np.sqrt(2.0 / (layers[i])) * np.random.randn(layers[i], layers[i+1])
        _b = np.zeros((1, layers[i+1]))
        W.append(_W)
        b.append(_b)
        _m = np.zeros_like(_W)
        _v = np.zeros_like(_b)
        m.append(_m)
        v.append(_v)
        mm.append(_m.copy())
        vv.append(_v.copy())
    
    # hyperparameters
    batch_size = 256
    # batch_size = data.shape[0]
    # batch_mask = np.arange(batch_size)
    reg = 1e-3 # regularization strength in back propagate
    lr = lr_rate
    
    
    #%% ---------------------------- for show ---------------------------- #
    iter_per_epoch = max(X.shape[0] / batch_size, 1)
    max_iter = int(epochs * iter_per_epoch)
    cur_epoch = 0
    cur_iter = 0
    record_acc_interval = 100
    
    #%% ---------------------------- train ---------------------------- #
    for ite in range(1, max_iter):
        cur_iter += 1
        
        # mini-batch
        batch_mask = np.random.choice(X.shape[0], batch_size, replace=True)
        # np.random.shuffle(batch_mask)
        x_batch = X[batch_mask]
        y_batch = y[batch_mask]
        
        # record mid variable for back propagate
        hidden_layers = []
        masks = []
        # input ->
        _hidden_layer = np.maximum(0, np.dot(x_batch, W[0]) + b[0])
        _mask = np.random.rand(*_hidden_layer.shape) >= dropout_rate
        _hidden_layer *= _mask
        hidden_layers.append(_hidden_layer)
        masks.append(_mask) 
        # hidden ->
        for i in range(1, len(layers) - 2):
            _hidden_layer = np.maximum(0, np.dot(hidden_layers[i-1], W[i]) + b[i]) # maximum: ReLU
            _mask = np.random.rand(*_hidden_layer.shape) >= dropout_rate
            _hidden_layer *= _mask
            hidden_layers.append(_hidden_layer)
            masks.append(_mask)
        # output ->
        scores = np.dot(_hidden_layer, W[-1]) + b[-1]
        
        # compute the class probabilities
        scores = scores - np.max(scores, axis=1).reshape(-1, 1) # avoid overflow
        probs = np.exp(scores) / np.sum(np.exp(scores), axis=1, keepdims=True)
        
        # back propagate
        # output <-
        dscores = probs
        dscores[range(batch_size), y_batch] -= 1
        dscores /= batch_size
        
        dWs = []
        dbs = []
        
        _dW = np.dot(hidden_layers[-1].T, dscores)
        _db = np.sum(dscores, axis=0, keepdims=True)
        _dhidden = np.dot(dscores, W[-1].T)
        _dhidden *= masks[-1] # back propagate through dropout
        _dhidden[hidden_layers[-1] <= 0] = 0 # back propagate through the ReLU
        dWs.append(_dW)
        dbs.append(_db)
        
        # hidden <-
        for i in range(len(layers) - 3, 0, -1):
            _dW = np.dot(hidden_layers[i-1].T, _dhidden)
            _db = np.sum(_dhidden, axis=0, keepdims=True)
            _dhidden = np.dot(_dhidden, W[i].T)
            _dhidden *= masks[i-1]
            _dhidden[hidden_layers[i-1] <= 0] = 0
            dWs.append(_dW)
            dbs.append(_db)
        # input <-
        _dW = np.dot(x_batch.T, _dhidden)
        _db = np.sum(_dhidden, axis=0, keepdims=True)
        dWs.append(_dW)
        dbs.append(_db)
        
        
        # update parameters
        dWs.reverse()
        dbs.reverse()
        
        lr_t = lr * np.sqrt(1.0 - beta2**ite) / (1.0 - beta1**ite)
        for i in range(len(layers) - 1):
            dWs[i] += reg * W[i]
            
            m[i] += (1-beta1) * (dWs[i] - m[i])
            mm[i] += (1-beta2) * (dWs[i]**2 - mm[i])
            W[i] = W[i] - lr_t * m[i] / (np.sqrt(mm[i]) + 1e-8)
            
            
            v[i] += (1-beta1) * (dbs[i] - v[i])
            vv[i] += (1-beta2) * (dbs[i]**2 - vv[i])
            b[i] = b[i] - lr_t * v[i] / (np.sqrt(vv[i]) + 1e-8)
            
            
            
            
        # show and record
        if cur_iter > iter_per_epoch:
            cur_iter -= iter_per_epoch
            cur_epoch += 1
            lr = 1/(1 + lr_decay_rate * cur_epoch) * lr_rate # learning rate decay
            # print(str(ite+1) + ' loss: ' + str(loss))
            if verbose and cur_epoch % record_acc_interval == 0:
                _hidden_layer = np.maximum(0, np.dot(X, W[0]) + b[0])
                for i in range(1, len(layers) - 2):
                    _hidden_layer = np.maximum(0, np.dot(_hidden_layer, W[i]) + b[i])
                scores = np.dot(_hidden_layer, W[-1]) + b[-1]
                predicted_class = np.argmax(scores, axis=1)
                train_acc = (np.mean(predicted_class == y))
                
                _hidden_layer = np.maximum(0, np.dot(t_X, W[0]) + b[0])
                for i in range(1, len(layers) - 2):
                    _hidden_layer = np.maximum(0, np.dot(_hidden_layer, W[i]) + b[i])
                scores = np.dot(_hidden_layer, W[-1]) + b[-1]
                predicted_class = np.argmax(scores, axis=1)
                test_acc = (np.mean(predicted_class == t_y))
                print ('epoch(%d/%d) training acc: %.3f  test acc: %.3f' % (cur_epoch, epochs, train_acc, test_acc))
    
    #%% ---------------------------- last test ----------------------------
    _hidden_layer = np.maximum(0, np.dot(X, W[0]) + b[0])
    for i in range(1, len(layers) - 2):
        _hidden_layer = np.maximum(0, np.dot(_hidden_layer, W[i]) + b[i])
    scores = np.dot(_hidden_layer, W[-1]) + b[-1]
    predicted_class = np.argmax(scores, axis=1)
    train_acc = (np.mean(predicted_class == y))
    
    
    _hidden_layer = np.maximum(0, np.dot(t_X, W[0]) + b[0])
    for i in range(1, len(layers) - 2):
        _hidden_layer = np.maximum(0, np.dot(_hidden_layer, W[i]) + b[i])
    scores = np.dot(_hidden_layer, W[-1]) + b[-1]
    scores = scores - np.max(scores, axis=1).reshape(-1, 1) # avoid overflow
    probs = np.exp(scores) / np.sum(np.exp(scores), axis=1, keepdims=True)
    predicted_class = np.argmax(scores, axis=1)
    test_acc = (np.mean(predicted_class == t_y))
    
    prediction = np.zeros((len(predicted_class), num_outputs))
    prediction[np.arange(len(predicted_class)), predicted_class] = 1
    return prediction * probs, train_acc, test_acc





