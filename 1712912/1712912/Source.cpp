#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <io.h> 
#include <fcntl.h>
#include <string.h>

int const field = 15;

struct SV
{
	char* mssv;
	wchar_t* hovaten;
	wchar_t* khoa;
	int khoatuyen;
	char* ngaysinh;
	char* email;
	wchar_t* hinhanhcanhan;
	wchar_t* sothich1;
	wchar_t* sothich2;
	wchar_t* motabanthan;
};

void DocFile(FILE *fpin, SV **dssv, int *sosinhvien, int ***sokitu)
{
	wchar_t wc, tempt[1000];
	int count = 0, demkitu = 0, demsosinhvien = 0;
	fseek(fpin, 3L, SEEK_CUR);
	bool flag = false;
	fwscanf(fpin, L"%d", sosinhvien);
	rewind(fpin);
	fseek(fpin, 3L, SEEK_CUR);
	while (1)
	{
		wc = fgetwc(fpin);
		if (wc == L'\"')    //Di chuyển tới dấu ngoặc kép của sinh viên đầu tiên, trường đầu tiên
		    break;
	}
	*dssv = (SV*)malloc(*sosinhvien*sizeof(SV));
	*sokitu = (int**)malloc((*sosinhvien)*sizeof(int*));
	**sokitu = (int*)malloc(field*sizeof(int));
	while (1)
	{
		wc = fgetwc(fpin);
		tempt[demkitu] = wc;
		demkitu++;
		if (wc == L'\"')
		{
			count++;
			flag = true;
			*(*((*sokitu)+demsosinhvien)+count) = demkitu - 1;
		}
		if (flag == true)
		{
			if (count == 1)
			{
				(*((*dssv) + demsosinhvien)).mssv = (char*)malloc((demkitu - 1)*sizeof(char));
				for (int i = 0; i < demkitu - 1; i++)
				{
					(*((*dssv) + demsosinhvien)).mssv[i] = tempt[i];
				}
				demkitu = 0;
			}
			else if (count == 2)
			{
				(*((*dssv) + demsosinhvien)).hovaten = (wchar_t*)malloc((demkitu - 1)*sizeof(wchar_t));
				for (int i = 0; i < demkitu - 1; i++)
				{
					(*((*dssv) + demsosinhvien)).hovaten[i] = tempt[i];
				}
				demkitu = 0;
			}
			else if (count == 3)
			{
				(*((*dssv) + demsosinhvien)).khoa = (wchar_t*)malloc((demkitu - 1)*sizeof(wchar_t));
				for (int i = 0; i < demkitu - 1; i++)
				{
					(*((*dssv) + demsosinhvien)).khoa[i] = tempt[i];
				}
				demkitu = 0;
			}
			else if (count == 4)
			{
				char *year = (char*)malloc((demkitu - 1) * sizeof(char));
				for (int i = 0; i < demkitu - 1; i++)
				{
					year[i] = tempt[i];
				}
				(*((*dssv) + demsosinhvien)).khoatuyen = atoi(year);
				demkitu = 0;
				free(year);
			}
			else if (count == 5)
			{
				(*((*dssv) + demsosinhvien)).ngaysinh = (char*)malloc((demkitu - 1)*sizeof(char));
				for (int i = 0; i < demkitu - 1; i++)
				{
					(*((*dssv) + demsosinhvien)).ngaysinh[i] = tempt[i];
				}
				demkitu = 0;
			}
			else if (count == 6)
			{
				(*((*dssv) + demsosinhvien)).email = (char*)malloc((demkitu - 1)*sizeof(char));
				for (int i = 0; i < demkitu - 1; i++)
				{
					(*((*dssv) + demsosinhvien)).email[i] = tempt[i];
				}
				demkitu = 0;
			}
			else if (count == 7)
			{
				(*((*dssv) + demsosinhvien)).hinhanhcanhan = (wchar_t*)malloc((demkitu - 1)*sizeof(wchar_t));
				for (int i = 0; i < demkitu - 1; i++)
				{
					(*((*dssv) + demsosinhvien)).hinhanhcanhan[i] = tempt[i];
				}
				demkitu = 0;
			}
			else if (count == 8)
			{
				(*((*dssv) + demsosinhvien)).sothich1 = (wchar_t*)malloc((demkitu - 1)*sizeof(wchar_t));
				for (int i = 0; i < demkitu - 1; i++)
				{
					(*((*dssv) + demsosinhvien)).sothich1[i] = tempt[i];
				}
				demkitu = 0;
			}
			else if (count == 9)
			{
				(*((*dssv) + demsosinhvien)).sothich2 = (wchar_t*)malloc((demkitu - 1)*sizeof(wchar_t));
				for (int i = 0; i < demkitu - 1; i++)
				{
					(*((*dssv) + demsosinhvien)).sothich2[i] = tempt[i];
				}
				demkitu = 0;
			}
			else if (count == 10)
			{
				(*((*dssv) + demsosinhvien)).motabanthan = (wchar_t*)malloc((demkitu - 1)*sizeof(wchar_t));
				for (int i = 0; i < demkitu - 1; i++)
				{
					(*((*dssv) + demsosinhvien)).motabanthan[i] = tempt[i];
				}
				demkitu = 0;
			}
			if (count == 10)
			{
				demsosinhvien++;
				count = 0;
				demkitu = 0;
				if (demsosinhvien == *sosinhvien)
					break;
				*((*sokitu) + demsosinhvien) = (int*)malloc(field*sizeof(int));
			}
			while (1)
			{
				wc = fgetwc(fpin);
				if (wc == L'\"')   //Di chuyển đến dấu ngoặc kép của trường tiếp theo
					break;
			}
			flag = false;
		}
	}
}

