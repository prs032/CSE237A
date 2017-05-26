'''
Eyes, Fist & Palm recognition using OpenCV
CSE 237A | Final Project
'''

import numpy as np
import cv2
import time

#Face Cascade
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
#Eye Cascade
eye_cascade = cv2.CascadeClassifier('haarcascade_eye.xml')
#Fist Cascade
fist_cascade = cv2.CascadeClassifier('fist.xml')
#Palm Cascade
palm_cascade = cv2.CascadeClassifier('palm.xml')

#Video capture & setting parameters
cap = cv2.VideoCapture(0)
#cap.set(3,800)
#cap.set(4,600)
#cap.set(5,0.1)

while 1:
    ret, img = cap.read()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)

    #Opening corresponding output files for input to supervised learning
    e = open("output_WC.txt","w+")
    e.write("0");

    f = open("output_HT.txt","w+")
    f.write("0");

    for (x,y,w,h) in faces:
        cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)
        roi_gray = gray[y:y+h, x:x+w]
        roi_color = img[y:y+h, x:x+w]

        eyes = eye_cascade.detectMultiScale(roi_gray)

        #Writing output for eyes detection
	if eyes is not None:
		e.seek(0);
		e.truncate();
		e.write("1");
		e.close();

        for (ex,ey,ew,eh) in eyes:
            cv2.rectangle(roi_color,(ex,ey),(ex+ew,ey+eh),(0,255,0),2)

    fist = fist_cascade.detectMultiScale(gray, 1.3, 5)
    a = 0;
    t = 0;

    for (x,y,w,h) in fist:
        cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)

        #Writing output for Fist detection
    	if fist is not None:
		f.seek(0);
		f.truncate();
		f.write("1");
		f.close();
        
    #Palm Detection currently on hold!
    #palm = palm_cascade.detectMultiScale(gray, 1.3, 5)

    '''
    elif palm is not None:
	f.seek(0);
	f.truncate();
	f.write("2");
	f.close();
    else:
	f.close();


    for (x,y,w,h) in palm:
    	cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)
    '''
    cv2.imshow('img',img)
    for i in range(1,5):
        for (x,y,w,h) in fist:
            if x>1:
              a = a + 1
              time.sleep(1)
              t = t +1 
        if (t == 5 and a>1):
            print(a)
      
    k = cv2.waitKey(30) & 0xff
    if k == 27:
        break

cap.release()
cv2.destroyAllWindows()
