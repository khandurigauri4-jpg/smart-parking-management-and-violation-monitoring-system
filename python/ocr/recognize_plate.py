import base64, json, re, sys
import cv2
import numpy as np
import pytesseract
from detection import detect_plate
from preprocessing import preprocess_plate

def normalize(text):
    return re.sub(r'[^A-Z0-9]', '', text.upper())

def main():
    payload = json.load(sys.stdin)
    raw = base64.b64decode(payload.get('imageBase64', ''), validate=True)
    if not raw:
        raise ValueError('Image data is empty.')
    image = cv2.imdecode(np.frombuffer(raw, dtype=np.uint8), cv2.IMREAD_COLOR)
    if image is None:
        raise ValueError('Could not decode image.')
    plate, coords = detect_plate(image)
    if plate is None:
        # Try OCR against the whole image as a fallback.
        plate = image
    processed = preprocess_plate(plate)
    # Upscale small plate crops and try several Tesseract page segmentation modes.
    enlarged = cv2.resize(plate, None, fx=2, fy=2, interpolation=cv2.INTER_CUBIC)
    candidates = [processed, enlarged, cv2.cvtColor(enlarged, cv2.COLOR_BGR2GRAY)]
    config_base = '-c tessedit_char_whitelist=ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'
    best_text = ''
    for candidate in candidates:
        for psm in (8, 11, 13, 7):
            text = pytesseract.image_to_string(candidate, config=f'--oem 3 --psm {psm} {config_base}').strip()
            normalized = normalize(text)
            if len(normalized) > len(normalize(best_text)):
                best_text = text
            # Typical Indian registration plates have 8-10 alphanumeric characters.
            if 8 <= len(normalized) <= 10:
                best_text = text
                break
        if 8 <= len(normalize(best_text)) <= 10:
            break
    plate_number = normalize(best_text)
    print(json.dumps({'detected': coords is not None, 'plateNumber': plate_number, 'rawText': best_text, 'coordinates': list(coords) if coords else None}))

if __name__ == '__main__':
    try: main()
    except Exception as exc:
        print(json.dumps({'error': str(exc)})); sys.exit(1)
