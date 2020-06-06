# Water Marker

you select some folder and the image you want to set to it.
edit it to your needs in the settings (location, size, radius). 

click on `WaterMark it!` and he will take all the pictures in the 
given folder, add the image to it and store it in a new folder.


# how to set up

development happend on visual studio (latest) on Windows so any issues on other enviroment 
can happen i will be happy to help (at least if i can)

at this program we are using `opencv` and as UI `wxWidgets` 
for this project to run properly you need to install those libraries.

the following links will help you with installing the libraries:
- https://www.deciphertechnic.com/install-opencv-with-visual-studio/ (opencv)
- https://docs.wxwidgets.org/trunk/plat_msw_install.html (wxWidgets)

if you done this correctly you need to check the following 
goto `Project > Properties > Configuration Properties >`
- the Directory paths in ` C/C++ > General > Additional Include Directories` need to match with the folder your libraries is in yours
- the Directory paths in ` Linker > General > Additional Library Directories` need to match with the folder your libraries is in yours

build the project and he should run like a sharm 

bye,

Niek Tuytel (@Okido)

some pictures:

