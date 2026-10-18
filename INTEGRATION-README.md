# Team integration notes

This project keeps the existing HTML/CSS/JS frontend, Express/MongoDB backend, and original C parking/graph engine. It adds:

- `dsa/mradul/`: Mradul's C stack, queue and linked-list modules plus a command-line bridge.
- `cpp-oop/`: Devansh's C++ classes plus a non-interactive JSON bridge used by the backend for ticket fee/overstay calculations.
- `python/ocr/`: Divya's plate detection/preprocessing plus a Tesseract OCR step.
- `backend/models/GateAction.js`: persisted entry/exit action records; the stack is rebuilt from MongoDB so process restarts do not erase history.

## Requirements

- Node.js and npm
- C compiler (`cc`), `make`, and C++ compiler (`g++` or compatible)
- MongoDB configured via `backend/.env` (`MONGODB_URI`)
- Python 3, Tesseract OCR executable, and Python packages in `python/requirements.txt`

Install OCR dependencies (macOS):

```bash
brew install tesseract
python3 -m pip install -r python/requirements.txt
```

Run backend:

```bash
cd backend
npm install
npm run check
npm start
```

## Added API endpoints

- `GET /api/parking/dsa/active-search?vehicleNumber=DL...` — linked-list search against active MongoDB occupancy.
- `GET /api/parking/dsa/active-vehicles` — linked-list occupancy index.
- `GET /api/parking/dsa/reservation-queue` — C queue view of future/current booked reservations (capacity 50).
- `GET /api/parking/dsa/action-stack` — LIFO C stack view of latest entry/exit actions.
- `POST /api/ocr/recognize` with JSON `{ "imageBase64": "..." }` — detects and reads a plate; OCR text must be verified by the operator.

C++ billing is used on release: two-wheelers cost ₹10 per started 30-minute block; four-wheelers cost ₹20 per started 30-minute block. Overstay penalty is charged again for overstay blocks. Vehicles not registered with category default to four-wheeler. Default allowed duration is 2 hours unless the vehicle has an active reservation, in which case reservation duration is used.

## Important integration boundary

MongoDB remains the persistent source of truth. C data structures are rebuilt from stored records per API request; they are not treated as durable storage. OCR is best-effort and should never automatically authorize a vehicle without operator confirmation.


## OCR setup (macOS)
From the project root, install the system OCR engine and Python dependencies:

```bash
brew install tesseract
python3 -m venv .venv
source .venv/bin/activate
python -m pip install --upgrade pip
python -m pip install -r python/requirements.txt
```

Restart the backend after setup. Check OCR with `curl -i http://localhost:3000/api/ocr/health`. On the Live Parking Slots page, upload a clear number-plate image; after successful recognition, the app automatically submits the plate to the parking API and assigns the nearest available slot.
