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

_locale_t vina;

void DocFile(FILE *fpin, SV **dssv, int *sosinhvien, int **arrdemsothich)
{
	wchar_t wc, tempt[1000];
	int count = 0, demkitu = 0, demsosinhvien = 0, demsothich = 0;
	fseek(fpin, 3L, SEEK_CUR);
	bool flag = false, doublequotes = false;
	*dssv = (SV*)malloc(1 * sizeof(SV));
	*arrdemsothich = (int*)malloc(1 * sizeof(int));
	while (1)
	{
		wc = fgetwc(fpin);
		tempt[demkitu] = wc;
		demkitu++;
		if (wc == L'\"' && doublequotes == false)       //Mở ngoặc kép
		{
			doublequotes = true;
			demkitu = 0;
		}
		else if (wc == L'\"' && doublequotes == true)  //Đóng ngoặc kép
		{
			doublequotes = false;
			demkitu--;
		}
		else if ((wc == L',' || wc == L';') && doublequotes == false)  //Dấu phẩy (chấm phẩy) dùng để ngăn cách trường (khác dấu phẩy (chấm phẩy) ở trong ngoặc kép)
			flag = true;
		else if (wc == L'\n' || wc == WEOF)
			flag = true;
		if (flag == true)
		{
			demkitu--;
			tempt[demkitu] = L'\0';
			printf("%ls\n", tempt);
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
				(*((*dssv) + demsosinhvien)).sothich = (wchar_t**)malloc(1 * sizeof(wchar_t*));//Cấp phát sở thích đầu tiên vì lát nữa sẽ realloc
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
				*arrdemsothich = (int*)realloc(*arrdemsothich, (demsosinhvien + 1)*sizeof(int));
				count = 0;
				demsothich = 0;
			}
			demkitu = 0;
			flag = false;
		}
	}
}

