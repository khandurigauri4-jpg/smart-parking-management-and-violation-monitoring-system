const API = "http://localhost:3000/api";
const PARKING = `${API}/parking`;
const $ = (id) => document.getElementById(id);
let toastTimer;
async function request(url, options = {}) { const response = await fetch(url, options); let data = {}; try { data = await response.json(); } catch (_) {} if (!response.ok) throw new Error(data.message || `Request failed (${response.status})`); return data; }
function message(text) { const box = $("message"); if (!box) return; box.textContent = text; box.style.display = "block"; clearTimeout(toastTimer); toastTimer = setTimeout(() => box.style.display = "none", 5000); }
function date(value) { return value ? new Date(value).toLocaleString() : "—"; }
function setActiveNav() { const file = location.pathname.split("/").pop() || "index.html"; const map = {"index.html":"dashboard","":"dashboard","reserve.html":"reserve","slots.html":"slots","history.html":"history"}; document.querySelectorAll("nav a[data-page]").forEach(a => a.classList.toggle("active", a.dataset.page === map[file])); }
function makeSlot(slot) { const article = document.createElement("article"); article.className = `slot ${slot.occupied ? "occupied" : "available"}`; const h = document.createElement("h3"); h.textContent = `Slot ${slot.slotId}`; const p = document.createElement("p"); p.textContent = slot.occupied ? `Occupied${slot.vehicleNumber ? ` · ${slot.vehicleNumber}` : ""}` : "Available"; article.append(h,p); return article; }
async function loadSlots() { const data = await request(PARKING); const slots = data.slots || []; if ($("totalSlots")) $("totalSlots").textContent = data.totalSlots ?? slots.length; if ($("availableSlots")) $("availableSlots").textContent = data.availableSlots ?? slots.filter(s=>!s.occupied).length; if ($("occupiedSlots")) $("occupiedSlots").textContent = data.occupiedSlots ?? slots.filter(s=>s.occupied).length; for (const id of ["dashboardSlots","liveSlots"]) { const box = $(id); if (!box) continue; box.replaceChildren(...slots.map(makeSlot)); }
 const reserveSelect = $("reservationSlot"); if (reserveSelect) { const old = reserveSelect.value; reserveSelect.replaceChildren(new Option("Choose an available slot", "")); slots.filter(s=>!s.occupied).forEach(s=>reserveSelect.add(new Option(`Slot ${s.slotId}`, String(s.slotId)))); if ([...reserveSelect.options].some(o=>o.value===old)) reserveSelect.value=old; }
 const releaseSelect = $("slotNumber"); if (releaseSelect) { const old = releaseSelect.value; releaseSelect.replaceChildren(new Option("Choose an occupied slot", "")); slots.filter(s=>s.occupied).forEach(s=>releaseSelect.add(new Option(`Slot ${s.slotId}${s.vehicleNumber ? ` — ${s.vehicleNumber}` : ""}`, String(s.slotId)))); if ([...releaseSelect.options].some(o=>o.value===old)) releaseSelect.value=old; }
}
async function safeLoadSlots() { try { await loadSlots(); } catch(e) { message(`Could not load parking slots: ${e.message}. Is the backend running?`); } }
async function loadReservations() { const body = $("reservationList"); if (!body) return; body.replaceChildren(); try { const rows = await request(`${API}/reservations`); if (!rows.length) { const tr=document.createElement("tr"), td=document.createElement("td"); td.colSpan=7; td.className="empty"; td.textContent="No reservation records yet."; tr.append(td); body.append(tr); return; } rows.forEach(r=>{ const tr=document.createElement("tr"); const values=[r.customerName || r.userName || "—",r.vehicleNumber || "—",r.slotId ?? "Unassigned",date(r.reservedFrom),date(r.reservedUntil),r.status || "—"]; values.forEach((v,i)=>{const td=document.createElement("td"); if(i===5){const span=document.createElement("span"); span.className=`status-pill ${(String(v).toLowerCase())}`; span.textContent=v; td.append(span);}else td.textContent=String(v); tr.append(td);}); const action=document.createElement("td"); if(r.status==="BOOKED"){const btn=document.createElement("button");btn.textContent="Cancel";btn.addEventListener("click",()=>cancelReservation(r._id));action.append(btn);}else action.textContent="—";tr.append(action);body.append(tr);}); } catch(e) { const tr=document.createElement("tr"),td=document.createElement("td");td.colSpan=7;td.textContent=e.message;tr.append(td);body.append(tr); } }
async function cancelReservation(id) { if(!confirm("Cancel this reservation?"))return; try{const d=await request(`${API}/reservations/${encodeURIComponent(id)}/cancel`,{method:"PATCH"});message(d.message||"Reservation cancelled.");await loadReservations();}catch(e){message(e.message);} }
async function submitReservation(event) { event.preventDefault(); const name=$("customerName").value.trim(), email=$("customerEmail").value.trim(), phone=$("customerPhone").value.trim(), vehicleNumber=$("reservationVehicle").value.trim().toUpperCase(), slotId=Number($("reservationSlot").value), start=new Date($("reservedFrom").value), end=new Date($("reservedUntil").value); if(!name||!email||!phone||!vehicleNumber||!$("reservedFrom").value||!$("reservedUntil").value){message("Please complete all fields.");return;} if(start<=new Date()||end<=start){message("Choose a future start time and an end time after the start.");return;} try{const d=await request(`${API}/reservations`,{method:"POST",headers:{"Content-Type":"application/json"},body:JSON.stringify({customerName:name,customerEmail:email,customerPhone:phone,vehicleNumber,slotId,reservedFrom:start.toISOString(),reservedUntil:end.toISOString()})});$("reservationForm").reset();message(d.message||"Reservation created successfully.");}catch(e){message(e.message);} }
async function parkVehicle(event){event.preventDefault();const vehicleNumber=$("vehicleNumber").value.trim().toUpperCase();try{const d=await request(`${PARKING}/park`,{method:"POST",headers:{"Content-Type":"application/json"},body:JSON.stringify({vehicleNumber})});$("parkForm").reset();message(`${d.vehicleNumber} parked in Slot ${d.slotId}. Ticket: ${d.ticketId}`);await safeLoadSlots();}catch(e){message(e.message);} }
async function releaseVehicle(event){event.preventDefault();const slotId=Number($("slotNumber").value);if(!slotId){message("Choose an occupied slot first.");return;}try{const d=await request(`${PARKING}/release`,{method:"POST",headers:{"Content-Type":"application/json"},body:JSON.stringify({slotId})});message(`Slot ${slotId} released. Opening final ticket…`);window.location.href=`ticket.html?id=${encodeURIComponent(d.ticketId)}`;}catch(e){message(e.message);} }

