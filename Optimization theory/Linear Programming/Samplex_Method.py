# -*- coding: utf-8 -*-
"""
Created on Sat Nov 21 16:45:40 2020
@author: lzq

"""
from func import *


# 将用户输入的式子转化为两阶段单纯形法的矩阵形式（以人工变量的负数为优化函数）
objfunc = input('input object function: ')
objfunc = objfunc.replace(' ', '')
n = int(objfunc[objfunc.rfind('x')+1:])
constraint = []
m = 0
extra = 0
b = []
while True:
    m = m + 1
    cons = input('input %d constraint(q to quit): '%(m))
    if cons=='q':
        break
    cons = cons.replace(' ', '')
    b.append(int(cons[cons.rfind('=')+1:]))
    if cons.find('>')>=0 or cons.find('<')>=0:
        extra = extra + 1
        pass
    cons = cons[:cons.rfind('=')]
    constraint.append(cons)
    pass
m = m - 1
mtx = np.zeros((m+1, n+m+extra+1))
slack_variables_index = 0
for i in range(m):
    cur_string = constraint[i]
    cur_l_num = 0
    cur_r_num = 0
    if cur_string[-1] == '<':
        mtx[i][n + slack_variables_index] = 1
        slack_variables_index = slack_variables_index + 1
        cur_string = cur_string[:-1]
    elif cur_string[-1] == '>':
        mtx[i][n + slack_variables_index] = -1
        slack_variables_index = slack_variables_index + 1
        cur_string = cur_string[:-1]
    while len(cur_string)>0:
        cur_l_num, cur_r_num, cur_string = findnum(cur_string, cur_string.find('x'))
        mtx[i][cur_r_num-1] = cur_l_num
    mtx[i][n+m+extra] = b[i]
    mtx[i][n+extra+i] = 1
    mtx[m][n+extra+i] = -1
objfuncarray = np.zeros(n+extra)
while len(objfunc)>0:
    cur_l_num, cur_r_num, objfunc = findnum(objfunc, objfunc.find('x'))
    objfuncarray[cur_r_num-1] = cur_l_num





# 一阶段优化：优化人工函数
basic_variable = np.array([ i+n+extra for i in range(m) ])
non_basic_variable = np.array([ i for i in range(basic_variable[0]) ])
if samplex(mtx, basic_variable, non_basic_variable, 1)==0:
    # 二阶段优化：优化目标函数
    non_basic_variable = non_basic_variable[non_basic_variable<n+extra]
    mtx = np.delete(mtx, range(n+extra, n+extra+m), axis=1)
    mtx[m][:-1] = objfuncarray
    solution = samplex(mtx, basic_variable, non_basic_variable, 2)
    if sum(mtx[m, :-1]>0) == 0:
        solution_vector = np.zeros(n+extra)
        solution_vector[basic_variable] = mtx[:-1, -1]
        print('optimal solution vector is: ', solution_vector)
        print('optimal solution value is:', solution)
    else:
        print('不存在有界解！')

else:
    print('无解!')
    

























