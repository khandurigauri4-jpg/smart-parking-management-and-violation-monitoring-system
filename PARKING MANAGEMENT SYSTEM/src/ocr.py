
import cv2
import pytesseract
import re
import os

# Existing Tesseract installation
pytesseract.pytesseract.tesseract_cmd = (
    r"C:\Program Files\Tesseract-OCR\tesseract.exe"
)

# Common Indian registration patterns:
# Two letters + two digits + one/two letters + four digits
# Some older registrations have one digit in the RTO code.
PLATE_PATTERNS = [
    re.compile(r"^[A-Z]{2}\d{1,2}[A-Z]{1,3}\d{4}$"),
]


def read_plate(plate):
    if plate is None or plate.size == 0:
        return ""

    if len(plate.shape) == 3:
        gray = cv2.cvtColor(plate, cv2.COLOR_BGR2GRAY)
    else:
        gray = plate.copy()

    # Upscale the existing crop without stretching its proportions
    gray = cv2.resize(
        gray, None, fx=3, fy=3,
        interpolation=cv2.INTER_CUBIC
    )

    # Add a border after resizing
    gray = cv2.copyMakeBorder(
        gray, 20, 20, 25, 25,
        cv2.BORDER_CONSTANT, value=255
    )

    os.makedirs("output", exist_ok=True)
    cv2.imwrite("output/ocr_enlarged.jpg", gray)

    # Lightweight OCR attempts
    configs = [
        "--oem 3 --psm 7",
        "--oem 3 --psm 8",
    ]

    candidates = []

    for config in configs:
        raw = pytesseract.image_to_string(
            gray,
            lang="eng",
            config=(
                config
                + " -c tessedit_char_whitelist=ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
            )
        )

        text = re.sub(r"[^A-Z0-9]", "", raw.upper())

        if text:
            print(f"{config}: {text}")
            candidates.append(text)

    if not candidates:
        print("OCR could not read the plate.")
        return ""

    # Rank candidates by common Indian registration format.
    # This is a heuristic, not proof of correctness.
    valid = [
        text for text in candidates
        if any(pattern.fullmatch(text) for pattern in PLATE_PATTERNS)
    ]

    if valid:
        result = max(valid, key=len)
        print("Format-matching candidate:", result)
        return result

    # No format match: report the most frequently returned candidate.
    result = max(set(candidates), key=candidates.count)
    print("Unverified OCR candidate:", result)
    return result