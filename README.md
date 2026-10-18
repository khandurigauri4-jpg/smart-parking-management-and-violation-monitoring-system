# Smart Parking Management System

Integrated local project using HTML/CSS/JavaScript, Node.js/Express, MongoDB/Mongoose, and C DSA (graph + Dijkstra shortest paths).

## Requirements (macOS)

- Node.js and npm
- MongoDB Community installed locally
- Apple Command Line Tools (`xcode-select --install`) for the `cc` C compiler

## Start the project

1. Start the local MongoDB service. If your installed version is 8.3:
   ```bash
   brew services start mongodb-community@8.3
   ```
   Use the version installed on your machine; do not downgrade an existing MongoDB data directory.

2. Configure the backend:
   ```bash
   cd backend
   cp .env.example .env
   npm install
   npm start
   ```
   If you already have a working `.env`, keep it; do not overwrite it. The backend compiles `dsa/parking/bridge.c`, `parking.c`, and `graph.c` on startup. The compiled binary is generated locally and is ignored by Git.

3. Open `frontend/index.html` in your browser, or use VS Code Live Server.

4. Verify the backend and database:
   ```bash
   curl http://localhost:3000/api/health
   curl http://localhost:3000/api/parking
   ```

## Integrated features

- Parking status, vehicle allocation, release, session history and fee calculation.
- MongoDB persistence for parking slots, sessions, reservations, vehicles, users and violations.
- Reservations: create/list/cancel; validates future dates, slot range, blacklisted vehicles and overlapping bookings for a slot.
- C DSA bridge called by Node.js:
  - `GET /api/parking/nearest`: selects the closest available slot using the graph and Dijkstra algorithm.
  - `GET /api/parking/route/:slotId`: computes a shortest path from the entrance to a slot.
  - `POST /api/parking/park`: asks the C graph engine to choose the nearest eligible slot, then persists the allocation in MongoDB.
- Frontend navigation controls display nearest slot, path and graph distance.

## API quick checks

```bash
curl http://localhost:3000/api/health
curl http://localhost:3000/api/parking
curl http://localhost:3000/api/parking/nearest
curl http://localhost:3000/api/parking/route/10
curl http://localhost:3000/api/reservations
```

## Important limitations

- The route distances are illustrative units for the sample 10-slot graph, not real-world metres.
- Reservation conflict checks cover overlapping bookings for the same slot. Since an active parking session has no scheduled departure time, the system cannot guarantee that a currently occupied slot will be free at a future reservation start.
- User creation hashes passwords, but login/authentication and role-based authorization are not implemented. Do not expose admin/user APIs publicly until those protections are added.
- This is a local academic project, not a production-ready payment or security system.
