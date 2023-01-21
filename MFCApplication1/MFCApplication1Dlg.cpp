
// MFCApplication1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include <vector>
#include <algorithm>
using std::min;
using std::max;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CImage imagesrc;
CImage imagedes;
using namespace std;
vector<vector<pix>>bitmap;
vector<vector<pix>>img_log[10];
byte log_head, log_tail=0;
vector<vector<pix>>bitmap2;
vector<string>Log;
//typedef unsigned char* byte*;
byte* psrc; byte* pdes;
int pitch, width, height;
byte bitcnt, operation;
double ratio;

void backup_push(vector<vector<pix>>& map)
{
	img_log[log_tail] = map;
	log_tail=(log_tail+1)%10;
	if(log_tail==log_head)
		log_head = (log_head + 1) % 10;
}
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框




CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
	, imgPath(_T(""))
	, grade_sil_value(0)
	, range_sli_value(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	imgPath = _T("");
	imgName = _T("");
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_img, image);
	DDX_Control(pDX, IDC_img, img);
	//  DDX_Control(pDX, open_BUTTON, imgPath);
	DDX_Control(pDX, IDC_SLIDER1, grade_sli);
	DDX_Slider(pDX, IDC_SLIDER1, grade_sil_value);
	DDX_Control(pDX, IDC_SLIDER2, range_sli);
	DDX_Slider(pDX, IDC_SLIDER2, range_sli_value);
	DDX_Control(pDX, pos_text, PosText);
	DDX_Control(pDX, log_text, LogText);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(open_BUTTON, &CMFCApplication1Dlg::OnBnClickedOpenButton)
	ON_BN_CLICKED(satura_BUTTON, &CMFCApplication1Dlg::OnBnClickedSaturaButton)
	ON_BN_CLICKED(vague_BUTTON, &CMFCApplication1Dlg::OnBnClickedVagueButton)
	ON_BN_CLICKED(light_BUTTON, &CMFCApplication1Dlg::OnBnClickedLightButton)
	ON_BN_CLICKED(choose_BUTTON, &CMFCApplication1Dlg::OnBnClickedChooseButton)
	ON_BN_CLICKED(origin_BUTTON, &CMFCApplication1Dlg::OnBnClickedOriginButton)
	ON_BN_CLICKED(save_BUTTON, &CMFCApplication1Dlg::OnBnClickedSaveButton)
	ON_BN_CLICKED(undo_BUTTON, &CMFCApplication1Dlg::OnBnClickedUndoButton)
	ON_BN_CLICKED(redo_BUTTON, &CMFCApplication1Dlg::OnBnClickedRedoButton)
	ON_BN_CLICKED(mosaic_BUTTON, &CMFCApplication1Dlg::OnBnClickedMosaicButton)
	ON_BN_CLICKED(gray_BUTTON, &CMFCApplication1Dlg::OnBnClickedGrayButton)
	ON_BN_CLICKED(pen_BUTTON, &CMFCApplication1Dlg::OnBnClickedPenButton)
	ON_STN_CLICKED(IDC_img, &CMFCApplication1Dlg::OnStnClickedimg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CMFCApplication1Dlg::OnNMCustomdrawSlider2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CMFCApplication1Dlg::OnNMCustomdrawSlider1)
	ON_STN_CLICKED(IDC_SLIDER1_text, &CMFCApplication1Dlg::OnStnClickedSlider1text)
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	slider_ini();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CMFCApplication1Dlg::OnSize(UINT nType, int cx, int cy)
{
	//CDialogEx::OnSize(nType, cx, cy);
	if (nType != SIZE_MINIMIZED)  //判断窗口是不是最小化了，因为窗口最小化之后 ，窗口的长和宽会变成0，当前一次变化的时就会出现除以0的错误操作
	{
		if(pdes!=NULL)
		display(imagedes);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCApplication1Dlg::OnBnClickedOpenButton()//open
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fileDlg(TRUE, _T("png"), NULL, 0, _T("image Files(*.bmp; *.jpg;*.png)|*.JPG;*.PNG;*.BMP|All Files (*.*) |*.*|"), this);
	fileDlg.DoModal();
	imgPath = fileDlg.GetPathName();		//文件路径
	imgName = fileDlg.GetFileName();	//文件名
	if (imgPath == _T(""))
	{
		return;
	}
	if (!imagesrc.IsNull())
	{
		imagesrc.Destroy();
		imagedes.Destroy();
		bitmap.clear();
		bitmap2.clear();
	}

	imagesrc.Load(imgPath);
	psrc = (byte*)imagesrc.GetBits();
	pitch = imagesrc.GetPitch();
	bitcnt = imagesrc.GetBPP()/8;
	height = imagesrc.GetHeight();
	width = imagesrc.GetWidth();
	ratio = (double)width / height;
	//ini(imagesrc);
	img2bitmap(psrc,bitmap);
	backup_push(bitmap);
	imgcopy((CImage*)psrc,imagedes);
	pdes = (byte*)imagedes.GetBits();

	display(imagesrc);

}
void CMFCApplication1Dlg::display(CImage& image)
{
	GetDlgItem(IDC_img)->ShowWindow(FALSE);
	GetDlgItem(IDC_img)->ShowWindow(TRUE);
	CRect rectControl;                        //控件矩形对象
	img.GetClientRect(rectControl);
	CDC* pDc = img.GetDC();			 //设备上下文对象的类
	int W = rectControl.Width(), H = rectControl.Height();
	if ((double)W / H > ratio) {
		int w = ratio * H;
		rectControl = CRect((W - w) / 2, 0, (W + w) / 2, H);
	}
	else {
		int h = W / ratio;
		rectControl = CRect(0,(H - h) / 2, W, (H + h) / 2);
	}
	//rectControl = CRect(rectControl.TopLeft(), CSize((int)rectControl.Width(), (int)rectControl.Height()));
	img.SetBitmap(NULL);				//清空picture
	image.Draw(pDc->m_hDC, rectControl);    //将图片绘制到Picture控件表示的矩形区域
	//image.Destroy();
	img.ReleaseDC(pDc);
}

void CMFCApplication1Dlg::OnBnClickedSaturaButton()//饱和度
{
	if(operation!=0&&operation!=16&&operation!=2&&operation != 32){
		error(5);
		return;
	}
	if (psrc == NULL) {
		error(1); return;
	}
	GetDlgItem(IDC_SLIDER1)->ShowWindow(TRUE);
	GetDlgItem(IDC_SLIDER1_text)->ShowWindow(TRUE);
	
	static  bool op = 1;
	if (op) {
	operation = 2;
		bitmap2 = bitmap;
		grade_sli.SetPos(50);//滚动条初始位置
	}
	else {
		operation=0;
		bitmap = bitmap2;
		bitmap2img(pdes, bitmap);
		backup_push(bitmap);

		stringstream ss;
		streambuf* buffer = cout.rdbuf(); //oldbuffer,STDOUT的缓冲区
		cout.rdbuf(ss.rdbuf());
		cout << "改变饱和度: S*" << (grade_sil_value + 50) / 100.0 << "\n";
		string s(ss.str());
		cout.rdbuf(buffer); // 重置，重新载入STDOUT的缓冲区
		Log.push_back(s);
		showlog();
		GetDlgItem(IDC_SLIDER1)->ShowWindow(FALSE);
		GetDlgItem(IDC_SLIDER1_text)->ShowWindow(FALSE);
		GetDlgItem(IDC_SLIDER1)->ShowWindow(FALSE);
		GetDlgItem(IDC_SLIDER1_text)->ShowWindow(FALSE);
	}
	op = !op;
}


void CMFCApplication1Dlg::OnBnClickedVagueButton()
{
	if(operation!=0&&operation!=16&&operation!=4&&operation != 32){
		error(5);
		return;
	}
	if (psrc == NULL) {
		error(1); return;
	}
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_SLIDER1)->ShowWindow(TRUE);
	GetDlgItem(IDC_SLIDER1_text)->ShowWindow(TRUE);

	GetDlgItem(IDC_SLIDER2)->ShowWindow(TRUE);
	GetDlgItem(IDC_SLIDER2_text)->ShowWindow(TRUE);
	static  bool op = 1;
	if (op) {
		operation=4;
		bitmap2 = bitmap;
		grade_sli.SetPos(100);//滚动条初始位置
	}
	else {
		operation = 0;
		bitmap = bitmap2;
		bitmap2img(pdes, bitmap);
		backup_push(bitmap);

		grade_sil_value = grade_sli.GetPos();
		stringstream ss;
		streambuf* buffer = cout.rdbuf(); //oldbuffer,STDOUT的缓冲区
		cout.rdbuf(ss.rdbuf());
		cout << "高斯模糊: 半径:" << grade_sil_value / 20.0 << "\n";
		string s(ss.str());
		cout.rdbuf(buffer); // 重置，重新载入STDOUT的缓冲区
		Log.push_back(s);
		showlog();
	}
	op = !op;
}


