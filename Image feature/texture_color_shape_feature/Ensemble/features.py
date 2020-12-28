# -*- coding: utf-8 -*-
"""
NJUST
Created on Mon Nov 30 17:42:01 2020
GLCM: Grey-Level Co-occurrence Matrix
Modify on Wed Dec 16 13:12:23 2020
COLOR histogram
@author: lzq 120106010718
"""
import os
import numpy as np
import cv2
from skimage import feature


def getentropy(glcm, nothing='entropy'):
    res = np.array([])
    for i in range(glcm.shape[2]):
        for j in range(glcm.shape[3]):
            s = glcm[:,:,i,j].copy()
            s = s/np.sum(s)
            res = np.append(res, np.sum(-s*np.log(s+1e-8)))
    return res.reshape(glcm.shape[2], glcm.shape[3])


def get_texture_features(img, rho, angle, grayscale):
    # (i, j, rho, theta)
    glcm = feature.greycomatrix(img, rho, angle, levels=grayscale, symmetric=True, normed=False)
    glcm[0,0,:,:]=0
    contrast = feature.greycoprops(glcm, 'contrast')
    dissimilarity = feature.greycoprops(glcm, 'dissimilarity')
    homogeneity = feature.greycoprops(glcm, 'homogeneity')
    correlation = feature.greycoprops(glcm, 'correlation')
    ASM = feature.greycoprops(glcm, 'ASM')
    entropy = getentropy(glcm, 'entropy')
    features = np.array([contrast, dissimilarity, homogeneity, correlation, ASM, entropy]).flatten()
    return features


def get_color_features(inimg, mask, thre):
    quantify = [256, 32, 32]
    img = cv2.cvtColor(inimg, cv2.COLOR_BGR2HSV)
    histh = cv2.calcHist([img], [0], mask, [quantify[0]], [0,255]).flatten()
    hists = cv2.calcHist([img], [1], mask, [quantify[1]], [0,255]).flatten()
    histv = cv2.calcHist([img], [2], mask, [quantify[2]], [0,255]).flatten()
    
    h_features = histh[thre[0]:thre[1]]
    h_features = np.append(h_features, np.sum(histh) - np.sum(histh[thre[0]:thre[1]]))
    
    s_features = hists[thre[2]:thre[3]]
    s_features = np.append(s_features, np.sum(hists) - np.sum(hists[thre[2]:thre[3]]))
    
    v_features = histv[thre[4]:thre[5]]
    v_features = np.append(v_features, np.sum(histv) - np.sum(histv[thre[4]:thre[5]]))
    
    features = h_features
    features = np.append(features, s_features)
    features = np.append(features, v_features)
    
    return features


def get_shape_features(img): 
    moments = cv2.moments(img)
    hu_moments = cv2.HuMoments(moments).flatten()
    hu_moments = np.log(abs(hu_moments))
    
    moments = np.array(list(moments.values()))
    features = np.append(moments, hu_moments)
    
    return features


def pre_deal(quantify=[256, 32, 32]):
    thre = [1000, 1000, 1000]
    directory = [i for i in os.listdir() if i.find('train')>=0][0]
    hl=quantify[0] - 1
    hr=0
    sl=quantify[1] - 1
    sr=0
    vl=quantify[2] - 1
    vr=0
    l = 0
    r = 0
    cur = None
    cur_sum = 0
    count = 0
    for filename in os.listdir(r'./' + directory):
        for img in os.listdir(r'./' + directory + '/' + filename):
            print('pre_thre-' + directory + '/' + filename + '/' + img)# show pic path
            img = cv2.imread(r'./' + directory + '/' + filename + '/' + img)    
            BGRimg = img
            GRAYimg = cv2.cvtColor(BGRimg, cv2.COLOR_BGR2GRAY)
            _, GRAYimg = cv2.threshold(GRAYimg, 0, 0, cv2.THRESH_TOZERO + cv2.THRESH_OTSU)
            _, BINimg = cv2.threshold(GRAYimg, 0, 1, cv2.THRESH_BINARY)
            
            img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
            histh = cv2.calcHist([img], [0], BINimg, [quantify[0]], [0,255]).flatten()
            hists = cv2.calcHist([img], [1], BINimg, [quantify[1]], [0,255]).flatten()
            histv = cv2.calcHist([img], [2], BINimg, [quantify[2]], [0,255]).flatten()
            
            cur = histh>thre[0]
            cur_sum = np.sum(cur)
            count = 0
            for i in range(0, quantify[0]):
                if cur[i]:
                    count = count + 1
                    if count == 1:
                        l = i
                    else:
                        r = i
                else:
                    if count > cur_sum/2:
                        r = i
                        break
                    else:
                        count = 0
            if l < hl:
                hl = l
            if r > hr:
                hr = r
                

            cur = hists>thre[1]
            cur_sum = np.sum(cur)
            count = 0
            for i in range(0, quantify[1]):
                if cur[i]:
                    count = count + 1
                    if count == 1:
                        l = i
                    else:
                        r = i
                else:
                    if count > cur_sum/2:
                        r = i
                        break
                    else:
                        count = 0
            if l < sl:
                sl = l
            if r > sr:
                sr = r
                
                
            cur = histv>thre[2]
            cur_sum = np.sum(cur)
            count = 0
            for i in range(0, quantify[2]):
                if cur[i]:
                    count = count + 1
                    if count == 1:
                        l = i
                    else:
                        r = i
                else:
                    if count > cur_sum/2:
                        r = i
                        break
                    else:
                        count = 0
            if l < vl:
                vl = l
            if r > vr:
                vr = r
                
    result = np.array([hl, hr, sl, sr, vl, vr])
    np.save(r'data/color_thre.npy', result)            
    return result




