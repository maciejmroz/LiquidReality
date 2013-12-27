#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace LRStudio {

	/// <summary>
	/// Summary for NewObjectForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class NewObjectForm : public System::Windows::Forms::Form
	{
		array<String^>^ _object_name_list;
	public:
		NewObjectForm(void) :
		_object_name_list(nullptr)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		property array<String^>^ ClassNameList
		{
			void set(array<String^>^ class_name_list)
			{
				pickObjectListBox->Items->Clear();
				pickObjectListBox->Items->AddRange(class_name_list);
				objectNameTextBox->Enabled=false;
			}
		}

		property array<String^>^ ExistingObjectNameList
		{
			void set(array<String^>^ object_name_list)
			{
				_object_name_list=object_name_list;
			}
		}

		property String^ SelectedClass
		{
			String^ get()
			{
				if(pickObjectListBox->SelectedIndex==-1)
				{
					return nullptr;
				}
				return pickObjectListBox->SelectedItem->ToString();
			}
		}

		property String ^SelectedObjectName
		{
			String^ get()
			{
				return objectNameTextBox->Text;
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~NewObjectForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  OKButton;
	private: System::Windows::Forms::Button^  CancelButton;
	private: System::Windows::Forms::ListBox^  pickObjectListBox;
	private: System::Windows::Forms::TextBox^  objectNameTextBox;

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
			this->OKButton = (gcnew System::Windows::Forms::Button());
			this->CancelButton = (gcnew System::Windows::Forms::Button());
			this->pickObjectListBox = (gcnew System::Windows::Forms::ListBox());
			this->objectNameTextBox = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// OKButton
			// 
			this->OKButton->Location = System::Drawing::Point(235, 308);
			this->OKButton->Name = L"OKButton";
			this->OKButton->Size = System::Drawing::Size(75, 23);
			this->OKButton->TabIndex = 0;
			this->OKButton->Text = L"Ok";
			this->OKButton->UseVisualStyleBackColor = true;
			this->OKButton->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &NewObjectForm::OKButton_MouseClick);
			// 
			// CancelButton
			// 
			this->CancelButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->CancelButton->Location = System::Drawing::Point(316, 308);
			this->CancelButton->Name = L"CancelButton";
			this->CancelButton->Size = System::Drawing::Size(75, 23);
			this->CancelButton->TabIndex = 1;
			this->CancelButton->Text = L"Cancel";
			this->CancelButton->UseVisualStyleBackColor = true;
			// 
			// pickObjectListBox
			// 
			this->pickObjectListBox->FormattingEnabled = true;
			this->pickObjectListBox->Location = System::Drawing::Point(12, 12);
			this->pickObjectListBox->Name = L"pickObjectListBox";
			this->pickObjectListBox->Size = System::Drawing::Size(379, 264);
			this->pickObjectListBox->TabIndex = 2;
			this->pickObjectListBox->SelectedIndexChanged += gcnew System::EventHandler(this, &NewObjectForm::pickObjectListBox_SelectedIndexChanged);
			// 
			// objectNameTextBox
			// 
			this->objectNameTextBox->Location = System::Drawing::Point(12, 282);
			this->objectNameTextBox->Name = L"objectNameTextBox";
			this->objectNameTextBox->Size = System::Drawing::Size(379, 20);
			this->objectNameTextBox->TabIndex = 3;
			// 
			// NewObjectForm
			// 
			this->AcceptButton = this->OKButton;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(403, 342);
			this->ControlBox = false;
			this->Controls->Add(this->objectNameTextBox);
			this->Controls->Add(this->pickObjectListBox);
			this->Controls->Add(this->CancelButton);
			this->Controls->Add(this->OKButton);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"NewObjectForm";
			this->ShowInTaskbar = false;
			this->Text = L"New object";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		bool checkObjectNameValid(String ^ref_name)
		{
			if(!ref_name)
			{
				return false;
			}
			for each(String ^obj_name in _object_name_list)
			{
				if(obj_name && obj_name->Equals(ref_name))
				{
					return false;
				}
			}
			return true;
		}

		System::Void pickObjectListBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		{
			if(pickObjectListBox->SelectedIndex==-1)
			{
				objectNameTextBox->Enabled=false;
				objectNameTextBox->Text="";
				return;
			}
			String ^full_class_name=pickObjectListBox->SelectedItem->ToString();
			if(full_class_name->Length==0)
			{
				objectNameTextBox->Enabled=false;
				objectNameTextBox->Text="";
				return;
			}
			int index=full_class_name->LastIndexOf('/');
			String ^short_class_name=((index==-1) || (index==full_class_name->Length-1)) ?
				full_class_name :
				full_class_name->Substring(index+1);
			String ^object_name=nullptr;//short_class_name+"1";
			int id=1;
			while(!checkObjectNameValid(object_name))
			{
				object_name=short_class_name+String::Format("{0:D}",id++);
			}
			objectNameTextBox->Text=object_name;
			objectNameTextBox->Enabled=true;
		}

		System::Void OKButton_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			if(!_object_name_list)
			{
				DialogResult=System::Windows::Forms::DialogResult::OK;
				return;
			}
			if(checkObjectNameValid(objectNameTextBox->Text))
			{
				DialogResult=System::Windows::Forms::DialogResult::OK;
			}
			else
			{
				System::Windows::Forms::MessageBox::Show(
					"Object with this name already exists, plaese choose another",
					"Error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Exclamation);
			}
		}
	};
}
