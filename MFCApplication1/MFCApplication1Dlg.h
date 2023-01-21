
// MFCApplication1Dlg.h: 头文件
//

#pragma once
#include<vector>
#include<sstream>
#include<iostream>
#include<Windows.h>
#include<string>
#include <atlstr.h>
using namespace std;

typedef unsigned char byte;
struct pix {
	byte r, g, b, op;
	float H, S, L;
};

// CMFCApplication1Dlg 对话框
class CMFCApplication1Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	//afx_msg void OnSizeIDD_MFCAPPLICATION1_DIALOG();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedLightButton();
	afx_msg void OnBnClickedOriginButton();
	afx_msg void OnBnClickedSaturaButton();
	afx_msg void OnBnClickedChooseButton();
	afx_msg void OnBnClickedVagueButton();
	afx_msg void OnBnClickedGrayButton();
	afx_msg void OnBnClickedMosaicButton();
	afx_msg void OnBnClickedPenButton();
	afx_msg void OnBnClickedUndoButton();
	afx_msg void OnBnClickedRedoButton();
	afx_msg void OnBnClickedOpenButton();
	afx_msg void OnBnClickedSaveButton();
	afx_msg void OnStnClickedimg();

	CStatic img;
//	CString imgPath;
	CString imgPath;
	CString imgName;
	afx_msg void OnNMCustomdrawSlider2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStnClickedSlider1text();
	void display(CImage& a);
	void imgcopy(CImage* src, CImage& des);
	void RGB2HSL(pix& a);
	void HSL2RGB(pix& a);
	double Hue2RGB(double v1, double v2, double vH);
	void change_light(byte value, vector<vector<pix>>& mapsrc, vector<vector<pix>>& mapdes);
	void change_satura(byte value, vector<vector<pix>>& mapsrc, vector<vector<pix>>& mapdes);
	void mosaic(byte value, vector<vector<pix>>& mapsrc, vector<vector<pix>>& mapdes);
	void vague(byte value, vector<vector<pix>>& mapsrc, vector<vector<pix>>& mapdes);
	//int TempltExcuteCl(vector<vector<pix>>& mapsrc, int w, int h, int* templt, int tw, int x, int y, int cn);
	void img2bitmap(byte* pimg,vector<vector<pix>>&map);
	void bitmap2img(byte* pimg,vector<vector<pix>>&map);
	void slider_ini();
	void error(byte code);
	void showlog();
	void pen_choose(CPoint point);
	CSliderCtrl grade_sli;
	int grade_sil_value;
	CSliderCtrl range_sli;
	int range_sli_value;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CStatic PosText;
	CStatic LogText;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
