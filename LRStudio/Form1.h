#pragma once

#include "utils.h"
#include "NewObjectForm.h"

namespace LRStudio
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//workaround for Windows Forms designer bug that was erasing event handlers
			//every time project is loaded
			this->envelopeEditor->EnvelopeChanged += gcnew LRControls::EnvelopeEditor::EnvelopeChangedDelegate(this, &Form1::envelopeEditor_EnvelopeChanged);
			this->envelopeEditor->KeyChanged += gcnew LRControls::EnvelopeEditor::KeyChangedDelegate(this, &Form1::envelopeEditor_KeyChanged);
			this->envelopeEditor->MouseModeChanged += gcnew LRControls::EnvelopeEditor::MouseModeChangedDelegate(this, &Form1::envelopeEditor_MouseModeChanged);
			InternalResize();
			//LiquidReality init
			initLR();
			//rendering related data init
			render_thread=gcnew ST::Thread(gcnew ST::ThreadStart(this,&Form1::idleHandler));
			render_thread->Start();
			is_playing=false;
			is_paused=false;
			start_tick=0;
			pause_tick=0;
			edited_envelope=gcnew LRControls::Envelope();
			envelopeEditor->EnvelopeObj=edited_envelope;
			//init recent projects list
			initRecentProjects();
		}
	protected: 
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">"description of the parameter"</param>
		~Form1()
		{
			//explicitly free unmanaged resources, in theory it is not needed
			//(it's all smart pointers) but this way is more predictable in behavior
			render_thread->Interrupt();
			render_thread->Join();
			globals.clean();
			globals.unloadAll();
			globals.filesystem=NULL;
			globals.renderer=NULL;
			globals.engine=NULL;
			//generated code goes below
		}
#pragma region Control member variables
	private: System::Windows::Forms::ToolStripMenuItem^  recentProjectMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadRecent1MenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadRecent2MenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadRecent3MenuItem;
	private: System::Windows::Forms::SplitContainer^  splitContainerVMain;
	private: System::Windows::Forms::ToolStrip^  toolStrip2;
	private: System::Windows::Forms::ListBox^  objectListBox;
	private: System::Windows::Forms::SplitContainer^  splitContainerHRight;
	private: System::Windows::Forms::ToolStripButton^  editTransformNodesButton;
	private: System::Windows::Forms::ToolStripButton^  editMeshNodesButton;
	private: System::Windows::Forms::ToolStripButton^  editSurfacesButton;
	private: System::Windows::Forms::ToolStripButton^  editCameraFxButton;
	private: System::Windows::Forms::ToolStripButton^  editTexturesButton;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  importLWS2MenuItem;
	private: System::Windows::Forms::SplitContainer^  splitContainerVSecondary;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  envelopeEditorTabPage;

	private: System::Windows::Forms::SplitContainer^  splitContainerProperties;
	private: System::Windows::Forms::ToolStrip^  toolStrip3;
	private: System::Windows::Forms::ToolStripLabel^  propertyListLabel;
	private: System::Windows::Forms::ToolStrip^  toolStrip4;
	private: System::Windows::Forms::ToolStripLabel^  toolStripLabel5;
	private: System::Windows::Forms::ToolStripButton^  listEditPanelAddItemBtn;
	private: System::Windows::Forms::ToolStripButton^  listEditPanelRemoveItemBtn;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog;
	private: System::Windows::Forms::ToolStripLabel^  toolStripFrameNumberLabel;
	private: System::Windows::Forms::ListView^  propertyList;
	private: System::Windows::Forms::ColumnHeader^  NamesCol;
	private: System::Windows::Forms::ColumnHeader^  ValuesCol;
	private: System::Windows::Forms::ToolStripButton^  editScenePropertiesButton;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::NumericUpDown^  propertyEditorNumeric;
	private: System::Windows::Forms::ComboBox^  propertyEditorChoice;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ToolStripButton^  rebuildActiveObjectBtn;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator6;
	private: System::Windows::Forms::ToolStrip^  toolStrip5;
	private: LRControls::EnvelopeEditor^  envelopeEditor;
	private: System::Windows::Forms::ToolStripButton^  envelopeEditModeAddDragButton;
	private: System::Windows::Forms::ToolStripButton^  envelopeEditModeRemoveButton;
	private: System::Windows::Forms::ToolStripButton^  envelopeEditModeZoomButton;
	private: System::Windows::Forms::ToolStripButton^  envelopeEditModePanButton;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator7;
	private: System::Windows::Forms::ToolStripMenuItem^  importImageMenuItem;
	private: System::Windows::Forms::OpenFileDialog^  openImageFileDialog;
	private: System::Windows::Forms::ToolStripButton^  newObjectButton;
	private: System::Windows::Forms::ImageList^  propertyTypesImageList;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator8;
	private: System::Windows::Forms::ToolStripLabel^  toolStripLabel3;
	private: System::Windows::Forms::ToolStripComboBox^  envelopeChannelComboBox;
	private: System::Windows::Forms::ToolStripMenuItem^  loadRecent4MenuItem;
	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: System::Windows::Forms::ToolStripDropDownButton^  projectDropDownButton;
	private: System::Windows::Forms::ToolStripMenuItem^  openProjectMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripLabel^  toolStripLabel1;
	private: System::Windows::Forms::ToolStripComboBox^  renderComboBox;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
	private: System::Windows::Forms::ToolStripLabel^  toolStripLabel2;
	private: System::Windows::Forms::ToolStripComboBox^  cameraComboBox;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator4;
	private: System::Windows::Forms::ToolStripButton^  playButton;
	private: System::Windows::Forms::ToolStripButton^  prevButton;
	private: System::Windows::Forms::ToolStripButton^  pauseButton;
	private: System::Windows::Forms::ToolStripButton^  nextButton;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator5;
	private: System::Windows::Forms::Panel^  renderPanel;
	private: System::Windows::Forms::FolderBrowserDialog^  folderBrowser;
private: System::Windows::Forms::ToolStripButton^  saveActiveObjectBtn;
private: System::Windows::Forms::ToolStripButton^  saveSceneButton;
	private: System::ComponentModel::IContainer^  components;
