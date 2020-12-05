# -*- coding: utf-8 -*-
"""
Created on Tue Dec  1 18:33:16 2020

@author: lzq
"""
import numpy as np
import matplotlib.pyplot as plt
from createdata import get_data
from inference import save_model

#%% ---------------------------- import data ----------------------------
data = get_data('train', form='normal', unity=False, unity_f=4)
X = data[:, :data.shape[-1]-1]
y = data[:, -1].astype(int)

t_data = get_data('test', form='normal', unity=False, unity_f=4)
t_X = t_data[:, :t_data.shape[-1]-1]
t_y = t_data[:, -1].astype(int)


#%% ---------------------------- hyperparameters ----------------------------
num_inputs = data.shape[-1] - 1 # number of features
num_outputs = 5 # number of classes
layers = [num_inputs, 128, 64, num_outputs]
W = []
b = []
for i in range(len(layers) - 1):
    _W = np.sqrt(2.0 / (layers[i])) * np.random.randn(layers[i], layers[i+1])
    _b = np.zeros((1, layers[i+1]))
    W.append(_W)
    b.append(_b)

# hyperparameters
batch_size = 256
reg = 1e-3 # regularization strength in back propagate
lr_rate = 0.2
lr = lr_rate
epochs = 5000
dropout_rate = 0.5
lr_decay_rate = 1e-2


#%% ---------------------------- for show ---------------------------- #
iter_per_epoch = max(X.shape[0] / batch_size, 1)
max_iter = int(epochs * iter_per_epoch)
cur_epoch = 0
cur_iter = 0
train_accuracy = []
test_accuracy = []
loss_record = []
record_acc_interval = 50


#%% ---------------------------- train ---------------------------- #
for ite in range(max_iter):
    cur_iter += 1
    
    # mini-batch
    batch_mask = np.random.choice(X.shape[0], batch_size)
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
    
    # compute the loss: 
    # average cross-entropy loss
    corect_logprobs = -np.log(probs[range(batch_size), y_batch])
    data_loss = np.sum(corect_logprobs)/batch_size
    # regularization loss
    reg_loss = 0.5 * reg * np.sum([ np.sum(w**2) for w in W ])
    loss = data_loss + reg_loss
    
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
    
    
    dWs.reverse()
    dbs.reverse()
    # update parameters
    for i in range(len(layers) - 1):
        dWs[i] += reg * W[i]
        W[i] += -lr * dWs[i]
        b[i] += -lr * dbs[i]
        
    # show and record
    if cur_iter > iter_per_epoch:
        cur_iter -= iter_per_epoch
        cur_epoch += 1
        loss_record.append(loss)
        
        # print(str(ite+1) + ' loss: ' + str(loss))
        if cur_epoch % record_acc_interval == 0:
            lr = 1/(1 + lr_decay_rate * cur_epoch) * lr_rate # learning rate decay

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
            train_accuracy.append(train_acc)
            test_accuracy.append(test_acc)

save_model(W, b)


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
predicted_class = np.argmax(scores, axis=1)
test_acc = (np.mean(predicted_class == t_y))

print ('training acc: %.3f  test acc: %.3f' % (train_acc, test_acc))
train_accuracy.append(train_acc)
test_accuracy.append(test_acc)


#%% ---------------------------- draw result ----------------------------
plt.subplot(121)
v = {'train': 'o', 'test': 's'}
x = np.arange((epochs)//record_acc_interval)
x = (x+1)*record_acc_interval
plt.plot(x, train_accuracy, marker='o', label='train', markevery=2)
plt.plot(x, test_accuracy, marker='s', label='test', markevery=2)
plt.xlabel("epochs")
plt.ylabel("accuracy")
plt.ylim(0, 1.0)
plt.legend(loc='lower right')

plt.subplot(122)
x = np.arange(len(loss_record))
plt.plot(x, loss_record)
plt.xlabel("epochs")
plt.ylabel("loss")

plt.show()