void CMFCApplication1Dlg::OnBnClickedLightButton()//曝光度
{
	if(operation!=0&&operation!=16&&operation!=1&&operation!=32){
		error(5);
		return;
	}
	if (psrc == NULL) {
		error(1); return;
	}
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_SLIDER1)->ShowWindow(TRUE);
	GetDlgItem(IDC_SLIDER1_text)->ShowWindow(TRUE);
	
	//change_light(100,bitmap);
	//display(imagedes);
	static  bool op = 1;
	if (op) {
		operation = 1;
		bitmap2 = bitmap;
		grade_sli.SetPos(50);//滚动条初始位置
	}
	else {
		operation = 0;
		bitmap = bitmap2;
		bitmap2img(pdes, bitmap);
		backup_push(bitmap);
		grade_sil_value = grade_sli.GetPos();


		stringstream ss;
		streambuf* buffer = cout.rdbuf(); //oldbuffer,STDOUT的缓冲区
		cout.rdbuf(ss.rdbuf());
		cout << "改变亮度: H*" << (grade_sil_value + 50) / 100.0 << "\n";
		string s(ss.str());
		cout.rdbuf(buffer); // 重置，重新载入STDOUT的缓冲区
		Log.push_back(s);
		showlog();

		GetDlgItem(IDC_SLIDER1)->ShowWindow(FALSE);
		GetDlgItem(IDC_SLIDER1_text)->ShowWindow(FALSE);
	}
	op = !op;
}


