import cv2
import numpy as np
import pandas as pd
from skimage.measure import label, regionprops, regionprops_table
from skimage.io import imread, imshow
import matplotlib.pyplot as plt

def mapping(image_path):
    image=cv2.imread(image_path)
    image=cv2.resize(image,(864,648))
    img_grey = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    imgor=image.copy()
    ret,thresh = cv2.threshold(img_grey,0,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)
    edges = cv2.Canny(image,50,110)
    kernel = np.ones((3,3),np.uint8)
    edges= cv2.dilate(edges,kernel,iterations = 2)
    image_fin=cv2.bitwise_xor(thresh,edges)
    image_fin = cv2.erode(image_fin, kernel, iterations=2)
    label_im = label(image_fin)
    regions = regionprops(label_im)
    properties = ["area","convex_area","bbox_area", "extent",  
                "mean_intensity", "solidity", "eccentricity", 
                "orientation"]
    pd.DataFrame(regionprops_table(label_im, img_grey, 
                properties=properties))

    masks = []
    bbox = []
    list_of_index = []
    for num, x in enumerate(regions):
        area = x.area
        convex_area = x.convex_area
        if (num!=0 and (area>10)):
            masks.append(regions[num].convex_image)
            bbox.append(regions[num].bbox)   
            list_of_index.append(num)
    count = len(masks)

    rgb_mask = np.zeros_like(label_im)
    for x in list_of_index:
        rgb_mask += (label_im==x+1).astype(int)
    red  =  imgor[:,:,0] * rgb_mask
    green = imgor[:,:,1] * rgb_mask
    blue  = imgor[:,:,2] * rgb_mask
    image = np.dstack([red, green, blue])
    masks_new=[]
    bbox_new=[]
    list_of_index_new = []
    labels_new=[]
    cv2.imwrite("try.jpg",image)
    x=cv2.imread("try.jpg")
    """
    for b in bbox:
        cv2.rectangle(x, (b[0], b[1]), (b[2], b[3]), (255,0,0), 2)
    """
    """
    cv2.imshow("try",x)
    def click_event(event, x, y, flags, params):
        if event == cv2.EVENT_LBUTTONDOWN:
            print(x, ' ', y)
            for num, r in enumerate(regions):
                if x>r.bbox[0] and x<r.bbox[2] and y>r.bbox[1] and y<r.bbox[3]:
                    masks_new.append(regions[num].convex_image)
                    bbox_new.append(regions[num].bbox)   
                    list_of_index_new.append(num)
            if label_im[x][y] not in labels_new:
                labels_new.append(label_im[y,x])
    cv2.setMouseCallback('try', click_event)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    """
    calib_points=np.load("calib_coordinates.npy")
    for point in calib_points:
        for num, r in enumerate(regions):
            if point[0]>r.bbox[0] and point[0]<r.bbox[2] and point[1]>r.bbox[1] and point[1]<r.bbox[3]:
                masks_new.append(regions[num].convex_image)
                bbox_new.append(regions[num].bbox)   
                list_of_index_new.append(num)
                labels_new.append(label_im[point[1],point[0]])
    for item in labels_new:
        if labels_new.count(item)<2:
            labels_new.remove(item)
    list(dict.fromkeys(labels_new))
    rgb_mask_new = np.zeros_like(label_im)
    for x in labels_new:
        if x!=0:
            rgb_mask_new += (label_im==x).astype('uint8')
    rgb_mask_new = rgb_mask_new.astype('uint8')
    kernel = np.ones((11,11),np.uint8)
    rgb_mask_new=cv2.morphologyEx(rgb_mask_new, cv2.MORPH_CLOSE, kernel)
    red_new  =  imgor[:,:,0] * rgb_mask_new
    green_new = imgor[:,:,1] * rgb_mask_new
    blue_new  = imgor[:,:,2] * rgb_mask_new
    image_new = np.dstack([red_new, green_new, blue_new])
    cv2.imwrite("try.jpg",image_new)
    x=cv2.imread("try.jpg")
    ret,y = cv2.threshold(x,1,255,cv2.THRESH_BINARY)
    y=cv2.cvtColor(y, cv2.COLOR_BGR2GRAY)
    imgfin=imgor.copy()
    for r in range(864):
        for c in range(648):
            if y[c,r]>=1:
                imgfin[c,r,1]=255
                imgfin[c,r,0]=255
                imgfin[c,r,2]=255
    cv2.imwrite("map.jpg",y)
    cv2.imshow("try",imgfin)
    cv2.waitKey(0)

