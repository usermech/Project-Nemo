#

## Nemo Raspberry Pi installation instructions
Follow the steps decribed in
[Install Instruction Link](https://ubuntu.com/tutorials/how-to-install-ubuntu-on-your-raspberry-pi#1-overview) for installing Ubuntu server on Raspberry Pi

### Before first boot

update *config.txt* and *network-config* files placed under **system-boot** partition.

```shell
/media/username/system-boot$ nano config.txt
```

update the end of file as

```txt
[all]

# The following settings are "defaults" expected to be overridden by the
# included configuration. The only reason they are included is, again, to
# support old firmwares which don't understand the "include" command.

enable_uart=1
cmdline=cmdline.txt
start_x=1
gpu_mem=256

include syscfg.txt
include usercfg.txt
```

```shell
/media/username/system-boot$ nano network-config

```

update all file according to your network

```yaml
network:
  ethernets:
    eth0:
      dhcp4: false
      addresses: [DESIRED WIRED IP/24]
      gateway4: 192.168.1.1
      nameservers:
        addresses: [192.168.1.1, 8.8.8.8, 8.8.4.4]
      optional: true
  version: 2
  wifis:
    wlan0:
      dhcp4: false
      addresses: [DESIRED WIFI IP/24]
      gateway4: 192.168.1.1
      nameservers:
        addresses: [192.168.1.1, 8.8.8.8, 8.8.4.4]
      optional: true
      access-points:
        "WIFI SSID":
          password: "WIFI PASSWORD"

```

you can boot now

### Package update & upgrades

```shell
~$ sudo apt-get update
~$ sudo apt-get upgrade
```

---

### Enabling raspi-config

```shell
~$ wget http://mirrors.ustc.edu.cn/archive.raspberrypi.org/debian/pool/main/r/raspi-config/raspi-config_20220506_all.deb
~$ sudo dpkg -i raspi-config_20220506_all.deb
```

### Setting up xfce desktop and vnc server

```shell
~$ sudo apt-get install -y xfce4 xfce4-goodies
```

update xfwm4 to solve some button bugs within vnc sharing.

```shell
~$ wget http://launchpadlibrarian.net/494484969/xfwm4_4.14.5-1_arm64.deb
~$ sudo dpkg -i xfwm4_4.14.5-1_arm64.deb
```

install and run tightvnc server

```shell
~$ sudo apt-get -y install tightvncserver
~$ vncserver
```

now kill wrt tightvnc id, id written as "Xtightvnc :ID" on output

```shell
~$ ps -ef | grep Xtightvnc
~$ vncserver kill :ID
```

update *~/.vnc/xstartup* file after taking backup

```shell
~$ cp ~/.vnc/xstartup ~/.vnc/xstartup_backup
~$ nano ~/.vnc/xstartup
```

```sh
#!/bin/sh
def
export XKL_XMODMAP_DISABLE=1
unset SESSION_MANAGER
unset DBUS_SESSION_BUS_ADDRESS

xrdb $HOME/.Xresources
xsetroot -solid grey

startxfce4 &
```

save and reboot

```shell
~$ sudo reboot
```

```shell
~$  tmux new-session -s vncserver
~$  vncserver -geometry 1920x1080 -depth 24

Control+B, D
```


you can connect to server within port 5901

### Installing general dependencies

```shell
~$ sudo apt install git
~$ sudo apt-get install python3-dev
~$ sudo apt-get install python3-pip
~$ export PATH="$PATH:$HOME/.local/bin"
~$ sudo chmod -R a+rwx ~/.local/lib/python3.8
~$ source ~/.bashrc
~$ python3 -m pip install --upgrade pip
~$ sudo apt-get install python3-numpy
~$ sudo apt install libopencv-dev python3-opencv
~$ sudo apt-get install build-essential cmake gfortran unzip pkg-config
~$ sudo apt-get install libcanberra-gtk*
~$ sudo apt-get install gstreamer1.0-gtk3 gstreamer1.0-gl
~$ sudo apt-get install libv4l-dev v4l-utils
~$ sudo apt-get install protobuf-compiler
~$ sudo apt-get install qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools
```

## Installing ros2 foxy

Ref : <https://docs.ros.org/en/foxy/Installation/Ubuntu-Install-Debians.html>

```shell
~$ sudo apt update && sudo apt install locales
~$ sudo locale-gen en_US en_US.UTF-8
~$ sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
~$ export LANG=en_US.UTF-8
~$ sudo apt install software-properties-common
~$ sudo add-apt-repository universe
~$ sudo apt update && sudo apt install curl gnupg2 lsb-release
~$ sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key  -o /usr/share/keyrings/ros-archive-keyring.gpg
~$ echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(source /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null
~$ sudo apt update
~$ sudo apt upgrade
~$ sudo apt install ros-foxy-desktop
~$ echo 'source /opt/ros/foxy/setup.bash' >> ~/.bashrc
~$ source ~/.bashrc
~$ sudo apt install python3-colcon-common-extensions
~$ mkdir -p ~/nemo_ws/src
~$ cd ~/nemo_ws
~$ git clone https://github.com/ros2/examples src/examples -b foxy
~$ colcon build
~$ echo 'source ~/nemo_ws/install/setup.bash' >> ~/.bashrc
~$ echo 'source /usr/share/colcon_cd/function/colcon_cd.sh' >> ~/.bashrc
~$ echo 'export _colcon_cd_root=/opt/ros/foxy/' >> ~/.bashrc
~$ echo 'source /usr/share/colcon_argcomplete/hook/colcon-argcomplete.bash' >> ~/.bashrc
~$ source ~/.bashrc
```
# Camera Publisher Node Ros2
To install camera publisher in your workspace run following commands

```shell
~$ cd ~/nemo_ws/src
~/nemo_ws/src$ git clone --branch foxy https://gitlab.com/boldhearts/ros2_v4l2_camera.git
~/nemo_ws/src$ git clone --branch ros2 https://github.com/ros-perception/vision_opencv.git
~/nemo_ws/src$ git clone --branch foxy https://github.com/ros-perception/image_common.git
~/nemo_ws/src$ git clone --branch foxy-devel https://github.com/ros-perception/image_transport_plugins.git
~/nemo_ws/src$ cd ..
~/nemo_ws$ sudo apt install python3-rosdep2
~/nemo_ws$ rosdep update
~/nemo_ws$ rosdep install --from-paths src -r -y
~/nemo_ws$ colcon build --packages-up-to v4l2_camera image_transport_plugins
~/nemo_ws$ source install/local_setup.bash
~/nemo_ws$ echo 'source ~/nemo_ws/install/local_setup.bash' >> ~/.bashrc
~/nemo_ws$ source ~/.bashrc
```
## ROS Camera Publisher Package Usage and Parameters
```shell
~$ ros2 run v4l2_camera v4l2_camera_node --ros-args -p <parameter>:=<value>
```
### Parameters

video_device - string, default: "/dev/video0"
The device the camera is on.


pixel_format - string, default: "YUYV"
The pixel format to request from the camera. Must be a valid four
character 'FOURCC' code supported by
V4L2
and by your camera. The node outputs the available formats
supported by your camera when started.
Currently supported: "YUYV" or "GREY"


output_encoding - string, default: "rgb8"
The encoding to use for the output image.
Currently supported: "rgb8", "yuv422" or "mono8".


image_size - integer_array, default: [640, 480]
Width and height of the image.


time_per_frame - integer_array, default: current device setting
