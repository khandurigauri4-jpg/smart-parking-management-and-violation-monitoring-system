import cv2


def preprocess_plate(plate):
    
    
    

    
    gray = cv2.cvtColor(plate, cv2.COLOR_BGR2GRAY)

    
    blur = cv2.GaussianBlur(gray, (3, 3), 0)


    _, threshold = cv2.threshold(
        blur,
        0,
        255,
        cv2.THRESH_BINARY + cv2.THRESH_OTSU
    )

    return threshold