#include <QtWidgets/QApplication>  
#include "HalconCpp.h" 
#include "HDevThread.h" 
#include <QPalette>  
#include <QWidget>  
#include <QMessageBox>  
#include <stdio.h>  
#include <QDebug>
#include "IR239.h"
using namespace HalconCpp;

// Procedure declarations 
// Chapter: Develop
// Short Description: Open a new graphics window that preserves the aspect ratio of the given image. 
void dev_open_window_fit_image(HObject ho_Image, HTuple hv_Row, HTuple hv_Column,
	HTuple hv_WidthLimit, HTuple hv_HeightLimit, HTuple *hv_WindowHandle);
// Chapter: Develop
// Short Description: Switch dev_update_pc, dev_update_var and dev_update_window to 'off'. 
void dev_update_off();
// Local procedures 
// Short Description: Out of the two possible orientations per ellipse, Cluster1 contains the ones, where the normal vectors point in a similar direction 
void p_cluster_normals(HTuple hv_Pose1, HTuple hv_Pose2, HTuple hv_CenterNormal1,
	HTuple hv_CenterNormal2, HTuple *hv_ClusterP1, HTuple *hv_ClusterP2, HTuple *hv_ClusterCN1,
	HTuple *hv_ClusterCN2);
// Short Description: Determines large and small elliptical contours. 
void p_determine_ellipse_contours(HObject ho_Image, HObject *ho_EllipseContoursLarge,
	HObject *ho_EllipseContoursSmall, HTuple hv_visualization, HTuple *hv_NumberLarge,
	HTuple *hv_NumberSmall);
void getcircleinformation(HObject ho_Image, HObject *ho_EllipseContoursLarge, HTuple *hv_Row,
	HTuple *hv_Column, HTuple *hv_R);

// Procedures 
// Chapter: Develop
// Short Description: Open a new graphics window that preserves the aspect ratio of the given image. 
void dev_open_window_fit_image(HObject ho_Image, HTuple hv_Row, HTuple hv_Column,
	HTuple hv_WidthLimit, HTuple hv_HeightLimit, HTuple *hv_WindowHandle)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_MinWidth, hv_MaxWidth, hv_MinHeight;
	HTuple  hv_MaxHeight, hv_ResizeFactor, hv_ImageWidth, hv_ImageHeight;
	HTuple  hv_TempWidth, hv_TempHeight, hv_WindowWidth, hv_WindowHeight;

	//This procedure opens a new graphics window and adjusts the size
	//such that it fits into the limits specified by WidthLimit
	//and HeightLimit, but also maintains the correct image aspect ratio.
	//
	//If it is impossible to match the minimum and maximum extent requirements
	//at the same time (f.e. if the image is very long but narrow),
	//the maximum value gets a higher priority,
	//
	//Parse input tuple WidthLimit
	if (0 != (HTuple((hv_WidthLimit.TupleLength()) == 0).TupleOr(hv_WidthLimit<0)))
	{
		hv_MinWidth = 500;
		hv_MaxWidth = 800;
	}
	else if (0 != ((hv_WidthLimit.TupleLength()) == 1))
	{
		hv_MinWidth = 0;
		hv_MaxWidth = hv_WidthLimit;
	}
	else
	{
		hv_MinWidth = ((const HTuple&)hv_WidthLimit)[0];
		hv_MaxWidth = ((const HTuple&)hv_WidthLimit)[1];
	}
	//Parse input tuple HeightLimit
	if (0 != (HTuple((hv_HeightLimit.TupleLength()) == 0).TupleOr(hv_HeightLimit<0)))
	{
		hv_MinHeight = 400;
		hv_MaxHeight = 600;
	}
	else if (0 != ((hv_HeightLimit.TupleLength()) == 1))
	{
		hv_MinHeight = 0;
		hv_MaxHeight = hv_HeightLimit;
	}
	else
	{
		hv_MinHeight = ((const HTuple&)hv_HeightLimit)[0];
		hv_MaxHeight = ((const HTuple&)hv_HeightLimit)[1];
	}
	//
	//Test, if window size has to be changed.
	hv_ResizeFactor = 1;
	GetImageSize(ho_Image, &hv_ImageWidth, &hv_ImageHeight);
	//First, expand window to the minimum extents (if necessary).
	if (0 != (HTuple(hv_MinWidth>hv_ImageWidth).TupleOr(hv_MinHeight>hv_ImageHeight)))
	{
		hv_ResizeFactor = (((hv_MinWidth.TupleReal()) / hv_ImageWidth).TupleConcat((hv_MinHeight.TupleReal()) / hv_ImageHeight)).TupleMax();
	}
	hv_TempWidth = hv_ImageWidth*hv_ResizeFactor;
	hv_TempHeight = hv_ImageHeight*hv_ResizeFactor;
	//Then, shrink window to maximum extents (if necessary).
	if (0 != (HTuple(hv_MaxWidth<hv_TempWidth).TupleOr(hv_MaxHeight<hv_TempHeight)))
	{
		hv_ResizeFactor = hv_ResizeFactor*((((hv_MaxWidth.TupleReal()) / hv_TempWidth).TupleConcat((hv_MaxHeight.TupleReal()) / hv_TempHeight)).TupleMin());
	}
	hv_WindowWidth = hv_ImageWidth*hv_ResizeFactor;
	hv_WindowHeight = hv_ImageHeight*hv_ResizeFactor;
	//Resize window
	SetWindowAttr("background_color", "black");
	OpenWindow(hv_Row, hv_Column, hv_WindowWidth, hv_WindowHeight, 0, "visible", "", &(*hv_WindowHandle));
	HDevWindowStack::Push((*hv_WindowHandle));
	if (HDevWindowStack::IsOpen())
		SetPart(HDevWindowStack::GetActive(), 0, 0, hv_ImageHeight - 1, hv_ImageWidth - 1);
	return;
}

