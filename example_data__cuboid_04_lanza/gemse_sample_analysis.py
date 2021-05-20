


##############################################################
### user definitions #########################################
##############################################################


# paths
abspath_gemse_analysis_directory = "/home/db1086/gemse_analysis/" # adapt this to match your  (but: don't denote the home directory by '~')
abspath_gemseana = abspath_gemse_analysis_directory +"./gemseana/"
abspath_measurement_folder = abspath_gemse_analysis_directory +"gemseana/example_data__cuboid_04_lanza/"


# sample definitions
sample_id = "cuboid_04_lanza"
sample_mass = 30.42 # sample mass in kg
mca_list_files = [ # filenames of the mca list files
    "20201216__cuboid_04_lanza__old_veto__list_file_ch000.txt", # second file (the first one was empty due to crash)
    "20210102__cuboid_04_lanza__old_veto__list_file_ch000.txt", # third file (after second crash)
    "20210114__cuboid_04_lanza__old_veto__list_file_ch000_ch000.txt"] # fourth file (after third crash)
mca_time_windows = [ # time intervals (in seconds) indicating the time window of the mca file utilized for analysis ([0,0] resembles the whole file)
    [[0, 0]], # total: 1400380 s
    [[0, 0]], # total: 976128 s
    [[0, 0]]] # total: 324078 s
filename_calibration = "calibration_spectrum.txt.root_spectrum_0-2050707s.root_calibration_function.root"
filename_background = "background_combined.root"
filename_efficiency_simulation = "simulated_efficiencies.root"
filename_energy_resolution = "komar_gaon_ch00_full.dat.root_spectrum_calibrated_0-1188660s.root_resolution_function.root"





##############################################################
### imports ##################################################
##############################################################


import os
import sys
sys.path.append(abspath_gemseana)
import gemseana as ga


import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
from scipy import stats
import datetime
import math
import subprocess
import json


#import getpass
#from matplotlib.ticker import AutoMinorLocator
#plt.style.use('file://' +abspath_miscellaneous_figures +"danielsmplstyle.mplstyle")
#import pprint
#import itertools
#abspath_gemse_analysis = list(os.path.split(os.path.join(os.getcwd()))[0].split("/")) +"gemseana/"





##############################################################
### rate assessment ##########################################
##############################################################


if [False, True][0]:

    # looping over all mca list files
    for pathstring_raw_data in [abspath_measurement_folder +i for i in mca_list_files]:

        # mca list file ---> root file
        ga.make_rootfile_list(
            input_pathstring_mca_list_file = pathstring_raw_data,
            input_pathstring_calibration_function = abspath_gemseana +ga.relpath_calibration_files +filename_calibration,
            input_abspath_gemse_root_scripts = abspath_gemseana +ga.relpath_mvs_gemse_root_scripts)

        # root file ---> calibrated spectrum
        ga.make_spectrum_list(
            input_pathstring_root_file = pathstring_raw_data +".root",
            input_abspath_gemse_root_scripts = abspath_gemseana +ga.relpath_mvs_gemse_root_scripts,
            input_time_window = [0,0])

        # plot rate for all adc channels
        ga.plot_rate(
            input_pathstring_root_file = pathstring_raw_data +".root",
            input_abspath_gemse_root_scripts = abspath_gemseana +ga.relpath_mvs_gemse_root_scripts,
            input_energy_calibration = False,
            input_binwidth = 1800, # s
            input_pulse_height_range = [0,16383]) # adc channels

        # plot rate for energy window
        ga.plot_rate(
            input_pathstring_root_file = pathstring_raw_data +".root",
            input_abspath_gemse_root_scripts = abspath_gemseana +ga.relpath_mvs_gemse_root_scripts,
            input_energy_calibration = True,
            input_binwidth = 1800,
            input_pulse_height_range = [50,2700]) # keV


# '20201216__cuboid_04_lanza__old_veto__list_file_ch000.txt': use [0,0] ()
# '20210102__cuboid_04_lanza__old_veto__list_file_ch000.txt': use [0,0] ()
# '20210114__cuboid_04_lanza__old_veto__list_file_ch000_ch000.txt': use [0,0] ()





##############################################################
### Bayesian analysis ########################################
##############################################################


if [False, True][0]:

    ga.all_in_one_gemse_analysis(
        input_pathstrings_mca_list_files = [abspath_measurement_folder +i for i in mca_list_files],
        input_time_windows = mca_time_windows,
        input_pathstring_gemse_analysis_configuration_file = ga.gen_analysis_configuration_file(
            pathstring_output = abspath_measurement_folder +ga.filename_analysis_configuration_file,
            sample_name = sample_id,
            abspath_isotope_parameters_folder = abspath_gemseana +ga.relpath_isotope_data_files,
            abspath_sample_spectrum_root_file = abspath_measurement_folder +ga.filename_final_calibrated_added_spectrum,
            abspath_background_spectrum_root_file = abspath_gemseana +ga.relpath_background_files +filename_background,
            abspath_efficiency_root_file = abspath_gemseana +ga.relpath_efficiency_simulation_files +filename_efficiency_simulation,
            abspath_resolution_root_file = abspath_gemseana +ga.relpath_resolution_files +filename_energy_resolution,
            abspath_results_folder = abspath_measurement_folder,
            accuracy_of_mcmc = ["low", "medium", "high"][0],
            threshold_on_bayes_factor = 0.33,
            cl_for_activity_limit = 0.95,
            fract_uncert_efficency = 0.02,
            list_isotopes_to_analyze = ["U238", "Ra226", "Th228", "Ra228", "Co60", "K40", "Cs137"]),
        input_abspath_gemse_root_scripts = abspath_gemseana +ga.relpath_mvs_gemse_root_scripts,
        input_abspath_gemse_analysis = abspath_gemseana +ga.relpath_mvs_gemse_analysis,
        input_pathstring_calibration_function = abspath_gemseana +ga.relpath_calibration_files +filename_calibration)





### analysis aftermath
if [False, True][1]:

    # summarizing the analysis ---> .json file, wiki syntax file, commented spectrum plot
    pathstring_analysis_results_json_pathstring = ga.gemse_analysis_aftermath(
        input_filenames = mca_list_files,
        input_time_windows = mca_time_windows,
        input_sample_mass = sample_mass,
        input_pathstring_calibration_function = abspath_gemseana +ga.relpath_calibration_files +filename_calibration,
        input_pathstring_added_root_spectrum = abspath_measurement_folder +ga.filename_final_calibrated_added_spectrum,
        input_pathstring_gemse_analysis_summary = abspath_measurement_folder +sample_id +ga.filename_activities_summary,
        input_pathstring_wiki_syntax_output = abspath_measurement_folder +ga.filename_analysis_summary_wiki,
        input_pathstring_json_output = abspath_measurement_folder +ga.filename_analysis_summary_json)


    # printing the retrieved analysis results
    ga.print_analysis_results_nicely(pathstring_analysis_results_json_pathstring)













