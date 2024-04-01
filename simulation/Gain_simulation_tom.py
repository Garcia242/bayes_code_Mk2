#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Mar 10 16:50:18 2024

@author: thomas
"""

#a programme written for simulating Bayes. It allows for rapid simulation and production of gain values 
#and allows for the ballancing of a number of factors, such as pitch, pitch vel.

import math
import numpy as np
import scipy 
import matplotlib.pyplot as plt

mass = 3
dt = 0.01#timestep
itterations = 10000#higher values produce unrealistic gain values. part of the job is choosing a reasonable total time to simulate
COM = 0.1
Rot_inertia = 3
wind_speed_average = 3 
wind_speed_varience = 0.5
initial_kinematic_data = [30,0]#in degrees. first one is initial pitch, second one is pitch velocity
filtervalues = [1,0,0]#these give different weightings to different facors in the optimisation proccess. The first value is pitch,then pitch vel, then force exherted by the EDF
thrust = 4

class motion(object): #this section produces the actual simulation
    def __init__(self, dt,itterations,COM,Rot_inertia,wind_speed_average,wind_speed_varience,initial_kinematic_data,gainvalues, filtervalues):
        self.dt = dt
        self.itterations = itterations
        self.COM = COM
        self.Rot_inertia = Rot_inertia
        self.wind_speed_average =  wind_speed_average
        self.wind_speed_varience = wind_speed_varience
        self.pitch = [initial_kinematic_data[0]]
        self.pitch_vel = [initial_kinematic_data[1]]
        self.Ki = gainvalues[0]
        self.Kp = gainvalues[1]
        self.Kd = gainvalues[2]
        self.filtervalues = filtervalues
        self.total_forces = [0]
        self.total_error = [0]
    def wind(self):#wind really fucks the gain values. Exclude from gain value searching but include in other simulations to test sturdyness of gain values
        translational_constant = 0.0
        wind_t = translational_constant*np.random.normal(self.wind_speed_average,self.wind_speed_varience)
        return wind_t
    def rotate(self,i):
        angular_rot = (math.pi/180)*self.pitch[i]*self.COM*self.Rot_inertia
        return angular_rot
    def TVC(self,i):
        if i > 0:
            total = self.Rot_inertia*(self.Kd*self.pitch_vel[i]+self.Kp*self.pitch[i] + self.Ki*sum(self.pitch[:100])/100)/(self.COM)
            if total > 0.5*thrust: 
                total = 0.5*thrust
            if total < -0.5*thrust:
                total  = -0.5*thrust
            return -1*total
        else:
            return 0
    def forces(self,i):
        total = self.TVC(i) + self.wind() + self.rotate(i)
        self.total_forces.append(total)
        return total
    def simulation(self,j):
        total = 0
        for i in range(0,self.itterations):
            next_pitch_vel = self.pitch_vel[i] + self.dt*self.forces(i)
            next_pitch = self.pitch[i] + self.dt*next_pitch_vel
            self.pitch.append(next_pitch)
            self.pitch_vel.append(next_pitch_vel)
            total += self.filter([next_pitch,next_pitch_vel,next_pitch_vel-self.pitch_vel[i]])
            self.total_error.append(total)
        if j == 0:
            return total
        if j == 1:
            return self.pitch
        if j ==2:
            return self.pitch, self.pitch_vel, self.total_forces, self.total_error 
    def filter(self,rotational_data):
        filter_value = 0
        for i in range(0,len(rotational_data)):
            filter_value += self.filtervalues[i]*(rotational_data[i])**2
        return filter_value

def gain(gainvalues): #function to begin simulations
    simulation = motion(dt,itterations,COM,Rot_inertia,wind_speed_average,wind_speed_varience,initial_kinematic_data,gainvalues,filtervalues)
    total_pitch_squared = simulation.simulation(0)
    return total_pitch_squared

def optimise():# this function is designed to produce optimal gain values, and then if the user desires simulate with those gain values
    initial_guess = [0,0,0]
    simulation = scipy.optimize.minimize(gain,initial_guess, method='BFGS', tol = 0.01)
    print(simulation.x)
    model = int(input("would you like to simulate with these gain values? Enter 0 for yes, 1 for no"))
    if model == 0:
        plot(simulation.x)
def plot(gainvalues):#this function allows the manual input of gain values for simulation
    simulation = motion(dt, itterations, COM, Rot_inertia,wind_speed_average,wind_speed_varience, initial_kinematic_data, gainvalues, filtervalues)
    pitch, pitch_vel, forces, error = simulation.simulation(2)
    time = []
    for i in range(0,len(pitch)):
        time.append(dt*i)
    fig = plt.figure()
    ax1 = fig.add_subplot(2, 2, 1)
    ax1.plot(time,pitch)
    ax1.set_ylabel("Pitch [degs]")
    ax1.set_xlabel("Time [s]")
    ax2 = fig.add_subplot(2, 2, 2)
    ax2.plot(time, pitch_vel) 
    ax2.set_ylabel("Pitch Velocity [degs/s]")
    ax2.set_xlabel("Time [s]")
    ax3 = fig.add_subplot(2, 2, 3)
    ax3.plot(time, forces)
    ax3.set_ylabel("Angular Forces [Nm]")
    ax3.set_xlabel("Time [s]")
    ax4 = fig.add_subplot(2, 2, 4)
    ax4.plot(time, error)
    ax4.set_ylabel("Total Gain Errors [unitless]")
    ax4.set_xlabel("Time[s]")
    plt.show()
def main(i):
    if i == 0:
        optimise()
    if i == 1:
        gainvalues = [-0.00503818,  2.19397827,  1.24042202]
        plot(gainvalues)
main(0)#enter 0 to use optimise, 1 to mannually plot
