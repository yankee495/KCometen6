/* ============================================================================
 * KCOMETEN6 NATIVE WAYLAND INPUT IDLE NOTIFICATION DAEMON
 * Leak-Proof Pure Input Tracking with Integrated Hardware Sleep Detection
 * Engineered by John (Yankee) — 100% Standalone & Portably Unified
 * ============================================================================ */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <string>

extern "C" {
    #include <wayland-client.h>
}

/* Forward declarations of interfaces matching opaque structures */
struct ext_idle_notifier_v1;
struct ext_idle_notification_v1;

extern "C" {
    extern const struct wl_interface wl_seat_interface;
    extern const struct wl_interface ext_idle_notifier_v1_interface;
    extern const struct wl_interface ext_idle_notification_v1_interface;
}

struct ext_idle_notification_v1_listener {
    void (*idled)(void *data, struct ext_idle_notification_v1 *ext_idle_notification_v1);
    void (*resumed)(void *data, struct ext_idle_notification_v1 *ext_idle_notification_v1);
};

/* Runtime Context Elements */
struct wl_display *display = nullptr;
struct wl_registry *registry = nullptr;
struct wl_seat *seat = nullptr;
struct ext_idle_notifier_v1 *idle_notifier = nullptr;
struct ext_idle_notification_v1 *idle_notification = nullptr;

int timeout_seconds = 60;
bool is_idle = false;

// HELPER FUNCTION: Safely checks if the desktop session lock screen is currently active
bool is_screen_locked() {
    bool locked = false;
    FILE* pipe = popen("dbus-send --session --dest=org.freedesktop.ScreenSaver --type=method_call --print-reply /ScreenSaver org.freedesktop.ScreenSaver.GetActive 2>/dev/null", "r");
    if (pipe) {
        char buffer[128];
        std::string result = "";
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
        pclose(pipe);
        if (result.find("boolean true") != std::string::npos) {
            locked = true;
        }
    }
    return locked;
}

// HELPER FUNCTION: Checks systemd-logind natively to see if a hardware sleep operation is actively pending
bool is_system_suspending() {
    bool suspending = false;
    FILE* pipe = popen("dbus-send --system --dest=org.freedesktop.login1 --type=method_call --print-reply /org/freedesktop/login1 org.freedesktop.login1.Manager.PreparingForSleep 2>/dev/null", "r");
    if (pipe) {
        char buffer[128];
        std::string result = "";
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
        pclose(pipe);
        if (result.find("boolean true") != std::string::npos) {
            suspending = true;
        }
    }
    return suspending;
}

// CALLBACK 1: Executed by KWin the exact second the mouse/keyboard sit still on timeout!
static void handle_idled(void *data, struct ext_idle_notification_v1 *notification) {
    if (!is_idle) {
        // Safe Shield: Intercept launch if the screen is locked or system is going to sleep
        if (is_screen_locked() || is_system_suspending()) {
            std::cout << "[IDLE INTERCEPT] Active system constraint detected. Bypassing engine spawn..." << std::endl;
            is_idle = true;
            return;
        }

        std::cout << "[IDLE NOTIFY] Input inactivity threshold breached! Launching comets..." << std::endl;
        int status = std::system("./kcometen6_run &");
        (void)status;
        is_idle = true;
    }
}

// CALLBACK 2: Executed by KWin the exact microsecond you wiggle the mouse or touch a key!
static void handle_resumed(void *data, struct ext_idle_notification_v1 *notification) {
    if (is_idle) {
        std::cout << "[ACTIVITY] User input detected. Squelching active visual nodes..." << std::endl;
        int status = std::system("pkill -x kcometen6_run");
        (void)status;
        is_idle = false;
    }
}

static const struct ext_idle_notification_v1_listener idle_listener = {
    .idled = handle_idled,
    .resumed = handle_resumed
};

// Listen to the global system registry to bind our seat and the idle notifier interfaces
static void global_registry_handler(void *data, struct wl_registry *reg, uint32_t id,
                                    const char *interface, uint32_t version) {
    if (std::strcmp(interface, "wl_seat") == 0) {
        seat = (struct wl_seat*)wl_registry_bind(reg, id, &wl_seat_interface, 1);
    } else if (std::strcmp(interface, "ext_idle_notifier_v1") == 0) {
        idle_notifier = (struct ext_idle_notifier_v1*)wl_registry_bind(reg, id, &ext_idle_notifier_v1_interface, 1);
    }
}

