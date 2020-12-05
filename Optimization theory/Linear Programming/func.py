# -*- coding: utf-8 -*-
"""
Created on Sat Nov 21 21:32:11 2020

@author: lzq
"""
import numpy as np


def findlnum(s, index):
    '''
    s:传入的字符串
    index:当前字符串第一个字符'x'的下标
    findnum的子函数：
    找到字符串中'x'左侧的一个数字
    '''
    realnum = 0
    if index == 0:
        realnum = 1
    elif s[index-1] == '+':
        realnum = 1
    elif s[index-1] == '-':
        realnum = -1
    else:
        realnum = float(s[:index])
    return realnum



def findrnum(s, index):
    '''
    s:传入的字符串
    index:当前字符串第一个字符'x'的下标
    findnum的子函数：
    找到字符串中'x'右侧的一个数字
    '''
    num=[]
    realnum = 0
    index = index + 1
    while index<len(s):
        if s[index].isnumeric():
            num.append(int(s[index]))
            index = index+1
            continue
        break 
    for i in range(len(num)):
        realnum = realnum * 10 + num[i]
        pass
    return realnum, s[index:]



def findnum(s, index):
    '''
    s:传入的字符串
    index:当前字符串第一个字符'x'的下标
    找到字符串中'x'左右侧的数字，返回左边数字、右边数字、截取后的字符串
    例如：
    (1) 'x2-1.3x3<=0'   => 1, 2, '-1.3x3<=0'
    (2) '-3.2x5+7x6=1'  => -3.2, 5, '+7x6=1'
    (3) '+7x6=1'        => 7, 6, '=1'
    '''
    lnum = findlnum(s, index)
    rnum, s = findrnum(s, index)
    return lnum, rnum, s




def samplex(mtx, basic_variable, non_basic_variable, phase):
    '''
    mtx:单纯形表矩阵
    basic_variable:基变量数组
    non_basic_variable:非基变量数组
    处理一副单纯形表
    '''
    cur_m, cur_n = mtx.shape
    # 检验数初始化 为 目标函数系数
    checking_number = mtx[cur_m-1][:-1]
    # 目标函数系数
    factor_number = checking_number.copy()
    while True:
        B = np.column_stack(mtx[:-1, basic_variable]).T
        B = np.matrix(B)
        N = np.column_stack(mtx[:-1, non_basic_variable]).T
        b = np.array(mtx[:-1,cur_n-1]).reshape(-1, 1)
        CB = factor_number[basic_variable]
        CN = factor_number[non_basic_variable]
        newB = np.identity(cur_m-1)
        newN = np.dot(B.I, N)
        newb = np.dot(B.I, b)
        checking_number[basic_variable] = 0
        checking_number[non_basic_variable] = CN - np.dot(CB, np.dot(B.I, N))
        mtx[:-1, basic_variable] = newB
        mtx[:-1, non_basic_variable] = newN
        mtx[:-1,cur_n-1] = newb.flatten()
        # 检验数皆为正数说明达到最优解
        if sum(checking_number>0) == 0:
            # 如果是二阶段则直接最优解， 如果是一阶段则验证基变量是否全为非人工变量，是则最优解
            if sum(basic_variable>=cur_n-cur_m+1) == 0  or phase==2:
                break
            # 如果基变量存在人工变量，则需要将人工变量出基，随机选取非人工变量入基
            else:
                substitute = np.array([i for i in range(cur_n-cur_m+1) if i not in basic_variable])
                in_basic_variable = substitute[np.random.choice(len(substitute), 1)]
                out_basic_variable = basic_variable[basic_variable>=cur_n-cur_m-1][0]
                basic_variable[basic_variable == out_basic_variable] = in_basic_variable
                non_basic_variable[non_basic_variable == in_basic_variable] = out_basic_variable

                continue
        
        for j in range(sum(checking_number>0)):
            in_basic_variable = np.where(checking_number>0)[0][j]
            smalli = -1
            smallv = -1
            for i in range(cur_m-1):
                if mtx[i, in_basic_variable] <= 0:
                    continue
                if smalli==-1:
                    smalli = i
                    smallv = mtx[i, cur_n-1]/mtx[i, in_basic_variable]
                else:
                    if mtx[i, cur_n-1]/mtx[i, in_basic_variable] < smallv:
                        smalli = i
                        smallv = mtx[i, cur_n-1]/mtx[i, in_basic_variable]
            if smalli>=0:
                out_basic_variable = basic_variable[smalli]
                break
        if smalli < 0:
            break

        basic_variable[basic_variable == out_basic_variable] = in_basic_variable
        non_basic_variable[non_basic_variable == in_basic_variable] = out_basic_variable

    return np.dot(CB, np.dot(B.I, b))

