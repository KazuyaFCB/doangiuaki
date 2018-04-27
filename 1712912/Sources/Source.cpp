//LƯU Ý: MÀN HÌNH CONSOLE CHỈNH FONT CONSOLAS

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <io.h> 
#include <fcntl.h>
#include <string.h>

struct SV
{
	wchar_t* mssv;
	wchar_t* hovaten;
	wchar_t* khoa;
	int khoatuyen;
	wchar_t* ngaysinh;
	wchar_t* email;
	wchar_t* hinhanhcanhan;
	wchar_t** sothich;
	wchar_t* motabanthan;
};

int field = 9;
_locale_t vina;
bool **logic;

void DocFile(FILE *, SV **, int *, int **);
void GhiFile(SV* , int , int *, FILE **);
void GiaiPhong(SV **, int , int **);
void InConsole(SV *, int , int *);
void KhoiTaoBool(int );
void ResetBool(int );
void TuyChonIn(SV *, int , int *);


void DocFile(FILE *fpin, SV **dssv, int *sosinhvien, int **arrdemsothich)
{
	wchar_t wc, tempt[1000];
	int count = 0, demkitu = 0, demsosinhvien = 0, demsothich = 0;
	fseek(fpin, 3L, SEEK_CUR);
	bool flag = false, doublequotes = false, pairofdoublequotes = false;
	*dssv = (SV*)malloc(1 * sizeof(SV));
	*arrdemsothich = (int*)malloc(1 * sizeof(int));
	(*((*dssv) + demsosinhvien)).sothich = (wchar_t**)malloc(1 * sizeof(wchar_t*));//Cấp phát sở thích đầu tiên vì lát nữa sẽ realloc và free
	while (1)
	{
		wc = fgetwc(fpin);
		tempt[demkitu] = wc;
		demkitu++;
		if (wc == L'\"')
		{
			if (doublequotes == true)
			{
				wc = fgetwc(fpin);
				if (wc == L'\"')   //Dấu mở ngoặc hoặc đóng ngoặc trong trường (dấu highlight)
				{
					if (pairofdoublequotes == false)   //Mở
						pairofdoublequotes = true;
					else                               //Đóng
						pairofdoublequotes = false;   
				}

				else if (wc == L',' || wc == L';' || wc == L'\n')         //Đóng ngoặc kép (kết thúc trường). Đằng sau nó là dấu , ; \n WEOF
				{
					doublequotes = false;
					
					if (pairofdoublequotes == true)      //TH """" (highlight trống và trường đó không bị bao bởi dấu ngoặc kép)
					{
						tempt[demkitu] = L'\"';
						demkitu++;
						pairofdoublequotes = false;
					}
					demkitu--;                                  //Trừ dấu đóng ngoặc kép
					fseek(fpin, -1L, SEEK_CUR);
				}
				else if (wc == WEOF)
				{
					if (pairofdoublequotes == true)      //TH """" (highlight trống và trường đó không bị bao bởi dấu ngoặc kép)
					{
						tempt[demkitu] = L'\"';
						demkitu++;
						pairofdoublequotes = false;
					}
					demkitu--;                                 //Trừ dấu đóng ngoặc kép
				}
			}
			else                    // Có thể là "" hoặc """ (đầu trường)
			{
				wc = fgetwc(fpin);
				if (wc == L'\"')
				{
					wc = fgetwc(fpin);
					if (wc == L'\"')  //Mở ngoặc kép (bắt đầu trường) (TH có highlight ngay sau dấu mở ngoặc kép """)
					{
						demkitu = 0;
						doublequotes = true;
						fseek(fpin, -2L, SEEK_CUR);  //Lùi 2 ô để trở lại dấu mở ngoặc kép bắt đầu trường và quay lại TH bên trên
					}
					else if ((wc == L',' || wc == L';' || wc == L'\n' || wc == WEOF) && pairofdoublequotes == false)
					{
						demkitu = 0;
						fseek(fpin, -1L, SEEK_CUR);
					}
					else if (wc != L'\"' || wc == WEOF)     //Dấu "" (mở hoặc đóng ngoặc highlight) (Trường trong TH này không được bao bởi dấu ngoặc kép)
					{
						if (pairofdoublequotes == false)   //Mở
							pairofdoublequotes = true;
						else                             //Đóng
							pairofdoublequotes = false;
						if (wc == WEOF)
						{
							tempt[demkitu] = WEOF;
							demkitu++;
							flag = true;
						}
						else if (wc >= 32 && wc <= 126)        //Kí tự char thì lùi 1 ô, kí tự wchar_t thì lùi 2 ô để tránh lỗi kí tự đầu nếu nó là wchar_t
							fseek(fpin, -1L, SEEK_CUR);
						else
							fseek(fpin, -2L, SEEK_CUR);
					}
				}
				else if (wc != L'\"')   //Dấu mở ngoặc kép (TH không có highlight ở đầu)
				{
					demkitu = 0;
					doublequotes = true;
					if (wc >= 32 && wc <=126)
						fseek(fpin, -1L, SEEK_CUR);
					else 
						fseek(fpin, -2L, SEEK_CUR);
				}
			}
		}
		else if ((wc == L',' || wc == L';') && doublequotes == false && pairofdoublequotes == false)  //Dấu phẩy (chấm phẩy) dùng để ngăn cách trường (khác dấu phẩy (chấm phẩy) ở trong ngoặc kép)
			flag = true;
		else if (wc == L'\n' || wc == WEOF)
			flag = true;
		if (flag == true)
		{
			demkitu--;
			tempt[demkitu] = L'\0';
			count++;
			if (count == 1)
			{
				(*((*dssv) + demsosinhvien)).mssv = (wchar_t*)malloc((demkitu+1)*sizeof(wchar_t));
				wcscpy((*((*dssv) + demsosinhvien)).mssv, tempt);
			}
			else if (count == 2)
			{
				(*((*dssv) + demsosinhvien)).hovaten = (wchar_t*)malloc((demkitu+1)*sizeof(wchar_t));
				wcscpy((*((*dssv) + demsosinhvien)).hovaten, tempt);
			}
			else if (count == 3)
			{
				(*((*dssv) + demsosinhvien)).khoa = (wchar_t*)malloc((demkitu+1)*sizeof(wchar_t));
				wcscpy((*((*dssv) + demsosinhvien)).khoa, tempt);
			}
			else if (count == 4)
				(*((*dssv) + demsosinhvien)).khoatuyen = _wtoi(tempt);
			else if (count == 5)
			{
				(*((*dssv) + demsosinhvien)).ngaysinh = (wchar_t*)malloc((demkitu+1)*sizeof(wchar_t));
				wcscpy((*((*dssv) + demsosinhvien)).ngaysinh, tempt);
			}
			else if (count == 6)
			{
				(*((*dssv) + demsosinhvien)).email = (wchar_t*)malloc((demkitu+1)*sizeof(wchar_t));
				wcscpy((*((*dssv) + demsosinhvien)).email, tempt);
			}
			else if (count == 7)
			{
				(*((*dssv) + demsosinhvien)).hinhanhcanhan = (wchar_t*)malloc((demkitu+1)*sizeof(wchar_t));
				wcscpy((*((*dssv) + demsosinhvien)).hinhanhcanhan, tempt);
			}
			else if (count == 8)
			{
				(*((*dssv) + demsosinhvien)).motabanthan = (wchar_t*)malloc((demkitu + 1)*sizeof(wchar_t));
				wcscpy((*((*dssv) + demsosinhvien)).motabanthan, tempt);
			}
			else if (count == 9)
			{
				(*((*dssv) + demsosinhvien)).sothich[0] = (wchar_t*)malloc((demkitu + 1) * sizeof(wchar_t));
				wcscpy((*((*dssv) + demsosinhvien)).sothich[0], tempt);
				demsothich++;
			}
			else if (count > 9)
			{
				(*((*dssv) + demsosinhvien)).sothich = (wchar_t**)realloc((*((*dssv) + demsosinhvien)).sothich, (demsothich + 1)*sizeof(wchar_t*));
				(*((*dssv) + demsosinhvien)).sothich[demsothich] = (wchar_t*)malloc((demkitu + 1)*sizeof(wchar_t));
				wcscpy((*((*dssv) + demsosinhvien)).sothich[demsothich], tempt);
				demsothich++;
			}
			if (wc == WEOF)
			{
				(*((*arrdemsothich) + demsosinhvien)) = demsothich;
				*sosinhvien = demsosinhvien + 1;
				break;
			}
			if (wc == L'\n')
			{
				(*((*arrdemsothich) + demsosinhvien)) = demsothich;
				demsosinhvien++;
				*dssv = (SV*)realloc(*dssv, (demsosinhvien + 1) * sizeof(SV));
				*arrdemsothich = (int*)realloc(*arrdemsothich, (demsosinhvien + 1) * sizeof(int));
				(*((*dssv) + demsosinhvien)).sothich = (wchar_t**)malloc(1 * sizeof(wchar_t*));//Cấp phát sở thích đầu tiên cho sinh viên tiếp theo vì lát nữa sẽ realloc và free
				count = 0;
				demsothich = 0;
			}
			demkitu = 0;
			flag = false;
		}
	}
}

