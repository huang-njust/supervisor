# Supervisor Synthesis for Petri nets of RASs

Reachability graph analyzer: This program which is written by C++ and to compute the set of legal markings M_L, the set of first-met bad markings M_F, the minimal covering set of M_L, the minimal covered set of M_F, the minimal covering set of M_L with K-cover, the minimal covered set of M_F with K-cover, and the set marking/transition separation instances.

Petri net analyzer with uncontrollable transitions: This program which is written by C++ and to compute the set of legal markings M_L, the set of first-met bad markings M_F, the minimal covering set of M_L, the minimal covered set of M_F, and the set marking/transition separation instances in the presence of uncontrollable transitions.

Redundant constraints identifying: This program which runs in Matlab is to identify redundant constraints in the ILP for supervisor synthesis.

ILP solving: This program which runs in LINGO is to solve the ILP to obtain coefficients of the minitors to be added to a plant net.

coefficients of PI to supervisor: This program which runs in Matlab is to compute the monitors to be added from the coefficients obtained by solving an ILP.

The related papers are:

[1]	Bo Huang, MengChu Zhou, GongXuan Zhang, Ahmed Chiheb Ammari, Ahmed Alabdulwahab, Ayman G. Fayoumi. Lexicographic multiobjective integer programming for optimal and structurally minimal Petri net supervisors of automated manufacturing systems. IEEE Transactions on Systems, Man, and Cybernetics: Systems, 2015, 45(11): 1459-1470.

[2]	Bo Huang, MengChu Zhou, GongXuan Zhang. Synthesis of Petri net supervisors for FMS via redundant constraint elimination. Automatica, 2015, 61: 156-163.

[3]	Bo Huang, MengChu Zhou, PeiYun Zhang, Jian Yang. Speedup Techniques for Multiobjective Integer Programs in Designing Optimal and Structurally Simple Supervisors of AMS. IEEE Transactions on Systems, Man, and Cybernetics: Systems, 2018, 48(1): 77-88.

[4]	Bo Huang, MengChu Zhou, YiSheng Huang, YuWang Yang. Supervisor Synthesis for FMS based on Critical Activity Places. IEEE Transactions on Systems, Man, and Cybernetics: Systems, 2017, DOI: 10.1109/TSMC.2017.2732442.
