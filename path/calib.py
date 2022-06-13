import cv2
import numpy as np
def calib(img_path):
    image=cv2.imread(img_path)
    image=cv2.resize(image,(864,648))
    coordinates=[]
    cv2.imshow("Calibrate",image)
    def click_event(event, x, y, flags, params):
        if event == cv2.EVENT_LBUTTONDOWN:
            print(x, ' ', y)
            coordinates.append([x,y])
    cv2.setMouseCallback('Calibrate', click_event)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    np.save("calib_coordinates.npy",coordinates)
    c=np.load("calib_coordinates.npy")
    print(c)