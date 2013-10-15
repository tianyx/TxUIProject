#pragma once
//#import "E:\\toolswin7\\Microsoft Office\\Office12\\EXCEL.EXE" //no_namespace rename("DialogBox", "ExcelDialogBox"),rename("RGB", "ExcelRGB"), rename("IPicture", "IExcelPicture")


#import "MSO.DLL" raw_interfaces_only  rename_namespace("Office")
using namespace Office;

// #import "d:\Program Files\Common Files\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB" raw_interfaces_only  rename_namespace("VBE6")
// using namespace VBE6;

#import "EXCEL.EXE" raw_interfaces_only,named_guids rename("RGB","RGBEx"),rename("DialogBox","DialogBoxEx"),rename_namespace("ExcelEx") exclude("IFont","IPicture")
using namespace ExcelEx;