if __name__ == '__main__':
    # ============================= test texture =================================
    # rho = [1]
    # angle = [0, np.pi/4, np.pi/2, np.pi*3/4]
    
    # img = cv2.imread('img.jpg')
    # h, w = img.shape[:2]
    # img = cv2.resize(img, (w//8, h//8))
    
    # BGRimg = img
    # GRAYimg = cv2.cvtColor(BGRimg, cv2.COLOR_BGR2GRAY)
    # _, GRAYimg = cv2.threshold(GRAYimg, 0, 0, cv2.THRESH_TOZERO + cv2.THRESH_OTSU)
    # glcm = feature.greycomatrix(GRAYimg, rho, angle, levels=256, symmetric=True, normed=False)
    # glcm[0,0,:,:]=0
    
    # contrast = feature.greycoprops(glcm, 'contrast')
    # dissimilarity = feature.greycoprops(glcm, 'dissimilarity')
    # homogeneity = feature.greycoprops(glcm, 'homogeneity')
    # energy = feature.greycoprops(glcm, 'energy')
    # correlation = feature.greycoprops(glcm, 'correlation')
    # ASM = feature.greycoprops(glcm, 'ASM')
    # entropy = getentropy(glcm, 'entropy')
    # features = np.array([contrast, dissimilarity, homogeneity, energy, correlation, ASM, entropy]).flatten()
    
    
    
    # img = np.array([[0,0,0,0,1,1,1,1],[0,0,0,0,1,1,1,1],[0,0,0,0,1,1,1,1],[0,0,0,0,1,1,1,1],\
    #                 [2,2,2,2,2,2,3,3],[2,2,2,2,2,2,3,3],[2,2,2,2,2,2,3,3],[2,2,2,2,2,2,3,3]])
    # img = np.uint8(img)
    # rho = [1]
    # angle = [0, np.pi/4, np.pi/2, np.pi*3/4]
    # glcm = feature.greycomatrix(img, rho, angle, levels=4, symmetric=True, normed=False)
    # glcm1 = glcm[:,:,0,0]
    # features = get_texture_features(img, rho, angle, grayscale=4)
    
    
    # ============================= test color =================================
    # quantify = [256, 64, 32]
    # img = cv2.imread('img.jpg')
    # h, w = img.shape[:2]
    # img = cv2.resize(img, (w//8, h//8))
    
    # BGRimg = img
    # GRAYimg = cv2.cvtColor(BGRimg, cv2.COLOR_BGR2GRAY)
    # _, GRAYimg = cv2.threshold(GRAYimg, 0, 0, cv2.THRESH_TOZERO + cv2.THRESH_OTSU)
    # _, BINimg = cv2.threshold(GRAYimg, 0, 1, cv2.THRESH_BINARY)
    

    # img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    # histh = cv2.calcHist([img], [0], BINimg, [quantify[0]], [0,255]).flatten()
    # hists = cv2.calcHist([img], [1], BINimg, [quantify[1]], [0,255]).flatten()
    # histv = cv2.calcHist([img], [2], BINimg, [quantify[2]], [0,255]).flatten()
    # cv2.imshow('a', GRAYimg)
    
    
    # l = pre_deal()
    
    # ============================= test shape =================================
    # img = cv2.imread('img5.jpg')
    # img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # h, w = img.shape[:2]
    # img = cv2.resize(img, (w//8, h//8))
    
    
    # ret0, img = cv2.threshold(img, 0, 0, cv2.THRESH_TOZERO + cv2.THRESH_OTSU)
    # ret1, img = cv2.threshold(img, 0, 1, cv2.THRESH_BINARY)
    

    
    # moments = cv2.moments(img)
    # hu_moments = cv2.HuMoments(moments).flatten()
    # moments = np.array(list(moments.values()))
    # features = np.append(moments, hu_moments)
    
    # features = np.log(abs(hu_moments))
    
    # features = hu_moments

    # cv2.imshow('a', img)
    

    pass




    
    
    
    
