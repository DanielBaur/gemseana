
<h1><center> Readme: gemseana </center></h1>

<br>
<br>
<br>
<br>
<br>

This repository is meant to provide documentation on my personal use of the containered [GeMSE_environment](https://github.com/AG-Schumann/GeMSE_environment) to analyze GeMSE samples. Hopefully this readme can give you a introduction to your very own GeMSE analysis.

Chapters 2 and 3 of this readme  resemble documentation of outdated analysis infrastructures; the aforementioned container solution fully replaced those.

Hier könnte Ihre Werbung stehen.

<br>
<br>

### Contents of this Repository

- `readme.ipynb`: this file
- ``gemseana.py``: a collection of Python functions (mainly calling [Moritz von Sievers' analysis scripts]())

<br>
<br>


### Table of Contents


1. **[Local Analysis via Singularity Image](#1.-Local-Analysis-via-Singularity-Image)**<br>


2. **[Deprecated: Local Analysis via Docker Image](#2.-Deprecated:-Local-Analysis-via-Docker-Image)**<br>


3. **[Deprecated: Remote Analysis on bcfr102](#3.-Deprecated:-Remote-Analysis-on-bcfr102)**<br>


<br>
<br>
<br>
<br>
<br>


# 1. Local Analysis via Singularity Image

(former: The Dumbshit's Guide to GeMSE Analysis)

<br>
<br>

Make your GeMSE analyses easy by utilizing the `GeMSE_environment` via `singularity`.

`singularity` is, just like `Docker`, a free software to run containerized applications (such as the the `GeMSE_environment`) on top of your local OS - guaranteeing that they will run without any problems.

Listed below you find a step-by-step documentation of my exemplary analysis workflow. I therefore use the APP machine. But in principle the following steps should work exactly the same for any machine running singularity (v3.7.3+).

<br>

#### Preparations

The following steps you only have to do once when setting the GeMSE analysis environment

- `$ . /usr/local/etc/bash_completion.d/singularity`<br><br>

- Generate and change into the `gemse_analysis` folder within your `home`:<br>`$ mkdir ~/gemse_analysis; cd ~/gemse_analysis`<br>The whole analysis will take place in this directory.<br><br>

- Build the GeMSE environment singularity image from the Docker container (grab a cup of tea, this takes ~20 min):<br>`$ SINGULARITY_TMPDIR=/scratch/db1086 singularity build ~/gemse_analysis/gemse_env_latest.simg docker://ramirezdiego/gemse_env:latest`<br>This environment will allow easy access to the basic analysis software. Adding `SINGULARITY_TMPDIR=/scratch/db1086` is crucial as the storage allocated to the user's root partition is relatively small and `\tmp` might otherwise not suffice to hold the temporary build data.<br><br>

- Install Moritz von Siever's `GeMSE_ROOT_scripts` and `GeMSE_analysis` repositories, containing the standard analysis scripts:
  - Clone the repositories:<br>`$ cd ~/gemse_analysis; git clone https://github.com/vonsivers/GeMSE_ROOT_scripts.git`<br>`$ cd ~/gemse_analysis; git clone https://github.com/vonsivers/GeMSE_analysis`<br>
  - Rename `ifstream` with `std::ifstream` in the following files:
    - `jfk`
  - To finally compile the corresponding executables execute:<br>`$ cd ~/gemse_analysis/GeMSE_ROOT_scripts; make clean; make`<br>`$ cd ~/gemse_analysis/GeMSE_analysis; make clean; make`<br><br>

- Clone Daniel's `gemseana` repository (the one including this readme):<br>`$ cd ~/gemse_analysis; git clone https://github.com/DanielBaur/gemseana ~/gemse_analysis/gemse_analysis_documentation`<br>Besides some irrelevant stuff this repository contains my personal analysis scripts. I force myself to explicitly use the analysis code therein so that this repository is more or less up to date; maybe someone can benefit from it.

<br>

#### Actual Analysis


- Obtain the latest GeMSE data, either via `rsync`<br>`$ rsync XXX`<br> or via `scp`<br>`$ scp XXX`<br><br>

- Access the just built GeMSE environment singularity image and mount (bind) your local analysis directory:<br>`$ singularity shell --bind /scratch/db1086/gemse_analysis /scratch/db1086/gemse_analysis/gemse_env_latest.simg`<br><br>

- Adapt the `gemse_sample_analysis.py` Python script:<br>`$ vim /scratch/db1086/gemse_analysis/measurements/2021-04-02_cuboid_08_flsls/gemse_sample_analysis`<br>This is the main work that needs to be done. The file should be sufficently commented to be self-explanatory. Alternatively you can also call the functions therin from a Jupyter notebook; I just opted for a Python script so one could easily analyze via ssh.<br><br>

- Finally execute the `gemse_sample_analysis.py` Python script:<br>`$ cd /scratch/gemse_analysis`

<br>

#### Notes

- Obviously you'll have to adapt the infrastructure denoted above: E.g., replace `db1086` with `pm1620`.<br><br>

- Once you cloned the `gemse_analysis_documentation` repository make sure you also adapt the executable paths within the `gemse_analysis.py` file to match your file structure.

<br>
<br>
<br>
<br>
<br>


# 2. Deprecated: Local Analysis via Docker Image

<br>
<br>

If you have been referred to this repository because you'd like to perform some GeMSE analysis on your local machine then this section is made specifically for you! Instead of the manually executing the GeMSE analysis scripts on a remote server you can set up a GeMSE analysis docker container allowing to locally analyze screening data using automatized scripts in a familiar Jupyter Notebook environment. The following lines are supposed to guide you through both the setup process as well as an exemplary analysis using the example measurement data saved within this repository.

<br>

#### Short Introduction to Docker

<br>

Hier könnte Ihre Werbung stehen.

<br>

#### Prerequisites: Installing Docker, Pulling the GeMSE Analysis Image and Saving the Exemplary Measurement Data

- In order to install `Docker` follow the instructions for your local system in the [official documentation](https://docs.docker.com/engine/install/ubuntu/). Test whether your installation was successful by executing:<br>
`$ sudo docker run hello-world`


- Obtain the GeMSE analysis Docker image by executing:<br>
`$ sudo docker pull nequiem/gemse_analysis:latest`


- Clone this repository to some path (e.g., your `home` directory):<br>
`$ cd; git clone https://github.com/DanielBaur/gemse_analysis_documentation.git`

<br>

#### Running the Container

Run the `gemseana` container with an interactive terminal (`-it`) based upon the just downloaded `gemse_analysis:1.0` image:<br><br>
  `$ sudo docker run -p 8880:8880 --name gemseana -v ~/gemse_analysis_documentation/exemplary_gemse_measurements:/home/gemse_analysis_infrastructure/ex -it nequiem/gemse_analysis:latest`<br><br>
This command runs the `gemse_analysis` container within which you have access to Moritz' analysis scripts just like you'd have on the `bcfr102` machine. The executables are located within the `/home/gemse_analysis_infrastructure` directory. The flag `-v ~/gemse_analysis_documentation/exemplary_gemse_measurements:/home/gemse_analysis_infrastructure/ex` makes sure a local directory (here: the directory with the exemplary data from this repository) is mounted to the container file system. Within there you can persistently modify your local data.

<br>

#### Jupyter Notebook

Of course it would be even more convenient to run the analysis in a jupyter notebook environment. Therefore start a jupyter notebook serer from within the container:<br>
`$ jupyter notebook --port=8880 --no-browser --ip=0.0.0.0 --allow-root`<br>
Then, on your local machine open any browser and access the followiwng URL<br>
`http://127.0.0.1:8880/?token=<token>`<br>
with `<token>` being the forwarded token from the above `jupyter notebook` call.<br>
Once logged in, open up the exemplary analysis notebook `exemplary_gemse_analysis_notebook.ipynb`. This notebook contains function calls to the C++ executables of Moritz' scripts that are supposed to make the analysis even more efficient.

<br>
<br>
<br>
<br>
<br>

# 3. Deprecated: Remote Analysis on bcfr102

<br>
<br>

Data recorded with the GeMSE screening facility is/was analyzed utilizing the GeMSE analysis framework installed on ``bcfr102``, a server that was brought from Bern to Freiburg. You can access the machine via ``ssh`` - but only from ``pcfr31``. (In order to do so you first need to generate an ssh key pair. Send the public one to Diego; he needs to register it on the GeMSE machine. Also implement the other one into your ssh configuration file on `pcfr31`. Also see [here](https://askubuntu.com/questions/311558/ssh-permission-denied-publickey) (I had to add `IdentitiesOnly yes`).)

<br>

The following lines give insight to usage of the analysis scripts installed on ``bcfr102``.

<br>

### Data Submission

1. Copy the raw data from your local machine onto `pcfr31`:<br>
`$ scp -r <abspath_to_raw_data_directory> <your_pcfr31_account>@pcfr31.physik.uni-freiburg.de:<abspath_to_intermediate_folder_on_pcfr31>`<br>
e.g.,:<br>
`$ scp -r ./data dbaur@pcfr31.physik.uni-freiburg.de:~/notebooks/ptfesc/data`


2. Log into `pcfr31` (default login and password: ``dbaur`` and ``%b%%%1%%%!``):<br>
`$ ssh -XY <your_pcfr31_account>@pcfr31.physik.uni-freiburg.de`


3. Once logged into `pcfr31`, copy the raw data directory further onto `bcfr102`:
`$ scp -r <abspath_to_intermediate_folder_on_pcfr31> gemse@bcfr102:<abspath_to_raw_data_directory_on_bcfr102>`<br>
e.g.,:<br>
`$ scp -r ~/notebooks/ptfesc/data gemse@bcfr102:~/daniel/data`


4. From `pcfr31` log into `bcfr102`:<br>
`$ ssh -Y gemse@bcfr102`


5. Once the analysis (i.e., the steps below) is finished retrieve the data in the same way that you uploaded it.

<br>

### GeMSE Analysis

<br>

- `make_rootfile_list`
- `make_spectrum_list`
- `add_spectrum`
- `gemse_analysis`


1. **Generating a Calibrated Energy Spectrum**<br>


  - In order to generate a calibrated energy spectrum one first needs to convert the list file saved by the MCA into a .root file. Therefore execute the following:<br>
`$ <abspath_to_make_rootfile_list_executable> <abspath_to_mca_list_file> <abspath_to_calibration_function>`<br>
e.g.:<br>
`$ ~/GeMSE_ROOT_scripts_copy_20200904/make_rootfile_list ~/daniel/20200904__data/20191010__ptfe_01_gf/2019-10-28_PTFE_Daniel_ch000_Copy20191115_rightAfterClosingGN2valve.txt /home/gemse/measurements/samples/2018-10-08_RockSample_Beda/sample_spectrum/energy_calibration/calibration_spectrum.txt.root_spectrum_0-2050707s.root_calibration_function.root`


  - Next you need to generate a spectrum out of the just generated .root file. Execute<br>
  `$ <abspath_to_make_spectrum_list_executable> --file <abspath_to_rootfile_list> --energy`<br>
  e.g.:<br>
  `$ ~/GeMSE_ROOT_scripts_copy_20200904/make_spectrum_list --file ~/daniel/20200904__data/20191010__ptfe_01_gf/2019-10-28_PTFE_Daniel_ch000_Copy20191115_rightAfterClosingGN2valve.txt.root --energy`<br> 


2. **Cutting the Recorded Data in Time (Optional)**


  - 
  - 
  - 


3. **Running the actual Analysis**

In order to execute the final analysis script, you have to specify a configuration file indicating the analysis settings (chapter 2 contains an example):


`$ <abspath_to_GeMSE_analysis_executable> <abspath_to_analysis_config_file>`


e.g.,


`$ ~/GeMSE_analysis_copy_20200908/GeMSE_analysis ~/daniel/20200908_ptfe_bricks_simple_analysis/results/kljhsdfkl.txt`

<br>
<br>
<br>
<br>
<br>



