#include "Myform.h"    // Include the GUI form definition

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]  // Marks the application entry point as using a single-threaded apartment
void main() {
    // Enable visual styles for controls (gives buttons, textboxes, etc. a modern look)
    Application::EnableVisualStyles();

    // Use default text rendering to ensure consistent font rendering
    Application::SetCompatibleTextRenderingDefault(false);

    // Instantiate the main form of the application
    KeyLoggerGUI::Myform form;

    // Run the application with 'form' as the startup window
    Application::Run(% form);
}