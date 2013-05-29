Pingpong
========

use opencv detect biggest color contour then apply to pingpong game


##############################
### How to setup ###
- add include path
- add library path

### Debug Input Library ###
opencv_core243d.lib
opencv_highgui243d.lib
opencv_imgproc243d.lib
opencv_features2d243d.lib
opencv_calib3d243d.lib
opencv_ml243d.lib
Winmm.lib

### Release Input Library ###
opencv_core243.lib
opencv_highgui243.lib
opencv_imgproc243.lib
opencv_features2d243.lib
opencv_calib3d243.lib
opencv_ml243.lib
Winmm.lib

============

# Change Log #

### v1.0 [2013.05.28]
- fixed bug when get winner, show text
- fixed other bug


### v1.01beta2 [2013.05.22]
[for anniversary of 20th years faculty of Engineering NU]
- normalize color before detect color processing = detecting color better
- use smaller image to processing = faster performance
- can select display screen resolution [support 640x480, 800,600, 1024x768, 1366x768, 1280x1024, 1920x1080]


### v1.0beta [2013.03]
[for present subject project]
- detect blue and pink
- added ball speed function