// Chapter: Develop
// Short Description: Switch dev_update_pc, dev_update_var and dev_update_window to 'off'. 
void dev_update_off()
{

	//This procedure sets different update settings to 'off'.
	//This is useful to get the best performance and reduce overhead.
	//
	// dev_update_pc(...); only in hdevelop
	// dev_update_var(...); only in hdevelop
	// dev_update_window(...); only in hdevelop
	return;
}

// Local procedures 
// Short Description: Out of the two possible orientations per ellipse, Cluster1 contains the ones, where the normal vectors point in a similar direction 
void p_cluster_normals(HTuple hv_Pose1, HTuple hv_Pose2, HTuple hv_CenterNormal1,
	HTuple hv_CenterNormal2, HTuple *hv_ClusterP1, HTuple *hv_ClusterP2, HTuple *hv_ClusterCN1,
	HTuple *hv_ClusterCN2)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_Number, hv_NA, hv_NB, hv_i, hv_N1;
	HTuple  hv_N2, hv_N1NA, hv_N1NB, hv_N2NA, hv_N2NB, hv_Indices;
	HTuple  hv_idx, hv_DeviationC1, hv_DeviationC2, hv_Help;

	hv_Number = (hv_CenterNormal1.TupleLength()) / 6;
	(*hv_ClusterP1) = hv_Pose1.TupleSelectRange(0, 6);
	(*hv_ClusterP2) = hv_Pose2.TupleSelectRange(0, 6);
	(*hv_ClusterCN1) = hv_CenterNormal1.TupleSelectRange(0, 5);
	(*hv_ClusterCN2) = hv_CenterNormal2.TupleSelectRange(0, 5);
	hv_NA = (*hv_ClusterCN1).TupleSelectRange(3, 5);
	hv_NB = (*hv_ClusterCN2).TupleSelectRange(3, 5);
	{
		HTuple end_val7 = hv_Number - 1;
		HTuple step_val7 = 1;
		for (hv_i = 1; hv_i.Continue(end_val7, step_val7); hv_i += step_val7)
		{
			hv_N1 = hv_CenterNormal1.TupleSelectRange((hv_i * 6) + 3, (hv_i * 6) + 5);
			hv_N2 = hv_CenterNormal2.TupleSelectRange((hv_i * 6) + 3, (hv_i * 6) + 5);
			hv_N1NA = (hv_N1*hv_NA).TupleSum();
			hv_N1NB = (hv_N1*hv_NB).TupleSum();
			hv_N2NA = (hv_N2*hv_NA).TupleSum();
			hv_N2NB = (hv_N2*hv_NB).TupleSum();
			TupleSortIndex((((hv_N1NA.TupleConcat(hv_N1NB)).TupleConcat(hv_N2NA)).TupleConcat(hv_N2NB))*-1,
				&hv_Indices);
			if (0 != (HTuple(HTuple(hv_Indices[0]) == 0).TupleOr(HTuple(hv_Indices[0]) == 3)))
			{
				(*hv_ClusterP1) = (*hv_ClusterP1).TupleConcat(hv_Pose1.TupleSelectRange(hv_i * 7, (hv_i * 7) + 6));
				(*hv_ClusterP2) = (*hv_ClusterP2).TupleConcat(hv_Pose2.TupleSelectRange(hv_i * 7, (hv_i * 7) + 6));
				(*hv_ClusterCN1) = (*hv_ClusterCN1).TupleConcat(hv_CenterNormal1.TupleSelectRange(hv_i * 6, (hv_i * 6) + 5));
				(*hv_ClusterCN2) = (*hv_ClusterCN2).TupleConcat(hv_CenterNormal2.TupleSelectRange(hv_i * 6, (hv_i * 6) + 5));
			}
			else
			{
				(*hv_ClusterP1) = (*hv_ClusterP1).TupleConcat(hv_Pose2.TupleSelectRange(hv_i * 7, (hv_i * 7) + 6));
				(*hv_ClusterP2) = (*hv_ClusterP2).TupleConcat(hv_Pose1.TupleSelectRange(hv_i * 7, (hv_i * 7) + 6));
				(*hv_ClusterCN1) = (*hv_ClusterCN1).TupleConcat(hv_CenterNormal2.TupleSelectRange(hv_i * 6, (hv_i * 6) + 5));
				(*hv_ClusterCN2) = (*hv_ClusterCN2).TupleConcat(hv_CenterNormal1.TupleSelectRange(hv_i * 6, (hv_i * 6) + 5));
			}
		}
	}
	//
	hv_idx = HTuple();
	{
		HTuple end_val29 = hv_Number - 1;
		HTuple step_val29 = 1;
		for (hv_i = 0; hv_i.Continue(end_val29, step_val29); hv_i += step_val29)
		{
			hv_idx = hv_idx.TupleConcat((hv_i * 6) + 3);
		}
	}
	hv_DeviationC1 = ((((*hv_ClusterCN1).TupleSelect(hv_idx)).TupleDeviation()) + (((*hv_ClusterCN1).TupleSelect(hv_idx + 1)).TupleDeviation())) + (((*hv_ClusterCN1).TupleSelect(hv_idx + 2)).TupleDeviation());
	hv_DeviationC2 = ((((*hv_ClusterCN2).TupleSelect(hv_idx)).TupleDeviation()) + (((*hv_ClusterCN2).TupleSelect(hv_idx + 1)).TupleDeviation())) + (((*hv_ClusterCN2).TupleSelect(hv_idx + 2)).TupleDeviation());
	if (0 != (hv_DeviationC2<hv_DeviationC1))
	{
		hv_Help = (*hv_ClusterP1);
		(*hv_ClusterP1) = (*hv_ClusterP2);
		(*hv_ClusterP2) = hv_Help;
		hv_Help = (*hv_ClusterCN1);
		(*hv_ClusterCN1) = (*hv_ClusterCN2);
		(*hv_ClusterCN2) = hv_Help;
	}
	return;
}