void GhiFile(SV* dssv, int sosinhvien, int *arrdemsothich, FILE **fhtml)
{
	FILE **fpout;
	wchar_t tenfile[30], upper[30];
	wchar_t tempt[200], wc, *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9, *p10, *p11, *p12, *p13, *p14;
	int i, dem = 0, demsothich = 0, demkitu = 0, demxuongdong = 0;
	fpout = (FILE**)malloc(sosinhvien*sizeof(FILE*));
	*fhtml = fopen("Website//template.html", "r");
	if (*fhtml == NULL)
	{
		wprintf(L"File template HTML không tồn tại.\n");
		return;
	}
	_setmode(_fileno(*fhtml), _O_U8TEXT);
	while (dem < sosinhvien)
	{
		if (logic[dem][0] || logic[dem][1] || logic[dem][2] || logic[dem][3] || logic[dem][4] || logic[dem][5] || logic[dem][6] || logic[dem][7] || logic[dem][8])
		{
			wcscpy(tenfile, L"Website//");
			wcscat(tenfile, dssv[dem].mssv);
			wcscat(tenfile, L".html");
			*(fpout + dem) = _wfopen(tenfile, L"w+");
			_setmode(_fileno(*(fpout + dem)), _O_U8TEXT);
		}
		dem++;
	}
	dem = 0;
	fseek(*fhtml, 3L, SEEK_SET);
	while (1)
	{
		do {
			wc = fgetwc(*fhtml);
			tempt[demkitu] = wc;
			demkitu++;
		} while (wc != L'\n' && wc != WEOF);
		tempt[demkitu] = L'\0';
		demkitu = 0;
		p1 = wcsstr(tempt, L"<title>");
		p2 = wcsstr(tempt, L"Personal_FullName");
		p3 = wcsstr(tempt, L"Personal_Department");
		p4 = wcsstr(tempt, L"Personal_Phone");            //Phân biệt 2 dòng email
		p5 = wcsstr(tempt, L"Personal_HinhcanhanKhung");
		p6 = wcsstr(tempt, L"Họ và tên");
		p7 = wcsstr(tempt, L"MSSV:");
		p8 = wcsstr(tempt, L"Sinh viên khoa");
		p9 = wcsstr(tempt, L"Khóa");
		p10 = wcsstr(tempt, L"Ngày sinh:");
		p11 = wcsstr(tempt, L"Email:");
		p12 = wcsstr(tempt, L"InfoGroup\">Sở thích");
		p13 = wcsstr(tempt, L"Mô tả");
		p14 = wcsstr(tempt, L"@2013");
		if (p1 != NULL)
		{
			while (dem < sosinhvien)
			{
				if (logic[dem][0] || logic[dem][1] || logic[dem][2] || logic[dem][3] || logic[dem][4] || logic[dem][5] || logic[dem][6] || logic[dem][7] || logic[dem][8])
				{
					if (!logic[dem][1])
						dssv[dem].hovaten[0] = '\0';   //Họ và tên sẽ không hiện, chỉ hiện HCMUS -
					i = 0;
					while (&tempt[i] != p1) //Ghi từ đầu cho đến hết HCMUS -
					{
						fwprintf(*(fpout + dem), L"%lc", tempt[i]);
						i++;
					}
					fwprintf(*(fpout + dem), L"<title>HCMUS - %ls</title>\n", dssv[dem].hovaten);
				}
				dem++;
			}
			dem = 0;
		}
		else if (p2 != NULL)
		{
			while (dem < sosinhvien)
			{
				if (logic[dem][0] || logic[dem][1] || logic[dem][2] || logic[dem][3] || logic[dem][4] || logic[dem][5] || logic[dem][6] || logic[dem][7] || logic[dem][8])
				{
					if (!logic[dem][0])
						dssv[dem].mssv[0] = '\0';
					if (!logic[dem][1])
						dssv[dem].hovaten[0] = '\0';
					i = 0;
					while (&tempt[i] != p2)
					{
						fwprintf(*(fpout + dem), L"%lc", tempt[i]);
						i++;
					}
					wcscpy(upper, dssv[dem].hovaten);
					fwprintf(*(fpout + dem), L"Personal_Fullname\">%ls - %ls</span>", _wcsupr_l(upper, vina), dssv[dem].mssv); //upper
				}
				dem++;
			}
			dem = 0;
		}
		else if (p3 != NULL)
		{
			while (dem < sosinhvien)
			{
				if (logic[dem][0] || logic[dem][1] || logic[dem][2] || logic[dem][3] || logic[dem][4] || logic[dem][5] || logic[dem][6] || logic[dem][7] || logic[dem][8])
				{
					if (!logic[dem][2])
						dssv[dem].khoa[0] = '\0';
					i = 0;
					while (&tempt[i] != p3)
					{
						fwprintf(*(fpout + dem), L"%lc", tempt[i]);
						i++;
					}
					wcscpy(upper, dssv[dem].khoa);
					fwprintf(*(fpout + dem), L"Personal_Department\">KHOA %ls</div>\n", _wcsupr_l(upper, vina)); //upper
				}
				dem++;
			}
			dem = 0;
		}
		else if (p4 != NULL)
		{
			while (dem < sosinhvien)
			{
				if (logic[dem][0] || logic[dem][1] || logic[dem][2] || logic[dem][3] || logic[dem][4] || logic[dem][5] || logic[dem][6] || logic[dem][7] || logic[dem][8])
				{
					fwprintf(*(fpout + dem), L"%ls", tempt);
					if (logic[dem][5])
						fwprintf(*(fpout + dem), L"							Email: %ls\n", dssv[dem].email);
				}
				dem++;
			}
			do {
				wc = fgetwc(*fhtml);
			} while (wc != L'\n' && wc != WEOF);
			dem = 0;
		}
		else if (p5 != NULL)
		{
			while (dem < sosinhvien)
			{
				if (logic[dem][0] || logic[dem][1] || logic[dem][2] || logic[dem][3] || logic[dem][4] || logic[dem][5] || logic[dem][6] || logic[dem][7] || logic[dem][8])
				{
					if (!logic[dem][6])
						dssv[dem].hinhanhcanhan[0] = '\0';
					i = 0;
					while (&tempt[i] != p5)
					{
						fwprintf(*(fpout + dem), L"%lc", tempt[i]);
						i++;
					}
					fwprintf(*(fpout + dem), L"Personal_HinhcanhanKhung\">\n");
					fwprintf(*(fpout + dem), L"						<img src=\"Images/%ls\" class=\"Personal_Hinhcanhan\" />\n", dssv[dem].hinhanhcanhan);
				}
				dem++;
			}
			dem = 0;
			do {
				wc = fgetwc(*fhtml);
			} while (wc != L'\n' && wc != WEOF);
		}

		else if (p6 != NULL)
		{
			while (dem < sosinhvien)
			{
				if (logic[dem][1])
				{
					i = 0;
					while (&tempt[i] != p6)
					{
						fwprintf(*(fpout + dem), L"%lc", tempt[i]);
						i++;
					}
					fwprintf(*(fpout + dem), L"Họ và tên: %ls</li>\n", dssv[dem].hovaten);
				}
				dem++;
			}
			dem = 0;
		}
		else if (p7 != NULL)
		{
			while (dem < sosinhvien)
			{
				if (logic[dem][0])
				{
					i = 0;
					while (&tempt[i] != p7)
					{
						fwprintf(*(fpout + dem), L"%lc", tempt[i]);
						i++;
					}
					fwprintf(*(fpout + dem), L"MSSV: %ls</li>\n", dssv[dem].mssv);
				}
				dem++;
			}
			dem = 0;
		}
		else if (p8 != NULL)
		{
			while (dem < sosinhvien)
			{
				if (logic[dem][2])
				{
					i = 0;
					while (&tempt[i] != p8)
					{
						fwprintf(*(fpout + dem), L"%lc", tempt[i]);
						i++;
					}
					fwprintf(*(fpout + dem), L"Sinh viên khoa %ls</li>\n", dssv[dem].khoa);
				}
				dem++;
			}
			dem = 0;
		}
		else if (p9 != NULL)
		{
			while (dem < sosinhvien)
			{
				if (logic[dem][3])
				{
					i = 0;
					while (&tempt[i] != p9)
					{
						fwprintf(*(fpout + dem), L"%lc", tempt[i]);
						i++;
					}
					fwprintf(*(fpout + dem), L"Khóa %d</li>\n", dssv[dem].khoatuyen);
				}
				dem++;
			}
			dem = 0;
		}
		else if (p10 != NULL)
		{
			while (dem < sosinhvien)
			{
				if (logic[dem][4])
				{
					i = 0;
					while (&tempt[i] != p10)
					{
						fwprintf(*(fpout + dem), L"%lc", tempt[i]);
						i++;
					}
					fwprintf(*(fpout + dem), L"Ngày sinh: %ls</li>\n", dssv[dem].ngaysinh);
				}
				dem++;
			}
			dem = 0;
		}
		else if (p11 != NULL)
		{
			while (dem < sosinhvien)
			{
				if (logic[dem][5])
				{
					i = 0;
					while (&tempt[i] != p11)
					{
						fwprintf(*(fpout + dem), L"%lc", tempt[i]);
						i++;
					}
					fwprintf(*(fpout + dem), L"Email: %ls</li>\n", dssv[dem].email);
				}
				dem++;
			}
			dem = 0;
		}
		else if (p12 != NULL)
		{
			while (dem < sosinhvien)
			{
				if (logic[dem][7])
				{
					i = 0;
					while (&tempt[i] != p12)
					{
						fwprintf(*(fpout + dem), L"%lc", tempt[i]);
						i++;
					}
					fwprintf(*(fpout + dem), L"InfoGroup\">Sở thích</div>\n");
					fwprintf(*(fpout + dem), L"                       <div>\n");
					fwprintf(*(fpout + dem), L"                            <ul class=\"TextInList\">\n");
					while (demsothich < *(arrdemsothich + dem))
					{
						fwprintf(*(fpout + dem), L"                              <li>%ls</li>\n", dssv[dem].sothich[demsothich]);
						demsothich++;
					}
					demsothich = 0;
					fwprintf(*(fpout + dem), L"						 	  </ul>\n");
					fwprintf(*(fpout + dem), L"						 </div>\n");
				}
				dem++;
			}
			dem = 0;
			while (demxuongdong < 2)
			{
				do {
					wc = fgetwc(*fhtml);
				} while (wc != L'\n' && wc != WEOF);
				demxuongdong++;
			}
			demxuongdong = 0;
			do {
				do {
					wc = fgetwc(*fhtml);
					tempt[demkitu] = wc;
					demkitu++;
				} while (wc != L'\n' && wc != WEOF);
				tempt[demkitu] = '\0';
				demkitu = 0;
			} while (wcsstr(tempt, L"</div>") == NULL);
		}
		else if (p13 != NULL)
		{
			while (dem < sosinhvien)
			{
				if (logic[dem][8])
				{
					i = 0;
					while (&tempt[i] != p13)
					{
						fwprintf(*(fpout + dem), L"%lc", tempt[i]);
						i++;
					}
					fwprintf(*(fpout + dem), L"Mô tả</div>");
					fwprintf(*(fpout + dem), L"						<div class=\"Description\">\n");
					fwprintf(*(fpout + dem), L"							%ls\n", dssv[dem].motabanthan);
					fwprintf(*(fpout + dem), L"						</div>\n");
				}
				dem++;
			}
			dem = 0;
			do {
				wc = fgetwc(*fhtml);
			} while (wc != L'\n' && wc != WEOF);
			do {
				wc = fgetwc(*fhtml);
			} while (wc != L'<');
			do {
				wc = fgetwc(*fhtml);
			} while (wc != L'\n' && wc != WEOF);
		}
		else if (p14 != NULL)
		{
			while (dem < sosinhvien)
			{
				if (logic[dem][0] || logic[dem][1] || logic[dem][2] || logic[dem][3] || logic[dem][4] || logic[dem][5] || logic[dem][6] || logic[dem][7] || logic[dem][8])
				{
					i = 0;
					while (&tempt[i] != p14)
					{
						fwprintf(*(fpout + dem), L"%lc", tempt[i]);
						i++;
					}
					fwprintf(*(fpout + dem), L"@2018</br>\n");
					fwprintf(*(fpout + dem), L"					Đồ án giữa kì</br>\n");
					fwprintf(*(fpout + dem), L"				Kỹ thuật lập trình</br>\n");
					fwprintf(*(fpout + dem), L"				TH2018/04</br>\n");
					fwprintf(*(fpout + dem), L"				1712912 - Nguyễn Hoàng Vinh</br>\n");
				}
				dem++;
			}
			dem = 0;
			while (demxuongdong < 4)
			{
				do {
					wc = fgetwc(*fhtml);
				} while (wc != L'\n' && wc != WEOF);
				demxuongdong++;
			}
			demxuongdong = 0;
		}
		else
		{
			while (dem < sosinhvien)
			{
				if (logic[dem][0] || logic[dem][1] || logic[dem][2] || logic[dem][3] || logic[dem][4] || logic[dem][5] || logic[dem][6] || logic[dem][7] || logic[dem][8])
					fwprintf(*(fpout + dem), L"%ls", tempt);
				dem++;
			}
			dem = 0;
		}
		demkitu = 0;
		if (wc == WEOF)
			break;
	}
	fclose(*fhtml);
	while (dem < sosinhvien)
	{
		if (logic[dem][0] || logic[dem][1] || logic[dem][2] || logic[dem][3] || logic[dem][4] || logic[dem][5] || logic[dem][6] || logic[dem][7] || logic[dem][8])
			fclose(*(fpout + dem));
		dem++;
	}
}