void CMFCApplication1Dlg::OnBnClickedChooseButton()
{
	if(operation!=0&&operation!=16){
		error(5);
		return;
	}
	if (psrc == NULL) {
		error(1); return;
	}
	operation=16;
	//AfxMessageBox(_T("请沿左上至右下框选！"));
	// TODO: 在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnBnClickedOriginButton()//显示原图
{
	if (psrc == NULL) {
		error(1); return;
	}
	static bool a = 1;
	if (a) display(imagesrc);
	else display(imagedes);

	stringstream ss;
	streambuf* buffer = cout.rdbuf(); //oldbuffer,STDOUT的缓冲区
	cout.rdbuf(ss.rdbuf());
	if (a)cout << "显示原图" << "\n";
	else cout << "取消显示原图\n";
	string s(ss.str());
	cout.rdbuf(buffer); // 重置，重新载入STDOUT的缓冲区
	Log.push_back(s);
	showlog();

	a = !a;
}


void CMFCApplication1Dlg::OnBnClickedSaveButton()
{
	if (psrc == NULL) {
		error(1); return;
	}
	// TODO: 在此添加控件通知处理程序代码
	
	CString saveFilePath;
	{
		CString  strFilter = _T("位图文件(*.bmp)|*.bmp|JPEG 图像文件|*.jpg|GIF图像文件 |*.gif| PNG图像文件 |*.png|其他格式(*.*)| *.* ||");
		CFileDialog dlg(FALSE, _T("bmp"), _T("Picture01.bmp"), NULL, strFilter);
		if (dlg.DoModal() != IDOK)
		return;
		CString strFileName; //文件扩展名
		CString strExtension;
		strFileName = dlg.m_ofn.lpstrFile;
		if (dlg.m_ofn.nFileExtension == 0)               
		{
			switch (dlg.m_ofn.nFilterIndex)
			{
			case 1:
				strExtension = "bmp"; break;
			case 2:
				strExtension = "jpg"; break;
			case 3:
				strExtension = "gif"; break;
			case 4:
				strExtension = "png"; break;
			default:
				break;
			}
			strFileName = strFileName + "." + strExtension;
		}	
		saveFilePath = strFileName;     
	}
	 imagedes.Save(saveFilePath);     //保存图像
}


void CMFCApplication1Dlg::OnBnClickedUndoButton()
{
	if(operation!=0&&operation!=16 && operation != 32){
		error(5);
		return;
	}
	if (psrc == NULL) {
		error(1); return;
	}
	if ((log_head + 1) % 10 == log_tail) {
		error(3); return;
	}
	log_tail = log_tail ? log_tail - 1 : 9;
	if (log_tail) {
		bitmap2img(pdes, img_log[log_tail - 1]); 
		bitmap = img_log[log_tail - 1];
	}
	else {
		bitmap2img(pdes, img_log[9]);
		bitmap = img_log[9];
	}
	display(imagedes);

	stringstream ss;
	streambuf* buffer = cout.rdbuf(); //oldbuffer,STDOUT的缓冲区
	cout.rdbuf(ss.rdbuf());
	cout << "撤销\n";
	string s(ss.str());
	cout.rdbuf(buffer); // 重置，重新载入STDOUT的缓冲区
	Log.push_back(s);
	showlog();
	// TODO: 在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnBnClickedRedoButton()
{
	if(operation!=0&&operation!=16){
		error(5);
		return;
	}
	if (psrc == NULL) {
		error(1); return;
	}

	stringstream ss;
	streambuf* buffer = cout.rdbuf(); //oldbuffer,STDOUT的缓冲区
	cout.rdbuf(ss.rdbuf());
	cout << "重做\n";
	string s(ss.str());
	cout.rdbuf(buffer); // 重置，重新载入STDOUT的缓冲区
	Log.push_back(s);
	showlog();
	// TODO: 在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnBnClickedMosaicButton()
{
	if(operation!=0&&operation!=16&&operation!=8&&operation!=32){
		error(5);
		return;
	}
	if (psrc == NULL) {
		error(1); return;
	}
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_SLIDER1)->ShowWindow(TRUE);
	GetDlgItem(IDC_SLIDER1_text)->ShowWindow(TRUE);

	static  bool op = 1;
	if (op) {
		operation=8;
		bitmap2 = bitmap;
		grade_sli.SetPos(10);//滚动条初始位置
	}
	else {
		operation = 0;
		bitmap = bitmap2;
		bitmap2img(pdes, bitmap);
		backup_push(bitmap);
		grade_sil_value = grade_sli.GetPos();

		stringstream ss;
		streambuf* buffer = cout.rdbuf(); //oldbuffer,STDOUT的缓冲区
		cout.rdbuf(ss.rdbuf());
		cout <<"马赛克: 半径: "<<grade_sil_value<< "\n";
		string s(ss.str());
		cout.rdbuf(buffer); // 重置，重新载入STDOUT的缓冲区
		Log.push_back(s);
		showlog();
		GetDlgItem(IDC_SLIDER1)->ShowWindow(FALSE);
		GetDlgItem(IDC_SLIDER1_text)->ShowWindow(FALSE);
	}
	op = !op;
}


void CMFCApplication1Dlg::OnBnClickedGrayButton()
{
	if(operation!=0&&operation!=16&&operation!=32){
		error(5);
		return;
	}
	//灰度值处理
	if (psrc == NULL) {
		error(1); return;
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if(!bitmap[i][j].op){bitmap[i][j].op = 1;continue;}
			byte x=(double)bitmap[i][j].b * 0.114+bitmap[i][j].g * 0.587+bitmap[i][j].r *0.299;
				bitmap[i][j].b =bitmap[i][j].g =bitmap[i][j].r =x;
		}
	}
	bitmap2img(pdes,bitmap);
	display(imagedes);
	backup_push(bitmap);
	operation = 0;

	stringstream ss;
	streambuf* buffer = cout.rdbuf(); //oldbuffer,STDOUT的缓冲区
	cout.rdbuf(ss.rdbuf());
	cout << "灰度\n";
	string s(ss.str());
	cout.rdbuf(buffer); // 重置，重新载入STDOUT的缓冲区
	Log.push_back(s);
	showlog();
}


void CMFCApplication1Dlg::OnBnClickedPenButton()
{
	if (operation != 0 && operation != 32) {
		error(5);
		return;
	}
	if (psrc == NULL) {
		error(1); return;
	}
	operation = 32;

	static bool op = 1;

	if (op)
	{
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				bitmap[i][j].op = 0;

		stringstream ss;
		streambuf* buffer = cout.rdbuf(); //oldbuffer,STDOUT的缓冲区
		cout.rdbuf(ss.rdbuf());
		cout << "画笔选择" << "\n";
		string s(ss.str());
		cout.rdbuf(buffer); // 重置，重新载入STDOUT的缓冲区
		Log.push_back(s);
		showlog();
	}
	else {
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				bitmap[i][j].op = 1;

		stringstream ss;
		streambuf* buffer = cout.rdbuf(); //oldbuffer,STDOUT的缓冲区
		cout.rdbuf(ss.rdbuf());
		cout << "清除画笔选择"  << "\n";
		string s(ss.str());
		cout.rdbuf(buffer); // 重置，重新载入STDOUT的缓冲区
		Log.push_back(s);
		showlog();
	}
	op = !op;
}


void CMFCApplication1Dlg::OnStnClickedimg()
{
	// TODO: 在此添加控件通知处理程序代码
}



void CMFCApplication1Dlg::imgcopy(CImage* src, CImage& des)
{
	des.Create(width, height, bitcnt * 8);//BOOL Create(	int nWidth,int nHeight,int nBPP,DWORD dwFlags = 0) throw();
	byte* p = (byte*)des.GetBits();
	bitmap2img(p,bitmap);
	
}




void CMFCApplication1Dlg::OnNMCustomdrawSlider2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CMFCApplication1Dlg::OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CMFCApplication1Dlg::OnStnClickedSlider1text()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::RGB2HSL(pix& a)
{
	double R, G, B, Max, Min, del_R, del_G, del_B, del_Max;
	R = a.r / 255.0;       //Where RGB values = 0 ÷ 255
	G = a.g / 255.0;
	B = a.b / 255.0;

	Min = min(R, min(G, B));    //Min. value of RGB
	Max = max(R, max(G, B));    //Max. value of RGB
	del_Max = Max - Min;        //Delta RGB value

	a.L = (Max + Min) / 2.0;

	if (del_Max == 0)           //This is a gray, no chroma...
	{
		//H = 2.0/3.0;          //Windows下S值为0时，H值始终为160（2/3*240）
		a.H = 0;                  //HSL results = 0 ÷ 1
		a.S = 0;
	}
	else                        //Chromatic data...
	{
		if (a.L < 0.5) a.S = del_Max / (Max + Min);
		else         a.S = del_Max / (2 - Max - Min);

		del_R = (((Max - R) / 6.0) + (del_Max / 2.0)) / del_Max;
		del_G = (((Max - G) / 6.0) + (del_Max / 2.0)) / del_Max;
		del_B = (((Max - B) / 6.0) + (del_Max / 2.0)) / del_Max;

		if (R == Max) a.H = del_B - del_G;
		else if (G == Max) a.H = (1.0 / 3.0) + del_R - del_B;
		else if (B == Max) a.H = (2.0 / 3.0) + del_G - del_R;

		while (a.H < 0)  a.H += 1;
		while (a.H > 1)  a.H -= 1;
	}
}

double CMFCApplication1Dlg::Hue2RGB(double v1, double v2, double vH)
{
	if (vH < 0) vH += 1;
	if (vH > 1) vH -= 1;
	if (6.0 * vH < 1) return v1 + (v2 - v1) * 6.0 * vH;
	if (2.0 * vH < 1) return v2;
	if (3.0 * vH < 2) return v1 + (v2 - v1) * ((2.0 / 3.0) - vH) * 6.0;
	return (v1);
}
void CMFCApplication1Dlg::change_light(byte value,vector<vector<pix>>&mapsrc, vector<vector<pix>>& mapdes)
{

	float rate=(value+50)/100.0;
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			if(!mapsrc[i][j].op){mapdes[i][j].op = 1;continue;}
			RGB2HSL(mapsrc[i][j]);
			mapdes[i][j] = mapsrc[i][j];
				mapdes[i][j].L=rate*mapsrc[i][j].L;

			mapdes[i][j].L = (mapdes[i][j].L > 1) ? 1 : mapdes[i][j].L;
			HSL2RGB(mapdes[i][j]);
		}
	}
	CImage imgtemp;
	imgtemp.Create(width, height, bitcnt * 8);
	bitmap2img((byte*)imgtemp.GetBits(),mapdes);
	display(imgtemp);
}
void CMFCApplication1Dlg::change_satura(byte value,vector<vector<pix>>& mapsrc, vector<vector<pix>>& mapdes)
{
	float rate=(value+50)/100.0;
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			if(!mapsrc[i][j].op){mapdes[i][j].op = 1;continue;}
			RGB2HSL(mapsrc[i][j]);
			mapdes[i][j] = mapsrc[i][j];
				mapdes[i][j].S=rate*mapsrc[i][j].S;
			mapdes[i][j].S = (mapdes[i][j].S > 1) ? 1 : mapdes[i][j].S;
			HSL2RGB(mapdes[i][j]);
		}
	}
	CImage imgtemp;
	imgtemp.Create(width, height, bitcnt * 8);
	bitmap2img((byte*)imgtemp.GetBits(),mapdes);
	display(imgtemp);
}
void CMFCApplication1Dlg::HSL2RGB(pix& a)
{
	double R, G, B;
	double var_1, var_2;
	if (a.S == 0)                       //HSL values = 0 ÷ 1
	{
		R = a.L * 255.0;                   //RGB results = 0 ÷ 255
		G = a.L * 255.0;
		B = a.L * 255.0;
	}
	else
	{
		if (a.L < 0.5) var_2 = a.L * (1 + a.S);
		else         var_2 = (a.L + a.S) - (a.S * a.L);

		var_1 = 2.0 * a.L - var_2;

		R = 255.0 * Hue2RGB(var_1, var_2, a.H + (1.0 / 3.0));
		G = 255.0 * Hue2RGB(var_1, var_2, a.H);
		B = 255.0 * Hue2RGB(var_1, var_2, a.H - (1.0 / 3.0));
	}
	a.r = R; a.g = G; a.b = B;
}

