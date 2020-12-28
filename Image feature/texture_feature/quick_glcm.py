# -*- coding: utf-8 -*-
"""
Created on Mon Nov 30 17:42:01 2020
GLCM: Grey-Level Co-occurrence Matrix
@author: lzq
"""

import numpy as np
from skimage import feature

def get_all_features(img, rho, angle, grayscale):
    # (i, j, rho, theta)
    glcm = feature.greycomatrix(img, rho, angle, levels=grayscale, symmetric=True, normed=False)
    glcm[0,0,:,:]=0
    contrast = feature.greycoprops(glcm, 'contrast')
    dissimilarity = feature.greycoprops(glcm, 'dissimilarity')
    homogeneity = feature.greycoprops(glcm, 'homogeneity')
    energy = feature.greycoprops(glcm, 'energy')
    correlation = feature.greycoprops(glcm, 'correlation')
    ASM = feature.greycoprops(glcm, 'ASM')
    
    features = np.array([contrast, dissimilarity, homogeneity, energy, correlation, ASM]).flatten()
    
    return features


if __name__ == '__main__':
    img = np.array([[0,0,0,0,1,1,1,1],[0,0,0,0,1,1,1,1],[0,0,0,0,1,1,1,1],[0,0,0,0,1,1,1,1],\
                    [2,2,2,2,2,2,3,3],[2,2,2,2,2,2,3,3],[2,2,2,2,2,2,3,3],[2,2,2,2,2,2,3,3]])
    img = np.uint8(img)
    rho = [1]
    angle = [0, np.pi/4, np.pi/2, np.pi*3/4]
    glcm = feature.greycomatrix(img, rho, angle, levels=4, symmetric=True, normed=False)
    glcm1 = glcm[:,:,0,0]
    features = get_all_features(img, rho, angle, grayscale=4)
    
    
    
    
    
