import cv2


def detect_plate(image):

    
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    
    blur = cv2.GaussianBlur(gray, (5, 5), 0)

    
    edges = cv2.Canny(blur, 100, 200)

    
    contours, _ = cv2.findContours(
        edges,
        cv2.RETR_LIST,
        cv2.CHAIN_APPROX_SIMPLE
    )

    best_plate = None
    best_coordinates = None
    best_score = 0

    
    for contour in contours:

        x, y, w, h = cv2.boundingRect(contour)

        
        if w < 80 or h < 15:
            continue

    
        aspect_ratio = w / float(h)

        
        if aspect_ratio < 2 or aspect_ratio > 6:
            continue

        
        contour_area = cv2.contourArea(contour)

        
        rectangle_area = w * h

        if rectangle_area == 0:
            continue

        
        rectangularity = contour_area / rectangle_area

    
        if rectangularity < 0.3:
            continue

    
        score = 0

        aspect_score = 1 - abs(aspect_ratio - 4) / 4
        score += aspect_score * 50

        score += rectangularity * 50

        if score > best_score:

            best_score = score

            best_plate = image[y:y+h, x:x+w]

            best_coordinates = (x, y, w, h)

    return best_plate, best_coordinates