#pragma endregion
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			LRControls::Envelope^  envelope1 = (gcnew LRControls::Envelope());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->projectDropDownButton = (gcnew System::Windows::Forms::ToolStripDropDownButton());
			this->recentProjectMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadRecent1MenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadRecent2MenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadRecent3MenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadRecent4MenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openProjectMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->importLWS2MenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->importImageMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStripLabel1 = (gcnew System::Windows::Forms::ToolStripLabel());
			this->renderComboBox = (gcnew System::Windows::Forms::ToolStripComboBox());
			this->editScenePropertiesButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->saveSceneButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStripLabel2 = (gcnew System::Windows::Forms::ToolStripLabel());
			this->cameraComboBox = (gcnew System::Windows::Forms::ToolStripComboBox());
			this->toolStripSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->prevButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->playButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->pauseButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->nextButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator5 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStripFrameNumberLabel = (gcnew System::Windows::Forms::ToolStripLabel());
			this->renderPanel = (gcnew System::Windows::Forms::Panel());
			this->folderBrowser = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->splitContainerVMain = (gcnew System::Windows::Forms::SplitContainer());
			this->objectListBox = (gcnew System::Windows::Forms::ListBox());
			this->toolStrip2 = (gcnew System::Windows::Forms::ToolStrip());
			this->editTransformNodesButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->editMeshNodesButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->editSurfacesButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->editTexturesButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->editCameraFxButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator7 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->newObjectButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->splitContainerVSecondary = (gcnew System::Windows::Forms::SplitContainer());
			this->splitContainerProperties = (gcnew System::Windows::Forms::SplitContainer());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->propertyList = (gcnew System::Windows::Forms::ListView());
			this->NamesCol = (gcnew System::Windows::Forms::ColumnHeader());
			this->ValuesCol = (gcnew System::Windows::Forms::ColumnHeader());
			this->propertyTypesImageList = (gcnew System::Windows::Forms::ImageList(this->components));
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->propertyEditorChoice = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->propertyEditorNumeric = (gcnew System::Windows::Forms::NumericUpDown());
			this->toolStrip3 = (gcnew System::Windows::Forms::ToolStrip());
			this->propertyListLabel = (gcnew System::Windows::Forms::ToolStripLabel());
			this->toolStripSeparator6 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->rebuildActiveObjectBtn = (gcnew System::Windows::Forms::ToolStripButton());
			this->saveActiveObjectBtn = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStrip4 = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripLabel5 = (gcnew System::Windows::Forms::ToolStripLabel());
			this->listEditPanelAddItemBtn = (gcnew System::Windows::Forms::ToolStripButton());
			this->listEditPanelRemoveItemBtn = (gcnew System::Windows::Forms::ToolStripButton());
			this->splitContainerHRight = (gcnew System::Windows::Forms::SplitContainer());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->envelopeEditorTabPage = (gcnew System::Windows::Forms::TabPage());
			this->envelopeEditor = (gcnew LRControls::EnvelopeEditor());
			this->toolStrip5 = (gcnew System::Windows::Forms::ToolStrip());
			this->envelopeEditModeAddDragButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->envelopeEditModeRemoveButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->envelopeEditModePanButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->envelopeEditModeZoomButton = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator8 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStripLabel3 = (gcnew System::Windows::Forms::ToolStripLabel());
			this->envelopeChannelComboBox = (gcnew System::Windows::Forms::ToolStripComboBox());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->openImageFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->toolStrip1->SuspendLayout();
			this->splitContainerVMain->Panel1->SuspendLayout();
			this->splitContainerVMain->Panel2->SuspendLayout();
			this->splitContainerVMain->SuspendLayout();
			this->toolStrip2->SuspendLayout();
			this->splitContainerVSecondary->Panel1->SuspendLayout();
			this->splitContainerVSecondary->Panel2->SuspendLayout();
			this->splitContainerVSecondary->SuspendLayout();
			this->splitContainerProperties->Panel1->SuspendLayout();
			this->splitContainerProperties->Panel2->SuspendLayout();
			this->splitContainerProperties->SuspendLayout();
			this->tableLayoutPanel1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->propertyEditorNumeric))->BeginInit();
			this->toolStrip3->SuspendLayout();
			this->toolStrip4->SuspendLayout();
			this->splitContainerHRight->Panel1->SuspendLayout();
			this->splitContainerHRight->Panel2->SuspendLayout();
			this->splitContainerHRight->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->envelopeEditorTabPage->SuspendLayout();
			this->toolStrip5->SuspendLayout();
			this->SuspendLayout();
			// 
			// toolStrip1
			// 
			this->toolStrip1->GripMargin = System::Windows::Forms::Padding(2, 2, 2, 0);
			this->toolStrip1->GripStyle = System::Windows::Forms::ToolStripGripStyle::Hidden;
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(17) {this->toolStripMenuItem1, 
				this->projectDropDownButton, this->toolStripSeparator2, this->toolStripLabel1, this->renderComboBox, this->editScenePropertiesButton, 
				this->saveSceneButton, this->toolStripSeparator3, this->toolStripLabel2, this->cameraComboBox, this->toolStripSeparator4, this->prevButton, 
				this->playButton, this->pauseButton, this->nextButton, this->toolStripSeparator5, this->toolStripFrameNumberLabel});
			this->toolStrip1->Location = System::Drawing::Point(0, 0);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Size = System::Drawing::Size(1011, 25);
			this->toolStrip1->TabIndex = 0;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(12, 25);
			// 
			// projectDropDownButton
			// 
			this->projectDropDownButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->projectDropDownButton->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->recentProjectMenuItem, 
				this->openProjectMenuItem, this->toolStripSeparator1, this->importLWS2MenuItem, this->importImageMenuItem});
			this->projectDropDownButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"projectDropDownButton.Image")));
			this->projectDropDownButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->projectDropDownButton->Name = L"projectDropDownButton";
			this->projectDropDownButton->Size = System::Drawing::Size(54, 22);
			this->projectDropDownButton->Text = L"Project";
			// 
			// recentProjectMenuItem
			// 
			this->recentProjectMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->loadRecent1MenuItem, 
				this->loadRecent2MenuItem, this->loadRecent3MenuItem, this->loadRecent4MenuItem});
			this->recentProjectMenuItem->Name = L"recentProjectMenuItem";
			this->recentProjectMenuItem->Size = System::Drawing::Size(165, 22);
			this->recentProjectMenuItem->Text = L"Recent";
			// 
			// loadRecent1MenuItem
			// 
			this->loadRecent1MenuItem->Enabled = false;
			this->loadRecent1MenuItem->Name = L"loadRecent1MenuItem";
			this->loadRecent1MenuItem->Size = System::Drawing::Size(125, 22);
			this->loadRecent1MenuItem->Text = L"Recent1";
			this->loadRecent1MenuItem->Click += gcnew System::EventHandler(this, &Form1::loadRecent1MenuItem_Click);
			// 
			// loadRecent2MenuItem
			// 
			this->loadRecent2MenuItem->Enabled = false;
			this->loadRecent2MenuItem->Name = L"loadRecent2MenuItem";
			this->loadRecent2MenuItem->Size = System::Drawing::Size(125, 22);
			this->loadRecent2MenuItem->Text = L"Recent2";
			this->loadRecent2MenuItem->Click += gcnew System::EventHandler(this, &Form1::loadRecent2MenuItem_Click);
			// 
			// loadRecent3MenuItem
			// 
			this->loadRecent3MenuItem->Enabled = false;
			this->loadRecent3MenuItem->Name = L"loadRecent3MenuItem";
			this->loadRecent3MenuItem->Size = System::Drawing::Size(125, 22);
			this->loadRecent3MenuItem->Text = L"Recent3";
			this->loadRecent3MenuItem->Click += gcnew System::EventHandler(this, &Form1::loadRecent3MenuItem_Click);
			// 
			// loadRecent4MenuItem
			// 
			this->loadRecent4MenuItem->Enabled = false;
			this->loadRecent4MenuItem->Name = L"loadRecent4MenuItem";
			this->loadRecent4MenuItem->Size = System::Drawing::Size(125, 22);
			this->loadRecent4MenuItem->Text = L"Recent4";
			this->loadRecent4MenuItem->Click += gcnew System::EventHandler(this, &Form1::loadRecent4MenuItem_Click);
			// 
			// openProjectMenuItem
			// 
			this->openProjectMenuItem->Name = L"openProjectMenuItem";
			this->openProjectMenuItem->Size = System::Drawing::Size(165, 22);
			this->openProjectMenuItem->Text = L"Open ...";
			this->openProjectMenuItem->Click += gcnew System::EventHandler(this, &Form1::openProjectMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(162, 6);
			// 
			// importLWS2MenuItem
			// 
			this->importLWS2MenuItem->Enabled = false;
			this->importLWS2MenuItem->Name = L"importLWS2MenuItem";
			this->importLWS2MenuItem->Size = System::Drawing::Size(165, 22);
			this->importLWS2MenuItem->Text = L"Import LWS 2 ...";
			this->importLWS2MenuItem->Click += gcnew System::EventHandler(this, &Form1::importLWS2MenuItem_Click);
			// 
			// importImageMenuItem
			// 
			this->importImageMenuItem->Enabled = false;
			this->importImageMenuItem->Name = L"importImageMenuItem";
			this->importImageMenuItem->Size = System::Drawing::Size(165, 22);
			this->importImageMenuItem->Text = L"Import image ...";
			this->importImageMenuItem->Click += gcnew System::EventHandler(this, &Form1::importImageMenuItem_Click);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(6, 25);
			// 
			// toolStripLabel1
			// 
			this->toolStripLabel1->Name = L"toolStripLabel1";
			this->toolStripLabel1->Size = System::Drawing::Size(40, 22);
			this->toolStripLabel1->Text = L"Scene:";
			// 
			// renderComboBox
			// 
			this->renderComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->renderComboBox->Enabled = false;
			this->renderComboBox->MaxDropDownItems = 16;
			this->renderComboBox->Name = L"renderComboBox";
			this->renderComboBox->Size = System::Drawing::Size(121, 25);
			this->renderComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::renderComboBox_SelectedIndexChanged);
			// 
			// editScenePropertiesButton
			// 
			this->editScenePropertiesButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->editScenePropertiesButton->Enabled = false;
			this->editScenePropertiesButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"editScenePropertiesButton.Image")));
			this->editScenePropertiesButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->editScenePropertiesButton->Name = L"editScenePropertiesButton";
			this->editScenePropertiesButton->Size = System::Drawing::Size(23, 22);
			this->editScenePropertiesButton->Text = L"Edit";
			this->editScenePropertiesButton->ToolTipText = L"Edit currently active scene properties";
			this->editScenePropertiesButton->Click += gcnew System::EventHandler(this, &Form1::editScenePropertiesButton_Click);
			// 
			// saveSceneButton
			// 
			this->saveSceneButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->saveSceneButton->Enabled = false;
			this->saveSceneButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"saveSceneButton.Image")));
			this->saveSceneButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->saveSceneButton->Name = L"saveSceneButton";
			this->saveSceneButton->Size = System::Drawing::Size(23, 22);
			this->saveSceneButton->Text = L"toolStripButton1";
			this->saveSceneButton->ToolTipText = L"Save active scene\r\n(and nothing else!)";
			this->saveSceneButton->Click += gcnew System::EventHandler(this, &Form1::saveSceneButton_Click);
			// 
			// toolStripSeparator3
			// 
			this->toolStripSeparator3->Name = L"toolStripSeparator3";
			this->toolStripSeparator3->Size = System::Drawing::Size(6, 25);
			// 
			// toolStripLabel2
			// 
			this->toolStripLabel2->Name = L"toolStripLabel2";
			this->toolStripLabel2->Size = System::Drawing::Size(48, 22);
			this->toolStripLabel2->Text = L"Camera:";
			// 
			// cameraComboBox
			// 
			this->cameraComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cameraComboBox->Enabled = false;
			this->cameraComboBox->Name = L"cameraComboBox";
			this->cameraComboBox->Size = System::Drawing::Size(121, 25);
			// 
			// toolStripSeparator4
			// 
			this->toolStripSeparator4->Name = L"toolStripSeparator4";
			this->toolStripSeparator4->Size = System::Drawing::Size(6, 25);
			// 
			// prevButton
			// 
			this->prevButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->prevButton->Enabled = false;
			this->prevButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"prevButton.Image")));
			this->prevButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->prevButton->Name = L"prevButton";
			this->prevButton->Size = System::Drawing::Size(23, 22);
			this->prevButton->Text = L"Previous";
			// 
			// playButton
			// 
			this->playButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->playButton->Enabled = false;
			this->playButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"playButton.Image")));
			this->playButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->playButton->Name = L"playButton";
			this->playButton->Size = System::Drawing::Size(23, 22);
			this->playButton->Text = L"Play";
			this->playButton->Click += gcnew System::EventHandler(this, &Form1::playButton_Click);
			// 
			// pauseButton
			// 
			this->pauseButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->pauseButton->Enabled = false;
			this->pauseButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pauseButton.Image")));
			this->pauseButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->pauseButton->Name = L"pauseButton";
			this->pauseButton->Size = System::Drawing::Size(23, 22);
			this->pauseButton->Text = L"Pause";
			this->pauseButton->Click += gcnew System::EventHandler(this, &Form1::pauseButton_Click);
			// 
			// nextButton
			// 
			this->nextButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->nextButton->Enabled = false;
			this->nextButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"nextButton.Image")));
			this->nextButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->nextButton->Name = L"nextButton";
			this->nextButton->Size = System::Drawing::Size(23, 22);
			this->nextButton->Text = L"Next";
			// 
			// toolStripSeparator5
			// 
			this->toolStripSeparator5->Name = L"toolStripSeparator5";
			this->toolStripSeparator5->Size = System::Drawing::Size(6, 25);
			// 
			// toolStripFrameNumberLabel
			// 
			this->toolStripFrameNumberLabel->Name = L"toolStripFrameNumberLabel";
			this->toolStripFrameNumberLabel->Size = System::Drawing::Size(13, 22);
			this->toolStripFrameNumberLabel->Text = L"0";
			// 
			// renderPanel
			// 
			this->renderPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->renderPanel->Location = System::Drawing::Point(4, 3);
			this->renderPanel->Name = L"renderPanel";
			this->renderPanel->Size = System::Drawing::Size(503, 319);
			this->renderPanel->TabIndex = 1;
			// 
			// folderBrowser
			// 
			this->folderBrowser->RootFolder = System::Environment::SpecialFolder::MyComputer;
			// 
			// splitContainerVMain
			// 
			this->splitContainerVMain->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainerVMain->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainerVMain->FixedPanel = System::Windows::Forms::FixedPanel::Panel1;
			this->splitContainerVMain->Location = System::Drawing::Point(0, 25);
			this->splitContainerVMain->Name = L"splitContainerVMain";
			// 
			// splitContainerVMain.Panel1
			// 
			this->splitContainerVMain->Panel1->Controls->Add(this->objectListBox);
			this->splitContainerVMain->Panel1->Controls->Add(this->toolStrip2);
			// 
			// splitContainerVMain.Panel2
			// 
			this->splitContainerVMain->Panel2->Controls->Add(this->splitContainerVSecondary);
			this->splitContainerVMain->Size = System::Drawing::Size(1011, 664);
			this->splitContainerVMain->SplitterDistance = 196;
			this->splitContainerVMain->TabIndex = 2;
			this->splitContainerVMain->Text = L"splitContainer1";
			this->splitContainerVMain->SplitterMoved += gcnew System::Windows::Forms::SplitterEventHandler(this, &Form1::splitContainerVMain_SplitterMoved);
			// 
			// objectListBox
			// 
			this->objectListBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->objectListBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->objectListBox->FormattingEnabled = true;
			this->objectListBox->Location = System::Drawing::Point(0, 29);
			this->objectListBox->Name = L"objectListBox";
			this->objectListBox->Size = System::Drawing::Size(192, 624);
			this->objectListBox->TabIndex = 1;
			this->objectListBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::objectListBox_SelectedIndexChanged);
			// 
			// toolStrip2
			// 
			this->toolStrip2->GripStyle = System::Windows::Forms::ToolStripGripStyle::Hidden;
			this->toolStrip2->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->editTransformNodesButton, 
				this->editMeshNodesButton, this->editSurfacesButton, this->editTexturesButton, this->editCameraFxButton, this->toolStripSeparator7, 
				this->newObjectButton});
			this->toolStrip2->Location = System::Drawing::Point(0, 0);
			this->toolStrip2->Name = L"toolStrip2";
			this->toolStrip2->Size = System::Drawing::Size(192, 29);
			this->toolStrip2->TabIndex = 0;
			this->toolStrip2->Text = L"toolStrip2";
			// 
			// editTransformNodesButton
			// 
			this->editTransformNodesButton->Checked = true;
			this->editTransformNodesButton->CheckOnClick = true;
			this->editTransformNodesButton->CheckState = System::Windows::Forms::CheckState::Checked;
			this->editTransformNodesButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->editTransformNodesButton->Enabled = false;
			this->editTransformNodesButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"editTransformNodesButton.Image")));
			this->editTransformNodesButton->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->editTransformNodesButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->editTransformNodesButton->Name = L"editTransformNodesButton";
			this->editTransformNodesButton->Size = System::Drawing::Size(26, 26);
			this->editTransformNodesButton->Text = L"toolStripButton1";
			this->editTransformNodesButton->ToolTipText = L"Transform nodes";
			this->editTransformNodesButton->Click += gcnew System::EventHandler(this, &Form1::editTransformNodesButton_Click);
			// 
			// editMeshNodesButton
			// 
			this->editMeshNodesButton->CheckOnClick = true;
			this->editMeshNodesButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->editMeshNodesButton->Enabled = false;
			this->editMeshNodesButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"editMeshNodesButton.Image")));
			this->editMeshNodesButton->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->editMeshNodesButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->editMeshNodesButton->Name = L"editMeshNodesButton";
			this->editMeshNodesButton->Size = System::Drawing::Size(26, 26);
			this->editMeshNodesButton->Text = L"toolStripButton2";
			this->editMeshNodesButton->ToolTipText = L"Mesh nodes";
			this->editMeshNodesButton->Click += gcnew System::EventHandler(this, &Form1::editMeshNodesButton_Click);
			// 
			// editSurfacesButton
			// 
			this->editSurfacesButton->CheckOnClick = true;
			this->editSurfacesButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->editSurfacesButton->Enabled = false;
			this->editSurfacesButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"editSurfacesButton.Image")));
			this->editSurfacesButton->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->editSurfacesButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->editSurfacesButton->Name = L"editSurfacesButton";
			this->editSurfacesButton->Size = System::Drawing::Size(26, 26);
			this->editSurfacesButton->Text = L"toolStripButton3";
			this->editSurfacesButton->ToolTipText = L"Surfaces";
			this->editSurfacesButton->Click += gcnew System::EventHandler(this, &Form1::editSurfacesButton_Click);
			// 
			// editTexturesButton
			// 
			this->editTexturesButton->CheckOnClick = true;
			this->editTexturesButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->editTexturesButton->Enabled = false;
			this->editTexturesButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"editTexturesButton.Image")));
			this->editTexturesButton->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->editTexturesButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->editTexturesButton->Name = L"editTexturesButton";
			this->editTexturesButton->Size = System::Drawing::Size(26, 26);
			this->editTexturesButton->Text = L"toolStripButton5";
			this->editTexturesButton->ToolTipText = L"Textures";
			this->editTexturesButton->Click += gcnew System::EventHandler(this, &Form1::editTexturesButton_Click);
			// 
			// editCameraFxButton
			// 
			this->editCameraFxButton->CheckOnClick = true;
			this->editCameraFxButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->editCameraFxButton->Enabled = false;
			this->editCameraFxButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"editCameraFxButton.Image")));
			this->editCameraFxButton->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->editCameraFxButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->editCameraFxButton->Name = L"editCameraFxButton";
			this->editCameraFxButton->Size = System::Drawing::Size(26, 26);
			this->editCameraFxButton->Text = L"toolStripButton4";
			this->editCameraFxButton->ToolTipText = L"Camera fx groups";
			this->editCameraFxButton->Click += gcnew System::EventHandler(this, &Form1::editCameraFxButton_Click);
			// 
			// toolStripSeparator7
			// 
			this->toolStripSeparator7->Name = L"toolStripSeparator7";
			this->toolStripSeparator7->Size = System::Drawing::Size(6, 29);
			// 
			// newObjectButton
			// 
			this->newObjectButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->newObjectButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"newObjectButton.Image")));
			this->newObjectButton->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->newObjectButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->newObjectButton->Name = L"newObjectButton";
			this->newObjectButton->Size = System::Drawing::Size(26, 26);
			this->newObjectButton->ToolTipText = L"Click here to create new object";
			this->newObjectButton->Click += gcnew System::EventHandler(this, &Form1::newObjectButton_Click);
			// 
			// splitContainerVSecondary
			// 
			this->splitContainerVSecondary->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainerVSecondary->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainerVSecondary->FixedPanel = System::Windows::Forms::FixedPanel::Panel1;
			this->splitContainerVSecondary->Location = System::Drawing::Point(0, 0);
			this->splitContainerVSecondary->Name = L"splitContainerVSecondary";
			// 
			// splitContainerVSecondary.Panel1
			// 
			this->splitContainerVSecondary->Panel1->Controls->Add(this->splitContainerProperties);
			// 
			// splitContainerVSecondary.Panel2
			// 
			this->splitContainerVSecondary->Panel2->Controls->Add(this->splitContainerHRight);
			this->splitContainerVSecondary->Size = System::Drawing::Size(811, 664);
			this->splitContainerVSecondary->SplitterDistance = 298;
			this->splitContainerVSecondary->TabIndex = 0;
			this->splitContainerVSecondary->SplitterMoved += gcnew System::Windows::Forms::SplitterEventHandler(this, &Form1::splitContainerVSecondary_SplitterMoved);
			// 
			// splitContainerProperties
			// 
			this->splitContainerProperties->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainerProperties->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainerProperties->Location = System::Drawing::Point(0, 0);
			this->splitContainerProperties->Name = L"splitContainerProperties";
			this->splitContainerProperties->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainerProperties.Panel1
			// 
			this->splitContainerProperties->Panel1->Controls->Add(this->tableLayoutPanel1);
			this->splitContainerProperties->Panel1->Controls->Add(this->toolStrip3);
			// 
			// splitContainerProperties.Panel2
			// 
			this->splitContainerProperties->Panel2->BackColor = System::Drawing::Color::White;
			this->splitContainerProperties->Panel2->Controls->Add(this->toolStrip4);
			this->splitContainerProperties->Size = System::Drawing::Size(298, 664);
			this->splitContainerProperties->SplitterDistance = 415;
			this->splitContainerProperties->TabIndex = 0;
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 1;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				100)));
			this->tableLayoutPanel1->Controls->Add(this->propertyList, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->groupBox1, 0, 1);
			this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel1->Location = System::Drawing::Point(0, 29);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 2;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 86)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(294, 382);
			this->tableLayoutPanel1->TabIndex = 3;
			// 
			// propertyList
			// 
			this->propertyList->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {this->NamesCol, this->ValuesCol});
			this->propertyList->Dock = System::Windows::Forms::DockStyle::Fill;
			this->propertyList->FullRowSelect = true;
			this->propertyList->GridLines = true;
			this->propertyList->HeaderStyle = System::Windows::Forms::ColumnHeaderStyle::Nonclickable;
			this->propertyList->HideSelection = false;
			this->propertyList->Location = System::Drawing::Point(3, 3);
			this->propertyList->MultiSelect = false;
			this->propertyList->Name = L"propertyList";
			this->propertyList->ShowItemToolTips = true;
			this->propertyList->Size = System::Drawing::Size(288, 290);
			this->propertyList->SmallImageList = this->propertyTypesImageList;
			this->propertyList->TabIndex = 2;
			this->propertyList->UseCompatibleStateImageBehavior = false;
			this->propertyList->View = System::Windows::Forms::View::Details;
			this->propertyList->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::propertyList_SelectedIndexChanged);
			// 
			// NamesCol
			// 
			this->NamesCol->Text = L"Name";
			this->NamesCol->Width = 129;
			// 
			// ValuesCol
			// 
			this->ValuesCol->Text = L"Value";
			this->ValuesCol->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->ValuesCol->Width = 126;
			// 
			// propertyTypesImageList
			// 
			this->propertyTypesImageList->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"propertyTypesImageList.ImageStream")));
			this->propertyTypesImageList->TransparentColor = System::Drawing::Color::Transparent;
			this->propertyTypesImageList->Images->SetKeyName(0, L"Envelope");
			this->propertyTypesImageList->Images->SetKeyName(1, L"Transform Node");
			this->propertyTypesImageList->Images->SetKeyName(2, L"Mesh Node");
			this->propertyTypesImageList->Images->SetKeyName(3, L"Surface");
			this->propertyTypesImageList->Images->SetKeyName(4, L"Texture");
			this->propertyTypesImageList->Images->SetKeyName(5, L"Color");
			this->propertyTypesImageList->Images->SetKeyName(6, L"Motion");
			this->propertyTypesImageList->Images->SetKeyName(7, L"FX Stack");
			this->propertyTypesImageList->Images->SetKeyName(8, L"File");
			this->propertyTypesImageList->Images->SetKeyName(9, L"Numeric");
			this->propertyTypesImageList->Images->SetKeyName(10, L"Choice");
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->propertyEditorChoice);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->propertyEditorNumeric);
			this->groupBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->groupBox1->Location = System::Drawing::Point(3, 299);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(288, 80);
			this->groupBox1->TabIndex = 3;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Simple editors";
			// 
			// propertyEditorChoice
			// 
			this->propertyEditorChoice->DropDownHeight = 250;
			this->propertyEditorChoice->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->propertyEditorChoice->DropDownWidth = 250;
			this->propertyEditorChoice->Enabled = false;
			this->propertyEditorChoice->FormattingEnabled = true;
			this->propertyEditorChoice->IntegralHeight = false;
			this->propertyEditorChoice->Location = System::Drawing::Point(58, 49);
			this->propertyEditorChoice->MaxDropDownItems = 30;
			this->propertyEditorChoice->Name = L"propertyEditorChoice";
			this->propertyEditorChoice->Size = System::Drawing::Size(203, 21);
			this->propertyEditorChoice->TabIndex = 3;
			this->propertyEditorChoice->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::propertyEditorChoice_SelectedIndexChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(6, 57);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(40, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Choice";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(6, 26);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(46, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Numeric";
			// 
			// propertyEditorNumeric
			// 
			this->propertyEditorNumeric->Enabled = false;
			this->propertyEditorNumeric->Location = System::Drawing::Point(58, 19);
			this->propertyEditorNumeric->Name = L"propertyEditorNumeric";
			this->propertyEditorNumeric->Size = System::Drawing::Size(203, 20);
			this->propertyEditorNumeric->TabIndex = 0;
			this->propertyEditorNumeric->ValueChanged += gcnew System::EventHandler(this, &Form1::propertyEditorNumeric_ValueChanged);
			// 
			// toolStrip3
			// 
			this->toolStrip3->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->propertyListLabel, 
				this->toolStripSeparator6, this->rebuildActiveObjectBtn, this->saveActiveObjectBtn});
			this->toolStrip3->Location = System::Drawing::Point(0, 0);
			this->toolStrip3->Name = L"toolStrip3";
			this->toolStrip3->Size = System::Drawing::Size(294, 29);
			this->toolStrip3->TabIndex = 1;
			this->toolStrip3->Text = L"toolStrip3";
			// 
			// propertyListLabel
			// 
			this->propertyListLabel->AutoSize = false;
			this->propertyListLabel->AutoToolTip = true;
			this->propertyListLabel->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->propertyListLabel->Name = L"propertyListLabel";
			this->propertyListLabel->Size = System::Drawing::Size(130, 22);
			this->propertyListLabel->Text = L"Properties (empty)";
			this->propertyListLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// toolStripSeparator6
			// 
			this->toolStripSeparator6->Name = L"toolStripSeparator6";
			this->toolStripSeparator6->Size = System::Drawing::Size(6, 29);
			// 
			// rebuildActiveObjectBtn
			// 
			this->rebuildActiveObjectBtn->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->rebuildActiveObjectBtn->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"rebuildActiveObjectBtn.Image")));
			this->rebuildActiveObjectBtn->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->rebuildActiveObjectBtn->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->rebuildActiveObjectBtn->Name = L"rebuildActiveObjectBtn";
			this->rebuildActiveObjectBtn->Size = System::Drawing::Size(26, 26);
			this->rebuildActiveObjectBtn->Text = L"Rebuild";
			this->rebuildActiveObjectBtn->ToolTipText = L"Rebuild active object\r\n(required for some changes to take effect)";
			// 
			// saveActiveObjectBtn
			// 
			this->saveActiveObjectBtn->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->saveActiveObjectBtn->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"saveActiveObjectBtn.Image")));
			this->saveActiveObjectBtn->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->saveActiveObjectBtn->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->saveActiveObjectBtn->Name = L"saveActiveObjectBtn";
			this->saveActiveObjectBtn->Size = System::Drawing::Size(26, 26);
			this->saveActiveObjectBtn->Text = L"toolStripButton1";
			this->saveActiveObjectBtn->ToolTipText = L"Save active object\r\n(and nothing else)";
			this->saveActiveObjectBtn->Click += gcnew System::EventHandler(this, &Form1::saveActiveObjectBtn_Click);
			// 
			// toolStrip4
			// 
			this->toolStrip4->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->toolStripLabel5, 
				this->listEditPanelAddItemBtn, this->listEditPanelRemoveItemBtn});
			this->toolStrip4->Location = System::Drawing::Point(0, 0);
			this->toolStrip4->Name = L"toolStrip4";
			this->toolStrip4->Size = System::Drawing::Size(294, 29);
			this->toolStrip4->TabIndex = 0;
			this->toolStrip4->Text = L"toolStrip4";
			// 
			// toolStripLabel5
			// 
			this->toolStripLabel5->Name = L"toolStripLabel5";
			this->toolStripLabel5->Size = System::Drawing::Size(51, 26);
			this->toolStripLabel5->Text = L"List items";
			// 
			// listEditPanelAddItemBtn
			// 
			this->listEditPanelAddItemBtn->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->listEditPanelAddItemBtn->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"listEditPanelAddItemBtn.Image")));
			this->listEditPanelAddItemBtn->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->listEditPanelAddItemBtn->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->listEditPanelAddItemBtn->Name = L"listEditPanelAddItemBtn";
			this->listEditPanelAddItemBtn->Size = System::Drawing::Size(26, 26);
			this->listEditPanelAddItemBtn->Text = L"Add";
			this->listEditPanelAddItemBtn->ToolTipText = L"Add item to list";
			// 
			// listEditPanelRemoveItemBtn
			// 
			this->listEditPanelRemoveItemBtn->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->listEditPanelRemoveItemBtn->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"listEditPanelRemoveItemBtn.Image")));
			this->listEditPanelRemoveItemBtn->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->listEditPanelRemoveItemBtn->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->listEditPanelRemoveItemBtn->Name = L"listEditPanelRemoveItemBtn";
			this->listEditPanelRemoveItemBtn->Size = System::Drawing::Size(26, 26);
			this->listEditPanelRemoveItemBtn->Text = L"Remove";
			this->listEditPanelRemoveItemBtn->ToolTipText = L"Remove item from list";
			// 
			// splitContainerHRight
			// 
			this->splitContainerHRight->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainerHRight->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainerHRight->Location = System::Drawing::Point(0, 0);
			this->splitContainerHRight->Name = L"splitContainerHRight";
			this->splitContainerHRight->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainerHRight.Panel1
			// 
			this->splitContainerHRight->Panel1->Controls->Add(this->renderPanel);
			// 
			// splitContainerHRight.Panel2
			// 
			this->splitContainerHRight->Panel2->Controls->Add(this->tabControl1);
			this->splitContainerHRight->Size = System::Drawing::Size(509, 664);
			this->splitContainerHRight->SplitterDistance = 332;
			this->splitContainerHRight->TabIndex = 2;
			this->splitContainerHRight->Text = L"splitContainer1";
			this->splitContainerHRight->SplitterMoved += gcnew System::Windows::Forms::SplitterEventHandler(this, &Form1::splitContainerHRight_SplitterMoved);
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->envelopeEditorTabPage);
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->HotTrack = true;
			this->tabControl1->Location = System::Drawing::Point(0, 0);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(505, 324);
			this->tabControl1->TabIndex = 0;
			// 
			// envelopeEditorTabPage
			// 
			this->envelopeEditorTabPage->Controls->Add(this->envelopeEditor);
			this->envelopeEditorTabPage->Controls->Add(this->toolStrip5);
			this->envelopeEditorTabPage->Location = System::Drawing::Point(4, 22);
			this->envelopeEditorTabPage->Name = L"envelopeEditorTabPage";
			this->envelopeEditorTabPage->Padding = System::Windows::Forms::Padding(3);
			this->envelopeEditorTabPage->Size = System::Drawing::Size(497, 298);
			this->envelopeEditorTabPage->TabIndex = 0;
			this->envelopeEditorTabPage->Text = L"Envelope";
			this->envelopeEditorTabPage->UseVisualStyleBackColor = true;
			// 
			// envelopeEditor
			// 
			this->envelopeEditor->ControlBarHeight = 14;
			this->envelopeEditor->CurveColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(0)), 
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->envelopeEditor->Dock = System::Windows::Forms::DockStyle::Fill;
			this->envelopeEditor->EndFrame = 100;
			envelope1->PostBehavior = LRControls::Envelope::Behavior::BEH_RESET;
			envelope1->PreBehavior = LRControls::Envelope::Behavior::BEH_RESET;
			this->envelopeEditor->EnvelopeObj = envelope1;
			this->envelopeEditor->HighValue = 1;
			this->envelopeEditor->KeyDraggedColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->envelopeEditor->KeyHitTolerance = 5;
			this->envelopeEditor->KeyNormalColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->envelopeEditor->KeyOverColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->envelopeEditor->KeySelectedColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), 
				static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->envelopeEditor->Location = System::Drawing::Point(3, 32);
			this->envelopeEditor->LowValue = 0;
			this->envelopeEditor->MouseMode = LRControls::EnvelopeEditor::eMouseMode::AddDrag;
			this->envelopeEditor->Name = L"envelopeEditor";
			this->envelopeEditor->OutlineLowSize = 3;
			this->envelopeEditor->OutlineSize = 5;
			this->envelopeEditor->RangeSelectionColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->envelopeEditor->Size = System::Drawing::Size(491, 263);
			this->envelopeEditor->StartFrame = 0;
			this->envelopeEditor->TabIndex = 1;
			this->envelopeEditor->VertexSize = 3;
			// 
			// toolStrip5
			// 
			this->toolStrip5->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->envelopeEditModeAddDragButton, 
				this->envelopeEditModeRemoveButton, this->envelopeEditModePanButton, this->envelopeEditModeZoomButton, this->toolStripSeparator8, 
				this->toolStripLabel3, this->envelopeChannelComboBox});
			this->toolStrip5->Location = System::Drawing::Point(3, 3);
			this->toolStrip5->Name = L"toolStrip5";
			this->toolStrip5->Size = System::Drawing::Size(491, 29);
			this->toolStrip5->TabIndex = 0;
			this->toolStrip5->Text = L"toolStrip5";
			// 
			// envelopeEditModeAddDragButton
			// 
			this->envelopeEditModeAddDragButton->Checked = true;
			this->envelopeEditModeAddDragButton->CheckState = System::Windows::Forms::CheckState::Checked;
			this->envelopeEditModeAddDragButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->envelopeEditModeAddDragButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"envelopeEditModeAddDragButton.Image")));
			this->envelopeEditModeAddDragButton->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->envelopeEditModeAddDragButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->envelopeEditModeAddDragButton->Name = L"envelopeEditModeAddDragButton";
			this->envelopeEditModeAddDragButton->Size = System::Drawing::Size(26, 26);
			this->envelopeEditModeAddDragButton->ToolTipText = L"Add new key/Drag existing key mode";
			this->envelopeEditModeAddDragButton->Click += gcnew System::EventHandler(this, &Form1::envelopeEditModeAddDragButton_Click);
			// 
			// envelopeEditModeRemoveButton
			// 
			this->envelopeEditModeRemoveButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->envelopeEditModeRemoveButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"envelopeEditModeRemoveButton.Image")));
			this->envelopeEditModeRemoveButton->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->envelopeEditModeRemoveButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->envelopeEditModeRemoveButton->Name = L"envelopeEditModeRemoveButton";
			this->envelopeEditModeRemoveButton->Size = System::Drawing::Size(26, 26);
			this->envelopeEditModeRemoveButton->ToolTipText = L"Remove key mode";
			this->envelopeEditModeRemoveButton->Click += gcnew System::EventHandler(this, &Form1::envelopeEditModeRemoveButton_Click);
			// 
			// envelopeEditModePanButton
			// 
			this->envelopeEditModePanButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->envelopeEditModePanButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"envelopeEditModePanButton.Image")));
			this->envelopeEditModePanButton->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->envelopeEditModePanButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->envelopeEditModePanButton->Name = L"envelopeEditModePanButton";
			this->envelopeEditModePanButton->Size = System::Drawing::Size(26, 26);
			this->envelopeEditModePanButton->ToolTipText = L"Pan mode";
			this->envelopeEditModePanButton->Click += gcnew System::EventHandler(this, &Form1::envelopeEditModePanButton_Click);
			// 
			// envelopeEditModeZoomButton
			// 
			this->envelopeEditModeZoomButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->envelopeEditModeZoomButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"envelopeEditModeZoomButton.Image")));
			this->envelopeEditModeZoomButton->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->envelopeEditModeZoomButton->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->envelopeEditModeZoomButton->Name = L"envelopeEditModeZoomButton";
			this->envelopeEditModeZoomButton->Size = System::Drawing::Size(26, 26);
			this->envelopeEditModeZoomButton->ToolTipText = L"Zoom mode";
			this->envelopeEditModeZoomButton->Click += gcnew System::EventHandler(this, &Form1::envelopeEditModeZoomButton_Click);
			// 
			// toolStripSeparator8
			// 
			this->toolStripSeparator8->Name = L"toolStripSeparator8";
			this->toolStripSeparator8->Size = System::Drawing::Size(6, 29);
			// 
			// toolStripLabel3
			// 
			this->toolStripLabel3->Name = L"toolStripLabel3";
			this->toolStripLabel3->Size = System::Drawing::Size(46, 26);
			this->toolStripLabel3->Text = L"Channel";
			// 
			// envelopeChannelComboBox
			// 
			this->envelopeChannelComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->envelopeChannelComboBox->Name = L"envelopeChannelComboBox";
			this->envelopeChannelComboBox->Size = System::Drawing::Size(121, 29);
			this->envelopeChannelComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::envelopeChannelComboBox_SelectedIndexChanged);
			// 
			// openFileDialog
			// 
			this->openFileDialog->Filter = L"Lightwave scene files (*.lws)|*.lws";
			// 
			// openImageFileDialog
			// 
			this->openImageFileDialog->Filter = L"Image files (*.jpg;*.tga;*.png)|*.jpg;*.tga;*.png";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1011, 689);
			this->Controls->Add(this->splitContainerVMain);
			this->Controls->Add(this->toolStrip1);
			this->Name = L"Form1";
			this->Text = L"LRStudio";
			this->Resize += gcnew System::EventHandler(this, &Form1::Form1_Resize);
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->splitContainerVMain->Panel1->ResumeLayout(false);
			this->splitContainerVMain->Panel1->PerformLayout();
			this->splitContainerVMain->Panel2->ResumeLayout(false);
			this->splitContainerVMain->ResumeLayout(false);
			this->toolStrip2->ResumeLayout(false);
			this->toolStrip2->PerformLayout();
			this->splitContainerVSecondary->Panel1->ResumeLayout(false);
			this->splitContainerVSecondary->Panel2->ResumeLayout(false);
			this->splitContainerVSecondary->ResumeLayout(false);
			this->splitContainerProperties->Panel1->ResumeLayout(false);
			this->splitContainerProperties->Panel1->PerformLayout();
			this->splitContainerProperties->Panel2->ResumeLayout(false);
			this->splitContainerProperties->Panel2->PerformLayout();
			this->splitContainerProperties->ResumeLayout(false);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->propertyEditorNumeric))->EndInit();
			this->toolStrip3->ResumeLayout(false);
			this->toolStrip3->PerformLayout();
			this->toolStrip4->ResumeLayout(false);
			this->toolStrip4->PerformLayout();
			this->splitContainerHRight->Panel1->ResumeLayout(false);
			this->splitContainerHRight->Panel2->ResumeLayout(false);
			this->splitContainerHRight->ResumeLayout(false);
			this->tabControl1->ResumeLayout(false);
			this->envelopeEditorTabPage->ResumeLayout(false);
			this->envelopeEditorTabPage->PerformLayout();
			this->toolStrip5->ResumeLayout(false);
			this->toolStrip5->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: 
	private:
		bool					is_playing;
		bool					is_paused;
		DWORD					start_tick;
		DWORD					pause_tick;
		ST::Thread				^render_thread;
		LRControls::Envelope	^edited_envelope;
		delegate				System::Void RenderDelegate();

		System::Void Form1_Resize(System::Object^  sender, System::EventArgs^  e)
		{
			InternalResize();
		}

		System::Void InternalResize()
		{
			System::Drawing::Size left_size=splitContainerHRight->Panel1->ClientSize;
			const float ref_aspect=16.0f/9.0f;
			if((float)(left_size.Width)/left_size.Height>ref_aspect)
			{
				//full height, margins on left and right side
				int new_width=(int)(left_size.Height*ref_aspect);
				this->renderPanel->Size=System::Drawing::Size(new_width,left_size.Height);
				this->renderPanel->Location=System::Drawing::Point((left_size.Width-new_width)/2,0);
			}
			else
			{
				//full width, margins on top and bottom
				int new_height=(int)(left_size.Width/ref_aspect);
				this->renderPanel->Size=System::Drawing::Size(left_size.Width,new_height);
				this->renderPanel->Location=System::Drawing::Point(0,(left_size.Height-new_height)/2);
			}
		}

		bool checkHardwareCaps(const D3DCAPS9 *pHardwareCaps)
		{
			if(!pHardwareCaps)
			{
				MessageBox::Show("Something went terribly wrong :(",
					"D3D check error",MessageBoxButtons::OK,MessageBoxIcon::Exclamation);
				return false;
			}
			if(pHardwareCaps->PixelShaderVersion<D3DPS_VERSION(2,0))
			{
				MessageBox::Show("Your graphics card does not support pixel shader 2.0",
					"D3D check error",MessageBoxButtons::OK,MessageBoxIcon::Exclamation);
				return false;
			}
			if(pHardwareCaps->VertexShaderVersion<D3DVS_VERSION(2,0))
			{
				MessageBox::Show("Your graphics card does not support vertex shader 2.0",
					"D3D check error",MessageBoxButtons::OK,MessageBoxIcon::Exclamation);
				return false;
			}
			return true;
		}

		System::Void initLR()
		{
			LR::GetEngine(&globals.engine);
			if(globals.engine.isNull())
			{
				MessageBox::Show(
					"Failed to create main engine object!\nApplication will exit!",
					"Initialization error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Exclamation);
				Application::Exit();
			}
			try
			{
				globals.engine->createD3DSubsystem(globals.renderer);
				globals.renderer->setRenderAspectRatio(LR::AR_16_9);
				if(!checkHardwareCaps(globals.renderer->getHardwareCaps()))
				{
					Application::Exit();
				}
				LR::DeviceCreateStruct2 createstruct;
				createstruct.bAllowDeviceChange=TRUE;
				createstruct.bAllowWindowedChange=FALSE;
				createstruct.bWindowed=TRUE;
				createstruct.dwDevice=0;	//HAL device
				createstruct.hWnd=NULL;
				createstruct.bDefaultAutoMipMaps=FALSE;
				createstruct.bDefaultVsync=TRUE;
				createstruct.dwDefaultDeviceAspect=LR::AR_4_3;
				createstruct.dwPreferredRTSize=1024;
				createstruct.dwPreferredScreenWidth=800;
				strcpy(createstruct.szProgramRegKey,"Software\\Addict Laboratories\\LRStudio");
				globals.renderer->displayAdvancedConfig2(&createstruct);
				globals.renderer->setDeviceWindowHandle((HWND)this->renderPanel->Handle.ToPointer());
				globals.renderer->createRenderingDevice();
				globals.engine->createFilesystem(globals.filesystem);
				globals.engine->setDefaultFilesystem(globals.filesystem);
				globals.filesystem->openFilesystem(500000);
			}
			catch(LR::Exception &e)
			{
				globals.filesystem=NULL;
				globals.renderer=NULL;
				globals.engine=NULL;
				MessageBox::Show(
					utils::makeManagedString(e.getDescription()),
					"Initialization error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Exclamation);
				Application::Exit();
			}
		}

		System::Void openProjectMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if(this->folderBrowser->ShowDialog()!=SWF::DialogResult::OK)
			{
				return;
			}
			String ^selected_path=this->folderBrowser->SelectedPath;
			if(!openProject(selected_path))
			{
				return;
			}
			addRecentProject(selected_path);
		}

		System::Void renderComboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		{
			int sel=this->renderComboBox->SelectedIndex;
			if(sel==-1)
			{
				globals.engine->unloadAll();
				enableSceneControls(false);
				return;
			}
			try
			{
				System::String ^scene_name=(System::String^)renderComboBox->Items[sel];
				System::String ^scene_file=scene_name+".lrsc2";
				globals.clean();
				globals.engine->loadScene(utils::makeNativeString(scene_file).c_str(),globals.scene);
				refreshCameraList();
				refreshActiveObjectList();
			}
			catch(LR::Exception &e)
			{
				MessageBox::Show(
					utils::makeManagedString(e.getDescription()),
					"Scene loading error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Exclamation);
				return;
			}
			enableSceneControls(true);
		}

		System::Void enableSceneControls(bool enable)
		{
			this->prevButton->Enabled=enable;
			this->playButton->Enabled=enable;
			this->pauseButton->Enabled=enable;
			this->nextButton->Enabled=enable;
			this->editTransformNodesButton->Enabled=enable;
			this->editScenePropertiesButton->Enabled=enable;
			this->saveSceneButton->Enabled=enable;
		}

		System::Void playButton_Click(System::Object^  sender, System::EventArgs^  e)
		{
			is_playing=true;
			if(is_paused)
			{
				start_tick=Environment::TickCount-(pause_tick-start_tick);
				is_paused=false;
			}
			else
			{
				start_tick=Environment::TickCount;
			}
		}

		System::Void pauseButton_Click(System::Object^  sender, System::EventArgs^  e)
		{
			is_paused=true;
			is_playing=false;
			pause_tick=Environment::TickCount;
		}

		System::Void renderFrame()
		{
			//render frame
			static DWORD dwFrameStats[90];
			static DWORD dwCurStatFrame=0;
			static bool bStatsValid=false;
			if(globals.scene.isNull())
			{
				return;
			}
			if(globals.active_camera==-1)
			{
				return;
			}
			int iStartFrame=globals.scene->getStartFrame();
			int iEndFrame=globals.scene->getEndFrame();
			float fSceneTime=0.0f;
			DWORD dwCurrentTime=Environment::TickCount;
			dwFrameStats[dwCurStatFrame++]=dwCurrentTime;
			DWORD dwDiff=0;
			if(is_playing)
			{
				dwDiff=dwCurrentTime-start_tick;
			}
			else if(is_paused)
			{
				dwDiff=pause_tick-start_tick;
			}
			if(dwCurStatFrame==90)
			{
				bStatsValid=true;
				dwCurStatFrame=0;
			}
			fSceneTime=iStartFrame*33.33f+
				(float)fmod(dwDiff/33.33f,(iEndFrame-iStartFrame))*33.33f;
			int frame=(int)(fSceneTime/33.33f);
			toolStripFrameNumberLabel->Text=String::Format("{0:D}",frame);
			D3DCOLOR ClearCol;
			if(globals.scene->getFogEnable())
			{
				float fFogR=0.0f;
				float fFogG=0.0f;
				float fFogB=0.0f;
				LR::ColorPtr pCol;
				globals.scene->getFogColor(&pCol);
				fFogR=pCol->getChannelValue(LR::COLOR_CHANNEL_R,0.001f*fSceneTime);
				fFogG=pCol->getChannelValue(LR::COLOR_CHANNEL_G,0.001f*fSceneTime);
				fFogB=pCol->getChannelValue(LR::COLOR_CHANNEL_B,0.001f*fSceneTime);
				ClearCol=D3DCOLOR_COLORVALUE(fFogR,fFogG,fFogB,0.0f);
			}
			else
			{
				ClearCol=D3DCOLOR_ARGB(0,0,0,0);
			}
			DWORD dwNumCameras=globals.scene->getCameraTNCount();
			globals.renderer->beginFrame(ClearCol,TRUE);	//by default don't require feedback
			globals.scene->renderFrame(0.001f*fSceneTime,globals.active_camera);
			if(bStatsValid)
			{
				char szFpsMsg[256];
				DWORD dwRefFrame=(dwCurStatFrame==0) ? 89 : dwCurStatFrame-1;
				float fFps=90000.0f/(dwFrameStats[dwRefFrame]-dwFrameStats[dwCurStatFrame]);
				sprintf(szFpsMsg,"fps: %.2f",fFps);
				globals.renderer->drawText(0,0,D3DCOLOR_ARGB(255,0,255,255),szFpsMsg);
			}
			globals.renderer->endFrame();
		}

		System::Void idleHandler()
		{
			try
			{
				while(1)
				{
					if(globals.scene.isNull())
					{
						ST::Thread::Sleep(50);
						continue;
					}
					this->Invoke(gcnew RenderDelegate(this,&Form1::renderFrame));
					ST::Thread::Sleep(0);
				}
			}
			catch(ST::ThreadInterruptedException^)
			{
			}
		}

		System::Void initRecentProjects()
		{
			Microsoft::Win32::RegistryKey ^lrkey=
				Microsoft::Win32::Registry::CurrentUser->CreateSubKey(
				"Software\\Addict Laboratories\\LRStudio");
			array<String^> ^rec=gcnew array<String^>(4);
			rec[0]=lrkey->GetValue("Recent1","Recent1",Microsoft::Win32::RegistryValueOptions::None)->ToString();
			rec[1]=lrkey->GetValue("Recent2","Recent2",Microsoft::Win32::RegistryValueOptions::None)->ToString();
			rec[2]=lrkey->GetValue("Recent3","Recent3",Microsoft::Win32::RegistryValueOptions::None)->ToString();
			rec[3]=lrkey->GetValue("Recent4","Recent4",Microsoft::Win32::RegistryValueOptions::None)->ToString();
			String ^str=rec[0];
			if(!str->Equals("Recent1"))
			{
				loadRecent1MenuItem->Enabled=true;
				loadRecent1MenuItem->Text=str;
			}
			else
			{
				loadRecent1MenuItem->Enabled=false;
			}
			str=rec[1];
			if(!str->Equals("Recent2"))
			{
				loadRecent2MenuItem->Enabled=true;
				loadRecent2MenuItem->Text=str;
			}
			else
			{
				loadRecent2MenuItem->Enabled=false;
			}
			str=rec[2];
			if(!str->Equals("Recent3"))
			{
				loadRecent3MenuItem->Enabled=true;
				loadRecent3MenuItem->Text=str;
			}
			else
			{
				loadRecent3MenuItem->Enabled=false;
			}
			str=rec[3];
			if(!str->Equals("Recent4"))
			{
				loadRecent4MenuItem->Enabled=true;
				loadRecent4MenuItem->Text=str;
			}
			else
			{
				loadRecent4MenuItem->Enabled=false;
			}
		}

		System::Void addRecentProject(String ^str)
		{
			if(loadRecent1MenuItem->Text->Equals(str))
			{
				return;
			}
			if(loadRecent2MenuItem->Text->Equals(str))
			{
				return;
			}
			if(loadRecent3MenuItem->Text->Equals(str))
			{
				return;
			}
			if(loadRecent4MenuItem->Text->Equals(str))
			{
				return;
			}
			if(loadRecent1MenuItem->Text->Equals("Recent1"))
			{
				loadRecent1MenuItem->Text=str;
				loadRecent1MenuItem->Enabled=true;
				dumpRecentProjectListToRegistry();
				return;
			}
			if(loadRecent2MenuItem->Text->Equals("Recent2"))
			{
				loadRecent2MenuItem->Text=str;
				loadRecent2MenuItem->Enabled=true;
				dumpRecentProjectListToRegistry();
				return;
			}
			if(loadRecent3MenuItem->Text->Equals("Recent3"))
			{
				loadRecent3MenuItem->Text=str;
				loadRecent3MenuItem->Enabled=true;
				dumpRecentProjectListToRegistry();
				return;
			}
			if(loadRecent4MenuItem->Text->Equals("Recent4"))
			{
				loadRecent4MenuItem->Text=str;
				loadRecent4MenuItem->Enabled=true;
				dumpRecentProjectListToRegistry();
				return;
			}
			loadRecent4MenuItem->Text=loadRecent3MenuItem->Text;
			loadRecent3MenuItem->Text=loadRecent2MenuItem->Text;
			loadRecent2MenuItem->Text=loadRecent1MenuItem->Text;
			loadRecent1MenuItem->Text=str;
			loadRecent1MenuItem->Enabled=true;
			dumpRecentProjectListToRegistry();
		}

		System::Void dumpRecentProjectListToRegistry()
		{
			Microsoft::Win32::RegistryKey ^lrkey=
				Microsoft::Win32::Registry::CurrentUser->CreateSubKey(
				"Software\\Addict Laboratories\\LRStudio");
			lrkey->SetValue("Recent1",loadRecent1MenuItem->Text,Microsoft::Win32::RegistryValueKind::String);
			lrkey->SetValue("Recent2",loadRecent2MenuItem->Text,Microsoft::Win32::RegistryValueKind::String);
			lrkey->SetValue("Recent3",loadRecent3MenuItem->Text,Microsoft::Win32::RegistryValueKind::String);
			lrkey->SetValue("Recent4",loadRecent4MenuItem->Text,Microsoft::Win32::RegistryValueKind::String);
		}

		System::Void loadRecentProject(ToolStripMenuItem ^item,String ^text)
		{
			if(item->Text->Equals(text))
			{
				return;
			}
			if(openProject(item->Text))
			{
				return;
			}
			item->Text=text;
			item->Enabled=false;
			dumpRecentProjectListToRegistry();
		}

		System::Void loadRecent1MenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			loadRecentProject(loadRecent1MenuItem,"Recent1");
		}

		System::Void loadRecent2MenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			loadRecentProject(loadRecent2MenuItem,"Recent2");
		}

		System::Void loadRecent3MenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			loadRecentProject(loadRecent3MenuItem,"Recent3");
		}

		System::Void loadRecent4MenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			loadRecentProject(loadRecent4MenuItem,"Recent4");
		}

		System::Void splitContainerVMain_SplitterMoved(System::Object^  sender,
			System::Windows::Forms::SplitterEventArgs^  e)
		{
			InternalResize();
		}

		System::Void splitContainerHRight_SplitterMoved(System::Object^  sender,
			System::Windows::Forms::SplitterEventArgs^  e)
		{
			InternalResize();
		}

		System::Void splitContainerVSecondary_SplitterMoved(System::Object^  sender,
			System::Windows::Forms::SplitterEventArgs^  e)
		{
			InternalResize();
		}

		System::Void uncheckObjectSelectionButtons()
		{
			editTransformNodesButton->Checked=false;
			editMeshNodesButton->Checked=false;
			editSurfacesButton->Checked=false;
			editCameraFxButton->Checked=false;
			editTexturesButton->Checked=false;
		}

		System::Void editTransformNodesButton_Click(System::Object^  sender, System::EventArgs^  e)
		{
			uncheckObjectSelectionButtons();
			editTransformNodesButton->Checked=true;
			refreshActiveObjectList();
		}

		System::Void editMeshNodesButton_Click(System::Object^  sender, System::EventArgs^  e)
		{
			uncheckObjectSelectionButtons();
			editMeshNodesButton->Checked=true;
			refreshActiveObjectList();
		}

		System::Void editSurfacesButton_Click(System::Object^  sender, System::EventArgs^  e)
		{
			uncheckObjectSelectionButtons();
			editSurfacesButton->Checked=true;
			refreshActiveObjectList();
		}

		System::Void editCameraFxButton_Click(System::Object^  sender, System::EventArgs^  e)
		{
			uncheckObjectSelectionButtons();
			editCameraFxButton->Checked=true;
			refreshActiveObjectList();
		}

		System::Void editTexturesButton_Click(System::Object^  sender, System::EventArgs^  e)
		{
			uncheckObjectSelectionButtons();
			editTexturesButton->Checked=true;
			refreshActiveObjectList();
		}

		/**
		 *  Open project, flushing all existing content from memory
		 */
		bool openProject(String ^path)
		{
			std::string str=utils::makeNativeString(path);
			str+='\\';
			if(globals.project==str)
			{
				//project already loaded
				return true;
			}
			if(!System::IO::Directory::Exists(path))
			{
				//very unlikely, but still need to handle this one
				return false;
			}
			if(globals.project.size())
			{
				//unload all
				globals.clean();
				globals.unloadAll();
			}
			globals.project=str;
			globals.filesystem->setFilesystemRootDirectory(globals.project.c_str());
			//update form title bar
			this->Text="LRStudio ["+path+"]";
			refreshSceneList();
			//enable global project options
			editMeshNodesButton->Enabled=true;
			editSurfacesButton->Enabled=true;
			editCameraFxButton->Enabled=true;
			editTexturesButton->Enabled=true;
			importLWS2MenuItem->Enabled=true;
			importImageMenuItem->Enabled=true;
			return true;
		}

		array<String^>^ getFilesOfTypeUnstripped(String ^mask)
		{
			String^ dirname=utils::makeManagedString(globals.project.c_str());
			if(!System::IO::Directory::Exists(dirname))
			{
				//extremely unlikely
				return nullptr;
			}
			array<String^> ^files=System::IO::Directory::GetFiles(dirname,mask);
			if(files->Length==0)
			{
				return files;
			}
			int i=0;
			for(i=0;i<files->Length;i++)
			{
				files[i]=files[i]->Substring(files[i]->LastIndexOf('\\')+1);
			}
			return files;
		}

		array<String^>^ stripExtensions(array<String^>^ files)
		{
			if(!files)
			{
				return files;
			}
			if(files->Length==0)
			{
				return files;
			}
			int i=0;
			for(i=0;i<files->Length;i++)
			{
				files[i]=files[i]->Substring(0,files[i]->LastIndexOf('.'));
			}
			return files;
		}

		array<String^>^ getFilesOfType(String ^mask)
		{
			return stripExtensions(getFilesOfTypeUnstripped(mask));
		}

		/**
		 *  Refresh left panel containing objects from currently active category, possibly
		 *  filtered by active scene
		 */
		System::Void	refreshActiveObjectList()
		{
			objectListBox->Items->Clear();
			if(globals.scene.isNull())
			{
				return;
			}
			if(editTransformNodesButton->Checked)
			{
				DWORD dwTNCount=globals.scene->getTransformNodeCount();
				DWORD i=0;
				for(;i<dwTNCount;i++)
				{
					LR::TransformPtr node;
					globals.scene->getTransformNodeByID(i,node);
					const char *pszName=node->getObjectName();
					if(pszName)
					{
						objectListBox->Items->Add(utils::makeManagedString(pszName));
					}
				}
				return;
			}
			if(editMeshNodesButton->Checked)
			{
				DWORD dwMNCount=globals.engine->getMeshNodeCount();
				DWORD i=0;
				for(;i<dwMNCount;i++)
				{
					LR::MeshPtr node;
					globals.engine->getMeshNodeByID(i,node);
					const char *pszName=node->getObjectName();
					if(pszName)
					{
						objectListBox->Items->Add(utils::makeManagedString(pszName));
					}
				}
				return;
			}
			if(editTexturesButton->Checked)
			{
				DWORD dwTxCount=globals.engine->getTextureCount();
				DWORD i=0;
				for(;i<dwTxCount;i++)
				{
					LR::TexturePtr tx;
					globals.engine->getTextureByID(i,tx);
					const char *pszName=tx->getObjectName();
					if(pszName)
					{
						objectListBox->Items->Add(utils::makeManagedString(pszName));
					}
				}
				return;
			}
			if(editSurfacesButton->Checked)
			{
				DWORD dwSPBCount=globals.engine->getSurfaceCount();
				DWORD i=0;
				for(;i<dwSPBCount;i++)
				{
					LR::SurfacePtr spb;
					globals.engine->getSurfaceByID(i,spb);
					const char *pszName=spb->getObjectName();
					if(pszName)
					{
						objectListBox->Items->Add(utils::makeManagedString(pszName));
					}
				}
				return;
			}
		}

		/**
		 *  Refresh contents of combo box containing all scenes - neccessary after any
		 *  project wide change (open project, import scene, create scene etc)
		 */
		void			refreshSceneList()
		{
			String ^selection_str=nullptr;
			if(!globals.scene.isNull() && (renderComboBox->SelectedIndex!=-1))
			{
				selection_str=(String^)renderComboBox->Items[renderComboBox->SelectedIndex];
			}
			renderComboBox->Items->Clear();
			array<String^> ^files=getFilesOfType("*.lrsc2");
			if(!files || (files->Length==0))
			{
				renderComboBox->Enabled=false;
				return;
			}
			renderComboBox->Enabled=true;
			for each(String^ str in files)
			{
				renderComboBox->Items->Add(str);
			}
			if(selection_str)
			{
				for each(Object^ obj in renderComboBox->Items)
				{
					if(selection_str->Equals(obj))
					{
						renderComboBox->SelectedIndex=renderComboBox->Items->IndexOf(obj);
					}
				}
			}
		}

		/**
		 *  Refresh camera list for active scene (helper for scene load sequence)
		 *  LR Exception handling is required from caller.
		 */
		System::Void	refreshCameraList()
		{
			cameraComboBox->Items->Clear();
			if(globals.scene.isNull())
			{
				return;
			}
			DWORD dwCameraCount=globals.scene->getCameraTNCount();
			DWORD i=0;
			for(i=0;i<dwCameraCount;i++)
			{
				LR::AutoPtr<LR::Camera> pCam;
				globals.scene->getCameraTNByID(i,pCam);
				cameraComboBox->Items->Add(utils::makeManagedString(pCam->getObjectName()));
			}
			if(dwCameraCount>0)
			{
				cameraComboBox->Enabled=true;
				cameraComboBox->SelectedIndex=0;
				globals.active_camera=cameraComboBox->SelectedIndex;
			}
			else
			{
				cameraComboBox->Enabled=false;
				globals.active_camera=-1;
			}
		}

		System::Void	importLWS2MenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if(openFileDialog->ShowDialog() == SWF::DialogResult::OK)
			{
				LWSImportDescStruct ImportDesc;
				ZeroMemory(&ImportDesc,sizeof(LWSImportDescStruct));
				ImportDesc.bDeformers=false;
				ImportDesc.bMakeTangentSpace=false;
				ImportDesc.bNPatches=false;
				ImportDesc.bOutputGeometry=true;
				ImportDesc.bOutputMaterial=true;
				ImportDesc.bOutputMesh=true;
				ImportDesc.bOutputTextures=true;
				ImportDesc.bOutputScene=true;
				ImportDesc.bOverwriteGeometry=true;
				ImportDesc.bOverwriteImageFiles=true;
				ImportDesc.bOverwriteMaterial=true;
				ImportDesc.bOverwriteMesh=true;
				ImportDesc.bOverwriteTextures=true;
				ImportDesc.bOverwriteSceneFile=true;
				ImportDesc.bRenderable=true;
				ImportDesc.bSpaceWarps=false;
				ImportDesc.dwLockabilityMask=0;
				strcpy(ImportDesc.szFileName,
					utils::makeNativeString(openFileDialog->FileName).c_str());
				strcpy(ImportDesc.szOutputRootDir,
					globals.project.c_str());

				if(!importLWSFile(ImportDesc))
				{
					MessageBox::Show("Failed to import LWS file!!!");
				}
				refreshSceneList();
			}
		}

		/**
		 *  User selected some other object on list - we need to refresh property list
		 */
		System::Void	objectListBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		{
			if(objectListBox->SelectedIndex==-1)
			{
				return;
			}
			if(globals.engine.isNull())
			{
				return;
			}
			String ^managed_obj_name=(String^)objectListBox->SelectedItem;
			std::string obj_name=utils::makeNativeString(managed_obj_name);
			if(editTransformNodesButton->Checked)
			{
				if(globals.scene.isNull())
				{
					return;
				}
				LR::AutoPtr<LR::TransformNode> trn;
				globals.scene->getTransformNodeByName(obj_name.c_str(),trn);
				globals.current_obj.lr_dyn_cast(trn);
				if(!globals.current_obj.isNull())
				{
					globals.current_obj_path_prefix="TN";
				}
			}
			if(editMeshNodesButton->Checked)
			{
				LR::AutoPtr<LR::MeshNode> mn;
				globals.engine->getMeshNodeByName(obj_name.c_str(),mn);
				globals.current_obj.lr_dyn_cast(mn);
				if(!globals.current_obj.isNull())
				{
					globals.current_obj_path_prefix="MN";
				}
			}
			if(editTexturesButton->Checked)
			{
				LR::TexturePtr tx;
				globals.engine->getTextureByName(obj_name.c_str(),tx);
				globals.current_obj.lr_dyn_cast(tx);
				if(!globals.current_obj.isNull())
				{
					globals.current_obj_path_prefix="TX";
				}
			}
			if(editSurfacesButton->Checked)
			{
				LR::SurfacePtr pb;
				globals.engine->getSurfaceByName(obj_name.c_str(),pb);
				globals.current_obj.lr_dyn_cast(pb);
				if(!globals.current_obj.isNull())
				{
					globals.current_obj_path_prefix="SF";
				}
			}
			listObjectProperties(globals.current_obj);
			initSimpleEditors();
		}

		void			listObjectProperties(LR::AutoPtr<LR::BaseObject> &obj)
		{
			if(obj.isNull())
			{
				propertyList->Items->Clear();
				propertyListLabel->Text="Properties (empty)";
				return;
			}
			propertyListLabel->Text=String::Format("Properties ({0:S})",utils::makeManagedString(obj->getObjectName()));
			DWORD dwPropertyCount=obj->getNumProperties();
			int iOriginalCount=propertyList->Items->Count;
			if(iOriginalCount>(int)dwPropertyCount)
			{
				int iRemoveCount=iOriginalCount-(int)dwPropertyCount;
				int i;
				for(i=0;i<iRemoveCount;i++)
				{
					propertyList->Items->RemoveAt((int)dwPropertyCount);
				}
			}
			DWORD dwI=0;
			for(dwI=0;dwI<dwPropertyCount;dwI++)
			{
				LR::AutoPtr<LR::Property> prop;
				obj->getPropertyByID(dwI,prop);
				const char *szName=prop->getPropertyName();
				ListViewItem ^lvi=gcnew ListViewItem(utils::makeManagedString(szName));
				switch(prop->getPropertyType())
				{
					case LR::PT_COLOR:
						{
							LR::ColorPtr col;
							prop->getPropertyValue(&col);
							bool static_color=true;
							LR::EnvelopePtr env;
							int i=0;
							for(i=0;i<4;i++)
							{
								col->getChannel(i,&env);
								if(env->getNumKeys()>1)
								{
									static_color=false;
									break;
								}
							}
							if(static_color)
							{
								lvi->SubItems->Add(String::Format(
									"({0:F2} {1:F2} {2:F2} {3:F2})",
									col->getChannelValue(0,0.0f),
									col->getChannelValue(1,0.0f),
									col->getChannelValue(2,0.0f),
									col->getChannelValue(3,0.0f)));
							}
							else
							{
								lvi->SubItems->Add("(anim)");
							}
							lvi->ImageIndex=5;
						}
						break;
					case LR::PT_ENVELOPE:
						{
							lvi->ImageIndex=0;
							LR::EnvelopePtr pEnv;
							prop->getPropertyValue(&pEnv);
							long lKeys=pEnv->getNumKeys();
							switch(lKeys)
							{
								case 0:
									lvi->SubItems->Add("(0.0000)");
									break;
								case 1:
									lvi->SubItems->Add(String::Format(
										"({0:F4})",pEnv->getKeyValueAtIndex(0)));
									break;
								default:
									lvi->SubItems->Add("(anim)");
									break;
							}
						}
						break;
					case LR::PT_MOTION:
						lvi->SubItems->Add("");
						lvi->ImageIndex=6;
						break;
					case LR::PT_ENUM:
						lvi->SubItems->Add(utils::makeManagedString(
							prop->getEnumChoice(prop->getDWORDPropertyValue())));
						lvi->ImageIndex=10;
						break;
					case LR::PT_NUMERIC:
						switch(prop->getNumericSubType())
						{
							case LR::NPS_INTEGER:
								lvi->SubItems->Add(String::Format(
									"{0:D}",(int)prop->getFloatPropertyValue()));
								break;
							case LR::NPS_FLOAT:
								lvi->SubItems->Add(String::Format(
									"{0:F4}",prop->getFloatPropertyValue()));
								break;
							default:
								lvi->SubItems->Add("TODO!!!!!!!!");
								break;
						}
						lvi->ImageIndex=9;
						break;
					case LR::PT_FILENAME:
					case LR::PT_MESH_NODE:
					case LR::PT_TEXTURE:
					case LR::PT_SURFACE:
					case LR::PT_TRANSFORM_NODE:
						{
							const char *pszName=prop->getStringPropertyValue();
							if(pszName)
							{
								lvi->SubItems->Add(utils::makeManagedString(pszName));
							}
							else
							{
								lvi->SubItems->Add("(none)");
							}
							switch(prop->getPropertyType())
							{
								case LR::PT_TRANSFORM_NODE:
									lvi->ImageIndex=1;
									break;
								case LR::PT_MESH_NODE:
									lvi->ImageIndex=2;
									break;
								case LR::PT_SURFACE:
									lvi->ImageIndex=3;
									break;
								case LR::PT_TEXTURE:
									lvi->ImageIndex=4;
									break;
								case LR::PT_FILENAME:
									lvi->ImageIndex=8;
									break;
							}
						}
						break;
					case LR::PT_ENUM_LIST:
					case LR::PT_ENVELOPE_LIST:
					case LR::PT_NUMERIC_LIST:
					case LR::PT_MESH_NODE_LIST:
					case LR::PT_MOTION_LIST:
					case LR::PT_TRANSFORM_NODE_LIST:
						lvi->SubItems->Add("L");
					default:
						lvi->SubItems->Add("TODO!!!!!!!!");
						break;
				}
				if((int)dwI<iOriginalCount)
				{
					ListViewItem ^dst_lvi=propertyList->Items[(int)dwI];
					dst_lvi->ImageIndex=lvi->ImageIndex;
					if(!dst_lvi->SubItems[0]->ToString()->Equals(lvi->SubItems[0]->ToString()))
					{
						dst_lvi->SubItems[0]=lvi->SubItems[0];
					}
					if(!dst_lvi->SubItems[1]->ToString()->Equals(lvi->SubItems[1]->ToString()))
					{
						dst_lvi->SubItems[1]=lvi->SubItems[1];
					}
				}
				else
				{
					propertyList->Items->Add(lvi);
				}
			}
		}

		System::Void	editScenePropertiesButton_Click(System::Object^  sender, System::EventArgs^  e)
		{
			LR::AutoPtr<LR::BaseObject> obj;
			obj.lr_dyn_cast(globals.scene);
			if(!obj.isNull())
			{
				globals.current_obj_path_prefix="SC";
			}
			listObjectProperties(obj);
		}

		void			getSelectedProperty(LR::AutoPtr<LR::Property> &prop)
		{
			if(propertyList->SelectedItems->Count==0)
			{
				propertyEditorNumeric->Enabled=false;
				propertyEditorChoice->Enabled=false;
				return;
			}
			if(globals.current_obj.isNull())
			{
				return;
			}
			ListViewItem ^lvi=propertyList->SelectedItems[0];
			std::string prop_name=utils::makeNativeString(lvi->SubItems[0]->Text);
			try
			{
				globals.current_obj->getPropertyByName(prop_name.c_str(),prop);
			}
			catch(LR::InvalidParameterException &)
			{
				return;
			}
		}

		System::Void	propertyEditorNumeric_ValueChanged(System::Object^ sender, System::EventArgs^ e)
		{
			LR::AutoPtr<LR::Property> prop;
			getSelectedProperty(prop);
			if(prop.isNull())
			{
				return;
			}
			prop->setPropertyValue(Decimal::ToSingle(propertyEditorNumeric->Value));
			listObjectProperties(globals.current_obj);
		}

		System::Void	propertyEditorChoice_SelectedIndexChanged(System::Object^ sender, System::EventArgs^  e)
		{
			LR::AutoPtr<LR::Property> prop;
			getSelectedProperty(prop);
			if(prop.isNull())
			{
				return;
			}
			switch(prop->getPropertyType())
			{
				case LR::PT_ENUM:
					prop->setPropertyValue((DWORD)propertyEditorChoice->SelectedIndex);
					break;
				case LR::PT_TEXTURE:
					{
						LR::TexturePtr tx;
						if(propertyEditorChoice->SelectedIndex==0)
						{
							prop->setPropertyValue("");
							prop->resolveTexture(tx);
						}
						else
						{
							std::string tx_name=utils::makeNativeString(propertyEditorChoice->SelectedItem->ToString());
							std::string tx_file_name=tx_name+".lrtx";
							try
							{
								globals.engine->loadTexture(globals.filesystem,tx_file_name.c_str(),tx);
								prop->setPropertyValue(tx_name.c_str());
								prop->resolveTexture(tx);
							}
							catch(LR::Exception &)
							{
								//TODO: error info
							}
						}
					}
					break;
				case LR::PT_SURFACE:
					{
						LR::SurfacePtr surf;
						if(propertyEditorChoice->SelectedIndex==0)
						{
							prop->setPropertyValue("");
							prop->resolveSurface(surf);
						}
						else
						{
							std::string surf_name=utils::makeNativeString(propertyEditorChoice->SelectedItem->ToString());
							std::string surf_file_name=surf_name+".lrsh";
							try
							{
								globals.engine->loadSurface(globals.filesystem,surf_file_name.c_str(),surf);
								prop->setPropertyValue(surf_name.c_str());
								prop->resolveSurface(surf);
							}
							catch(LR::Exception &)
							{
								//TODO: error info
							}
						}
					}
					break;
				case LR::PT_TRANSFORM_NODE:
					if(!globals.scene.isNull())
					{
						LR::TransformPtr trn;
						if((propertyEditorChoice->SelectedIndex==0))
						{
							prop->setPropertyValue("");
							prop->resolveTransformNode(trn);
						}
						else
						{
							std::string trn_name=utils::makeNativeString(propertyEditorChoice->SelectedItem->ToString());
							try
							{
								const char *ref_name=globals.current_obj->getObjectName();
								LR::TransformPtr cur_node;
								globals.scene->getTransformNodeByName(trn_name.c_str(),trn);
								cur_node=trn;
								//look for parenting cycle
								bool is_cycle=false;
								while(1)
								{
									if(cur_node.isNull())
									{
										break;
									}
									const char *cur_node_name=cur_node->getObjectName();
									if(strcmp(cur_node_name,ref_name)==0)
									{
										is_cycle=true;
										break;
									}
									const char *parent_name=cur_node->getParentNodeName();
									if((!parent_name) || (strlen(parent_name)==0))
									{
										break;
									}
									//TODO: exception used as control flow statement
									try
									{
										globals.scene->getTransformNodeByName(parent_name,cur_node);
									}
									catch(LR::Exception &)
									{
										break;
										//TODO: error info
									}
								}
								if(!is_cycle)
								{
									prop->setPropertyValue(trn_name.c_str());
									prop->resolveTransformNode(trn);
								}
							}
							catch(LR::Exception &)
							{
								//TODO: error info
							}
						}
					}
					break;
				case LR::PT_MESH_NODE:
					{
						LR::MeshPtr mesh;
						if(propertyEditorChoice->SelectedIndex==0)
						{
							prop->setPropertyValue("");
							prop->resolveMeshNode(mesh);
						}
						else
						{
							std::string mesh_name=utils::makeNativeString(propertyEditorChoice->SelectedItem->ToString());
							std::string mesh_file_name=mesh_name+".lrmh";
							try
							{
								globals.engine->loadMesh(globals.filesystem,mesh_file_name.c_str(),mesh);
								prop->setPropertyValue(mesh_name.c_str());
								prop->resolveMeshNode(mesh);
							}
							catch(LR::Exception &)
							{
								//TODO: error info
							}
						}
					}
					break;
				case LR::PT_FILENAME:
					if(propertyEditorChoice->SelectedIndex==0)
					{
						prop->setPropertyValue("");
					}
					else
					{
						std::string file_name=utils::makeNativeString(propertyEditorChoice->SelectedItem->ToString());
						prop->setPropertyValue(file_name.c_str());
					}
					break;
			}
			listObjectProperties(globals.current_obj);
		}


		void			initSimpleEditors()
		{
			LR::AutoPtr<LR::Property> prop;
			getSelectedProperty(prop);
			if(prop.isNull())
			{
				return;
			}
			DWORD dwEnumChoiceCount=0;
			DWORD dwI=0;
			switch(prop->getPropertyType())
			{
				case LR::PT_NUMERIC:
					propertyEditorNumeric->Enabled=true;
					propertyEditorChoice->Enabled=false;
					if(prop->getNumericSubType()==LR::NPS_FLOAT)
					{
						propertyEditorNumeric->DecimalPlaces=4;
					}
					else
					{
						propertyEditorNumeric->DecimalPlaces=0;
					}
					propertyEditorNumeric->Minimum=-100000;
					propertyEditorNumeric->Maximum=100000;
					propertyEditorNumeric->Value=Decimal(prop->getFloatPropertyValue());
					break;
				case LR::PT_ENUM:
					propertyEditorNumeric->Enabled=false;
					propertyEditorChoice->Enabled=true;
					propertyEditorChoice->Items->Clear();
					dwEnumChoiceCount=prop->getEnumChoiceCount();
					for(dwI=0;dwI<dwEnumChoiceCount;dwI++)
					{
						propertyEditorChoice->Items->Add(utils::makeManagedString(prop->getEnumChoice(dwI)));
					}
					propertyEditorChoice->SelectedIndex=prop->getDWORDPropertyValue();
					break;
				case LR::PT_TEXTURE:
					propertyEditorNumeric->Enabled=false;
					propertyEditorChoice->Enabled=true;
					propertyEditorChoice->Items->Clear();
					propertyEditorChoice->Items->Add("(none)");
					{
						String ^selection_str=utils::makeManagedString(prop->getStringPropertyValue());
						int selected_obj=0;
						array<String^> ^textures=getFilesOfType("*.lrtx");
						if(textures)
						{
							int i=0;
							for(i=0;i<textures->Length;i++)
							{
								String ^tx=textures[i];
								propertyEditorChoice->Items->Add(tx);
								if(selection_str && tx->Equals(selection_str))
								{
									selected_obj=i+1;
								}
							}
						}
						propertyEditorChoice->SelectedIndex=selected_obj;
					}
					break;
				case LR::PT_SURFACE:
					propertyEditorNumeric->Enabled=false;
					propertyEditorChoice->Enabled=true;
					propertyEditorChoice->Items->Clear();
					propertyEditorChoice->Items->Add("(none)");
					{
						String ^selection_str=utils::makeManagedString(prop->getStringPropertyValue());
						int selected_obj=0;
						array<String^> ^textures=getFilesOfType("*.lrsh");
						if(textures)
						{
							int i=0;
							for(i=0;i<textures->Length;i++)
							{
								String ^tx=textures[i];
								propertyEditorChoice->Items->Add(tx);
								if(selection_str && tx->Equals(selection_str))
								{
									selected_obj=i+1;
								}
							}
						}
						propertyEditorChoice->SelectedIndex=selected_obj;
					}
					break;
				case LR::PT_TRANSFORM_NODE:
					propertyEditorNumeric->Enabled=false;
					propertyEditorChoice->Enabled=true;
					propertyEditorChoice->Items->Clear();
					propertyEditorChoice->Items->Add("(none)");
					if(!globals.scene.isNull())
					{
						const char *pszCurrentValue=prop->getStringPropertyValue();
						int selected_obj=0;
						DWORD tn_count=globals.scene->getTransformNodeCount();
						DWORD i=0;
						for(i=0;i<tn_count;i++)
						{
							LR::TransformPtr trn;
							globals.scene->getTransformNodeByID(i,trn);
							const char *pszNodeName=trn->getObjectName();
							propertyEditorChoice->Items->Add(utils::makeManagedString(pszNodeName));
							if(pszCurrentValue && pszNodeName && (strcmp(pszCurrentValue,pszNodeName)==0))
							{
								selected_obj=i+1;
							}
						}
						propertyEditorChoice->SelectedIndex=selected_obj;
					}
					break;
				case LR::PT_MESH_NODE:
					propertyEditorNumeric->Enabled=false;
					propertyEditorChoice->Enabled=true;
					propertyEditorChoice->Items->Clear();
					propertyEditorChoice->Items->Add("(none)");
					{
						String ^selection_str=utils::makeManagedString(prop->getStringPropertyValue());
						int selected_obj=0;
						array<String^> ^meshes=getFilesOfType("*.lrmh");
						if(meshes)
						{
							int i=0;
							for(i=0;i<meshes->Length;i++)
							{
								String ^mh=meshes[i];
								propertyEditorChoice->Items->Add(mh);
								if(selection_str && mh->Equals(selection_str))
								{
									selected_obj=i+1;
								}
							}
						}
						propertyEditorChoice->SelectedIndex=selected_obj;
					}
					break;
				case LR::PT_FILENAME:
					propertyEditorNumeric->Enabled=false;
					propertyEditorChoice->Enabled=true;
					propertyEditorChoice->Items->Clear();
					propertyEditorChoice->Items->Add("(none)");
					{
						String ^selection_str=utils::makeManagedString(prop->getStringPropertyValue());
						String ^ext_string=utils::makeManagedString(prop->getFileNameExtension());
						String ^ext_string_pure=nullptr;
						if(ext_string)
						{
							int left_delimiter=ext_string->IndexOf('|');
							if((left_delimiter>=0) && (left_delimiter<ext_string->Length-1))
							{
								int right_delimiter=ext_string->IndexOf('|',left_delimiter+1);
								if(right_delimiter>=0)
								{
									ext_string_pure=ext_string->Substring(
										left_delimiter+1,right_delimiter-left_delimiter-1);
								}
							}
						}
						int item_id=0;
						int selected_obj=0;
						if(ext_string_pure)
						{
							array<String^>^ ext_list=ext_string_pure->Split(';');
							int i=0;
							for(i=0;i<ext_list->Length;i++)
							{
								array<String^>^ files=getFilesOfTypeUnstripped(ext_list[i]);
								if(!files)
								{
									continue;
								}
								int j=0;
								for(j=0;j<files->Length;j++)
								{
									propertyEditorChoice->Items->Add(files[j]);
									item_id++;
									if(selection_str && files[j]->Equals(selection_str))
									{
										selected_obj=item_id;
									}
								}
							}
						}
						propertyEditorChoice->SelectedIndex=selected_obj;
					}
					break;
				default:
					propertyEditorNumeric->Enabled=false;
					propertyEditorChoice->Enabled=false;
					break;
			}
		}

		System::Void	propertyList_SelectedIndexChanged(System::Object^ sender,
			System::EventArgs^ e)
		{
			initSimpleEditors();
			//check for complex properties
			LR::AutoPtr<LR::Property> prop;
			getSelectedProperty(prop);
			if(prop.isNull())
			{
				return;
			}
			DWORD dwEnumChoiceCount=0;
			DWORD dwI=0;
			switch(prop->getPropertyType())
			{
				case LR::PT_ENVELOPE:
					{
						LR::EnvelopePtr env;
						prop->getPropertyValue(&env);
						envelopeEditor->EnvelopeObj=utils::copyNativeEnvelope(env);
						globals.current_env=env;
						globals.channel_id=ecStandalone;
						String ^tab_name=utils::makeManagedString(globals.current_obj_path_prefix.c_str());
						tab_name+="/";
						tab_name+=utils::makeManagedString(globals.current_obj->getObjectName());
						tab_name+="/";
						tab_name+=utils::makeManagedString(prop->getPropertyName());
						envelopeEditorTabPage->Text=tab_name;
						envelopeChannelComboBox->Items->Clear();
						envelopeChannelComboBox->Items->Add("main");
						envelopeChannelComboBox->SelectedIndex=0;
					}
					break;
				case LR::PT_MOTION:
					{
						LR::MotionPtr mot;
						prop->getPropertyValue(&mot);
						globals.current_motion=mot;
						globals.channel_id=ecMotion;
						LR::EnvelopePtr env;
						mot->getChannel(0,&env);
						envelopeEditor->EnvelopeObj=utils::copyNativeEnvelope(env);
						globals.current_env=env;
						String ^tab_name=utils::makeManagedString(globals.current_obj_path_prefix.c_str());
						tab_name+="/";
						tab_name+=utils::makeManagedString(globals.current_obj->getObjectName());
						tab_name+="/";
						tab_name+=utils::makeManagedString(prop->getPropertyName());
						envelopeEditorTabPage->Text=tab_name;
						envelopeChannelComboBox->Items->Clear();
						envelopeChannelComboBox->Items->Add("PositionX");
						envelopeChannelComboBox->Items->Add("PositionY");
						envelopeChannelComboBox->Items->Add("PositionZ");
						envelopeChannelComboBox->Items->Add("Heading");
						envelopeChannelComboBox->Items->Add("Pitch");
						envelopeChannelComboBox->Items->Add("Bank");
						envelopeChannelComboBox->Items->Add("ScaleX");
						envelopeChannelComboBox->Items->Add("ScaleY");
						envelopeChannelComboBox->Items->Add("ScaleZ");
						envelopeChannelComboBox->SelectedIndex=0;
					}
					break;
				case LR::PT_COLOR:
					{
						LR::ColorPtr col;
						prop->getPropertyValue(&col);
						globals.current_color=col;
						globals.channel_id=ecColor;
						LR::EnvelopePtr env;
						col->getChannel(0,&env);
						envelopeEditor->EnvelopeObj=utils::copyNativeEnvelope(env);
						globals.current_env=env;
						String ^tab_name=utils::makeManagedString(globals.current_obj_path_prefix.c_str());
						tab_name+="/";
						tab_name+=utils::makeManagedString(globals.current_obj->getObjectName());
						tab_name+="/";
						tab_name+=utils::makeManagedString(prop->getPropertyName());
						envelopeEditorTabPage->Text=tab_name;
						envelopeChannelComboBox->Items->Clear();
						envelopeChannelComboBox->Items->Add("Red");
						envelopeChannelComboBox->Items->Add("Green");
						envelopeChannelComboBox->Items->Add("Blue");
						envelopeChannelComboBox->Items->Add("Alpha");
						envelopeChannelComboBox->SelectedIndex=0;
					}
					break;
			}
		}

		System::Void	importImageMenuItem_Click(System::Object^ sender,System::EventArgs^ e)
		{
			if(openImageFileDialog->ShowDialog() == SWF::DialogResult::OK)
			{
				importFileToProject(
					utils::makeNativeString(openImageFileDialog->FileName).c_str(),
					false);
			}
		}

		System::Void	newObjectButton_Click(System::Object^ sender,System::EventArgs^ e)
		{
			if(globals.engine.isNull())
			{
				return;
			}
			NewObjectForm ^nof=gcnew NewObjectForm();
			if(editTransformNodesButton->Checked)
			{
				DWORD dwTNClass=0;
				if(!globals.engine->getFirstTransformNodeClass(&dwTNClass))
				{
					return;
				}
				System::Collections::ArrayList ^arr=gcnew System::Collections::ArrayList();
				do
				{
					arr->Add(utils::makeManagedString(globals.engine->getSuperClassName(dwTNClass))+
						"/"+utils::makeManagedString(globals.engine->getClassName(dwTNClass)));
				}
				while(globals.engine->getNextTransformNodeClass(dwTNClass,&dwTNClass));
				nof->ClassNameList=(array<String^>^)arr->ToArray(String::typeid);
				arr=gcnew System::Collections::ArrayList();
				if(!globals.scene.isNull())
				{
					DWORD dwTNCount=globals.scene->getTransformNodeCount();
					DWORD i=0;
					for(i=0;i<dwTNCount;i++)
					{
						LR::TransformPtr node;
						globals.scene->getTransformNodeByID(i,node);
						arr->Add(utils::makeManagedString(node->getObjectName()));
					}
				}
				nof->ExistingObjectNameList=(array<String^>^)arr->ToArray(String::typeid);
			}
			else if(editMeshNodesButton->Checked)
			{
				DWORD dwMeshClass=0;
				if(!globals.engine->getFirstMeshNodeClass(&dwMeshClass))
				{
					return;
				}
				System::Collections::ArrayList ^arr=gcnew System::Collections::ArrayList();
				do
				{
					arr->Add(utils::makeManagedString(globals.engine->getSuperClassName(dwMeshClass))+
						"/"+utils::makeManagedString(globals.engine->getClassName(dwMeshClass)));
				}
				while(globals.engine->getNextMeshNodeClass(dwMeshClass,&dwMeshClass));
				nof->ClassNameList=(array<String^>^)arr->ToArray(String::typeid);
				arr=gcnew System::Collections::ArrayList();
				DWORD dwMNCount=globals.engine->getMeshNodeCount();
				DWORD i=0;
				for(i=0;i<dwMNCount;i++)
				{
					LR::MeshPtr node;
					globals.engine->getMeshNodeByID(i,node);
					arr->Add(utils::makeManagedString(node->getObjectName()));
				}
				nof->ExistingObjectNameList=(array<String^>^)arr->ToArray(String::typeid);
			}
			else if(editTexturesButton->Checked)
			{
				DWORD dwTxClass=0;
				if(!globals.engine->getFirstTextureClass(&dwTxClass))
				{
					return;
				}
				System::Collections::ArrayList ^arr=gcnew System::Collections::ArrayList();
				do
				{
					arr->Add(utils::makeManagedString(globals.engine->getSuperClassName(dwTxClass))+
						"/"+utils::makeManagedString(globals.engine->getClassName(dwTxClass)));
				}
				while(globals.engine->getNextTextureClass(dwTxClass,&dwTxClass));
				nof->ClassNameList=(array<String^>^)arr->ToArray(String::typeid);
				arr=gcnew System::Collections::ArrayList();
				DWORD dwTXCount=globals.engine->getTextureCount();
				DWORD i=0;
				for(i=0;i<dwTXCount;i++)
				{
					LR::TexturePtr node;
					globals.engine->getTextureByID(i,node);
					arr->Add(utils::makeManagedString(node->getObjectName()));
				}
				nof->ExistingObjectNameList=(array<String^>^)arr->ToArray(String::typeid);
			}
			else if(editSurfacesButton->Checked)
			{
				DWORD dwShClass=0;
				if(!globals.engine->getFirstShaderClass(&dwShClass))
				{
					return;
				}
				System::Collections::ArrayList ^arr=gcnew System::Collections::ArrayList();
				do
				{
					arr->Add(utils::makeManagedString(globals.engine->getSuperClassName(dwShClass))+
						"/"+utils::makeManagedString(globals.engine->getClassName(dwShClass)));
				}
				while(globals.engine->getNextShaderClass(dwShClass,&dwShClass));
				nof->ClassNameList=(array<String^>^)arr->ToArray(String::typeid);
				arr=gcnew System::Collections::ArrayList();
				DWORD dwSFCount=globals.engine->getSurfaceCount();
				DWORD i=0;
				for(i=0;i<dwSFCount;i++)
				{
					LR::SurfacePtr node;
					globals.engine->getSurfaceByID(i,node);
					arr->Add(utils::makeManagedString(node->getObjectName()));
				}
				nof->ExistingObjectNameList=(array<String^>^)arr->ToArray(String::typeid);
			}
			else
			{
				return;
			}
			if(nof->ShowDialog()!=SWF::DialogResult::OK)
			{
				return;
			}
			array<String^> ^arr=nof->SelectedClass->Split('/');
			if(!arr || (arr->Length!=2))
			{
				return;
			}
			std::string obj_superclass=utils::makeNativeString(arr[0]);
			std::string obj_class=utils::makeNativeString(arr[1]);
			if(editTransformNodesButton->Checked)
			{
				LR::TransformPtr pNode;
				globals.engine->createTransformNode(obj_class.c_str(),obj_superclass.c_str(),pNode);
				pNode->setObjectName(utils::makeNativeString(nof->SelectedObjectName).c_str());
				globals.scene->addTransformNode(pNode);
			}
			else if(editMeshNodesButton->Checked)
			{
				LR::MeshPtr pNode;
				LR::MeshCreateStruct MeshCreateStruct;
				ZeroMemory(&MeshCreateStruct,sizeof(LR::MeshCreateStruct));
				MeshCreateStruct.bRenderable=true;
				strcpy(MeshCreateStruct.szClass,obj_class.c_str());
				strcpy(MeshCreateStruct.szSurface,"");
				std::string MeshName=utils::makeNativeString(nof->SelectedObjectName);
				MeshName+=".lrmh";
				strcpy(MeshCreateStruct.szMeshFileName,MeshName.c_str());
				strcpy(MeshCreateStruct.szSuperClass,obj_superclass.c_str());
				try
				{
					globals.engine->createMesh(globals.filesystem,&MeshCreateStruct,pNode);
				}
				catch(LR::Exception &)
				{
					//TODO
					MessageBox::Show("Something went terribly wrong :(",
						"Exception occured when trying to create mesh",MessageBoxButtons::OK,
						MessageBoxIcon::Exclamation);
				}
			}
			else if(editTexturesButton->Checked)
			{
				LR::TexturePtr pNode;
				std::string obj_file_name=utils::makeNativeString(nof->SelectedObjectName);
				obj_file_name+=".lrtx";
				globals.engine->createTexture(globals.filesystem,obj_class.c_str(),obj_superclass.c_str(),
					obj_file_name.c_str(),pNode);
			}
			else if(editSurfacesButton->Checked)
			{
				LR::ShaderPtr			pTmpShader=NULL;
				LR::SurfacePtr			pNode=NULL;
				globals.engine->createShader(obj_class.c_str(),obj_superclass.c_str(),pTmpShader);
				pTmpShader->createSurface(utils::makeNativeString(nof->SelectedObjectName).c_str(),pNode);
			}			
			refreshActiveObjectList();
		}

		void			syncMouseModeButtons(LRControls::EnvelopeEditor::eMouseMode new_mouse_mode)
		{
			envelopeEditModeAddDragButton->Checked=
				(new_mouse_mode==LRControls::EnvelopeEditor::eMouseMode::AddDrag);
			envelopeEditModeRemoveButton->Checked=
				(new_mouse_mode==LRControls::EnvelopeEditor::eMouseMode::Remove);
			envelopeEditModeZoomButton->Checked=
				(new_mouse_mode==LRControls::EnvelopeEditor::eMouseMode::Zoom);
			envelopeEditModePanButton->Checked=
				(new_mouse_mode==LRControls::EnvelopeEditor::eMouseMode::Pan);
		}

		System::Void	envelopeEditor_MouseModeChanged(LRControls::EnvelopeEditor::eMouseMode new_mouse_mode)
		{
			syncMouseModeButtons(new_mouse_mode);
		}

		System::Void	envelopeEditModeAddDragButton_Click(System::Object^ sender,System::EventArgs^ e)
		{
			envelopeEditor->MouseMode=LRControls::EnvelopeEditor::eMouseMode::AddDrag;
			syncMouseModeButtons(envelopeEditor->MouseMode);
		}

		System::Void	envelopeEditModeRemoveButton_Click(System::Object^ sender,System::EventArgs^ e)
		{
			envelopeEditor->MouseMode=LRControls::EnvelopeEditor::eMouseMode::Remove;
			syncMouseModeButtons(envelopeEditor->MouseMode);
		}

		System::Void	envelopeEditModePanButton_Click(System::Object^ sender,System::EventArgs^ e)
		{
			envelopeEditor->MouseMode=LRControls::EnvelopeEditor::eMouseMode::Pan;
			syncMouseModeButtons(envelopeEditor->MouseMode);
		}

		System::Void	envelopeEditModeZoomButton_Click(System::Object^ sender,System::EventArgs^ e)
		{
			envelopeEditor->MouseMode=LRControls::EnvelopeEditor::eMouseMode::Zoom;
			syncMouseModeButtons(envelopeEditor->MouseMode);
		}

		void invalidateSceneTransformCaches()
		{
			if(!globals.scene.isNull())
			{
				DWORD dwTNCount=globals.scene->getTransformNodeCount();
				DWORD i=0;
				for(;i<dwTNCount;i++)
				{
					LR::TransformPtr trn;
					globals.scene->getTransformNodeByID(i,trn);
					trn->invalidateTransformCache();
				}
			}
		}

		System::Void	envelopeEditor_KeyChanged(System::Int32 index)
		{
			if(!globals.current_env)
			{
				return;
			}
			LRControls::EnvelopeKey ^key=envelopeEditor->EnvelopeObj->Keys[index];
			globals.current_env->setKeyValueAtIndex(index,key->Value);
			globals.current_env->setKeyPositionAtIndex(index,key->Position);
			if(globals.channel_id==ecMotion)
			{
				invalidateSceneTransformCaches();
			}
			if(!globals.current_obj.isNull())
			{
				listObjectProperties(globals.current_obj);
			}
		}

		System::Void	envelopeEditor_EnvelopeChanged()
		{
			if(!globals.current_env)
			{
				return;
			}
			utils::copyManagedEnvelope(envelopeEditor->EnvelopeObj,globals.current_env);
			if(globals.channel_id==ecMotion)
			{
				invalidateSceneTransformCaches();
			}
			if(!globals.current_obj.isNull())
			{
				listObjectProperties(globals.current_obj);
			}
		}


		System::Void	envelopeChannelComboBox_SelectedIndexChanged(System::Object^  sender,System::EventArgs^  e)
		{
			switch(globals.channel_id)
			{
				case ecMotion:
					{
						if(!globals.current_motion)
						{
							break;
						}
						LR::EnvelopePtr env;
						globals.current_motion->getChannel(
							envelopeChannelComboBox->SelectedIndex,&env);
						globals.current_env=env;
						envelopeEditor->EnvelopeObj=utils::copyNativeEnvelope(env);
					}
					break;
				case ecColor:
					{
						if(!globals.current_color)
						{
							break;
						}
						LR::EnvelopePtr env;
						globals.current_color->getChannel(
							envelopeChannelComboBox->SelectedIndex,&env);
						globals.current_env=env;
						envelopeEditor->EnvelopeObj=utils::copyNativeEnvelope(env);
					}
					break;
			}
		}
		System::Void	saveActiveObjectBtn_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if(globals.current_obj.isNull())
			{
				return;
			}
			DWORD dwObjectCategory=globals.current_obj->getObjectCategory();
			switch(dwObjectCategory)
			{
				case LR::OBJECT_TYPE_MESH_NODE:
					{
						LR::MeshPtr cast_obj;
						cast_obj.lr_dyn_cast(globals.current_obj);
						globals.engine->saveMesh(globals.filesystem,cast_obj);
					}
					break;
				case LR::OBJECT_TYPE_TRANSFORM_NODE:
					//do nothing, they are saved along with scene
					//TODO: save button shouldn't even be active!!!
					break;
				case LR::OBJECT_TYPE_TEXTURE:
					{
						LR::TexturePtr cast_obj;
						cast_obj.lr_dyn_cast(globals.current_obj);
						globals.engine->saveTexture(globals.filesystem,cast_obj);
					}
					break;
				case LR::OBJECT_TYPE_SURFACE:
					{
						LR::SurfacePtr cast_obj;
						cast_obj.lr_dyn_cast(globals.current_obj);
						globals.engine->saveSurface(globals.filesystem,cast_obj);
					}
					break;
				case LR::OBJECT_TYPE_SCENE:
					{
						LR::ScenePtr cast_obj;
						cast_obj.lr_dyn_cast(globals.current_obj);
						globals.engine->saveScene(globals.filesystem,cast_obj);
					}
					break;
			}
		}

		System::Void	saveSceneButton_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if(globals.scene.isNull())
			{
				return;
			}
			globals.engine->saveScene(globals.filesystem,globals.scene);
		}
	};
}