void GiaiPhong(SV **dssv, int sosinhvien, int **arrdemsothich)
{
	int dem;
	for (int i = 0; i < sosinhvien; i++)
	{
		dem = 0;
		free((*((*dssv) + i)).mssv);
		free((*((*dssv) + i)).hovaten);
		free((*((*dssv) + i)).khoa);
		free((*((*dssv) + i)).ngaysinh);
		free((*((*dssv) + i)).email);
		free((*((*dssv) + i)).hinhanhcanhan);
		while (dem < (*((*arrdemsothich) + i)))
		{
			free((*((*dssv) + i)).sothich[dem]);
			dem++;
		}
		free((*((*dssv) + i)).motabanthan);
		free((*((*dssv) + i)).sothich);
		free(*(logic + i));
	}
	free(*dssv);
	free(*arrdemsothich);
	free(logic);
}

void InConsole(SV *dssv, int sosinhvien, int *arrdemsothich)
{
	int demsosinhvien = 0, demsothich;
	wprintf(L"THÔNG TIN CÁC SINH VIÊN:\n\n");
	while (demsosinhvien < sosinhvien)
	{
		demsothich = 0;
		if (logic[demsosinhvien][0] || logic[demsosinhvien][1] || logic[demsosinhvien][2] || logic[demsosinhvien][3] || logic[demsosinhvien][4] || logic[demsosinhvien][5] || logic[demsosinhvien][6] || logic[demsosinhvien][7] || logic[demsosinhvien][8])
		wprintf(L"Sinh viên thứ %d:\n\n", demsosinhvien + 1);
		if (logic[demsosinhvien][0])
			wprintf(L"Mã số sinh viên: %ls\n", dssv[demsosinhvien].mssv);
		if (logic[demsosinhvien][1])
			wprintf(L"Họ và tên: %ls\n", dssv[demsosinhvien].hovaten);
		if (logic[demsosinhvien][2])
			wprintf(L"Khoa: %ls\n", dssv[demsosinhvien].khoa);
		if (logic[demsosinhvien][3])
			wprintf(L"Khóa: %d\n", dssv[demsosinhvien].khoatuyen);
		if (logic[demsosinhvien][4])
			wprintf(L"Ngày sinh: %ls\n", dssv[demsosinhvien].ngaysinh);
		if (logic[demsosinhvien][5])
			wprintf(L"Email: %ls\n", dssv[demsosinhvien].email);
		if (logic[demsosinhvien][6])
			wprintf(L"Link hình ảnh cá nhân: Images//%ls\n", dssv[demsosinhvien].hinhanhcanhan);
		if (logic[demsosinhvien][7])
		{
			while (demsothich < arrdemsothich[demsosinhvien])
			{
				wprintf(L"Sở thích %d: %ls\n", demsothich + 1, dssv[demsosinhvien].sothich[demsothich]);
				demsothich++;
			}
		}
		if (logic[demsosinhvien][8])
			wprintf(L"Mô tả bản thân: %ls\n\n", dssv[demsosinhvien].motabanthan);
		demsosinhvien++;
	}
}