// Short Description: Determines large and small elliptical contours. 
void p_determine_ellipse_contours(HObject ho_Image, HObject *ho_EllipseContoursLarge,
	HObject *ho_EllipseContoursSmall, HTuple hv_visualization, HTuple *hv_NumberLarge,
	HTuple *hv_NumberSmall)
{


}

void getcircleinformation(HObject ho_Image, HObject *ho_EllipseContoursLarge, HTuple *hv_Row,
	HTuple *hv_Column, HTuple *hv_R)
{

	// Local iconic variables
	HObject  ho_Dark, ho_DarkRegions, ho_Circles;
	HObject  ho_ROIOuter, ho_ROIInner, ho_ROI, ho_ROIEdges, ho_RimReduced;
	HObject  ho_Edges, ho_EdgesLarge;

	// Local control variables
	HTuple  hv_NumberLarge, hv_Phi, hv_Ra, hv_Rb;
	HTuple  hv_StartPhi, hv_EndPhi, hv_PointOrder;

	Threshold(ho_Image, &ho_Dark, 0, 120);
	if (HDevWindowStack::IsOpen())
		DispObj(ho_Image, HDevWindowStack::GetActive());
	if (HDevWindowStack::IsOpen())
		SetColor(HDevWindowStack::GetActive(), "green");
	if (HDevWindowStack::IsOpen())
		SetDraw(HDevWindowStack::GetActive(), "fill");
	if (HDevWindowStack::IsOpen())
		DispObj(ho_Dark, HDevWindowStack::GetActive());
	//
	Connection(ho_Dark, &ho_DarkRegions);
	SelectShape(ho_DarkRegions, &ho_Circles, (HTuple("circularity").Append("area")),
		"and", (HTuple(0.75).Append(50)), (HTuple(1.0).Append(99999)));
	if (HDevWindowStack::IsOpen())
		DispObj(ho_Image, HDevWindowStack::GetActive());
	if (HDevWindowStack::IsOpen())
		SetColored(HDevWindowStack::GetActive(), 12);
	if (HDevWindowStack::IsOpen())
		DispObj(ho_Circles, HDevWindowStack::GetActive());
	//
	DilationCircle(ho_Circles, &ho_ROIOuter, 14.5);
	ErosionCircle(ho_Circles, &ho_ROIInner, 8.5);
	Difference(ho_ROIOuter, ho_ROIInner, &ho_ROI);
	Union1(ho_ROI, &ho_ROIEdges);
	if (HDevWindowStack::IsOpen())
		DispObj(ho_Image, HDevWindowStack::GetActive());
	if (HDevWindowStack::IsOpen())
		SetDraw(HDevWindowStack::GetActive(), "margin");
	if (HDevWindowStack::IsOpen())
		SetColor(HDevWindowStack::GetActive(), "green");
	if (HDevWindowStack::IsOpen())
		DispObj(ho_ROIEdges, HDevWindowStack::GetActive());
	//
	ReduceDomain(ho_Image, ho_ROIEdges, &ho_RimReduced);
	EdgesSubPix(ho_RimReduced, &ho_Edges, "canny", 2, 10, 50);
	//
	SelectContoursXld(ho_Edges, &ho_EdgesLarge, "contour_length", 80, 300, 0, 0);
	CountObj(ho_EdgesLarge, &hv_NumberLarge);
	FitEllipseContourXld(ho_EdgesLarge, "ftukey", -1, 2, 0, 200, 3, 2, &(*hv_Row),
		&(*hv_Column), &hv_Phi, &hv_Ra, &hv_Rb, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
	GenEllipseContourXld(&(*ho_EllipseContoursLarge), (*hv_Row), (*hv_Column), HTuple(hv_NumberLarge, 0),
		hv_Ra, hv_Rb, HTuple(hv_NumberLarge, 0), HTuple(hv_NumberLarge, HTuple(360).TupleRad()),
		HTuple(hv_NumberLarge, "positive"), 1.5);
	//
	if (0 != (hv_Ra>hv_Rb))
	{
		(*hv_R) = hv_Ra;
	}
	else
	{
		(*hv_R) = hv_Rb;
	}
	if (HDevWindowStack::IsOpen())
		ClearWindow(HDevWindowStack::GetActive());
	if (HDevWindowStack::IsOpen())
		DispObj((*ho_EllipseContoursLarge), HDevWindowStack::GetActive());
	return;

}

#ifndef NO_EXPORT_MAIN
// Main procedure 
double action()
{

	// Local iconic variables
	HObject  ho_Image1, ho_EllipseContoursLarge2;
	HObject  ho_Image2, ho_EllipseContoursLarge1, ho_Line, ho_save;

	// Local control variables
	HTuple  hv_Width, hv_Height, hv_WindowHandle;
	HTuple  hv_Row1, hv_Column1, hv_R1, hv_Row2, hv_Column2;
	HTuple  hv_R2, hv_Length, hv_D;

	//This example program shows how to use the operator get_circle_pose.
	//First, the contours of the drill-holes of a rim are extracted.
	//Then, the 3D position of the center of these contours
	//is determined with the operator get_circle_pose.
	//
	//Read the image and initialize the program.
	dev_update_off();
	ReadImage(&ho_Image1, "C:/Users/Administrator/Desktop/1.tif");
	GetImageSize(ho_Image1, &hv_Width, &hv_Height);
	dev_open_window_fit_image(ho_Image1, 0, 0, hv_Width / 2, hv_Height / 2, &hv_WindowHandle);
	if (HDevWindowStack::IsOpen())
		SetLineWidth(HDevWindowStack::GetActive(), 3);
	getcircleinformation(ho_Image1, &ho_EllipseContoursLarge2, &hv_Row1, &hv_Column1,
		&hv_R1);


	dev_update_off();
	ReadImage(&ho_Image2, "C:/Users/Administrator/Desktop/2.tif");
	getcircleinformation(ho_Image2, &ho_EllipseContoursLarge1, &hv_Row2, &hv_Column2,
		&hv_R2);

	if (HDevWindowStack::IsOpen())
		ClearWindow(HDevWindowStack::GetActive());
	if (HDevWindowStack::IsOpen())
		DispObj(ho_Image1, HDevWindowStack::GetActive());
	if (HDevWindowStack::IsOpen())
		DispObj(ho_Image2, HDevWindowStack::GetActive());
	if (HDevWindowStack::IsOpen())
		DispObj(ho_EllipseContoursLarge1, HDevWindowStack::GetActive());
	if (HDevWindowStack::IsOpen())
		DispObj(ho_EllipseContoursLarge2, HDevWindowStack::GetActive());
	GenContourPolygonXld(&ho_Line, hv_Row1.TupleConcat(hv_Row2), hv_Column1.TupleConcat(hv_Column2));
	if (HDevWindowStack::IsOpen())
		DispObj(ho_Line, HDevWindowStack::GetActive());
	LengthXld(ho_Line, &hv_Length);
	hv_D = (hv_Length*1.75) / hv_R1;
	double d = hv_D[0].D();
	
	//***********************************************************************************
	DumpWindowImage(&ho_save, hv_WindowHandle);
	WriteImage(ho_save, "tiff", 0, "C:/Users/Administrator/Desktop/3.tif");
	if (HDevWindowStack::IsOpen())
		CloseWindow(HDevWindowStack::Pop());
	return d;
}
#endif