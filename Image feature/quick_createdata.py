# -*- coding: utf-8 -*-
"""
Created on Mon Nov 30 15:31:07 2020

@author: lzq
"""
import cv2
import os
from quick_glcm import *


def quick_extract_feature(for_what, rho=[1, 3, 5, 10], angle = [0, np.pi/4, np.pi/2, np.pi*3/4], grayscale=256):
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
            # h, w = img.shape[:2]
            # img = cv2.resize(img, (w//8, h//8))
            _, img = cv2.threshold(img, 25, 0, cv2.THRESH_TOZERO)
            feature = get_all_features(img, rho, angle, grayscale)
            feature = np.append(feature, classes)
            features.append(feature)
        classes += 1
        
    features = np.array(features)

    # Save Data
    np.save(r'./data/fast/' + for_what + '_data_' + str(rho) + '.npy', features)
    pass



if __name__ == '__main__':
    quick_extract_feature('train', rho=[1,2,3,5], angle = [0, np.pi/4, np.pi/2, np.pi*3/4], grayscale=256)
    quick_extract_feature('test', rho=[1,2,3,5], angle = [0, np.pi/4, np.pi/2, np.pi*3/4], grayscale=256)
    
    quick_extract_feature('train', rho=[8,13,21,34], angle = [0, np.pi/4, np.pi/2, np.pi*3/4], grayscale=256)
    quick_extract_feature('test', rho=[8,13,21,34], angle = [0, np.pi/4, np.pi/2, np.pi*3/4], grayscale=256)
    
    quick_extract_feature('train', rho=[55,89,144,233], angle = [0, np.pi/4, np.pi/2, np.pi*3/4], grayscale=256)
    quick_extract_feature('test', rho=[55,89,144,233], angle = [0, np.pi/4, np.pi/2, np.pi*3/4], grayscale=256)
    
    print('end')






