void CMFCApplication1Dlg::img2bitmap(byte* pimg,vector<vector<pix>>&map)
{
	vector<pix>vec;
	for (int i = 0; i < height; i++)
	{
		vec.clear();
		for (int j = 0; j < width; j++)
		{
			pix a;
			a.b=*(pimg + pitch * i + j * bitcnt);		//B
			a.g=*(pimg + pitch * i + j * bitcnt + 1);	//G
			a.r=*(pimg + pitch * i + j * bitcnt + 2);	//R
			a.op = 1;
			RGB2HSL(a);
			//RGB2HSL(a);
			vec.push_back(a);
		}
		map.push_back(vec);
	}
}
void CMFCApplication1Dlg::bitmap2img(byte* pimg,vector<vector<pix>>&map)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{

			*(pimg + pitch * i + j * bitcnt) = map[i][j].b;
			*(pimg + pitch * i + j * bitcnt + 1) = map[i][j].g;
			*(pimg + pitch * i + j * bitcnt + 2) = map[i][j].r;
		}
	}
}

void CMFCApplication1Dlg::slider_ini()
{
	// TODO: 在此添加额外的初始化代码

//滚动条1

 //设置滚动条滚动范围
	grade_sli.SetRange(0, 100);
	//每十个单位画一个刻度
	grade_sli.SetTicFreq(10);
	//设置初始位置
	grade_sli.SetPos(50);//滚动条初始位置

   //滚动条2

	//设置滚动条滚动范围
	range_sli.SetRange(0, 100);
	//每十个单位画一个刻度
	range_sli.SetTicFreq(10);
	//设置初始位置
	range_sli.SetPos(50);
}

