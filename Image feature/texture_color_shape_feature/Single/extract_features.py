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


def quick_extract_feature(for_what):
    rho=[1, 2, 4, 8, 16, 32, 64, 128]
    angle = [0, np.pi/4, np.pi/2, np.pi*3/4]
    grayscale=256
    feature = None
    features = []
    thre = np.load(r'data/color_thre.npy')
    directory = [i for i in os.listdir() if i.find(for_what)>=0][0]
    classes = 0
    
    
    for filename in os.listdir(r'./' + directory):
        for img in os.listdir(r'./' + directory + '/' + filename):
            feature = np.array([])
            print(str(rho) + ':' + for_what + '/' + directory + '/' + filename + '/' + img)# show pic path
            img = cv2.imread(r'./' + directory + '/' + filename + '/' + img)
            # h, w = img.shape[:2]
            # img = cv2.resize(img, (w//8, h//8))
            
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
            
            features.append(feature)
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


def get_data(forwhat):
    features = None
    data = None
    l = os.listdir(r'data')
    train_file = [i for i in l if i.find(forwhat) >=0]
    features = np.load(r'data/' + train_file[0])

    # save specified shape features
    de_mask = [s for s in range(0, 31) if s not in [0,1,2,4,5,8,9,10,12]]
    de_mask = np.array(de_mask) + 268
    features = np.delete(features, de_mask, axis = 1)

    data = normalization(features, forwhat)
    return data


if __name__ == '__main__':
    
    # quick_extract_feature('train')
    quick_extract_feature('test')
    

    
    pass






