void GhiFile(SV* dssv, int sosinhvien, int *arrdemsothich)
{
	FILE *fhtml, **fpout;
	wchar_t tenfile[30], upper[30];
	wchar_t tempt[200], wc, *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9, *p10, *p11, *p12, *p13, *p14;
	int i, dem = 0, demsothich = 0, demkitu = 0;
	p1 = (wchar_t*)malloc(30 * sizeof(wchar_t));
	p2 = (wchar_t*)malloc(30 * sizeof(wchar_t));
	p3 = (wchar_t*)malloc(30 * sizeof(wchar_t));
	p4 = (wchar_t*)malloc(30 * sizeof(wchar_t));
	p5 = (wchar_t*)malloc(30 * sizeof(wchar_t));
	p6 = (wchar_t*)malloc(30 * sizeof(wchar_t));
	p7 = (wchar_t*)malloc(30 * sizeof(wchar_t));
	p8 = (wchar_t*)malloc(30 * sizeof(wchar_t));
	p9 = (wchar_t*)malloc(30 * sizeof(wchar_t));
	p10 = (wchar_t*)malloc(30 * sizeof(wchar_t));
	p11 = (wchar_t*)malloc(30 * sizeof(wchar_t));
	p12 = (wchar_t*)malloc(30 * sizeof(wchar_t));
	p13 = (wchar_t*)malloc(30 * sizeof(wchar_t));
	p14 = (wchar_t*)malloc(30 * sizeof(wchar_t));
	fpout = (FILE**)malloc(sosinhvien*sizeof(FILE*));
	fhtml = fopen("Website//1212123.htm", "r");
	_setmode(_fileno(fhtml), _O_U8TEXT);
	while (dem < sosinhvien)
	{
		wcscpy(tenfile, L"Website//");
		wcscat(tenfile, dssv[dem].mssv);
		wcscat(tenfile, L".html");
		*(fpout + dem) = _wfopen(tenfile, L"w+");
		_setmode(_fileno(*(fpout+dem)), _O_U8TEXT);
		dem++;
	}
	dem = 0;
	fseek(fhtml, 3L, SEEK_SET);
	while (1)
	{
		do {
			wc = fgetwc(fhtml);
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
		p9 = wcsstr(tempt, L"Khóa:");
		p10 = wcsstr(tempt, L"Ngày sinh:");
		p11 = wcsstr(tempt, L"Email:");
		p12 = wcsstr(tempt, L"InfoGroup\">Sở thích");
		p13 = wcsstr(tempt, L"Description");
		p14 = wcsstr(tempt, L"@2013");
		if (p1 != NULL)
		{
			while (dem < sosinhvien)
			{
				i = 0;
				while (&tempt[i] != p1) //Ghi từ đầu cho đến hết HCMUS -
				{
					fwprintf(*(fpout + dem), L"%lc", tempt[i]);
					i++;
				}
				fwprintf(*(fpout + dem), L"<title>HCMUS - %ls</title>\n", dssv[dem].hovaten);
				dem++;
			}
			dem = 0;
		}
		else if (p2 != NULL)
		{
			while (dem < sosinhvien)
			{
				i = 0;
				while (&tempt[i] != p2)
				{
					fwprintf(*(fpout + dem), L"%lc", tempt[i]);
					i++;
				}
				wcscpy(upper, dssv[dem].hovaten);
				fwprintf(*(fpout + dem), L"Personal_Fullname\">%ls - %ls</span>", _wcsupr_l(upper, vina), dssv[dem].mssv); //upper
				dem++;
			}
			dem = 0;
		}
		else if (p3 != NULL)
		{
			while (dem < sosinhvien)
			{
				i = 0;
				while (&tempt[i] != p3)
				{
					fwprintf(*(fpout + dem), L"%lc", tempt[i]);
					i++;
				}
				wcscpy(upper, dssv[dem].khoa);
				fwprintf(*(fpout + dem), L"Personal_Department\">KHOA %ls</div>\n", _wcsupr_l(upper, vina)); //upper
				dem++;
			}
			dem = 0;
		}
		else if (p4 != NULL)
		{
			while (dem < sosinhvien)
			{
				fwprintf(*(fpout + dem), L"%ls", tempt);
				fwprintf(*(fpout + dem), L"							Email: %ls\n", dssv[dem].email);
				dem++;
			}
			do {
				wc = fgetwc(fhtml);
			} while (wc != L'\n' && wc != WEOF);
			dem = 0;
		}
		else if (p5 != NULL)
		{
			while (dem < sosinhvien)
			{
				i = 0;
				while (&tempt[i] != p5)
				{
					fwprintf(*(fpout + dem), L"%lc", tempt[i]);
					i++;
				}
				fwprintf(*(fpout + dem), L"Personal_HinhcanhanKhung\">\n");
				fwprintf(*(fpout + dem), L"						<img src=\"Images/%ls\" class=\"Personal_Hinhcanhan\" />\n", dssv[dem].hinhanhcanhan);
				dem++;
			}
			dem = 0;
			do {
				wc = fgetwc(fhtml);
			} while (wc != L'\n' && wc != WEOF);
		}
		else if (p6 != NULL)
		{
			while (dem < sosinhvien)
			{
				i = 0;
				while (&tempt[i] != p6)
				{
					fwprintf(*(fpout + dem), L"%lc", tempt[i]);
					i++;
				}
				fwprintf(*(fpout + dem), L"Họ và tên: %ls</li>\n", dssv[dem].hovaten);
				dem++;
			}
			dem = 0;
		}
		else if (p7 != NULL)
		{
			while (dem < sosinhvien)
			{
				i = 0;
				while (&tempt[i] != p7)
				{
					fwprintf(*(fpout + dem), L"%lc", tempt[i]);
					i++;
				}
				fwprintf(*(fpout + dem), L"MSSV: %ls</li>\n", dssv[dem].mssv);
				dem++;
			}
			dem = 0;
		}
		else if (p8 != NULL)
		{
			while (dem < sosinhvien)
			{
				i = 0;
				while (&tempt[i] != p8)
				{
					fwprintf(*(fpout + dem), L"%lc", tempt[i]);
					i++;
				}
				fwprintf(*(fpout + dem), L"Sinh viên khoa %ls</li>\n", dssv[dem].khoa);
				dem++;
			}
			dem = 0;
		}
		else if (p9 != NULL)
		{
			while (dem < sosinhvien)
			{
				i = 0;
				while (&tempt[i] != p9)
				{
					fwprintf(*(fpout + dem), L"%lc", tempt[i]);
					i++;
				}
				fwprintf(*(fpout + dem), L"Khóa: %d</li>\n", dssv[dem].khoatuyen);
				dem++;
			}
			dem = 0;
		}
		else if (p10 != NULL)
		{
			while (dem < sosinhvien)
			{
				i = 0;
				while (&tempt[i] != p10)
				{
					fwprintf(*(fpout + dem), L"%lc", tempt[i]);
					i++;
				}
				fwprintf(*(fpout + dem), L"Ngày sinh: %ls</li>\n", dssv[dem].ngaysinh);
				dem++;
			}
			dem = 0;
		}
		else if (p11 != NULL)
		{
			while (dem < sosinhvien)
			{
				i = 0;
				while (&tempt[i] != p11)
				{
					fwprintf(*(fpout + dem), L"%lc", tempt[i]);
					i++;
				}
				fwprintf(*(fpout + dem), L"Email: %ls</li>\n", dssv[dem].email);
				dem++;
			}
			dem = 0;
		}
		else if (p12 != NULL)
		{
			while (dem < sosinhvien)
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
				dem++;
			}
			dem = 0;
			do {
				wc = fgetwc(fhtml);
			} while (wc != L'\n' && wc != WEOF);
			do {
				wc = fgetwc(fhtml);
			} while (wc != L'\n' && wc != WEOF);
			do {
				do {
					wc = fgetwc(fhtml);
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
				i = 0;
				while (&tempt[i] != p13)
				{
					fwprintf(*(fpout + dem), L"%lc", tempt[i]);
					i++;
				}
				fwprintf(*(fpout + dem), L"Description\">\n");
				fwprintf(*(fpout + dem), L"							%ls\n", dssv[dem].motabanthan);
				fwprintf(*(fpout + dem), L"						</div>\n");
				dem++;
			}
			dem = 0;
			do {
				wc = fgetwc(fhtml);
			} while (wc != L'<');
			do {
				wc = fgetwc(fhtml);
			} while (wc != L'\n' && wc != WEOF);
		}
		else if (p14 != NULL)
		{
			while (dem < sosinhvien)
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
				dem++;
			}
			dem = 0;
			do {
				wc = fgetwc(fhtml);
			} while (wc != L'\n' && wc != WEOF);
			do {
				wc = fgetwc(fhtml);
			} while (wc != L'\n' && wc != WEOF);
			do {
				wc = fgetwc(fhtml);
			} while (wc != L'\n' && wc != WEOF);
			do {
				wc = fgetwc(fhtml);
			} while (wc != L'\n' && wc != WEOF);
		}
		else
		{
			while (dem < sosinhvien)
			{
				fwprintf(*(fpout + dem), L"%ls", tempt);
				dem++;
			}
			dem = 0;
		}
		demkitu = 0;
		if (wc == WEOF)
			break;
	}
	fclose(fhtml);
	while (dem < sosinhvien)
	{
		fclose(*(fpout + dem));
		dem++;
	}
}

void GiaiPhong(SV **dssv, int sosinhvien, int **arrdemsothich)
{
	int dem = 0;
	for (int i = 0; i < sosinhvien; i++)
	{
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
	}
	free(*dssv);
	free(*arrdemsothich);
}

void main()
{
	FILE *fpin;
	SV *dssv;
	int sosinhvien, dem = 0, *arrdemsothich, demm=0;
	fpin = fopen("thongtinsinhvien.csv", "r");
	vina = _wcreate_locale(LC_ALL, L"vi-VN");
	_setmode(_fileno(fpin), _O_U8TEXT);  //in tiếng việt từ những dòng tiếng việt trong fwprintf. Vd: TRƯỜNG ĐẠI HỌC KHOA HỌC TỰ NHIÊN, KHOA CÔNG NGHỆ THÔNG TIN
	DocFile(fpin, &dssv, &sosinhvien, &arrdemsothich);
	GhiFile(dssv, sosinhvien, arrdemsothich);
	fclose(fpin);
	GiaiPhong(&dssv, sosinhvien, &arrdemsothich);
	system("pause");
}