void CMFCApplication1Dlg::error(byte error_code)
{
	switch (error_code)
	{
	case 1: {
		AfxMessageBox(_T("非法操作：未打开文件或文件错误！"));
		break;
	}
	case 2: {
		AfxMessageBox(_T("非法操作：范围错误！"));
		break;
	}
	case 3: {
		AfxMessageBox(_T("非法操作：无法继续撤销！"));
		break;
	}
	case 4: {
		AfxMessageBox(_T("非法操作：无法恢复！"));
		break;
	}
	case 5: {
		AfxMessageBox(_T("非法操作：上一步操作未完成！"));
		break;
	}
	default:
		break;
	}
}

void CMFCApplication1Dlg::showlog()
{
	string str="";
	for (int i = Log.size() - 1; i >= 0;i--) {
		str += Log[i];
	}
	CString cst;
	cst = str.c_str();
	LogText.SetWindowText(cst);
}


void CMFCApplication1Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static bool op = 1;
	if (op) { op = !op; return; }
	op = !op;
	if (pScrollBar != NULL)
	{
		// 强制转换成CSliderCtrl
		CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;
		// 根据CSlider控件的ID 来判断是哪一个滚动条

		if (pSlider->GetDlgCtrlID() == IDC_SLIDER1)//对第一个滚动条进行操作
		{
			//获取当前位置值，放在变量m_int中
			grade_sil_value = grade_sli.GetPos();
			switch (operation)
			{
			case 1:
			{
				change_light(grade_sil_value, bitmap,bitmap2);
				break;
			}
			case 2:
			{
				change_satura(grade_sil_value, bitmap,bitmap2);
				break;
			}
			
			case 4:
			{
				vague(grade_sil_value, bitmap,bitmap2);
				break;
			}
			case 8:
			{
				mosaic(grade_sil_value, bitmap, bitmap2);
				break;
			}
			default:
				break;
			}
		}

		if (pSlider->GetDlgCtrlID() == IDC_SLIDER2)//对第二个滚动条操作
		{
			range_sli_value = range_sli.GetPos();

		}
	}
}
void CMFCApplication1Dlg::mosaic(byte value,vector<vector<pix>>& mapsrc, vector<vector<pix>>& mapdes)
{
	if(!value){
		mapdes=mapsrc;
	}
	for(int i=0;i<height;i++)
	{
		if(!value){
		mapdes=mapsrc;
		break;
	}
		for(int j=0;j<width;j++)
		{
			if(!mapsrc[i/value*value][i/value*value].op){mapdes[i][j].op = 1;continue;}
			RGB2HSL(mapsrc[i][j]);
				mapdes[i][j] = mapsrc[i/value*value][j/value*value];
			//else mapdes[i][j]=mapsrc[i][j];
			HSL2RGB(mapdes[i][j]);
		}
	}
	CImage imgtemp;
	imgtemp.Create(width, height, bitcnt * 8);
	bitmap2img((byte*)imgtemp.GetBits(),mapdes);
	display(imgtemp);
}
void CMFCApplication1Dlg::vague(byte value,vector<vector<pix>>& mapsrc, vector<vector<pix>>& mapdes)
{
	byte r=value/20;
	if(!r){
		mapdes=mapsrc;return;
	}
	static vector<int>templt[9];
	templt[1] = { 1 , 2,1,2, 4,2, 1,2,1 }; int s[6] = { 0,16,256,4096,64436,1048576 };
	templt[2] = { 1,4,6,4,1,4,16,24,16,4,6,24,36,24,6,4,16,24,16,4,1,4,6,4,1 };
	templt[3] = { 1,6,15,20,15,6,1,6,36,90,120,90,36,6,15,90,225,300,225,90,15,20,120,300,400,300,120,20,15,90,225,300,225,90,15,6,36,90,120,90,36,6,1,6,15,20,15,6,1 };
	templt[4] = { 1,8,28,56,70,56,28,8,1,8,64,224,448,560,448,224,64,8,28,224,784,1568,1960,1568,784,224,28,56,448,1568,3136,3920,3136,1568,448,56,70,560,1960,3920,4900,3920,1960,560,70,56,448,1568,3136,3920,3136,1568,448,56,28,224,784,1568,1960,1568,784,224,28,8,64,224,448,560,448,224,64,8,1,8,28,56,70,56,28,8,1};
	templt[5]={1,10,45,120,210,252,210,120,45,10,1,10,100,450,1200,2100,2520,2100,1200,450,100,10,45,450,2025,5400,9450,11340,9450,5400,2025,450,45,120,1200,5400,14400,25200,30240,25200,14400,5400,1200,120,210,2100,9450,25200,44100,52920,44100,25200,9450,2100,210,252,2520,11340,30240,52920,63504,52920,30240,11340,2520,252,210,2100,9450,25200,44100,52920,44100,25200,9450,2100,210,120,1200,5400,14400,25200,30240,25200,14400,5400,1200,120,45,450,2025,5400,9450,11340,9450,5400,2025,450,45,10,100,450,1200,2100,2520,2100,1200,450,100,10,1,10,45,120,210,252,210,120,45,10,1};

	int x,y,c;
	for(y=r; y<height-r; y++)
		for(x=r; x<width-r; x++)
		{
			if(!mapsrc[y][x].op){
				mapdes[y][x]=mapsrc[y][x];
				mapdes[y][x].op = 1;
				continue;
				}
			long long R = 0, G = 0, B = 0;              //用来存放加权和
			int px, py;
			for (int i = 0; i <= r * 2; i++)
			{
				for (int j = 0; j <= r * 2; j++)
				{
					//计算对应模板上位置的像素在原图像中的位置
					py = y - r + i;
					px = x - r + j;
					//加权求和
					int rate = templt[r][i * (r * 2 + 1) + j];
					R +=(long long) mapsrc[py][px].r * rate;
					G += (long long)mapsrc[py][px].g * rate;
					B += (long long)mapsrc[py][px].b * rate;
				}
			}
				//利用高斯模板对邻域进行处理
			pix a; a.r = min(R / s[r],255); a.g =min( G / s[r],255); a.b = min(B / s[r],255);
			mapdes[y][x]=a;
		}

	CImage imgtemp;
	imgtemp.Create(width, height, bitcnt * 8);
	bitmap2img((byte*)imgtemp.GetBits(),mapdes);
	display(imgtemp);
}




