#pragma once
#include "SendReceivePacket.h"
#include "StegLSB.h"
#include <msclr/marshal_cppstd.h>
#include <string>
#include <sstream>

namespace DataSecurityC {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Main
	/// </summary>
	public ref class Main : public System::Windows::Forms::Form
	{
	public:
		Main(void)
		{		
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
	private:
		//USER DEFINED VARS


	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Main()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ register_button;
	protected:

	private: System::Windows::Forms::TextBox^ tUsername;
	private: System::Windows::Forms::TextBox^ tPassword;
	private: System::Windows::Forms::Panel^ panel1;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Label^ label12;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::TextBox^ picturePath;
	private: System::Windows::Forms::Button^ SelectPicture;

	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TextBox^ lPassword;

	private: System::Windows::Forms::TextBox^ lUsername;

	private: System::Windows::Forms::Button^ login_button;
	private: System::Windows::Forms::Label^ label4;

	private: System::Windows::Forms::RichTextBox^ richTextBox1;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Button^ send_msg_button;
	private: System::Windows::Forms::Label^ userpwd;
	private: System::Windows::Forms::Label^ usrname;
	private: System::Windows::Forms::ComboBox^ comboBoxUsers;
	private: System::Windows::Forms::Button^ refresh_users_button;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::RichTextBox^ richTextBox2;
	private: System::Windows::Forms::Button^ refresh_mailbox_button;


	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->register_button = (gcnew System::Windows::Forms::Button());
			this->tUsername = (gcnew System::Windows::Forms::TextBox());
			this->tPassword = (gcnew System::Windows::Forms::TextBox());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->picturePath = (gcnew System::Windows::Forms::TextBox());
			this->SelectPicture = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->lPassword = (gcnew System::Windows::Forms::TextBox());
			this->lUsername = (gcnew System::Windows::Forms::TextBox());
			this->login_button = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->send_msg_button = (gcnew System::Windows::Forms::Button());
			this->userpwd = (gcnew System::Windows::Forms::Label());
			this->usrname = (gcnew System::Windows::Forms::Label());
			this->comboBoxUsers = (gcnew System::Windows::Forms::ComboBox());
			this->refresh_users_button = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->richTextBox2 = (gcnew System::Windows::Forms::RichTextBox());
			this->refresh_mailbox_button = (gcnew System::Windows::Forms::Button());
			this->panel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// register_button
			// 
			this->register_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(58)), static_cast<System::Int32>(static_cast<System::Byte>(58)),
				static_cast<System::Int32>(static_cast<System::Byte>(58)));
			this->register_button->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->register_button->Font = (gcnew System::Drawing::Font(L"Century Gothic", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->register_button->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->register_button->Location = System::Drawing::Point(326, 252);
			this->register_button->Name = L"register_button";
			this->register_button->Size = System::Drawing::Size(97, 42);
			this->register_button->TabIndex = 0;
			this->register_button->Text = L"Register";
			this->register_button->UseVisualStyleBackColor = false;
			this->register_button->Click += gcnew System::EventHandler(this, &Main::register_button_Click);
			// 
			// tUsername
			// 
			this->tUsername->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->tUsername->Location = System::Drawing::Point(326, 60);
			this->tUsername->Name = L"tUsername";
			this->tUsername->Size = System::Drawing::Size(226, 24);
			this->tUsername->TabIndex = 1;
			// 
			// tPassword
			// 
			this->tPassword->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->tPassword->Location = System::Drawing::Point(326, 130);
			this->tPassword->Name = L"tPassword";
			this->tPassword->Size = System::Drawing::Size(226, 24);
			this->tPassword->TabIndex = 2;
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(40)), static_cast<System::Int32>(static_cast<System::Byte>(40)),
				static_cast<System::Int32>(static_cast<System::Byte>(40)));
			this->panel1->Controls->Add(this->pictureBox1);
			this->panel1->Location = System::Drawing::Point(12, 12);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(284, 284);
			this->panel1->TabIndex = 33;
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::Black;
			this->pictureBox1->Location = System::Drawing::Point(2, 2);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(280, 280);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 27;
			this->pictureBox1->TabStop = false;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"Century Gothic", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->label12->ForeColor = System::Drawing::Color::White;
			this->label12->Location = System::Drawing::Point(322, 37);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(87, 20);
			this->label12->TabIndex = 34;
			this->label12->Text = L"Username:";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Century Gothic", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(322, 107);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(83, 20);
			this->label1->TabIndex = 35;
			this->label1->Text = L"Password:";
			// 
			// picturePath
			// 
			this->picturePath->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->picturePath->Location = System::Drawing::Point(326, 185);
			this->picturePath->Name = L"picturePath";
			this->picturePath->Size = System::Drawing::Size(119, 24);
			this->picturePath->TabIndex = 36;
			// 
			// SelectPicture
			// 
			this->SelectPicture->Location = System::Drawing::Point(451, 182);
			this->SelectPicture->Name = L"SelectPicture";
			this->SelectPicture->Size = System::Drawing::Size(101, 27);
			this->SelectPicture->TabIndex = 37;
			this->SelectPicture->Text = L"Select Picture";
			this->SelectPicture->UseVisualStyleBackColor = true;
			this->SelectPicture->Click += gcnew System::EventHandler(this, &Main::SelectPicture_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Century Gothic", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->label2->ForeColor = System::Drawing::Color::White;
			this->label2->Location = System::Drawing::Point(616, 107);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(83, 20);
			this->label2->TabIndex = 42;
			this->label2->Text = L"Password:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Century Gothic", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->label3->ForeColor = System::Drawing::Color::White;
			this->label3->Location = System::Drawing::Point(616, 37);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(87, 20);
			this->label3->TabIndex = 41;
			this->label3->Text = L"Username:";
			// 
			// lPassword
			// 
			this->lPassword->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->lPassword->Location = System::Drawing::Point(620, 130);
			this->lPassword->Name = L"lPassword";
			this->lPassword->Size = System::Drawing::Size(226, 24);
			this->lPassword->TabIndex = 40;
			// 
			// lUsername
			// 
			this->lUsername->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->lUsername->Location = System::Drawing::Point(620, 60);
			this->lUsername->Name = L"lUsername";
			this->lUsername->Size = System::Drawing::Size(226, 24);
			this->lUsername->TabIndex = 39;
			// 
			// login_button
			// 
			this->login_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(58)), static_cast<System::Int32>(static_cast<System::Byte>(58)),
				static_cast<System::Int32>(static_cast<System::Byte>(58)));
			this->login_button->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->login_button->Font = (gcnew System::Drawing::Font(L"Century Gothic", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->login_button->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->login_button->Location = System::Drawing::Point(620, 252);
			this->login_button->Name = L"login_button";
			this->login_button->Size = System::Drawing::Size(97, 42);
			this->login_button->TabIndex = 43;
			this->login_button->Text = L"Login";
			this->login_button->UseVisualStyleBackColor = false;
			this->login_button->Click += gcnew System::EventHandler(this, &Main::login_button_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Century Gothic", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->label4->ForeColor = System::Drawing::Color::White;
			this->label4->Location = System::Drawing::Point(911, 37);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(68, 20);
			this->label4->TabIndex = 45;
			this->label4->Text = L"Send To:";
			// 
			// richTextBox1
			// 
			this->richTextBox1->Location = System::Drawing::Point(915, 154);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(226, 128);
			this->richTextBox1->TabIndex = 46;
			this->richTextBox1->Text = L"";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Century Gothic", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->label5->ForeColor = System::Drawing::Color::White;
			this->label5->Location = System::Drawing::Point(911, 130);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(73, 20);
			this->label5->TabIndex = 47;
			this->label5->Text = L"Content:";
			// 
			// send_msg_button
			// 
			this->send_msg_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(58)), static_cast<System::Int32>(static_cast<System::Byte>(58)),
				static_cast<System::Int32>(static_cast<System::Byte>(58)));
			this->send_msg_button->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->send_msg_button->Font = (gcnew System::Drawing::Font(L"Century Gothic", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->send_msg_button->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->send_msg_button->Location = System::Drawing::Point(915, 298);
			this->send_msg_button->Name = L"send_msg_button";
			this->send_msg_button->Size = System::Drawing::Size(97, 42);
			this->send_msg_button->TabIndex = 48;
			this->send_msg_button->Text = L"send message";
			this->send_msg_button->UseVisualStyleBackColor = false;
			this->send_msg_button->Click += gcnew System::EventHandler(this, &Main::send_msg_button_Click);
			// 
			// userpwd
			// 
			this->userpwd->AutoSize = true;
			this->userpwd->Font = (gcnew System::Drawing::Font(L"Century Gothic", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->userpwd->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->userpwd->Location = System::Drawing::Point(7, 319);
			this->userpwd->Name = L"userpwd";
			this->userpwd->Size = System::Drawing::Size(175, 30);
			this->userpwd->TabIndex = 49;
			this->userpwd->Text = L"please sign in";
			// 
			// usrname
			// 
			this->usrname->AutoSize = true;
			this->usrname->Font = (gcnew System::Drawing::Font(L"Century Gothic", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->usrname->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->usrname->Location = System::Drawing::Point(9, 356);
			this->usrname->Name = L"usrname";
			this->usrname->Size = System::Drawing::Size(175, 30);
			this->usrname->TabIndex = 50;
			this->usrname->Text = L"please sign in";
			// 
			// comboBoxUsers
			// 
			this->comboBoxUsers->FormattingEnabled = true;
			this->comboBoxUsers->Location = System::Drawing::Point(915, 65);
			this->comboBoxUsers->Name = L"comboBoxUsers";
			this->comboBoxUsers->Size = System::Drawing::Size(151, 24);
			this->comboBoxUsers->TabIndex = 51;
			// 
			// refresh_users_button
			// 
			this->refresh_users_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(58)),
				static_cast<System::Int32>(static_cast<System::Byte>(58)), static_cast<System::Int32>(static_cast<System::Byte>(58)));
			this->refresh_users_button->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->refresh_users_button->Font = (gcnew System::Drawing::Font(L"Century Gothic", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->refresh_users_button->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->refresh_users_button->Location = System::Drawing::Point(1072, 60);
			this->refresh_users_button->Name = L"refresh_users_button";
			this->refresh_users_button->Size = System::Drawing::Size(70, 32);
			this->refresh_users_button->TabIndex = 52;
			this->refresh_users_button->Text = L"refresh";
			this->refresh_users_button->UseVisualStyleBackColor = false;
			this->refresh_users_button->Click += gcnew System::EventHandler(this, &Main::refresh_users_button_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Century Gothic", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->label6->ForeColor = System::Drawing::Color::White;
			this->label6->Location = System::Drawing::Point(1192, 37);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(70, 20);
			this->label6->TabIndex = 53;
			this->label6->Text = L"MailBox:";
			// 
			// richTextBox2
			// 
			this->richTextBox2->Font = (gcnew System::Drawing::Font(L"Century Gothic", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->richTextBox2->Location = System::Drawing::Point(1196, 65);
			this->richTextBox2->Name = L"richTextBox2";
			this->richTextBox2->ReadOnly = true;
			this->richTextBox2->Size = System::Drawing::Size(292, 217);
			this->richTextBox2->TabIndex = 54;
			this->richTextBox2->Text = L"";
			// 
			// refresh_mailbox_button
			// 
			this->refresh_mailbox_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(58)),
				static_cast<System::Int32>(static_cast<System::Byte>(58)), static_cast<System::Int32>(static_cast<System::Byte>(58)));
			this->refresh_mailbox_button->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->refresh_mailbox_button->Font = (gcnew System::Drawing::Font(L"Century Gothic", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(162)));
			this->refresh_mailbox_button->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->refresh_mailbox_button->Location = System::Drawing::Point(1418, 288);
			this->refresh_mailbox_button->Name = L"refresh_mailbox_button";
			this->refresh_mailbox_button->Size = System::Drawing::Size(70, 32);
			this->refresh_mailbox_button->TabIndex = 55;
			this->refresh_mailbox_button->Text = L"refresh";
			this->refresh_mailbox_button->UseVisualStyleBackColor = false;
			this->refresh_mailbox_button->Click += gcnew System::EventHandler(this, &Main::refresh_mailbox_button_Click);
			// 
			// Main
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(30)), static_cast<System::Int32>(static_cast<System::Byte>(30)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			this->ClientSize = System::Drawing::Size(1500, 395);
			this->Controls->Add(this->refresh_mailbox_button);
			this->Controls->Add(this->richTextBox2);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->refresh_users_button);
			this->Controls->Add(this->comboBoxUsers);
			this->Controls->Add(this->usrname);
			this->Controls->Add(this->userpwd);
			this->Controls->Add(this->send_msg_button);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->richTextBox1);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->login_button);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->lPassword);
			this->Controls->Add(this->lUsername);
			this->Controls->Add(this->SelectPicture);
			this->Controls->Add(this->picturePath);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->tPassword);
			this->Controls->Add(this->tUsername);
			this->Controls->Add(this->register_button);
			this->ForeColor = System::Drawing::SystemColors::ControlText;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Name = L"Main";
			this->ShowIcon = false;
			this->Text = L"Main";
			this->Load += gcnew System::EventHandler(this, &Main::Main_Load);
			this->panel1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void SelectPicture_Click(System::Object^ sender, System::EventArgs^ e) {
		OpenFileDialog^ ofd = gcnew OpenFileDialog();
		ofd->Filter = "Image Files|*.jpg;*.jpeg;*.png;*.bmp;*.gif|All files (*.*)|*.*";

		if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			picturePath->Text = ofd->FileName;

			// Önce varsa eski resmi temizle
			if (pictureBox1->Image != nullptr)
			{
				delete pictureBox1->Image;
				pictureBox1->Image = nullptr;
			}

			// Resmi PictureBox'a yükle
			pictureBox1->Image = System::Drawing::Image::FromFile(ofd->FileName);
		}
	}
private: System::Void register_button_Click(System::Object^ sender, System::EventArgs^ e) {
	fotografaSifreyiGom(msclr::interop::marshal_as<std::string>(picturePath->Text), msclr::interop::marshal_as<std::string>(tPassword->Text));
	signup(msclr::interop::marshal_as<std::string>(tUsername->Text), msclr::interop::marshal_as<std::string>(tPassword->Text), "encrypted.png");
}

private: System::Void login_button_Click(System::Object^ sender, System::EventArgs^ e) {
	login(msclr::interop::marshal_as<std::string>(lUsername->Text), msclr::interop::marshal_as<std::string>(lPassword->Text));
}
private: System::Void Main_Load(System::Object^ sender, System::EventArgs^ e) {
	if (!System::IO::File::Exists("savedat.txt"))
	{
		System::Windows::Forms::Application::Exit();
		return;
	}

	System::IO::StreamReader^ sr =
		gcnew System::IO::StreamReader("savedat.txt");

	System::String^ line = sr->ReadLine();
	sr->Close();

	array<System::String^>^ parts = line->Split(' ');

	if (parts->Length >= 2)
	{
		usrname->Text = parts[0];
		userpwd->Text = parts[1];
	}
}
private: System::Void send_msg_button_Click(System::Object^ sender, System::EventArgs^ e) {
	sendMessage(msclr::interop::marshal_as<std::string>(usrname->Text), msclr::interop::marshal_as<std::string>(comboBoxUsers->Text),
		msclr::interop::marshal_as<std::string>(richTextBox1->Text), msclr::interop::marshal_as<std::string>(userpwd->Text));
}
private: System::Void refresh_users_button_Click(System::Object^ sender, System::EventArgs^ e) {
	std::string alluser = listUsers();

	comboBoxUsers->Items->Clear();

	std::stringstream ss(alluser);
	std::string item;

	while (std::getline(ss, item, ','))  // virgüle göre böl
	{
		// boþ gelenleri atla (ör: "baris,,efe" gibi)
		if (item.empty()) continue;

		// System::String^'e çevir
		System::String^ managedItem = msclr::interop::marshal_as<System::String^>(item);

		if (managedItem == usrname->Text)
			continue;

		// Ýstersen baþ/son boþluklarý temizle
		managedItem = managedItem->Trim();

		if (managedItem->Length > 0) 
			comboBoxUsers->Items->Add(managedItem);
	}
}
private: System::Void refresh_mailbox_button_Click(System::Object^ sender, System::EventArgs^ e)
{
	std::string messages = refreshMailbox(
		msclr::interop::marshal_as<std::string>(usrname->Text),
		msclr::interop::marshal_as<std::string>(userpwd->Text)
	);

	richTextBox1->Clear();

	std::stringstream ss(messages);
	std::string token;

	while (std::getline(ss, token, ',')) // virgülle ayýr
	{
		if (token.empty()) continue;

		std::size_t pos = token.find(':');
		if (pos == std::string::npos) continue;

		std::string senderName = token.substr(0, pos);
		std::string msgText = token.substr(pos + 1);

		//FILTRELEME KISMI (istege bagli sonraki versiyonlrda eklenebilir).
		if (senderName == "eeee")
			continue;

		System::String^ managedMsg =
			msclr::interop::marshal_as<System::String^>(senderName + ": " + msgText);

		richTextBox2->AppendText(managedMsg + "\r\n");
	}
}
};
}