void KhoiTaoBool(int sosinhvien)
{
	int dem = 0, i;
	logic = (bool**)malloc(sosinhvien*sizeof(bool*));
	while (dem < sosinhvien)
	{
		i = 0;
		*(logic + dem) = (bool*)malloc(field * sizeof(bool));
		while (i < field)
		{
			logic[dem][i] = 0;
			i++;
		}
		dem++;
	}
}

void ResetBool(int sosinhvien)
{
	int i, j;
	for (i = 0; i < sosinhvien; i++)
	{
		for (j = 0; j < field; j++)
			logic[i][j] = 0;
	}
}

void TuyChonIn(SV *dssv, int sosinhvien, int *arrdemsothich)
{
	int stt, selection, dem = 0, dem1 = 0;
	wchar_t inputstt[10], inputsel[10];
	wprintf(L"Nhập lần lượt số thứ tự sinh viên (0 < số thứ tự <= %d) và thông tin cần in của sinh viên đó.\n",sosinhvien);
	wprintf(L"Muốn chọn hết tất cả các sinh viên thì nhập vào \"all\".\n");
	wprintf(L"1.Mã số sinh viên\n");
	wprintf(L"2.Họ và tên\n");
	wprintf(L"3.Khoa\n");
	wprintf(L"4.Khóa\n");
	wprintf(L"5.Ngày sinh\n");
	wprintf(L"6.Email\n");
	wprintf(L"7.Link hình ảnh cá nhân\n");
	wprintf(L"8.Sở thích\n");
	wprintf(L"9.Mô tả bản thân\n");
	wprintf(L"\"all\".In tất cả\n");
	wprintf(L"\"end\".Ngưng\n");
	wprintf(L"CHỖ THỨ TỰ CỦA SINH VIÊN, NẾU MUỐN CHỌN HẾT THÌ ẤN PHÍM 0.\n");
	wprintf(L"MUỐN KẾT THÚC PHẦN NÀY THÌ CHỖ NHẬP SỐ THỨ TỰ SINH VIÊN ẤN PHÍM \"exit\"\n\n");
	do {
		wprintf(L"Nhập số thứ tự của sinh viên: ");
		_getws(inputstt);
		fflush(stdin);
		stt = _wtoi(inputstt);
		if (stt > 0 && stt <= sosinhvien)
		{
			do {
				wprintf(L"Nhập sự lựa chọn: ");
				_getws(inputsel);
				fflush(stdin);
				selection = _wtoi(inputsel);
				if (selection > 0 && selection <= field)
					logic[stt - 1][selection - 1] = 1;
				else if (wcscmp(inputsel, L"all") == 0)       //Chọn hết tất cả lựa chọn
				{
					while (dem < field)
					{
						logic[stt - 1][dem] = 1;
						dem++;
					}
					dem = 0;
				}
				else if (wcscmp(inputsel, L"end") == 0)        //Thoát lựa chọn
					break;
				else
					wprintf(L"Sự lựa chọn không hợp lệ, nhập lai đi.\n");
			} while (1);
		}
		else if (wcscmp(inputstt, L"all") == 0)          //Chọn hết tất cả sinh viên
		{
			do {
				wprintf(L"Nhập sự lựa chọn: ");
				_getws(inputsel);
				fflush(stdin);
				selection = _wtoi(inputsel);
				if (selection > 0 && selection <= field)
				{
					while (dem < sosinhvien)
					{
						logic[dem][selection - 1] = 1;
						dem++;
					}
					dem = 0;
				}					
				else if (wcscmp(inputsel, L"all") == 0)     //Chọn hết tất cả lựa chọn
				{
					while (dem < sosinhvien)
					{
						while (dem1 < field)
						{
							logic[dem][dem1] = 1;
							dem1++;
						}
						dem1 = 0;
						dem++;
					}
					dem = 0;
				}
				else if (wcscmp(inputsel, L"end") == 0)
					break;
				else
					wprintf(L"Sự lựa chọn không hợp lệ, nhập lai đi.\n");

			} while (1);
		}
		else if (wcscmp(inputstt, L"exit") == 0)
			break;
		else
			wprintf(L"Số thứ tự không hợp lệ, nhập lai đi.\n");
	} while (1);
}