bool mousedown = false;;
int choose_x1,choose_x2,choose_y1,choose_y2;
void CMFCApplication1Dlg::OnLButtonDown(UINT nFlags, CPoint point)//Point:鼠标相对程序窗口的坐标
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	mousedown = true;
	if (operation != 16)return;
	CRect rect;
	CWnd* pWnd = GetDlgItem(IDC_img);
	pWnd->GetWindowRect(rect);

	ScreenToClient(rect);//图像区相对程序窗口的坐标

	int x, y;

	x = point.x - rect.left;
	y = point.y - rect.top;	

		int W = rect.Width(), H = rect.Height();
		if ((double)W / H > ratio) {
			int w = ratio * H;
			if (x > (W + w) / 2) x = w;
			else if (x < (W - w) / 2)x = 0;
			else x = x - (W - w) / 2;
			if (y < 0)y = 0;
			if (y > H)y = H;
			choose_x1 = x*width/w;
			choose_y1 = y * height / H;
		}
		else {
			int h = W / ratio;
			if (y > (H + h) / 2) y = h;
			else if (y < (H - h) / 2)y = 0;
			else y = y - (H - h) / 2;
			if (x < 0)x = 0;
			if (x > W)x = W;
			choose_y1 = y * height / h;
			choose_x1 = x * width / W;
		}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMFCApplication1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	mousedown = false;
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (operation != 16)return;

	CRect rect;
	CWnd* pWnd = GetDlgItem(IDC_img);
	pWnd->GetWindowRect(rect);

	ScreenToClient(rect);//图像区相对程序窗口的坐标

	int x, y;

		x = point.x - rect.left;
		y = point.y - rect.top;
		int W = rect.Width(), H = rect.Height();
		if ((double)W / H > ratio) {
			int w = ratio * H;
			if (x > (W + w) / 2) x = w;
			else if (x < (W - w) / 2)x = 0;
			else x = x - (W - w) / 2;
			if (y < 0)y = 0;
			if (y > H)y = H;
			choose_x2 = x * width / w;
			choose_y2 = y * height / H;
		}
		else {
			int h = W / ratio;
			if (y > (H + h) / 2) y = h;
			else if (y < (H - h) / 2)y = 0;
			else y = y - (H - h) / 2;
			if (x < 0)x = 0;
			if (x > W)x = W;
			choose_y2 = y * height / h;
			choose_x2 = x * width / W;
		}
		if (choose_x1 > choose_x2)swap(choose_x1, choose_x2);
		if(choose_y1>choose_y2)swap(choose_y1, choose_y2);

		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
			{
				if(i > choose_y1&& i < choose_y2&&j>choose_x1&&j<choose_x2)
					bitmap[i][j].op = 1;
				else bitmap[i][j].op = 0;
			}
		backup_push(bitmap);
		stringstream ss;
		streambuf* buffer = cout.rdbuf(); //oldbuffer,STDOUT的缓冲区
		cout.rdbuf(ss.rdbuf());
		cout << "框选区域: (" <<choose_x1<<","<<choose_y1<<")-("<< choose_x2 << "," << choose_y2 << ")\n";
		string s(ss.str());
		cout.rdbuf(buffer); // 重置，重新载入STDOUT的缓冲区
		Log.push_back(s);
		showlog();
	CDialogEx::OnLButtonUp(nFlags, point);
	
}


