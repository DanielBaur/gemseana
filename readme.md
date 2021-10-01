
<h1><center> Readme: gemseana </center></h1>

<br>
<br>
<br>
<br>
<br>

This repository is meant to provide documentation on my GeMSE analysis routine - utilizing both the containered [GeMSE_environment](https://github.com/AG-Schumann/GeMSE_environment)
Diego developed and [Moritz von Sievers' analysis scripts](https://github.com/vonsivers/GeMSE_ROOT_scripts).

Chapters 2 and 3 of this readme resemble documentation of deprecated analysis infrastructures (i.e., the `bcfr102` machine and my Docker container); the aforementioned `singularity` container solution fully replaced those.

<br>
<br>

### Contents of this Repository

- `analysis_input_files`: some files necessary for GeMSE analysis (i.e. calibration, background, isotope data, energy resolution and efficiency simulations)
- `example_data__cuboid_04_lanza`: exemplary data to to test this workflow yourself
- `GeMSE_analysis`: [Moritz von Sievers' `GeMSE_analysis` repository](https://github.com/vonsivers/GeMSE_analysis) with the executables compiled in the `GeMSE environment`
- `GeMSE_ROOT_scripts`: [Moritz von Sievers' `GeMSE_ROOT_scripts` repository](https://github.com/vonsivers/GeMSE_ROOT_scripts) with the executables compiled in the `GeMSE environment`
- `readme.md`: this readme file
- `gemseana.py`: collection of GeMSE analysis Python functions (mainly calling the executables from Moritz von Sievers' scripts)

<br>
<br>


### Table of Contents


1. **[Analysis via Singularity Image](#1.-Analysis-via-Singularity-Image)**<br>


2. **[Deprecated: Local Analysis via Docker Image](#2.-Deprecated:-Local-Analysis-via-Docker-Image)**<br>


3. **[Deprecated: Remote Analysis on bcfr102](#3.-Deprecated:-Remote-Analysis-on-bcfr102)**<br>


<br>
<br>
<br>
<br>
<br>


# 1. Analysis via Singularity Image

(former: The Dumbshit's Guide to GeMSE Analysis)

<br>
<br>

Make your GeMSE analyses easy by utilizing the `GeMSE_environment` via `singularity`.

`singularity` is, just like `Docker`, a free software to run containerized applications (such as the the `GeMSE_environment`) on top of your local OS - guaranteeing that they will run without any problems.

Listed below you find a step-by-step documentation of my exemplary analysis workflow.
- I therefore use my APP desktop machine - logging in locally or remotely via ssh from my private laptop.
- In principle this should work exactly the same for any machine running `singularity` (v3.7.3+).
- Obviously the steps listed in `Preparations` you only have to do once. 
- In the scenario below I'll exemplarily analyze the `cuboid_05_grfln` sample ([ELOG entry 245](http://login.physik.uni-freiburg.de:8284/GeMSE/245)).

<br>

#### Preparations


- `$ . /usr/local/etc/bash_completion.d/singularity`<br><br>

- **Generating the Analysis Directories**<br>
Generate and change into the `gemse_analysis` folder within your `home` directory:<br>
`$ mkdir ~/gemse_analysis; cd ~/gemse_analysis` .<br>
The whole analysis will take place therein.
Furthermore also create the following directories:<br>
`$ mkdir ~/gemse_analysis/measurements; mkdir ~/gemse_analysis/gemseana` .<br>
These will soon house both this repository and the individual GeMSE measurements you're going to analyze.<br>

- **Building the `GeMSE_environment` Singularity Image**<br>
Build the `GeMSE_environment` from the Docker container (grab a cup of tea, this takes ~20 min):<br>
`$ SINGULARITY_TMPDIR=/scratch/db1086 singularity build ~/gemse_analysis/gemse_env_latest.simg docker://ramirezdiego/gemse_env:latest` .<br>
This environment will allow easy access to the basic analysis software.
Adding `SINGULARITY_TMPDIR=/scratch/db1086` is crucial as the storage allocated to the user's root partition is relatively small and `\tmp` might otherwise not suffice to hold the temporary build data.<br><br>

- **Cloning the `gemseana` Repository**<br>
Now clone this repository into your analysis directory:<br>
`$ git clone https://github.com/DanielBaur/gemseana ~/gemse_analysis/gemseana` .<br>
I force myself to explicitly use the code therein for all my sample analyses, ensuring that this repository is more or less up to date. In order to update to a newer version at a later point simply execute:<br>
`$ cd ~/gemse_analysis/gemseana; git pull origin master` .<br><br>

- **Getting GeMSE Data**<br>
Obtain the latest GeMSE data, either by getting/updating all of the data via `rsync` (NOT recommended, more than 100 GB !)<br>
`$ rsync --numeric-ids -rlvPu -e "ssh -p 1***" g*********@2**.***.***.**:/mnt/d/gemse_measurements/ ~/gemse_analysis/measurements'`<br>
or grab one specific measurement via `scp` (reccommended, the steps below follow this example)<br>
`$ scp -p 1*** g*********@2**.***.***.**:/mnt/d/gemse_measurements/2021_01-18_cuboid_05_grfln ~/gemse_analysis/measurements/2021_01-18_cuboid_05_grfln`<br>
You'll find the login credentials (i.e., port, login, IP and the GeMSE DAQ password) in the [APP wiki](https://wiki.uni-freiburg.de/app/doku.php?id=start:gemse).<br><br>

<br>

#### Analysis


- **Accessing the `GeMSE_environment` Singularity Image**<br>
Access the just built GeMSE environment singularity imageand mount (bind) your local analysis directory:<br>
`$ singularity shell ~/gemse_analysis/gemse_env_latest.simg`<br>
Alternatively you can also mount (i.e., bind) additional directories to the image (here: my `scratch` directory)<br>
`$ singularity shell --bind /scratch/db1086/gemse_analysis ~/gemse_analysis/gemse_env_latest.simg`<br>
Per default your local Linux system and the singularity image share your home directory.
So you always have access to the `~/gemse_analysis` directory you created before.<br><br>

- **Specifying Analysis Parameters**<br>
The actual analysis is orchestrated by the `gemse_sample_analysis.py` Python3 script, sitting in your measurement folder.
It's comfortable to use an existing one as a template; for example copy and adapt the one from the `~/gemse_analysis/gemseana/example_data__cuboid_04_lanza/` directory into your measurement folder:<br>
`$ cp ~/gemse_analysis/gemseana/example_data__cuboid_04_lanza/ ~/gemse_analysis/measurements/`
Adapt the `gemse_sample_analysis.py` Python3 script within the measurement folder:<br>
`$ vim ~/gemse_analysis/measurements/2021-04-02_cuboid_08_flsls/gemse_sample_analysis`<br>
This is the main work that needs to be done. The file should be sufficently commented to be self-explanatory. Alternatively you can also call the functions therin from a Jupyter notebook; I just opted for a Python script so one could easily analyze via ssh.<br><br>

- **Automatized Analysis**<br>
Finally execute the `gemse_sample_analysis.py` Python script:<br>
`$ python3 cd /scratch/gemse_analysis`<br>
If successful you'll end up with a lot of files in the measurement folder; the important ones being:
  - `.root`: final energy spectrum (further analysis)
  - `.json`: detailed analysis outcome (human readable, further analysis)
  - `.txt`: summarized analysis outcome (ready to be copied into the PTFEsc wiki note)

From my private laptop I then grab those files via `scp`:<br>
`$ scp db1086@:~/gemse_analysis/measurements/XXX ~/Desktop/arbeitsstuff/ptfesc/data/gemse/XXX`

- **Optional: Efficiency Simulation**<br>
One of the inputs for the automatized analysis is the (sample-specific) efficiency simulation in the form of a `.root` file.
In the following I'll document how to generate a suchlike file on the basis of the 'plastic_bags' sample.
  - Access the GeMSE environment singularity shell:<br>
`$ singularity shell ~/gemse_analysis/gemse_env_latest.simg`<br>
  - Source stuff:<br>
`$ cd ~/gemse_analysis/GeMSE_MC; source /opt/GeMSE/setup_sims.sh; make -j`
  - Now beginning with the actual work: First you need to define the sample geometry. Therefore take and edit one of the template files:<br>
`$ cp ~/gemse_analysis/gemseana/analysis_input_files/efficiency_simulation/sample_geometry__zeolite_powder.cc ~/gemse_analysis/gemseana/analysis_input_files/efficiency_simulation/sample_geometry__plastic_bags.cc`<br>
`$ vim ~/gemse_analysis/gemseana/analysis_input_files/efficiency_simulation/sample_geometry__plastic_bags.cc`<br>
  - This newly generated sample geometry now needs to be referenced in both the detector construction and the simulation macro:
    - In the detector construction (`~/gemse_analysis/GeMSE_MC/src/GeMSE_DetectorConstruction.cc`) uncomment the line 1061 (`#include <abspath_sample_geometry_file.cc>`) and enter the abspath toyour own file.<br>
    - In the simulation macro (`~/gemse_analysis/gemseana/analysis_input_files/efficiency_simulation/macro__efficiency_standard_isotopes_G4103p3.mac`) you need to specify the sample confinement (here `plastic_bags_filling`) in line 28 (`/GeMSE/gun/confine plastic_bags_filling`).
  - Remake the GeMSE_MC and run the efficiency simulation:<br>
`$ cd ~/gemse_analysis/GeMSE_MC/; make clean; make -j<br>`
`$ ./bin/Linux-g++/GeMSE_MC -m ~/gemse_analysis/gemseana/analysis_input_files/efficiency_simulation/macro__efficiency_standard_isotopes_G4103p3.mac -o ~/gemse_analysis/gemseana/analysis_input_files/efficiency_simulation/`<br>
 - Add the output files together and delete the constituent files:<br>
`$ cd ~/gemse_analysis/gemseana/analysis_input_files/efficiency_simulation/; hadd 20210616__simulated_efficiencies__plastic_bags.root results_run1.root results_run2.root ... results_run31.root; rm ./results_run*`<br>
  - Optional (on apppc37): Check whether the output geometry matches your expecation. Therefore execute the visualization macro:<br>
`$ cd ~/gemse_analysis/GeMSE_MC/macros/visualization_vrml.mac`<br>
And afterwards double-click onto the newly generated `~/gemse_analysis/GeMSE_MC/g4_00.wrl` file. A GUI depicting the GeMSE cavity will open. With a little bit of zooming and shifting you should be able to see an illustration of your sample geometry.

- **Optional: Exemplary Data Analysis**<br>
Hier könnte Ihre Werbung stehen.<br>
`$ python3 ~/gemse_analysis/gemseana/example_data__cuboid_04_lanza/gemse_sample_analysis.py`


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