static void global_registry_remover(void *data, struct wl_registry *reg, uint32_t id) {}

static const struct wl_registry_listener registry_listener = {
    .global = global_registry_handler,
    .global_remove = global_registry_remover
};

int main(int argc, char *argv[]) {
    // Parse single parameter arguments cleanly from your PyQt6 slider configurations
    if (argc > 1 && argv != nullptr) {
        timeout_seconds = std::atoi(argv[1]);
        if (timeout_seconds <= 0) timeout_seconds = 60;
    }

    std::cout << "[INIT] Connecting to native Wayland client registry channel..." << std::endl;

    display = wl_display_connect(nullptr);
    if (!display) {
        std::cerr << "[ERROR] Critical: Could not connect to Wayland display socket!" << std::endl;
        return 1;
    }

    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, nullptr);
    wl_display_roundtrip(display);

    if (!idle_notifier || !seat) {
        std::cerr << "[ERROR] Compositor lacks native ext-idle-notify-v1 support or seat is missing!" << std::endl;
        wl_display_disconnect(display);
        return 1;
    }

    // Convert timeout seconds directly to the raw milliseconds requested by the spec!
    uint32_t timeout_ms = (uint32_t)timeout_seconds * 1000;

    // Type-safe argument pack explicitly structured for 64-bit stack alignment
    union wl_argument args[3];
    args[0].o = nullptr;                  // Placeholder allocation slot for the new interface ID
    args[1].u = timeout_ms;               // Input inactivity timeout value in milliseconds
    args[2].o = (struct wl_object*)seat;  // Target input hardware tracking seat reference pointer

    // Binds via Opcode 1 (get_input_idle_notification) to track user hardware input directly
    idle_notification = (struct ext_idle_notification_v1*)wl_proxy_marshal_array_constructor_versioned(
        (struct wl_proxy*)idle_notifier,
        1, // OPCODE 1: get_input_idle_notification
        args,
        &ext_idle_notification_v1_interface,
        1
    );

    if (!idle_notification) {
        std::cerr << "[ERROR] Failed to marshal native input-idle notification proxies!" << std::endl;
        wl_display_disconnect(display);
        return 1;
    }

    // Attach our active idled/resumed listener hooks to catch events natively
    wl_proxy_add_listener((struct wl_proxy*)idle_notification, (void (**)())&idle_listener, nullptr);

    std::cout << "[SUCCESS] Monitoring raw hardware input states cleanly under Wayland." << std::endl;
    std::cout << "[TRACKING] Standalone idle clock armed for: " << timeout_seconds << " seconds." << std::endl;

    // Main polling loop: Sleeps at 0% CPU until KWin flings a hardware input event
    while (wl_display_dispatch(display) != -1) {
        // SAFE HARNESS: If the system drops into preparation for sleep, flush graphics memory cleanly!
        if (is_system_suspending()) {
            std::system("pkill -x kcometen6_run >/dev/null 2>&1");
            is_idle = false;
        }
    }

    wl_proxy_destroy((struct wl_proxy*)idle_notification);
    wl_registry_destroy(registry);
    wl_display_disconnect(display);
    return 0;
}

// ============================================================================
// THE WAYLAND PROTOCOL WIRE ARRAYS (MANUAL SCANNER EMULATION)
// Informing the low-level library of function parameter signatures to prevent data drops
// ============================================================================
extern "C" {
    static const struct wl_message ext_idle_notifier_requests[] = {
        { "destroy", "", nullptr },
        { "get_idle_notification", "nuo", (const struct wl_interface*[]) { &ext_idle_notification_v1_interface, nullptr, &wl_seat_interface } },
        { "get_input_idle_notification", "nuo", (const struct wl_interface*[]) { &ext_idle_notification_v1_interface, nullptr, &wl_seat_interface } }
    };

    static const struct wl_message ext_idle_notification_requests[] = {
        { "destroy", "", nullptr }
    };

    static const struct wl_message ext_idle_notification_events[] = {
        { "idled", "", nullptr },
        { "resumed", "", nullptr }
    };

    const struct wl_interface ext_idle_notifier_v1_interface = {
        "ext_idle_notifier_v1", 2, 3, ext_idle_notifier_requests, 0, nullptr
    };

    const struct wl_interface ext_idle_notification_v1_interface = {
        "ext_idle_notification_v1", 2, 1, ext_idle_notification_requests, 2, ext_idle_notification_events
    };
}
