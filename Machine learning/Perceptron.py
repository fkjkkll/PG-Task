# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import numpy as np
import matplotlib.pyplot as plt

threshold = 0.5
lr = 1e-1
weights = [0, 0, 0]
data = [((1, 0, 0), 1), ((1, 0, 1), 1), ((1, 1, 0), 1), ((1, 1, 1), 0)]

def dot_product(values, weights):
    return sum(value * weight for value, weight in zip(values, weights))

while True:
    print('-' * 60)
    error_count = 0
    for input_vector, desired_output in data:
        print(weights)
        result = dot_product(input_vector, weights) > threshold
        error = desired_output - result
        if error != 0:
            error_count += 1
            for index, value in enumerate(input_vector):
                weights[index] += lr * error * value
    if error_count == 0:
        break
    