void GhiFile(SV ttsv, int *sokitu)
{
	FILE *fpout;
	char *tenfile;
	tenfile = (char*)malloc((15 + sokitu[1])*sizeof(char));
	strcpy(tenfile, "Website//");
	strcat(tenfile, ttsv.mssv);
	tenfile[0 + 9 + sokitu[1]] = '.';
	tenfile[1 + 9 + sokitu[1]] = 'h';
	tenfile[2 + 9 + sokitu[1]] = 't';
	tenfile[3 + 9 + sokitu[1]] = 'm';
	tenfile[4 + 9 + sokitu[1]] = 'l';
	tenfile[5 + 9 + sokitu[1]] = '\0';
	fpout = fopen(tenfile, "w+");
	_setmode(_fileno(fpout), _O_U8TEXT);
	fwprintf(fpout, L"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">");
	fwprintf(fpout, L"<html xmlns=\"http://www.w3.org/1999/xhtml\">\n");
	fwprintf(fpout, L"	<head>\n");
	fwprintf(fpout, L"		<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n");
	fwprintf(fpout, L"		<link rel=\"stylesheet\" type=\"text/css\" href=\"personal.css\" />\n");
	fwprintf(fpout, L"		<title>HCMUS - ");
	for (int i = 0; i < sokitu[2]; i++)
		fwprintf(fpout, L"%lc", ttsv.hovaten[i]);
	fwprintf(fpout, L"</title>\n");
	fwprintf(fpout, L"	</head>\n");
	fwprintf(fpout, L"	<body>\n");
	fwprintf(fpout, L"		<div class=\"Layout_container\">\n");
	fwprintf(fpout, L"			<!-- Begin Layout Banner Region -->\n");
	fwprintf(fpout, L"			<div class=\"Layout_Banner\" >\n");
	fwprintf(fpout, L"				<div><img id=\"logo\" src=\"Images/logo.jpg\" height=\"105\" /></div>\n");
	fwprintf(fpout, L"				<div class=\"Header_Title\">TRƯỜNG ĐẠI HỌC KHOA HỌC TỰ NHIÊN</div>\n");
	fwprintf(fpout, L"			</div>\n");
	fwprintf(fpout, L"			<!-- End Layout Banner Region -->\n");
	fwprintf(fpout, L"			<!-- Begin Layout MainContents Region -->\n");
	fwprintf(fpout, L"			<div class=\"Layout_MainContents\">\n");
	fwprintf(fpout, L"				<!-- Begin Below Banner Region -->\n");
	fwprintf(fpout, L"				<div class=\"Personal_Main_Information\">\n");
	fwprintf(fpout, L"					<!-- Begin Thông tin cá nhân của sinh viên ------------------------------------------------------------------------------------------->\n");
	fwprintf(fpout, L"					<div class=\"Personal_Location\"> \n");
	fwprintf(fpout, L"						<img src=\"Images/LogoFooter.jpg\" width=\"27\" height=\"33\" />\n");
	fwprintf(fpout, L"						<span class=\"Personal_FullName\">");
	for (int i = 0; i < sokitu[2]; i++)
		fwprintf(fpout, L"%lc", ttsv.hovaten[i]);
	fwprintf(fpout, L" - ");
	for (int i = 0; i < sokitu[1]; i++)
		fwprintf(fpout, L"%lc", ttsv.mssv[i]);
	fwprintf(fpout, L"</span>\n");
	fwprintf(fpout, L"						<div class=\"Personal_Department\">KHOA CÔNG NGHỆ THÔNG TIN</div>\n");
	fwprintf(fpout, L"						<br />\n");
	fwprintf(fpout, L"						<div class=\"Personal_Phone\">\n");
	fwprintf(fpout, L"                                 Email: ");
	for (int i = 0; i < sokitu[6]; i++)
		fwprintf(fpout, L"%lc", ttsv.email[i]);
	fwprintf(fpout, L"\n");
	fwprintf(fpout, L"						</div>\n");
	fwprintf(fpout, L"						<br />\n");
	fwprintf(fpout, L"						<br />\n");
	fwprintf(fpout, L"					</div>\n");
	fwprintf(fpout, L"					<!-- End Thông tin cá nhân của sinh viên ------------------------------------------------------------------------------------------->\n");
	fwprintf(fpout, L"					<div class=\"Personal_HinhcanhanKhung\">\n");
	fwprintf(fpout, L"						<img src=\"Images/");
	for (int i = 0; i < sokitu[7]; i++)
		fwprintf(fpout, L"%lc", ttsv.hinhanhcanhan[i]);
	fwprintf(fpout, L"\" class=\"Personal_Hinhcanhan\" />\n");
	fwprintf(fpout, L"					</div>\n");
	fwprintf(fpout, L"				</div>\n");
	fwprintf(fpout, L"				<!-- End Below Banner Region -->\n");
	fwprintf(fpout, L"				<!-- Begin MainContents Region -->\n");
	fwprintf(fpout, L"				<div class=\"MainContain\">\n");
	fwprintf(fpout, L"\n");
	fwprintf(fpout, L"					<!-- Begin Top Region -->\n");
	fwprintf(fpout, L"					<div class=\"MainContain_Top\">\n");
	fwprintf(fpout, L"\n");
	fwprintf(fpout, L"						<div class=\"InfoGroup\">Thông tin cá nhân</div>\n");
	fwprintf(fpout, L"                       <div>\n");
	fwprintf(fpout, L"                            <ul class=\"TextInList\">\n");
	fwprintf(fpout, L"                              <li>Họ và tên: ");
	for (int i = 0; i < sokitu[2]; i++)
		fwprintf(fpout, L"%lc", ttsv.hovaten[i]);
	fwprintf(fpout, L" </li>\n");
	fwprintf(fpout, L"								 <li>MSSV: ");
	for (int i = 0; i < sokitu[1]; i++)
		fwprintf(fpout, L"%lc", ttsv.mssv[i]);
	fwprintf(fpout, L" </li>\n");
	fwprintf(fpout, L"								 <li>Sinh viên khoa ");
	for (int i = 0; i < sokitu[3]; i++)
		fwprintf(fpout, L"%lc", ttsv.khoa[i]);
	fwprintf(fpout, L" </li>\n");
	fwprintf(fpout, L"								 <li>Khóa ");
	fwprintf(fpout, L"%d", ttsv.khoatuyen);
	fwprintf(fpout, L" </li>\n");
	fwprintf(fpout, L"								 <li>Ngày sinh: ");
	for (int i = 0; i < sokitu[5]; i++)
		fwprintf(fpout, L"%lc", ttsv.ngaysinh[i]);
	fwprintf(fpout, L" </li>\n");
	fwprintf(fpout, L"								 <li>Email: ");
	for (int i = 0; i < sokitu[6]; i++)
		fwprintf(fpout, L"%lc", ttsv.email[i]);
	fwprintf(fpout, L" </li>\n");
	fwprintf(fpout, L"							 </ul>\n");
	fwprintf(fpout, L"						</div>\n");
	fwprintf(fpout, L"                       <div class=\"InfoGroup\">Sở thích</div>\n");
	fwprintf(fpout, L"                       <div>\n");
	fwprintf(fpout, L"                            <ul class=\"TextInList\">\n");
	fwprintf(fpout, L"                              <li>");
	for (int i = 0; i < sokitu[8]; i++)
		fwprintf(fpout, L"%lc", ttsv.sothich1[i]);
	fwprintf(fpout, L"</li>\n");
	fwprintf(fpout, L"								 <li>");
	for (int i = 0; i < sokitu[9]; i++)
		fwprintf(fpout, L"%lc", ttsv.sothich2[i]);
	fwprintf(fpout, L"</li>\n");
	fwprintf(fpout, L"							 </ul>\n");
	fwprintf(fpout, L"						</div>\n");
	fwprintf(fpout, L"						<div class=\"InfoGroup\">Mô tả</div>\n");
	fwprintf(fpout, L"						<div class=\"Description\">\n");
	fwprintf(fpout, L"                            ");
	for (int i = 0; i < sokitu[10]; i++)
		fwprintf(fpout, L"%lc", ttsv.motabanthan[i]);
	fwprintf(fpout, L".\n");
	fwprintf(fpout, L"						</div>\n");
	fwprintf(fpout, L"\n");
	fwprintf(fpout, L"					</div>\n");
	fwprintf(fpout, L"				</div>\n");
	fwprintf(fpout, L"			</div>\n");
	fwprintf(fpout, L"\n");
	fwprintf(fpout, L"			<!-- Begin Layout Footer -->\n");
	fwprintf(fpout, L"			<div class=\"Layout_Footer\">\n");
	fwprintf(fpout, L"				<div class=\"divCopyright\">\n");
	fwprintf(fpout, L"					<br />\n");
	fwprintf(fpout, L"					<img src=\"Images/LogoFooter_gray.jpg\" width=\"34\" height=\"41\" /><br />\n");
	fwprintf(fpout, L"					<br />\n");
	fwprintf(fpout, L"				@2018</br>\n");
	fwprintf(fpout, L"				Đồ án giữa kì</br>\n");
	fwprintf(fpout, L"				Kỹ thuật lâp trình</br>\n");
	fwprintf(fpout, L"				TH2017/18</br>\n");
	fwprintf(fpout, L"				1712912 - Nguyễn Hoàng Vinh</br>\n");
	fwprintf(fpout, L"				</div>\n");
	fwprintf(fpout, L"			</div>\n");
	fwprintf(fpout, L"			<!-- End Layout Footer -->\n");
	fwprintf(fpout, L"		</div>\n");
	fwprintf(fpout, L"	</body>\n");
	fwprintf(fpout, L"</html>");
	fclose(fpout);
}

void GiaiPhong(SV **dssv, int sosinhvien, int ***sokitu)
{
	for (int i = 0; i < sosinhvien; i++)
	{
		free((*((*dssv) + i)).mssv);
		free((*((*dssv) + i)).hovaten);
		free((*((*dssv) + i)).khoa);
		free((*((*dssv) + i)).ngaysinh);
		free((*((*dssv) + i)).email);
		free((*((*dssv) + i)).hinhanhcanhan);
		free((*((*dssv) + i)).sothich1);
		free((*((*dssv) + i)).sothich2);
		free((*((*dssv) + i)).motabanthan);
		free(*((*sokitu) + i));
	}
	free(*dssv);
	free(*sokitu);
}

void main()
{
	FILE *fpin;
	SV *dssv;
	int sosinhvien, dem = 0, **sokitu;
	fpin = fopen("thongtinsinhvien.csv", "r");
	_setmode(_fileno(fpin), _O_U8TEXT);  //in tiếng việt từ những dòng tiếng việt trong fwprintf. Vd: TRƯỜNG ĐẠI HỌC KHOA HỌC TỰ NHIÊN, KHOA CÔNG NGHỆ THÔNG TIN
	DocFile(fpin, &dssv, &sosinhvien, &sokitu);
	while (dem < sosinhvien)
	{
		GhiFile(*(dssv + dem), *(sokitu + dem));
		dem++;
	}
	if (dem == sosinhvien)
		printf("--------Thanh cong--------\n");
	fclose(fpin);
	GiaiPhong(&dssv, sosinhvien, &sokitu);
	system("pause");
}