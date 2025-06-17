#pragma once
#include "KeyLogger.h"     // Include the keylogger logic (logKeys, encryptLog, etc.)
#include <thread>          // For std::thread

using namespace System;
using namespace System::Windows::Forms;

namespace KeyLoggerGUI {

    // Entry point for launching the logging function in a native thread
    void RunLoggerThread() {
        ::logKeys();
    }

    // Main GUI class for the Keylogger application
    public ref class Myform : public Form {
    public:
        Myform() {
            InitializeComponent();  // Constructor initializes all controls
        }

    private:
        // GUI Controls
        Panel^ headerPanel;         // Top header panel
        Label^ titleLabel;          // Header label
        Button^ btnLog;             // Start Logging button
        Button^ btnEncrypt;         // Encrypt Log button
        Button^ btnView;            // View Encrypted Log button
        Button^ btnCombine;         // Combine Logs button
        TextBox^ txtOutput;         // Output display box

        // Helper method to create a consistent styled button
        Button^ CreateButton(String^ text, System::Drawing::Color backColor, EventHandler^ clickHandler) {
            Button^ btn = gcnew Button();
            btn->Text = text;
            btn->Size = System::Drawing::Size(130, 35);
            btn->BackColor = backColor;
            btn->ForeColor = System::Drawing::Color::White;
            btn->FlatStyle = FlatStyle::Flat;
            btn->FlatAppearance->BorderSize = 0;
            btn->Font = gcnew System::Drawing::Font("Segoe UI", 10, System::Drawing::FontStyle::Regular);
            btn->Click += clickHandler;
            return btn;
        }

        // Sets up all GUI components and layout
        void InitializeComponent() {
            // Form settings
            this->Text = "Keylogger GUI";
            this->StartPosition = FormStartPosition::CenterScreen;
            this->ClientSize = System::Drawing::Size(700, 500);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->BackColor = System::Drawing::Color::FromArgb(30, 30, 30);

            // Header panel
            const int headerHeight = 70;
            headerPanel = gcnew Panel();
            headerPanel->Location = System::Drawing::Point(0, 0);
            headerPanel->Size = System::Drawing::Size(this->ClientSize.Width, headerHeight);
            headerPanel->BackColor = System::Drawing::Color::FromArgb(0, 122, 204);
            this->Controls->Add(headerPanel);

            // Title label
            titleLabel = gcnew Label();
            titleLabel->Text = "Keylogger Control Panel";
            titleLabel->ForeColor = System::Drawing::Color::White;
            titleLabel->Font = gcnew System::Drawing::Font("Segoe UI Semibold", 18, System::Drawing::FontStyle::Bold);
            titleLabel->AutoSize = true;
            titleLabel->Location = System::Drawing::Point(20, 20);
            headerPanel->Controls->Add(titleLabel);

            // Button row setup
            int topY = headerHeight + 20;
            int leftX = 20;
            int spacing = 15;

            // Start Logging button
            btnLog = CreateButton("Start Logging", System::Drawing::Color::FromArgb(37, 150, 190),
                gcnew EventHandler(this, &Myform::btnLog_Click));
            btnLog->Location = System::Drawing::Point(leftX, topY);
            this->Controls->Add(btnLog);

            // Encrypt Log button
            leftX += btnLog->Width + spacing;
            btnEncrypt = CreateButton("Encrypt Log", System::Drawing::Color::FromArgb(0, 153, 102),
                gcnew EventHandler(this, &Myform::btnEncrypt_Click));
            btnEncrypt->Location = System::Drawing::Point(leftX, topY);
            this->Controls->Add(btnEncrypt);

            // View Encrypted Log button
            leftX += btnEncrypt->Width + spacing;
            btnView = CreateButton("View Encrypted", System::Drawing::Color::FromArgb(204, 102, 0),
                gcnew EventHandler(this, &Myform::btnView_Click));
            btnView->Location = System::Drawing::Point(leftX, topY);
            this->Controls->Add(btnView);

            // Combine Logs button
            leftX += btnView->Width + spacing;
            btnCombine = CreateButton("Combine Logs", System::Drawing::Color::FromArgb(153, 51, 255),
                gcnew EventHandler(this, &Myform::btnCombine_Click));
            btnCombine->Location = System::Drawing::Point(leftX, topY);
            this->Controls->Add(btnCombine);

            // Output TextBox for displaying log content
            int margin = 20;
            txtOutput = gcnew TextBox();
            txtOutput->Multiline = true;
            txtOutput->ReadOnly = true;
            txtOutput->ScrollBars = ScrollBars::Vertical;
            txtOutput->Font = gcnew System::Drawing::Font("Consolas", 10, System::Drawing::FontStyle::Regular);
            txtOutput->BackColor = System::Drawing::Color::FromArgb(30, 30, 30);
            txtOutput->ForeColor = System::Drawing::Color::WhiteSmoke;
            txtOutput->Location = System::Drawing::Point(
                margin,
                topY + btnLog->Height + margin
            );
            txtOutput->Size = System::Drawing::Size(
                this->ClientSize.Width - 2 * margin,
                this->ClientSize.Height - txtOutput->Location.Y - margin
            );
            this->Controls->Add(txtOutput);
        }

        // Button click handlers

        // Starts the keylogging thread
        System::Void btnLog_Click(Object^, EventArgs^) {
            MessageBox::Show("Logging started. Press ESC anywhere to stop.", "Keylogger");
            std::thread t(RunLoggerThread);
            t.detach();
        }

        // Encrypts the log file
        System::Void btnEncrypt_Click(Object^, EventArgs^) {
            ::encryptLog();
            MessageBox::Show("Log encrypted to keylog_enc.txt", "Keylogger");
        }

        // Displays the encrypted log contents in the textbox
        System::Void btnView_Click(Object^, EventArgs^) {
            txtOutput->Text = ::viewEncrypted();
        }

        // Merges both logs into combined.txt
        System::Void btnCombine_Click(Object^, EventArgs^) {
            ::combineLogs();
            MessageBox::Show("Logs combined into combined.txt", "Keylogger");
        }
    };

} // namespace KeyLoggerGUI