void CMFCApplication1Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRect rect;
	CWnd* pWnd = GetDlgItem(IDC_img);
	pWnd->GetWindowRect(rect);
	//img.GetWindowRect(rect2);		//img框相对整个屏幕的范围
	//img.GetClientRect(rect2);		//img框的范围，相对于自身左上角
	int tempx = rect.left, tempy = rect.top;
	ScreenToClient(rect);//图像区相对程序窗口的坐标

	//先获取鼠标相对于屏幕的坐标
	//GetCursorPos(&point);		//鼠标相对屏幕的坐标
	int x, y;
	//if (point.x > rect.left && point.x<rect.right && point.y>rect.top && point.y < rect.bottom)
	x = point.x - rect.left;
	y = point.y - rect.top;


	HDC hDC = ::GetDC(NULL);

	COLORREF rgb = ::GetPixel(hDC, tempx + x, tempy + y);
	int r = GetRValue(rgb);
	int g = GetGValue(rgb);
	int b = GetBValue(rgb);


	stringstream ss;
	streambuf* buffer = cout.rdbuf(); //oldbuffer,STDOUT的缓冲区
	cout.rdbuf(ss.rdbuf());
	cout << "X:" << x << "   Y:" << y << "\nR:" << r << "  G:" << g << "  B:" << b;
	string s(ss.str());
	cout.rdbuf(buffer); // 重置，重新载入STDOUT的缓冲区

	CString cst;
	cst = s.c_str();
	if (point.x > rect.left && point.x<rect.right && point.y>rect.top && point.y < rect.bottom)
		PosText.SetWindowText(cst);
	if (operation == 32 && mousedown) pen_choose(point);
	CDialogEx::OnMouseMove(nFlags, point);
}
void CMFCApplication1Dlg::pen_choose(CPoint point)
{
	CRect rect;
	CWnd* pWnd = GetDlgItem(IDC_img);
	pWnd->GetWindowRect(rect);

	ScreenToClient(rect);//图像区相对程序窗口的坐标

	int x, y;

	x = point.x - rect.left;
	y = point.y - rect.top;
	int W = rect.Width(), H = rect.Height();
	if ((double)W / H > ratio) {
		int w = ratio * H;
		if (x > (W + w) / 2) x = w;
		else if (x < (W - w) / 2)x = 0;
		else x = x - (W - w) / 2;
		if (y < 0)y = 0;
		if (y > H)y = H;
		x = x * width / w;
		y = y * height / H;
	}
	else {
		int h = W / ratio;
		if (y > (H + h) / 2) y = h;
		else if (y < (H - h) / 2)y = 0;
		else y = y - (H - h) / 2;
		if (x < 0)x = 0;
		if (x > W)x = W;
		x = y * height / h;
		y = x * width / W;
	}
	for(int i=(y>>3)<<3;i< ((y >> 3) << 3)+8;i++)
		for(int j= (x >> 3) << 3;j< ((x >> 3) << 3) + 8;j++)
			bitmap[i][j].op = 1;

}



