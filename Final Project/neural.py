
from pandas import read_csv
import numpy
import math
from time import sleep
from numpy import ones, zeros, append, linspace, reshape, mean ,std, sum, array, dot,concatenate, split, trace
from numpy.random import rand
from scipy.optimize import fmin_bfgs, fmin_cg


def sig_funGrad(z):
        sig_gradient =  sig_fun(z)*(1-sig_fun(z))
        return sig_gradient

def cost_fun(theta,x,y, lamda):
    num_examples = x.shape[0]
    num_inputs = 4;
    num_hidden_layers = 4;
    number_output = 3;
    theta_1, theta_2 = split(theta,[num_inputs*num_inputs])
    theta_1 = reshape(theta_1,(num_inputs,num_hidden_layers))
    theta_2 = reshape(theta_2,(number_output,num_hidden_layers+1))


    act_1_node = append(ones((1,num_examples)),x.T,axis=0)
    act_2_node = append(ones((1,num_examples)),sig_fun(dot(theta_1,act_1_node)),axis=0)

    Y = zeros((3,num_examples))
    for i in range(num_examples):
        Y[y[i]%3,i] = 1
    #print Y
    cost = trace(-dot(Y.T,numpy.log(sig_fun(dot(theta_2,act_2_node))))-dot((1-Y.T),numpy.log(1-sig_fun(dot(theta_2,act_2_node)))))/num_examples
    cost += lamda*sum(numpy.square(theta_1[:,1:]))/(2*num_examples)+lamda*sum(numpy.square(theta_2[:,1:]))/(2*num_examples)

    return cost

def sig_fun(z):
    sig = 1/(1+numpy.exp(-z))
    return sig

def theta_grad(theta,x,y, lamda):
    num_examples = x.shape[0]

    Y = zeros((3,num_examples))
    for i in range(num_examples):
        Y[y[i]%3,i] = 1

    num_examples = x.shape[0]
    num_inputs = 4;
    num_hidden_layers = 4;
    number_output = 3;
    theta_1, theta_2 = split(theta,[num_inputs*num_inputs])
    theta_1 = reshape(theta_1,(num_inputs,num_hidden_layers))
    theta_2 = reshape(theta_2,(number_output,num_hidden_layers+1))
    act_1_node = append(ones((1,num_examples)),x.T,axis=0)
    act_2_node = append(ones((1,num_examples)),sig_fun(dot(theta_1,act_1_node)),axis=0)
    act_3_node = sig_fun(dot(theta_2,act_2_node))

    p_3 = sig_fun(dot(theta_2,act_2_node)) - Y
    p_2_a = dot(theta_2.T,p_3)*sig_funGrad(append(ones((1,num_examples)),dot(theta_1,act_1_node),axis=0))
    p_2 = p_2_a[1:,:]
    theta_1Grad = dot(p_2,act_1_node.T)/num_examples
    theta_2Grad = dot(p_3,act_2_node.T)/num_examples

    theta_1Grad[:,1:] += lamda*sum(theta_1[:,1:])/num_examples
    theta_2Grad[:,1:] += lamda*sum(theta_2[:,1:])/num_examples
    unroll_theta =  append(theta_1Grad.flatten(),theta_2Grad.flatten())
    return unroll_theta

def random_theta():
    theta_1 = rand(4,4)*2*0.1-0.1
    theta_2 = rand(3,5)*2*0.1-0.1
    #print theta_1
    #print theta_2
    theta = append(theta_1.flatten(),theta_2.flatten())
    return theta

def predict(theta,x):
    num_examples = x.shape[0]
    num_examples = x.shape[0]
    num_inputs = 4;
    num_hidden_layers = 4;
    number_output = 3;
    theta_1, theta_2 = split(theta,[num_inputs*num_inputs])
    theta_1 = reshape(theta_1,(num_inputs,num_hidden_layers))
    theta_2 = reshape(theta_2,(number_output,num_hidden_layers+1))

    x_with_bias = append(ones((num_examples,1)),x,axis=1)
    #print x_with_bias
    Hidden_1 = sig_fun(dot(x_with_bias,theta_1.T))
    Hidden_1_bias = append(ones((num_examples,1)),Hidden_1,axis=1)
    #print Hidden_1_bias
    Hidden_2 = sig_fun(dot(Hidden_1_bias,theta_2.T))
    #print Hidden_2
    pred = numpy.argmax(Hidden_2,axis=1) # note that here 3 is zero
    #print pred
    return pred

while True:
    data=numpy.genfromtxt("input_ML.txt", unpack=True, skiprows = 0)
    x = data[0:3,:].T
    y = data[3,:]
    #print x.shape
    #print y.shape[0]

    initial_theta = random_theta()

    lamda = 0.1
    maxIterations = 35

    theta = fmin_cg(cost_fun,initial_theta,fprime = theta_grad,args=(x,y,lamda), maxiter = maxIterations,disp = 1)

    pred = predict(theta,x)
    #print x

    acc =  (sum(pred.T == y%3)/float(len(y))) * 100
    #print acc
    num_examples = x.shape[0]
    num_inputs = 4;
    num_hidden_layers = 4;
    number_output = 3;
    theta_1, theta_2 = split(theta,[num_inputs*num_inputs])
    theta_1 = reshape(theta_1,(num_inputs,num_hidden_layers))
    theta_2 = reshape(theta_2,(number_output,num_hidden_layers+1))
#print theta_1;
#print theta_2;


    data=numpy.genfromtxt("current_status.txt", unpack=True, skiprows = 0)
    x = data[0:3].T
    #print x

    a = 1

    x_with_bias = numpy.append(a,x)
    #print x_with_bias
    Hidden_1 = sig_fun(dot(x_with_bias,theta_1.T))
    #print Hidden_1
    Hidden_1_bias = numpy.append(a,Hidden_1)
    Hidden_2 = sig_fun(dot(Hidden_1_bias,theta_2.T))
    pred = numpy.argmax(Hidden_2)
    if (pred==0):
        pred = 3
    #print Hidden_2
    print pred

    f = open("output_ML.txt","w")
    print >> f,pred
    f.close()
