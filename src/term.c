#include "common.h"

#include <poll.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

bool color_enabled;

static struct termios orig_termios;
static bool raw_active;
static bool alt_active;

static void restore_on_signal(int sig) {
    runner_kill_child();
    term_raw_disable();
    signal(sig, SIG_DFL);
    raise(sig);
}

void term_init(void) {
    const char *no_color = getenv("NO_COLOR");
    color_enabled = isatty(STDOUT_FILENO) && !(no_color && *no_color);
    signal(SIGINT, restore_on_signal);
    signal(SIGTERM, restore_on_signal);
    signal(SIGHUP, restore_on_signal);
    atexit(term_raw_disable);
}

void term_raw_enable(void) {
    if (raw_active || !isatty(STDIN_FILENO)) return;
    if (tcgetattr(STDIN_FILENO, &orig_termios) != 0) return;
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(tcflag_t)(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    raw_active = true;
}

void term_raw_disable(void) {
    if (alt_active) term_alt_screen(false);
    if (!raw_active) return;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    raw_active = false;
    fputs("\x1b[?25h", stdout); /* make sure the cursor is visible */
    fflush(stdout);
}

static int read_byte(int timeout_ms) {
    struct pollfd pfd = {.fd = STDIN_FILENO, .events = POLLIN};
    int r = poll(&pfd, 1, timeout_ms);
    if (r <= 0) return -1;
    unsigned char c;
    if (read(STDIN_FILENO, &c, 1) != 1) return -2;
    return c;
}

int term_read_key(int timeout_ms) {
    int c = read_byte(timeout_ms);
    if (c == -2) return 'q'; /* stdin closed: treat as quit */
    if (c < 0) return KEY_NONE;
    if (c == '\r' || c == '\n') return KEY_ENTER;
    if (c != 27) return c;

    int c2 = read_byte(30);
    if (c2 != '[' && c2 != 'O') return KEY_ESC;
    int c3 = read_byte(30);
    switch (c3) {
    case 'A': return KEY_UP;
    case 'B': return KEY_DOWN;
    case 'H': return KEY_HOME;
    case 'F': return KEY_END;
    default: break;
    }
    if (c3 >= '0' && c3 <= '9') {
        int c4 = read_byte(30);
        if (c4 == '~') {
            switch (c3) {
            case '1': case '7': return KEY_HOME;
            case '4': case '8': return KEY_END;
            case '5': return KEY_PGUP;
            case '6': return KEY_PGDN;
            default: break;
            }
        }
    }
    return KEY_NONE;
}

void term_size(int *rows, int *cols) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0) {
        *rows = ws.ws_row;
        *cols = ws.ws_col;
    } else {
        *rows = 24;
        *cols = 80;
    }
}

void term_clear(void) {
    if (isatty(STDOUT_FILENO)) fputs("\x1b[H\x1b[2J\x1b[3J", stdout);
}

void term_alt_screen(bool on) {
    if (!isatty(STDOUT_FILENO)) return;
    fputs(on ? "\x1b[?1049h\x1b[?25l" : "\x1b[?25h\x1b[?1049l", stdout);
    fflush(stdout);
    alt_active = on;
}
