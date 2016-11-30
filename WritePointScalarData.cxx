#define HAS_VTK 1
#define _IS_DEBUG 1

#include "vtkPointData.h"
#include <vtkPointPicker.h>
#include <vtkCommand.h>
#include <vtkMarchingCubes.h>
#include <vtkContourFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyDataMapper.h>
#include <vtkCamera.h>
#include <vtkMarchingCubes.h>
#include <vtkVectorNorm.h>
#include <vtkDataSetMapper.h>
#include <vtkImageToPolyDataFilter.h>
#include <vtkPolyDataReader.h>
#include <vtkLookupTable.h>
#include <vtkSphereSource.h>
#include <vtkCallbackCommand.h>
#include <vtkProperty.h>
#include <vtkImagePlaneWidget.h>
#include <vtkImageActor.h>
#include <vtkSmartPointer.h>
#include <vtkCellArray.h>
#include <vtkPolyDataWriter.h>
#include <vtkCellData.h>
#include <vtkPolyDataReader.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkLandmarkTransform.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkMaskPoints.h>
#include <vtkFloatArray.h>
#include <vtkPointLocator.h>


#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;


  
/*
*   For each vertex in target, gets the closest vertex on the source post-transformation, and copies the point ID 
*   Writes the target polygon with Point IDs from source as output 
*/
void WriteScalar(char* source_poly_fn, char* output_txt) 
{
  vtkSmartPointer<vtkPolyData> source_poly =vtkSmartPointer<vtkPolyData>::New();
  vtkSmartPointer<vtkPolyDataReader> reader1 = vtkSmartPointer<vtkPolyDataReader>::New(); 
  reader1->SetFileName(source_poly_fn); 
  reader1->Update();
  source_poly = reader1->GetOutput();


  // writing target scalars that were hit to file 
  ofstream output; 
  output.open(output_txt);

  vtkSmartPointer<vtkFloatArray> source_scalars = vtkSmartPointer<vtkFloatArray>::New();
  source_scalars = vtkFloatArray::SafeDownCast(source_poly->GetPointData()->GetScalars());

  for (vtkIdType i = 0; i < source_poly->GetNumberOfPoints(); ++i) {
      output << source_scalars->GetTuple1(i) << endl;
  }

  
  output.close();


}

int main(int argc, char **argv)
{
  
  char* source_poly_fn, *output_txt_fn;
   
  if (argc != 3)
  {
     cerr << "Not enough parameters\nUsage: <source.vtk> <output.txt>"<< endl; 
     exit(1);
  }
  
  source_poly_fn = argv[1];
  output_txt_fn = argv[2];
 
	WriteScalar(source_poly_fn, output_txt_fn);  
  
  
}