void main()
{
	FILE *fpin, *fhtml;
	SV *dssv;
	int sosinhvien, dem = 0, *arrdemsothich;
	_setmode(_fileno(stdout), _O_U8TEXT);
	_setmode(_fileno(stdin), _O_U8TEXT);
	fpin = fopen("thongtinsinhvien.csv", "r");
	if (fpin == NULL)
	{
		wprintf(L"File csv không tồn tại.\n");
		system("pause");
		return;
	}
	vina = _wcreate_locale(LC_ALL, L"vi-VN");
	_setmode(_fileno(fpin), _O_U8TEXT);  //in tiếng việt từ những dòng tiếng việt trong fwprintf. Vd: TRƯỜNG ĐẠI HỌC KHOA HỌC TỰ NHIÊN, KHOA CÔNG NGHỆ THÔNG TIN
	DocFile(fpin, &dssv, &sosinhvien, &arrdemsothich);
	KhoiTaoBool(sosinhvien);
	wprintf(L"LỰA CHỌN ĐỂ IN THÔNG TIN LÊN MÀN HÌNH CONSOLE VÀ LỰA CHỌN THÔNG TIN ĐỂ PHÁT SINH CÁC TRANG WEB HTML\n\n");
	wprintf(L"MÀN HÌNH CONSOLE\n\n");
	TuyChonIn(dssv, sosinhvien, arrdemsothich);
	InConsole(dssv, sosinhvien, arrdemsothich);
	ResetBool(sosinhvien);
	wprintf(L"PHÁT SINH RA CÁC TRANG HTML\n\n");
	TuyChonIn(dssv, sosinhvien, arrdemsothich);
	GhiFile(dssv, sosinhvien, arrdemsothich, &fhtml);
	fclose(fpin);
	GiaiPhong(&dssv, sosinhvien, &arrdemsothich);
	if (fhtml == NULL)
	{
		system("pause");
		return;
	}
	wprintf(L"Đọc và ghi file thành công.\n");
	system("pause");
}