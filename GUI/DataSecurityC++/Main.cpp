#include "Main.h"   // Form dosyanýn adý neyse onu include et

using namespace System;
using namespace System::Windows::Forms;

// Main.h dosyasýnýn baþýndaki namespace'i buraya yazacaksýn
// mesela: namespace MyProject { public ref class Main : Form { ... }; }
// o zaman aþaðýda MyProject kullanacaksýn.

[STAThreadAttribute]
int main(array<String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    DataSecurityC::Main form;
    Application::Run(% form);

    return 0;
}
