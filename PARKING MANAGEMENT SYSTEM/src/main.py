import cv2
import os

from detection import detect_plate
from preprocessing import preprocess_plate



image = cv2.imread("input/images.jpg")

if image is None:
    print("ERROR: Image could not be loaded.")
    exit()

print("SUCCESS: Image loaded!")


plate, coordinates = detect_plate(image)


if plate is not None:

    print("NUMBER PLATE DETECTED!")

    x, y, w, h = coordinates

    
    cv2.rectangle(
        image,
        (x, y),
        (x + w, y + h),
        (0, 255, 0),
        2
    )

    
    os.makedirs("output", exist_ok=True)

    
    cv2.imwrite("output/plate.jpg", plate)

    print("Plate saved to output/plate.jpg")



    processed_plate = preprocess_plate(plate)

    cv2.imwrite(
        "output/processed_plate.jpg",
        processed_plate
    )

    print("Processed plate saved!")


else:

    print("NO NUMBER PLATE DETECTED.")



cv2.imshow("Plate Detection", image)

cv2.waitKey(0)

cv2.destroyAllWindows()