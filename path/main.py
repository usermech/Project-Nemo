import mapping
import calib
import rrt
import shutil

calibration=True
img_path="3.jpg"
xstart=703
ystart=246
xfin=229
yfin=315
try:
    shutil.rmtree("media")
except:
    print("Dir already clean")
if calibration==True:
    calib.calib(img_path)
mapping.mapping(img_path)
rrt.RRT_call(xstart,ystart,xfin,yfin,"map.jpg")
