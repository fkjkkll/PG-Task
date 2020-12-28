# -*- coding: utf-8 -*-
"""
NJUST
Created on Mon Nov 30 15:31:07 2020
GLCM: Grey-Level Co-occurrence Matrix
Modify on Wed Dec 16 13:10:53 2020
COLOR histogram
@author: lzq 120106010718
"""
import cv2
import os
import numpy as np

from features import get_texture_features, get_color_features, get_shape_features

def extract_feature_per_img(img, rho, angle, grayscale, thre, classes):
    
    feature = np.array([])
    BGRimg = img
    GRAYimg = cv2.cvtColor(BGRimg, cv2.COLOR_BGR2GRAY)
    _, GRAYimg = cv2.threshold(GRAYimg, 0, 0, cv2.THRESH_TOZERO + cv2.THRESH_OTSU)
    _, BINimg = cv2.threshold(GRAYimg, 0, 1, cv2.THRESH_BINARY)
    
    # texture feature
    feature = np.append(feature, get_texture_features(GRAYimg, rho, angle, grayscale))
    
    # color feature
    feature = np.append(feature, get_color_features(BGRimg, BINimg, thre))

    # shape feature
    feature = np.append(feature, get_shape_features(GRAYimg))
    
    # insert class type
    feature = np.append(feature, classes)
    
    return feature
    

def quick_extract_feature(for_what):
    rho=[1, 2, 4, 8, 16, 32, 64, 128]
    angle = [0, np.pi/4, np.pi/2, np.pi*3/4]
    grayscale = 256
    features = []
    thre = np.load(r'data/color_thre.npy')
    directory = [i for i in os.listdir() if i.find(for_what)>=0][0]
    classes = 0

    for filename in os.listdir(r'./' + directory):
        for img in os.listdir(r'./' + directory + '/' + filename):     
            print(str(rho) + ':' + for_what + '/' + directory + '/' + filename + '/' + img)# show pic path
            img = cv2.imread(r'./' + directory + '/' + filename + '/' + img)
            features.append(extract_feature_per_img(img, rho, angle, grayscale, thre, classes))

        classes += 1
        
    features = np.array(features)
    # Save Data
    np.save(r'./data/' + for_what + '_data_' + str(rho) + '.npy', features)
    pass


def normalization(features, for_what):
    feature_num = features.shape[1] - 1
    # Normalization
    if for_what.find('train') >= 0:
        mean_std_set = []
        for fn in range(feature_num):
            _mean = np.mean(features[:, fn])
            _std = np.std(features[:, fn])
            features[:, fn] = (features[:, fn] - _mean) / _std
            mean_std_set.append([_mean, _std])
        mean_std_set = np.array(mean_std_set)
        np.save(r'data/mean_std.npy', mean_std_set)
    else:
        mean_std_set = np.load(r'data/mean_std.npy')
        for fn in range(features.shape[1] - 1):
            features[:, fn] = (features[:, fn] - mean_std_set[fn][0]) / mean_std_set[fn][1]    
    return features

def get_data(forwhat, f_num=1):
    features = None
    data = None
    l = os.listdir(r'data')
    train_file = [i for i in l if i.find(forwhat) >=0]
    features = np.load(r'data/' + train_file[0])
    
    # features optimization
    data = features.copy()
    selected_features = None
    if forwhat.find('train') >=0:
        scope = []
        label = data[:,-1]
        scope.append(0)
        for i in range(1, data.shape[0]):
            if label[i] != label[i-1]:
                scope.append(i)
        scope.append(i+1)
        
        cost = []
        fmean = np.zeros(len(scope)-1)
        for i in range(data.shape[-1]-1):
            feature = data[:, i]
            for j in range(len(scope)-1):
                num = scope[j+1] - scope[j]
                startnum = int(0.1 * num)
                feature_class = feature[scope[j]:scope[j+1]]
                feature_class = (feature_class).argsort()[startnum:num-startnum]
                fmean[j] = np.mean(feature_class)
                pass

            cost.append(abs(np.std(fmean)/np.mean(fmean)))

        cost = np.array(cost)
        
        selected_features = (-cost).argsort()[:f_num]
        selected_features = np.sort(selected_features)
        np.save(r'data/selected_feature.npy', selected_features)
    else:
        selected_features = np.load(r'data/selected_feature.npy')
        
    data = normalization(features.copy(), forwhat)
    selected_features = np.append(selected_features, data.shape[-1]-1)
    return data[:,selected_features]
    


if __name__ == '__main__':
    
    # quick_extract_feature('train')
    quick_extract_feature('test')
    
    # data, cost, se, f, scope = get_data('train', 200) # , cost, selected_features, features, scope
    # data, f = get_data('test') # , features
    
    
    
    # img = cv2.imread('img.jpg')
    # h, w = img.shape[:2]
    # w = w//8
    # h = h//8
    # img = cv2.resize(img, (w, h))
    # img = cv2.flip(img, 1)
    # cv2.imshow('a', img)
    pass






















