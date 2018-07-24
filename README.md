# Raw Video Container Format - GSoC 2018

This repository hosts a working proof of concept for proposed MLV file containerising systems for the AXIOM Beta's RAW12 frames.

For a wholesome analysis of this GSoC work, please visit [gsoc.supragyaraj.com](gsoc.supragyaraj.com)

GSoC project: [https://summerofcode.withgoogle.com/projects/#4617058810068992](https://summerofcode.withgoogle.com/projects/#4617058810068992)

## What does the code do?

The source code compiles a few binaries and runs them to emulate the usage scenario of the camera. The following are some
important modules:

1. **Generator**: This compiles two files - rawinfo and rawdata that models the two streams coming out
of the camera - rawdata being the high speed video frame transport and rawinfo being low speed meta transport.
2. **Stream Handler**: This application models the primary interface code for the recording unit. Uses two threads to
store two streams on disk as fast as it can. Highly I/O based application with very little processing involved.
3. **Joiner**: Joins the high speed output (video frames) and low speed output (metadata) into one MLV file. Uses `cat`
internally to join. However, there are a few considerations on whether this module may really be needed. See [gsoc.supragyaraj.com](gsoc.supragyaraj.com)
for more details.
4. **MLV_dump / MLVFS**: Final "publisher" system (3rd party) which is used to convert mlv files into corresponding DNG
files.

1, 2 and 3 model the `cam2mlv` portion of the system while 4 is solely responsible for `mlv2dng` conversion.

A few changes to `mlv2dng` are proposed for

- Removing the Joiner altogether
- Allowing PLR data to be used for linearization table calculations while mlv2dng conversion.

The information regarding the above proposals are given at: [gsoc.supragyaraj.com](gsoc.supragyaraj.com)


## Getting the code and setting up

For getting started with this codebase, consider checking out: [gsoc.supragyaraj.com](gsoc.supragyaraj.com) for all the 
analysis and the ideology behind the code seen in this repository.

1. Get the code using `git clone github.com/supragya/AXIOM_RawStreamHandler`
2. `cd AXIOM_RawStreamHandler` after this should put you in the project directory.
3. Make sure you have all the dependencies by running `./scripts/get_dependencies_apt.sh` (for debian based linux distributions).
4. Build the system using `./scripts/build_all.sh`
5. Run the emulation using `./scripts/run_emulation.sh`
6. DNG exports should now be available in `processed_data` folder.
7. Clean the project directory using `./scripts/clean_project.sh`

*Note: You can check your current dependencies version with `./scripts/check_verions.sh`*


## Get in touch
Freenode IRC `#apertus`