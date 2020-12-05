# -*- coding: utf-8 -*-
"""
Created on Mon Nov 30 15:31:07 2020

@author: lzq
"""
import cv2
import os
from glcm import *


def extract_feature(for_what, rho=5, grayscale=256):
    glcm = 0
    feature = 0
    features = []
    directory = [i for i in os.listdir() if i.find(for_what)>=0][0]
    classes = 0
    for filename in os.listdir(r'./' + directory):
        for img in os.listdir(r'./' + directory + '/' + filename):
            print(for_what + '/' + directory + '/' + filename + '/' + img)# show pic path
            img = cv2.imread(r'./' + directory + '/' + filename + '/' + img)
            img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
            h, w = img.shape[:2]
            img = cv2.resize(img, (w//8, h//8))
            _, img = cv2.threshold(img, 25, 0, cv2.THRESH_TOZERO)
            glcm = get_glcm(img, rho, grayscale)
            feature = get_all_features(glcm)
            feature = np.append(feature, classes)
            features.append(feature)
        classes += 1
        
    features = np.array(features)

    # Save Data
    np.save(r'./data/normal/' + for_what + '_data_' + str(rho) + '.npy', features)
    pass




def normalization(features, for_what, unity, unity_f):
    feature_num = features.shape[1] - 1
    
    # unite features from different angle matrix
    if unity:
        feature_num = feature_num//unity_f
        for i in range(feature_num):
            features[:, i] = np.max(features[:, unity_f*i:unity_f*(i+1)], axis=1)
        features = np.delete(features, [d for d in range(feature_num, feature_num*unity_f)], axis=1)
        pass
    
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

def get_data(forwhat, form='normal', unity=False, unity_f=4):
    l = os.listdir(r'data/' + form)
    train_file = [i for i in l if i.find(forwhat) >=0]
    data = []
    for i in range(len(train_file)):
        if i == len(train_file) - 1:
            data.append(np.load(r'data/' + form + r'/' + train_file[i]))
        else:
            data.append(np.load(r'data/' +  form + r'/' + train_file[i])[:,:-1])
    
    data = np.column_stack(tuple( data[i] for i in range(len(train_file))) )
    data = normalization(data, forwhat, unity, unity_f)
    return data




if __name__ == '__main__':
    extract_feature('train', rho=1)
    extract_feature('test', rho=1)

    extract_feature('train', rho=2)
    extract_feature('test', rho=2)
    
    extract_feature('train', rho=3)
    extract_feature('test', rho=3)
    
    extract_feature('train', rho=4)
    extract_feature('test', rho=4)

    extract_feature('train', rho=5)
    extract_feature('test', rho=5)
    print('end')






