async function recognizePlate(event) {
 event.preventDefault();
 const file = $("plateImage")?.files?.[0];
 if (!file) { message("Choose a plate image first."); return; }
 if (file.size > 5 * 1024 * 1024) { message("Choose an image smaller than 5 MB."); return; }
 const button = document.querySelector('#ocrForm button[type="submit"]');
 if (button) { button.disabled = true; button.textContent = "Reading plate…"; }
 try {
  const dataUrl = await new Promise((resolve, reject) => { const reader = new FileReader(); reader.onload = () => resolve(reader.result); reader.onerror = reject; reader.readAsDataURL(file); });
  const imageBase64 = String(dataUrl).split(",")[1];
  const result = await request(`${API}/ocr/recognize`, { method: "POST", headers: { "Content-Type": "application/json" }, body: JSON.stringify({ imageBase64 }) });
  const vehicleNumber = String(result.plateNumber || "").replace(/[^A-Z0-9]/gi, "").toUpperCase();
  if (!vehicleNumber) throw new Error("No readable vehicle number found. Use a clear image of the full number plate.");
  // OCR output goes directly into automatic allocation. No manual vehicle-number entry is needed.
  message(`Plate detected: ${vehicleNumber}. Finding the nearest available slot…`);
  const parked = await request(`${PARKING}/park`, { method: "POST", headers: { "Content-Type": "application/json" }, body: JSON.stringify({ vehicleNumber, source: "OCR" }) });
  if ($("vehicleNumber")) $("vehicleNumber").value = vehicleNumber;
  message(`Automatic parking complete: ${vehicleNumber} → Slot ${parked.slotId}. Opening ticket…`);
  window.location.href = `ticket.html?id=${encodeURIComponent(parked.ticketId)}`;
 } catch (e) {
  message(e.message || "OCR parking failed. Check the backend terminal.");
 } finally {
  if (button) { button.disabled = false; button.textContent = "Scan Plate & Park Automatically"; }
 }
}
async function searchActiveVehicle(event) {
 event.preventDefault(); const number = $("activeSearchNumber").value.trim().toUpperCase();
 const output = $("activeSearchResult"); if (!output) return;
 try { const result = await request(`${PARKING}/dsa/active-search?vehicleNumber=${encodeURIComponent(number)}`); output.textContent = result.found ? `${result.vehicleNumber} is currently in Slot ${result.slotId}.` : "This vehicle is not currently parked."; }
 catch (e) { output.textContent = e.message; }
}

document.addEventListener("DOMContentLoaded",()=>{setActiveNav();safeLoadSlots();loadReservations();if($("reservationForm"))$("reservationForm").addEventListener("submit",submitReservation);if($("parkForm"))$("parkForm").addEventListener("submit",parkVehicle);if($("ocrForm"))$("ocrForm").addEventListener("submit",recognizePlate);if($("activeSearchForm"))$("activeSearchForm").addEventListener("submit",searchActiveVehicle);if($("releaseForm"))$("releaseForm").addEventListener("submit",releaseVehicle);if($("refreshSlots"))$("refreshSlots").addEventListener("click",safeLoadSlots);if($("refreshReservations"))$("refreshReservations").addEventListener("click",loadReservations);const now=new Date(Date.now()+3600000);now.setMinutes(now.getMinutes()-now.getTimezoneOffset());const from=$("reservedFrom"),until=$("reservedUntil");if(from){from.min=now.toISOString().slice(0,16);from.addEventListener("change",()=>{if(until)until.min=from.value;});}if(until)until.min=from?from.value:now.toISOString().slice(0,16);});
