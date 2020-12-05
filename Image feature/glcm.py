# -*- coding: utf-8 -*-
"""
Created on Mon Nov 30 17:42:01 2020
GLCM: Grey-Level Co-occurrence Matrix
@author: lzq
"""

import numpy as np


def get_glcm(img, rho, grayscale):
    '''

    Parameters
    ----------
    img : image Array of uint8
        DESCRIPTION.
    Returns
    -------
    glcm : GLCM Array of float64
        DESCRIPTION.

    '''
    theta = np.array([[0, rho],[rho, rho],[rho, 0],[rho, -rho]])
    glcm = np.zeros((4, grayscale, grayscale))
    height, width = img.shape
    temp_h = 0
    temp_w = 0
    for i in range(4):
        for h in range(rho, height - rho):
            for w in range(rho, width - rho):
                temp_h = img[h][w]
                temp_w = img[h+theta[i][0]][w+theta[i][1]]
                glcm[i][temp_h][temp_w] += 1
                if temp_h != temp_w:
                    glcm[i][temp_w][temp_h] += 1
        glcm[i][0][0] = 0
        glcm[i] /= np.sum(glcm[i])

    return glcm


def get_entropy(glcm):
    entropy = np.zeros(4)
    eps = 1e-8
    for i in range(4):
        entropy[i] = -np.sum(glcm[i] * np.log10(glcm[i] +  eps))
    return entropy

def get_energy(glcm):
    energy = np.zeros(4)
    for i in range(4):
        energy[i] = np.sum(glcm[i] ** 2)
    return energy

def get_contrast(glcm):
    contrast = np.zeros(4)
    for i in range(4):
        for h in range(glcm.shape[-1]):
            for w in range(h, glcm.shape[-1]):
                if h!=w:
                    contrast[i] += 2 * (h - w)**2 * glcm[i][h][w]
                else:
                    contrast[i] += (h - w)**2 * glcm[i][h][w]
    return contrast


def get_homogeneity(glcm):
    homogeneity = np.zeros(4)
    for i in range(4):
        for h in range(glcm.shape[-1]):
            for w in range(h, glcm.shape[-1]):
                if h!=w:
                    homogeneity[i] += 2 * glcm[i][h][w] / (1.0 + np.abs(h - w))
                else:
                    homogeneity[i] += glcm[i][h][w] / (1.0 + np.abs(h - w))
    return homogeneity

def get_mean(glcm):
    mean = np.zeros(4)
    for i in range(4):
        for h in range(glcm.shape[-1]):
            for w in range(h, glcm.shape[-1]):
                if h!=w:
                    mean[i] += h * glcm[i][h][w] + w * glcm[i][h][w]
                else:
                    mean[i] += h * glcm[i][h][w]
    return mean

def get_var(glcm, mean):
    var = np.zeros(4)
    for i in range(4):
        for h in range(glcm.shape[-1]):
            for w in range(h, glcm.shape[-1]):
                if h!=w:
                    var[i] += (h - mean[i])**2 * glcm[i][h][w] + (w - mean[i])**2 * glcm[i][h][w]
                else:
                    var[i] += (h - mean[i])**2 * glcm[i][h][w]
    return var

def get_correlation(glcm, mean, var):
    correlation = np.zeros(4)
    eps = 1e-8
    for i in range(4):
        for h in range(glcm.shape[-1]):
            for w in range(h, glcm.shape[-1]):
                if h!=w:
                    correlation[i] += 2 * (h - mean[i]) * (w - mean[i]) * glcm[i][h][w]**2 / (var[i] + eps)
                else:
                    correlation[i] += (h - mean[i]) * (w - mean[i]) * glcm[i][h][w]**2 / (var[i] + eps)
    return correlation



def get_features(glcm):
    features = get_entropy(glcm)
    features = np.row_stack((features, get_energy(glcm)))
    features = np.row_stack((features, get_contrast(glcm)))
    features = np.row_stack((features, get_homogeneity(glcm)))
    mean = get_mean(glcm)
    features = np.row_stack((features, mean))
    var = get_var(glcm, mean)
    features = np.row_stack((features, var))
    features = np.row_stack((features, get_correlation(glcm, mean, var)))
    return features.flatten()

def get_all_features(glcm):
    entropy = np.zeros(4)
    energy = np.zeros(4)
    contrast = np.zeros(4)
    homogeneity = np.zeros(4)
    mean = np.zeros(4)
    var = np.zeros(4)
    correlation = np.zeros(4)
    
    eps = 1e-8
    for i in range(4):
        entropy[i] = -np.sum(glcm[i] * np.log10(glcm[i] +  eps))
        energy[i] = np.sum(glcm[i] ** 2)
        for h in range(glcm.shape[-1]):
            for w in range(h, glcm.shape[-1]):
                if h!=w:
                    contrast[i] += 2 * (h - w)**2 * glcm[i][h][w]
                    homogeneity[i] += 2 * glcm[i][h][w] / (1.0 + np.abs(h - w))
                    mean[i] += h * glcm[i][h][w] + w * glcm[i][h][w]
                    
                else:
                    contrast[i] += (h - w)**2 * glcm[i][h][w]
                    homogeneity[i] += glcm[i][h][w] / (1.0 + np.abs(h - w))
                    mean[i] += h * glcm[i][h][w]
                    
        for h in range(glcm.shape[-1]):
            for w in range(h, glcm.shape[-1]):
                if h!=w:
                    
                    var[i] += (h - mean[i])**2 * glcm[i][h][w] + (w - mean[i])**2 * glcm[i][h][w]
                else:
                    var[i] += (h - mean[i])**2 * glcm[i][h][w]
        
        for h in range(glcm.shape[-1]):
            for w in range(h, glcm.shape[-1]):
                if h!=w:
                    
                    correlation[i] += 2 * (h - mean[i]) * (w - mean[i]) * glcm[i][h][w]**2 / (var[i] + eps)
                else:
                    correlation[i] += (h - mean[i]) * (w - mean[i]) * glcm[i][h][w]**2 / (var[i] + eps)
    features = np.array([entropy, energy, contrast, homogeneity, mean, var, correlation]).flatten()
    return features


if __name__ == '__main__':
    img = np.array([[0,0,0,0,1,1,1,1],[0,0,0,0,1,1,1,1],[0,0,0,0,1,1,1,1],[0,0,0,0,1,1,1,1],\
                    [2,2,2,2,2,2,3,3],[2,2,2,2,2,2,3,3],[2,2,2,2,2,2,3,3],[2,2,2,2,2,2,3,3]])
    img = np.uint8(img)
    glcm = get_glcm(img, 1, 4)
    features = get_all_features(glcm)
    
    
    
    
    
