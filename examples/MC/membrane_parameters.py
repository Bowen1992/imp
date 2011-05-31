import math

# various parameters are specified here
# general parameters for restraints
kappa_=100.0
max_score_=0.01

# packing restraint cluster stuff
packing_nsig_=3
packing_ncl_=13

# scoring function
score_name_="pdbTM_nored.lib"

# other restraints
d0_inter_=8.0
# max diameter
# nTMH::  2   3   4   5   6   7   8   9   10   11   12   13   14
# d   :: 16  35  26  51  48  51  50  34   39   52   48   43   42
diameter_=51.0
# these two parameters should depend on helix length
z_range_=[-3.5,3.5]
tilt_range_=[0,math.radians(40)]

# monte carlo
mc_kt=4.0
mc_loops=3000000
mc_steps=50
mc_dx=0.5
mc_dang=0.2

# trajectory file
mc_traj_file="traj.rmf"

# protein data

# sequence
TM_seq=("M","V","G","L","T","T","L","F","W","L","G","A","I","G","M","L","V","G","T","L","A","F","A","W","A","G","R","D","A","G","S","G","E","R","R","Y","Y","V","T","L","V","G","I","S","G","I","A","A","V","A","Y","V","V","M","A","L","G","V","G","W","V","P","V","A","E","R","T","V","F","A","P","R","Y","I","D","W","I","L","T","T","P","L","I","V","Y","F","L","G","L","L","A","G","L","D","S","R","E","F","G","I","V","I","T","L","N","T","V","V","M","L","A","G","F","A","G","A","M","V","P","G","I","E","R","Y","A","L","F","G","M","G","A","V","A","F","L","G","L","V","Y","Y","L","V","G","P","M","T","E","S","A","S","Q","R","S","S","G","I","K","S","L","Y","V","R","L","R","N","L","T","V","I","L","W","A","I","Y","P","F","I","W","L","L","G","P","P","G","V","A","L","L","T","P","T","V","D","V","A","L","I","V","Y","L","D","L","V","T","K","V","G","F","G","F","I","A","L","D","A","A","A","T","L")

# TM regions
TM_res=[[3,25],[37,56],[70,87],[99,117],[121,141],[163,180],[190,211]]
# define the topology
TM_topo=[-1.0,1.0,-1.0,1.0,-1.0,1.0,-1.0]
# name of the TMH
TM_names=["TM0","TM1","TM2","TM3","TM4","TM5","TM6"]
# interacting pairs
TM_inter=[["TM0","TM1"],["TM0","TM6"],["TM1","TM6"],["TM5","TM6"]]
# adjacent pairs
TM_loop=[["TM0","TM1"],["TM1","TM2"],["TM2","TM3"],["TM3","TM4"],["TM4","TM5"],["TM5","TM6"]]
# structure file
TM_struct=["","","","","","",""]
