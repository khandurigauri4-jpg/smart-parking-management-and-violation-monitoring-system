(() => {
    const API = "http://localhost:3000/api/auth";
    const page = location.pathname.split("/").pop();
    const protectedPages = { "user.html": "USER", "operator.html": "ADMIN" };
    const currentRole = protectedPages[page];

    if (currentRole) {
        let session = null;
        try { session = JSON.parse(localStorage.getItem("smartParkingUser") || "null"); } catch (_) {}
        if (!session || session.role !== currentRole) {
            location.replace(currentRole === "USER" ? "user-auth.html" : "operator-auth.html");
            return;
        }
        document.addEventListener("DOMContentLoaded", () => {
            const header = document.querySelector("header");
            if (!header || header.querySelector(".auth-logout")) return;
            const button = document.createElement("button");
            button.type = "button";
            button.className = "auth-logout";
            button.textContent = "Logout";
            button.addEventListener("click", () => {
                localStorage.removeItem("smartParkingUser");
                location.href = "index.html";
            });
            header.append(button);
        });
        return;
    }

    document.addEventListener("DOMContentLoaded", () => {
        const form = document.getElementById("authForm");
        if (!form) return;
        const role = form.dataset.role;
        let mode = "login";
        const nameField = document.getElementById("nameField");
        const password = document.getElementById("authPassword");
        const submit = document.getElementById("authSubmit");
        const message = document.getElementById("authMessage");

        document.querySelectorAll(".auth-tab").forEach(tab => tab.addEventListener("click", () => {
            mode = tab.dataset.mode;
            document.querySelectorAll(".auth-tab").forEach(item => item.classList.toggle("active", item === tab));
            nameField.classList.toggle("auth-hidden", mode !== "signup");
            document.getElementById("authName").required = mode === "signup";
            password.autocomplete = mode === "signup" ? "new-password" : "current-password";
            submit.textContent = mode === "signup" ? "Create Account" : "Login";
            message.textContent = "";
        }));

        form.addEventListener("submit", async event => {
            event.preventDefault();
            message.textContent = "Please wait…";
            submit.disabled = true;
            try {
                const payload = {
                    role,
                    email: document.getElementById("authEmail").value.trim(),
                    password: password.value
                };
                if (mode === "signup") payload.name = document.getElementById("authName").value.trim();
                const response = await fetch(`${API}/${mode}`, {
                    method: "POST",
                    headers: { "Content-Type": "application/json" },
                    body: JSON.stringify(payload)
                });
                const data = await response.json().catch(() => ({}));
                if (!response.ok) throw new Error(data.message || "Authentication failed.");
                if (mode === "signup") {
                    message.textContent = "Account created. Please log in.";
                    document.querySelector('[data-mode="login"]').click();
                    password.value = "";
                } else {
                    localStorage.setItem("smartParkingUser", JSON.stringify(data.user));
                    location.href = role === "USER" ? "user.html" : "operator.html";
                }
            } catch (error) {
                message.textContent = error.message;
            } finally {
                submit.disabled = false;
            }
        });
    });
})();
